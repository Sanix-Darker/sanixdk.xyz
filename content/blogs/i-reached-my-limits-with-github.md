<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-reached-my-limits-with-github/gitci.png" loading="lazy" decoding="async" />

## I REACHED MY LIMITS WITH GITHUB

`2026-08-20 05:21PM` • 7 min read • **#github** **#actions** **#cicd** **#go** **#devops**

---

<div id="toc-container"></div>

---

I still use GitHub. I like pull requests and the social part of open source there.

Sooo... yes, the title is deliberate rage bait, but the problem in this post is mostly GitHub Actions.

Actions became the slowest part of my feedback loop. One small fix can require a push, a remote queue, and a hosted runner before I see the result. If the job fails, I repeat the trip.

The delay also breaks the thread in my head.

### AI MADE THE LIMIT SHOW UP EARLY

AI changed how often I push. I used to batch more work into one commit. Now one prompt can become four reviewed commits, with a new run after each adjustment. AI can produce commits faster than a monthly quota can forgive them.

Those extra runs made me reach the GitHub Actions monthly limit much earlier. The limit stopped being an abstract line on the billing page. It became a hard stop during normal work.

That hard stop pushed me to run the first CI pass near the code, before a hosted runner entered the path. It was only one part of the problem.

### THE HOSTED RUNNER IS THE DEPENDENCY

GitHub Actions is where I hit the limit. The same failure mode exists with any hosted runner, including shared GitLab runners, CircleCI, and Bitbucket Pipelines.

When the first test result depends on a vendor queue, my work also depends on that vendor's capacity, control plane, account rules, and billing meter. IMHO, hosted runners provide a useful clean environment, but every feedback round does not need one.

