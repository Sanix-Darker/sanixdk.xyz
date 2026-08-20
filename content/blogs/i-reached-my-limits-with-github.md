<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-reached-my-limits-with-github/gitci.png" loading="lazy" decoding="async" />

## I REACHED MY LIMITS WITH GITHUB

`2026-08-20 05:21PM` • 9 min read • **#github** **#actions** **#cicd** **#go** **#devops**

---

<div id="toc-container"></div>

---

I still use GitHub. I still like pull requests and the social part of open source there.

The title is deliberate rage bait, but the problem in this post is mostly GitHub Actions.

Actions became the slowest part of my feedback loop. One small fix becomes one push, one remote queue, and one hosted runner doing whatever it wants before it starts my job. If the job fails, I get the same loop again.

The cost reaches past minutes. It breaks the thread in my head.

### AI MADE THE LIMIT SHOW UP EARLY

AI changed my run rate.

Before, I batched more work before a push. Now I can ask an agent for a small change, review the diff, and push again. That is useful. It also multiplies CI runs.

One prompt can become four commits. A small refactor can cause a test run, a lint run, and more runs after each fix. AI can produce commits faster than a monthly quota can forgive them.

The code moved faster than my CI budget. With AI, I multiplied my run count, so the GitHub Actions monthly usage limit started to show up too early. The quota was no longer an abstract line in billing. It became a hard stop in the middle of normal work.

Waiting on hosted runners was already bad. Burning the monthly quota because AI made me iterate more often was worse.

That is when I stopped treating local CI as a comfort tool. I needed a first pass that I could run near the code, before GitHub Actions entered the path.

### THIS CAN HAPPEN WITH ANY HOSTED RUNNER

GitHub Actions is where I hit the limit. The same failure mode exists with any hosted runner.

It can be a shared GitLab runner, CircleCI, Bitbucket Pipelines, or another managed CI fleet. If the first useful test result depends on a vendor queue, my work depends on that vendor's capacity, control plane, account rules, and billing meter.

Changing the provider leaves the same dependency in place. My code is ready, but someone else's computer has not started it.

Hosted runners are still useful. They give me a clean environment and a final check outside my machine. I do not want them to control the full feedback loop.

### THE LAST SIX MONTHS DID NOT HELP

I looked again before writing this, because memory lies and status pages have timestamps.

