<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-reached-my-limits-with-github/gitci.png" loading="lazy" decoding="async" />

## I REACHED MY LIMITS WITH GITHUB

`2026-08-20 05:21PM` • 6 min read • **#github** **#actions** **#cicd** **#go** **#devops**

---

<div id="toc-container"></div>

---

I still use GitHub. I still like pull requests. I still like the social part of open source there.
The real problem is mostly GitHub Actions.
Actions became the slowest part of my feedback loop.
One small fix becomes one push, one remote queue, and one hosted runner doing whatever it wants before it starts my job. If the job fails, I get the same loop again.

The cost reaches past minutes. It breaks the thread in my head.

### AI MADE THE LIMIT SHOW UP EARLY

AI changed my run rate.

Before, I batched more work before a push. Now I can ask an agent for a small change, review the diff, and push again. That is useful. It also multiplies CI runs.
One prompt can become four commits. A small refactor can become one run for tests, one run for lint, one run after a rename, then another run after a fix.

The code moved faster than my CI budget.
With AI, I multiplied my run count, so the GitHub Actions monthly usage limit started to show up too early. The quota was no longer an abstract line in billing. It became a hard stop in the middle of normal work.
Waiting on hosted runners was already bad. Burning the monthly quota because AI made me iterate more often was worse.

That is when I stopped treating local CI as a comfort tool. I needed a first pass that I could run near the code, before GitHub Actions entered the path.

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

### WHY I CAME BACK TO GIT-CI

Last year I started a small Go project called [git-ci](https://github.com/sanix-darker/git-ci). The first version was simple: read a CI file and run its jobs locally.

At the time, it felt like a weekend tool. Useful, but not urgent.
Now it feels more like the default first pass.
I am keeping GitHub where it helps: code hosting, pull requests, review, and distribution.

The dumb wait belongs somewhere else. The laptop already has the code. The VPS already has Docker. A workflow file is just YAML plus commands. I should not need a hosted queue to know if `go test ./...` is broken.

The CLI path is still the core:

```bash
gci validate --strict
gci run --job test
gci run --docker --only "test-*"
```

It parses GitHub Actions and GitLab CI today. It has matrix expansion, `needs` ordering, Bash/Docker/Podman runners, filters, dry-runs, env files, resource limits, and a strict validator.

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

SQLite also keeps the service easy to move. Stop it, copy the state directory, start it on another box, and point it at the same project root shape.

That gives me a small control plane without turning my CI into another hosted platform.

### WHAT I WANT FROM THIS

I want the first CI answer in seconds, not after a queue clears.
I want to run the same workflow shape before I push.
I want a VPS to run the boring checks for my own projects without waiting on hosted capacity.
I want GitHub outages to hurt less. If GitHub is down, I may still be unable to merge a PR. Fine. But I should still be able to run the pipeline, read logs, fix the code, and prepare the next push.

The target is plain: a shorter loop.
GitHub can keep the remote checkmark.

My machine can do the first run.



-----------