The recent GitHub incidents made this dependency hard to ignore. On `2026-08-20`, the main [GitHub Status page](https://www.githubstatus.com/) showed Actions at `99.33%` uptime over the previous 90 days.

<table>
  <thead>
    <tr><th>Date</th><th>What happened</th></tr>
  </thead>
  <tbody>
    <tr><td>2026-03-05</td><td><a href="https://github.blog/news-insights/company-news/github-availability-report-march-2026/">95% of workflow runs</a> failed to start within 5 minutes. Average delay: 30 minutes.</td></tr>
    <tr><td>2026-05-05/06</td><td><a href="https://github.blog/news-insights/company-news/github-availability-report-may-2026/">Hosted runner incidents</a> hit standard runners twice, with failed or delayed jobs.</td></tr>
    <tr><td>2026-06-25</td><td><a href="https://github.blog/news-insights/company-news/github-availability-report-june-2026/">Background job delays</a> affected pushes, PRs, Actions workflows, and webhooks.</td></tr>
    <tr><td>2026-07-25</td><td><a href="https://github.blog/news-insights/company-news/github-availability-report-july-2026/">Actions degraded twice</a>. At peak, 30% of runs were delayed and 60% failed with infrastructure errors.</td></tr>
    <tr><td>2026-08-06</td><td><a href="https://www.githubstatus.com/incidents/qcvjkzcs7j74">Actions failed hard</a>. At peak, 71% of workflow runs had infrastructure failures.</td></tr>
    <tr><td>2026-08-17</td><td><a href="https://www.githubstatus.com/incidents/zkxwbgr0cnmx">GitHub.com was degraded for 7h47m</a>. Actions was one of the affected services.</td></tr>
  </tbody>
</table>

That sequence sits inside my daily path:

```text
push -> webhook -> queue -> runner -> logs -> green check
```

Reliability sent me toward local CI, and the pricing news gave me another reason to stay there.

### THE PRICING DRAMA NEEDS ONE CORRECTION

Standard GitHub-hosted runner rates fell by up to `39%` on `2026-01-01`, according to GitHub's [pricing announcement](https://github.com/resources/insights/2026-pricing-changes-for-github-actions).

The loud part was a proposed `$0.002` per-minute platform fee for private-repository jobs. The fee would also apply when users supplied their own self-hosted machines. People would pay for the server, power, and maintenance, then pay GitHub for each minute on that server. That was an ambitious invoice, lol...

The pushback was immediate. GitHub [postponed the self-hosted fee](https://github.com/orgs/community/discussions/182186) and said it had missed the mark.

The billing story still moved closer to AI. Since `2026-06-01`, [Copilot code reviews on private repositories consume GitHub Actions minutes](https://github.blog/changelog/2026-04-27-github-copilot-code-review-will-start-consuming-github-actions-minutes-on-june-1-2026/). They also consume AI credits. Usage above the included Actions quota is billed at the standard rate.

For me, the lower rate did not stop my included minutes from running out earlier.

I am also not a big Microsoft fan. GitHub is useful enough that I keep using it, but each new meter gives that opinion regular maintenance.

By then, I already had a tool that could remove most hosted runs from the inner loop.

### WHY I CAME BACK TO GIT-CI

Last year I started a small Go project called [git-ci](https://github.com/sanix-darker/git-ci). The first version was simple: read a CI file and run its jobs locally.

At the time, it felt like a useful weekend tool. Now I need it as the default first pass. My laptop already has the code, and my VPS already has Docker. Both can run `go test ./...` before I push.

The CLI path is still the core:

```bash
gci validate --strict
gci run --job test
gci run --docker --only "test-*"
```

It parses GitHub Actions and GitLab CI today. It supports matrix jobs, `needs` ordering, filters, Docker or Podman, resource limits, and strict validation.

The local loop becomes:

```text
edit -> validate -> run -> fix -> push
```

The CLI covers that loop in one terminal. Running the same checks on a VPS needs a service.

### THE NEW PART: SERVE MODE

That need led to `gci serve`. It lets a workstation or VPS run CI for checkouts it already owns.

The current service includes:

- loopback-only HTTP, token login, signed sessions, and CSRF checks
- approved project roots and workflow discovery
- durable state for runs, logs, schedules, and deployments
- webhooks, encrypted secrets, and environment gates

The public page is already live at [gci.sanixdk.xyz](https://gci.sanixdk.xyz/).

![git-ci public page](/assets/i-reached-my-limits-with-github/gci-landing.png)

It starts with a small command:

```bash
gci serve \
  --listen 127.0.0.1:8087 \
  --state-dir /var/lib/git-ci \
  --projects-root /srv/projects
```

Caddy or nginx can sit in front while the service stays on loopback. The worker uses the local checkout.

SQLite keeps the service portable. There is no database server to rebuild or reconnect. I can stop `gci`, copy its state directory to another machine, start it again, and use the same project layout.

Once the VPS can run CI, it also needs a way to receive commits without GitHub.

### GIT ALREADY GIVES ME A BACKUP REMOTE

Git already includes the parts needed for a small server. The official [Git server guide](https://git-scm.com/book/en/v2/Git-on-the-Server-Setting-Up-the-Server.html) uses a bare repository and SSH. On my VPS, the basic setup can be this small:

```bash
ssh git@my-vps \
  'mkdir -p /srv/git/my-app.git && git init --bare /srv/git/my-app.git'

git remote add vps git@my-vps:/srv/git/my-app.git
git push vps main
```

`origin` is only the conventional name for the main remote. I can keep GitHub as `origin` and add `vps` as a second remote. When GitHub is unavailable, or when its hosted runner is stuck, I can push the commit to my VPS and start checks there.

Git also ships `git daemon`, but its write service has no authentication and is disabled by default. SSH is the sensible choice for a private writable remote. A `post-receive` hook on the bare repository can later notify `gci` and start checks near the code.

The full path can stay under my control:

```text
edit -> validate -> test -> commit -> push to VPS -> run on VPS
```

During an outage, I can keep iterating on the app, validate my tests, inspect the logs, and record commits on the VPS. The pull request can wait until GitHub returns.

Local and VPS runs cannot prove that every hosted image behaves the same way. The hosted run still checks those differences after I push.

The next red status page will delay the merge, but it will not stop the work.

---