On `2026-08-20`, the main [GitHub Status page](https://www.githubstatus.com/) showed Actions at `99.33%` uptime over the previous 90 days. For a CI system, that is a lot of red time.

The recent reports are rough.

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

The failures hit the exact path I use every day.

My daily path is push, webhook, queue, runner, logs, green check.

If any part stalls, my local work is ready but my feedback is blocked somewhere else.

### THE PRICING DRAMA NEEDS ONE CORRECTION

Standard GitHub-hosted runner rates fell by up to `39%` on `2026-01-01`, according to GitHub's [pricing announcement](https://github.com/resources/insights/2026-pricing-changes-for-github-actions).

The loud part was a proposed `$0.002` per-minute platform fee for private-repository jobs. The fee would also apply when users supplied their own self-hosted machines. People would pay for the server, power, and maintenance, then pay GitHub for each minute on that server. That was an ambitious invoice.

The pushback was immediate. GitHub [postponed the self-hosted fee](https://github.com/orgs/community/discussions/182186) and said it had missed the mark.

The billing story still moved closer to AI. Since `2026-06-01`, [Copilot code reviews on private repositories consume GitHub Actions minutes](https://github.blog/changelog/2026-04-27-github-copilot-code-review-will-start-consuming-github-actions-minutes-on-june-1-2026/). They also consume AI credits. Usage above the included Actions quota is billed at the standard rate.

The unit price can fall while the monthly quota disappears sooner. AI creates more commits and more CI work. I can see that in my own workflow.

I am also not a big Microsoft fan. GitHub is useful enough that I keep using it, but each new meter gives that opinion regular maintenance.

### WHY I CAME BACK TO GIT-CI

Last year I started a small Go project called [git-ci](https://github.com/sanix-darker/git-ci). The first version was simple: read a CI file and run its jobs locally.

At the time, it felt like a useful weekend tool with no urgency.

Now it feels like the default first pass. I am keeping hosted CI where it helps: clean final runs, pull requests, review, and distribution.

The dumb wait belongs somewhere else. The laptop already has the code. The VPS already has Docker. A workflow file is YAML plus commands. I should not need a hosted queue to know if `go test ./...` is broken.

The CLI path is still the core:

```bash
gci validate --strict
gci run --job test
gci run --docker --only "test-*"
```

It parses GitHub Actions and GitLab CI today. It has matrix expansion, `needs` ordering, Bash/Docker/Podman runners, filters, dry-runs, env files, resource limits, and a strict validator. The goal is broader than one provider because the waiting problem is broader than one provider.

That already fixes the most painful local loop:

```text
edit -> run CI locally -> fix -> run again -> push when boring
```

No remote queue in that path.

### THE NEW PART: SERVE MODE

The thing I am working on now is `gci serve`.

The CLI is good when I am in one terminal. A small service is better when CI runs on a workstation or VPS that already owns the checkout.

The current service code has:

- loopback-only HTTP by default
- admin token login
- signed browser sessions
- CSRF checks on browser mutations
- approved project roots
- SQLite state
- workflow discovery
- durable runs, jobs, steps, and logs
- schedules
- webhooks
- encrypted secrets
- deployment records and environment gates

The public page is already live at [gci.sanixdk.xyz](https://gci.sanixdk.xyz/).

![git-ci public page](/assets/i-reached-my-limits-with-github/gci-landing.png)

The shape is boring on purpose:

```bash
gci serve \
  --listen 127.0.0.1:8087 \
  --state-dir /var/lib/git-ci \
  --projects-root /srv/projects
```

Caddy or nginx can sit in front. The service stays on loopback. Project paths come from approved roots. The state is local. The worker runs near the code.

SQLite keeps the service portable. There is no external database server to rebuild or reconnect. I can stop `gci`, copy its state directory to another machine, start it again, and point it at the same project root shape.

That gives me a small control plane without turning my CI into another hosted platform.

### GIT ALREADY GIVES ME A BACKUP REMOTE

CI is only one part of the dependency. I also want a place to push commits while a hosted Git provider is down.

Git already includes the parts needed for a small server. The official [Git server guide](https://git-scm.com/book/en/v2/Git-on-the-Server-Setting-Up-the-Server.html) uses a bare repository and SSH. On my VPS, the basic setup can be this small:

```bash
ssh git@my-vps \
  'mkdir -p /srv/git/my-app.git && git init --bare /srv/git/my-app.git'

git remote add vps git@my-vps:/srv/git/my-app.git
git push vps main
```

`origin` is only the conventional name for the main remote. I can keep GitHub as `origin` and add `vps` as a second remote. When GitHub is unavailable, or when a hosted runner is stuck, I can push the same commit to the VPS and start checks there.

Git also ships `git daemon`, but its write service has no authentication and is disabled by default. SSH is the sensible choice for a private writable remote. A `post-receive` hook on the bare repository can later notify `gci` and start checks near the code.

This gives me a path that does not depend on a hosted Git control plane:

```text
edit -> validate -> test -> commit -> push to VPS -> run on VPS
```

A blocked pull request should not stop work on my app.

### WHAT I WANT FROM THIS

I want the first CI answer in seconds, before a queue clears. I want to validate the workflow and run its test jobs before I push to a hosted provider.

If GitHub is down, I may be unable to merge a pull request. I should still keep iterating on the app, validate my tests, inspect the logs, commit the fixes, and push them to my VPS.

Local and VPS runs do not prove that every hosted image behaves the same way. The final hosted run still has value. It should confirm a tested change instead of discovering the first basic failure.

The next time a hosted status page turns red, I want to keep testing the app and recording commits. The provider can recover at its own pace.

---
