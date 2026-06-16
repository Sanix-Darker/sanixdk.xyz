<!-- Template for blog header -->
<!DOCTYPE html>
<html data-color-scheme="dark">
<head>
    <meta charSet='utf-8' />
    <link href='/favicon.ico' rel='icon' />
    <link rel='canonical' href='https://sanixdk.xyz' />
    <meta content='initial-scale=1.0,width=device-width' name='viewport' />
    <meta content='#131516' name='theme-color' />
    <meta http-equiv='content-language' content='en-us,fr'>
    <link rel="stylesheet" href="/assets/style.css">
    <link rel='stylesheet' href="https://cdn.jsdelivr.net/gh/highlightjs/cdn-release@11.9.0/build/styles/github-dark.min.css">
    <title>sanix | </title>
    <style>img{width: 100%;}pre{background: rgb(24, 27, 28);border: 1px solid white;padding: 0px!important; border-radius: 5px;font-size: auto;box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);}</style>
    <meta property='og:url' content='https://sanixdk.xyz'>
    <meta property='og:type' content='website'>
    <meta property='og:title' content=''>
    <meta name='twitter:title' content=''>
    <!-- <meta property='og:description' content=''> -->
    <meta property='og:image' content=''>
    <meta name='twitter:card' content=''>
    <meta property='twitter:url' content='https://sanixdk.xyz'>
    <meta property='twitter:domain' content='sanixdk.xyz'>
</head>
<body>
    <div class='container'>
    <br>
<center style="text-decoration: none">

[`home`](/)  [`blogs`](/blogs/) <!-- lazy guy, will code this later, • [`projects`](/projects/) --> [`about`](/about)

</center>

----


<!-- Template for blog header -->
<!DOCTYPE html>
<html data-color-scheme="dark">
<head>
    <meta charSet='utf-8' />
    <link href='/favicon.ico' rel='icon' />
    <link rel='canonical' href='https://sanixdk.xyz' />
    <meta content='initial-scale=1.0,width=device-width' name='viewport' />
    <meta content='#131516' name='theme-color' />
    <meta http-equiv='content-language' content='en-us,fr'>
    <link rel="stylesheet" href="/assets/style.css">
    <link rel='stylesheet' href="https://cdn.jsdelivr.net/gh/highlightjs/cdn-release@11.9.0/build/styles/github-dark.min.css">
    <title>sanix | I ADDED A BRAIN TO MY FRIDGE... it BACKFIRED HARD! (PART 1/3)</title>
    <style>img{width: 100%;}pre{background: rgb(24, 27, 28);border: 1px solid white;padding: 0px!important; border-radius: 5px;font-size: auto;box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);}</style>
    <meta property='og:url' content='https://sanixdk.xyz/blogs/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3'>
    <meta property='og:type' content='website'>
    <meta property='og:title' content='I ADDED A BRAIN TO MY FRIDGE... it BACKFIRED HARD! (PART 1/3)'>
    <meta name='twitter:title' content='I ADDED A BRAIN TO MY FRIDGE... it BACKFIRED HARD! (PART 1/3)'>
    <!-- <meta property='og:description' content=''> -->
    <meta property='og:image' content='https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/head.png'>
    <meta name='twitter:card' content='https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/head.png'>
    <meta property='twitter:url' content='https://sanixdk.xyz/blogs/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3'>
    <meta property='twitter:domain' content='sanixdk.xyz'>
</head>
<body>
    <div class='container'>
    <br>
<center style="text-decoration: none">

[`home`](/)  [`blogs`](/blogs/) <!-- lazy guy, will code this later, • [`projects`](/projects/) --> [`about`](/about)

</center>

----


<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/git-ci-preview.png" />

## I MADE GITHUB ACTIONS RUN ON MY LAPTOP... and i'm NEVER pushing to test again

`2026-06-16 01:23PM` • 8 min read • **#go** **#cicd** **#cli** **#docker**

---

<div id="toc-container"></div>

---

Okay okay, I know what you're thinkin...

Another dev writin about CI/CD. How original.

But hear me out for a sec.

**I hate waiting.** Like, I **reallly** hate it.

You know that feeling?

You make a tiny change. Push to GitHub. Wait 5 minutes for the pipeline to start. Another 3 minutes for it to run.

And then it fails because you forgot a semicolon.

**Five minutes. For a semicolon.**

You fix it. Push again. Wait again. Fail again.

And somewhere in between you've completly lost your train of thought. Your IDE is still open. Your coffee is cold.

And you're questioning why you became a programmer in the first place.

**I've been there too many times.**

So I asked myself: what if I could just... run CI locally?

Like, right here on my laptop? No pushing. No waiting. No 3AM debugging sessions?

And then I built it.

**Because of course I did.** (What else was i gonna do, actualy work?)

### THE PROBLEM (IT'S THE FEEDBACK LOOP, STUPID)

CI/CD pipelines are great in theory.

They test, lint, build, deploy... all the boring stuff we don't wanna do manualy.

But here's the thing nobody tells you:

**The feedback loop is horrible.**

| You think | CI does |
|-----------|---------|
| "quick fix, push it" | queues for 3 min, runs lint, fails, wait again |
| "add one more test" | triggers full pipeline, 7 more minutes |
| "lemme just check" | 15 commits later, still no green check |

I did this dance for years.

Push. Wait. Fail. Fix.

Push. Wait. Fail again. Curse softly.

Fix again. Push again. Wait again. Finaly pass.

**There had to be a better way.**

And yeah, I know Docker Compose exists. I know `go test` works.

But that's not the same thing.

CI pipelines have matrix builds. Service containers (databases, caches). Complex dependency graphs. Environment variables. Caching.

Running all of that manualy?

**A nightmare.**

So I decided to automate the automation.

Meta, I know. But that's how my brain works. (I blame the cofee.)

### ENTER GIT-CI (THE THING I PROBLY SHOULDN'T HAVE BUILT)

Here's what I built: **[git-ci](https://github.com/sanix-darker/git-ci)**.

A CLI tool. Written in Go. Runs CI/CD pipelines on your local machine.

The idea is stupid simple (the best kind of simple):

1. Point it at your `.github/workflows/ci.yml` or `.gitlab-ci.yml`
2. It parses the pipeline
3. It runs it using Bash, Docker, or Podman

**No pushing. No waiting. No 3AM debugging sessions.**

Here's how it looks:

```bash
# Just run whatever CI file it finds
$ git ci run

# Run with Docker (close to real CI env)
$ git ci run --docker

# Run a specific job
$ git ci run --job test

# Dry run to see what would happen
$ git ci run --dry-run --verbose
```

That's it.

Three commands and your CI pipeline is running on your machine.

**TADAN !!!**

### THE ARCHITECTURE (BORING BUT NECESSARY)

Okay i'm gonna get a little technical here.

But I promisse it's cool.

The core is **Go** (because Go compiles fast, runs everywhere, no runtime needed). It's structured in 3 layers:

| Layer | What it does |
|-------|-------------|
| **Parsers** | Read CI config files, create universal Pipeline types |
| **Handlers** | Manage commands, validate, expand matrix, resolve deps |
| **Runners** | Execute jobs via Bash, Docker, or Podman |

The cool part?

Every CI provider parser outputs the **same** data structure.

GitHub Actions, GitLab CI... all converted into the same `Job`/`Step` types.

```go
type Pipeline struct {
    Name        string
    Jobs        map[string]*Job
    Env         map[string]string
    Defaults    *Defaults
}

type Job struct {
    Name        string
    Steps       []Step
    Needs       []string
    Image       string
    Services    []Service
    Strategy    *Matrix
    Env         map[string]string
}
```

It's not rocket science.

It's just... reading YAML files and doing what they say.

Like a butler for your CI pipeline. A butler that never sleeps and doesn't judge your commit messages.

### THE MATRIX STUFF (MATH IS HARD)

One thing I had to figure out: **matrix expansion**.

You know when you see this:

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, macos-latest]
    go: ["1.21", "1.22"]
    include:
      - os: ubuntu-latest
        go: "1.22"
        experimental: true
    exclude:
      - os: macos-latest
        go: "1.21"
```

That's a Cartesian product.

2×2 = 4 combos, minus 1 exclude, plus 1 include = 4 actual jobs.

**Why didn't i study harder in math class ???**

Implementing this was... humbling.

The include/exclude logic especialy. `include` can add fields to existing combos OR create new ones. `exclude` needs to match precisely.

I stared at my screen for 3 hours. My cofee went cold twice.

**But I got it working eventualy.**

Map-based dedup. Matrix values as env vars (raw + `MATRIX_` prefixed). Because why not both.

```bash
# Each job gets:
#   os=ubuntu-latest, go=1.22, experimental=true
#   MATRIX_OS=UBUNTU_LATEST, MATRIX_GO_VERSION=1.22
```

### DEPENDENCY RESOLUTION (I FELT SMART FOR A MINUTE)

Jobs have `needs` dependencies:

```yaml
jobs:
  lint:
    steps: [echo linting]
  test:
    needs: [lint]
    steps: [echo testing]
  build:
    needs: [lint, test]
    steps: [echo building]
```

This creates a DAG.

Don't panic. It's just a fancy way to say "do things in the right order."

I used **Kahn's algorithm** for topological sorting.

The logic:

1. Start with jobs that have no dependencies
2. Run them
3. Unlock the jobs that depended on them
4. Repeat untill everything's done

Circular dependencies are detected and reported.

**No more infinite loops.** You're welcome.

### DOCKER SERVICES (THE DATABASE PROBLEM)

Integration tests need databases. Postgres, Redis, MySQL...

The usual suspects.

In CI, you define them as services:

```yaml
services:
  postgres:
    image: postgres:15
    env:
      POSTGRES_PASSWORD: test
  redis:
    image: redis:7
```

git-ci creates a dedicated bridge network. Starts each service with DNS aliases. Attaches the job container to the same network.

Your code connects to `postgres:5432` or `redis:6379`.

No config changes needed.

And cleanup is automatic.

**Because i'm not a monster.** (Mostly.)

```bash
$ git ci run --docker --job integration
# Postgres and Redis run. Tests connect. Everything cleans up.
```

### THE THINGS THAT WENT WRONG (SPOILER: A LOT)

Okay i'm gonna be honnest with you.

Not everything worked perfectly.

**Far from it.**

- **Image resolution**: GitHub uses `runs-on: ubuntu-latest`. Locally? You don't have that. I had to map to Docker images like `catthehacker/ubuntu:act-latest`. Finding the right mappings was anoying. Like, realy anoying.

- **Step parsing**: GitHub Actions has 5 different ways to define steps. `uses:`, `run:`, `with:`, `if:`, `env:` at step level... it's a LOT. **Why do they need 5 ways ???**

- **Shell escaping**: Passing commands through Docker exec with proper escaping? Nightmare. I probly got it 90% right. The other 10% are in a dark corner of my codebase that i don't talk about.

- **Windows support**: Yeah... about that. Let's just say it works on Linux and macOS for now. Windows users, i'm sorry. (Not that sorry.)

### THE VERDICT

Does it actualy work?

**Yes. SURPRISINGLY yes.**

I use it every day now.

Before pushing anything:

```bash
$ git ci run --docker
```

If it passes locally, it passes on CI.

And i don't have to wait 5 minutes to find out i forgot a stupid semicolon.

**Is it perfect?**

Hell no.

Edge cases everywhere. Providers i don't support (Azure, Bitbucket, CircleCI, Travis... the list is longer than my grocery list).

But it works for the things i need it to work for.

And even if it only saves me 20 minutes a day... that's 20 minutes i can spend building actual stuff.

Or drinking coffee.

Or questioning my life choices.

You know, the important things.

### LESSONS LEARNED (I'M A SLOW LEARNER)

1. **Go is awesome for CLI tools.** Single binary. Cross-compilation. Fast startup. Can't beat it.

2. **YAML is evil.** I mean it. The amount of YAML edge cases in CI config files is insane. Indentation matters. Anchors are confusing. Multi-line strings? Don't get me started.

3. **CI/CD is actualy complex.** When i started, i thought "how hard can it be to read a YAML file and run commands?" The answer: harder than you think. Like, **WAY** harder.

4. **Local CI is a game changer.** The feedback loop goes from 5-10 minutes to 5-10 seconds. That changes how you work. You stop fearing the push button.

### WHAT'S NEXT

I'm planning to add more providers. CircleCI is next. Then maybe Drone and Travis.

Also want to add a K8s runner at some point.

**Because why not.** YOLO.

But for now, it does what i need it to do.

And maybe it'll help you too.

Check it out on **[GitHub](https://github.com/sanix-darker/git-ci)**. PRs welcome. Issues welcome. Rants about my code style? Also welcome, i guess.

---

**PS:** I initially mentiond this project at the end of my [HOW I FIXED GIT REFLOG](https://sanixdk.xyz/blogs/how-i-fixed-git-reflog) post, back when it was just an idea. Now it's actualy workin and i use it daily. Funny how things evolve when you stop thinking and just start coding.

### FURTHER READING

- **[git-ci on GitHub](https://github.com/sanix-darker/git-ci)** - The actual repo
- **[GitHub Actions Docs](https://docs.github.com/en/actions)** - What i had to reverse-engineer
- **[Kahn's Algorithm](https://en.wikipedia.org/wiki/Topological_sorting#Kahn's_algorithm)** - Behind the dependency resolution
- **[catthehacker/ubuntu](https://github.com/catthehacker/docker_images)** - Docker images that mimic GitHub runners

-----------
[<< blogs](/blogs/)
-----------
[<< blogs](/blogs/)

<script src="https://giscus.app/client.js"
        data-repo="sanix-darker/sanixdk.xyz"
        data-repo-id="R_kgDOJSF_ow"
        data-category="General"
        data-category-id="DIC_kwDOJSF_o84CXz0s"
        data-mapping="pathname"
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="0"
        data-input-position="top"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
<script>hljs.highlightAll(); </script>
<script>
// To handle TOC
(function() {
    const tocContainer = document.getElementById('toc-container');

    if(!tocContainer) {
        return;
    }
    const headingSelectors = ['h2', 'h3', 'h4', 'h5', 'h6'];
    const headings = Array.from(document.querySelectorAll(headingSelectors.join(', ')));

    let tocHtml = '<ul>';
    let currentLevel = 2;

    headings.forEach((heading, index) => {
        if (!heading.id) {
            heading.id = 'toc-heading-' + index;
        }
        const level = parseInt(heading.tagName.substring(1));

        while (level > currentLevel) {
            tocHtml += '<ul>';
            currentLevel++;
        }

        while (level < currentLevel) {
            tocHtml += '</ul></li>';
            currentLevel--;
        }

        tocHtml += `<li><a href="#${heading.id}">${heading.textContent}</a>`;
    });

    while (currentLevel > 2) {
        tocHtml += '</ul></li>';
        currentLevel--;
    }

    tocHtml += '</ul>';
    tocContainer.innerHTML += tocHtml;
})();
</script>


<hr/>

<center style="text-decoration: none">

[`feed`](https://sanixdk.xyz/feed.xml)  [`github`](https://github.com/sanix-darker "Where i sleep in term of projects.")  [`telegram`](https://t.me/sanixdarker "You can Conctact me here anytime.")  [`email`](mailto:s4nixdk@gmail.com?subject=Hello%20there "You can email me anytime.")

</center>
            </div>
        <br/>
        <script>
            const blogList = document.getElementById("blog-list");
            if (blogList) {
                const blogCount = document.querySelectorAll("h3").length;
                blogList.style.setProperty("--total-items", `${blogCount}`);
                console.log("Blog count :", blogCount);
            }

            document.addEventListener('DOMContentLoaded', function() {
                const searchInput = document.getElementById('search-input');
                const blogItems = document.querySelectorAll('.blog-item, article');

                if (!searchInput) return;

                searchInput.addEventListener('input', function() {
                    const searchTerm = this.value.toLowerCase().trim();

                    blogItems.forEach(function(item) {
                        const title = item.querySelector('h1, h2, h3, .blog-title');
                        const content = item.querySelector('p, .blog-excerpt, .content');
                        const tags = item.querySelectorAll('.tag, .tags span');

                        let searchableText = '';

                        // Add title text
                        if (title) {
                            searchableText += title.textContent.toLowerCase() + ' ';
                        }

                        // Add content text
                        if (content) {
                            searchableText += content.textContent.toLowerCase() + ' ';
                        }

                        // Add tags text
                        tags.forEach(function(tag) {
                            searchableText += tag.textContent.toLowerCase() + ' ';
                        });

                        // Show/hide based on search
                        if (searchTerm === '' || searchableText.includes(searchTerm)) {
                            item.style.display = 'block';
                            item.style.opacity = '1';
                        } else {
                            item.style.display = 'none';
                            item.style.opacity = '0';
                        }
                    });

                    // Show "no results" message if needed
                    const visibleItems = Array.from(blogItems).filter(item =>
                        item.style.display !== 'none'
                    );

                    let noResultsMsg = document.getElementById('no-results');
                    if (visibleItems.length === 0 && searchTerm !== '') {
                        if (!noResultsMsg) {
                            noResultsMsg = document.createElement('div');
                            noResultsMsg.id = 'no-results';
                            noResultsMsg.className = 'no-results';
                            noResultsMsg.innerHTML = '<p>// no posts found matching your search</p>';

                            const blogContainer = document.querySelector('.blog-list, .blog-posts, main');
                            if (blogContainer) {
                                blogContainer.appendChild(noResultsMsg);
                            }
                        }
                        noResultsMsg.style.display = 'block';
                    } else if (noResultsMsg) {
                        noResultsMsg.style.display = 'none';
                    }
                });
            });
        </script>
    </body>
</html>
[<< blogs](/blogs/)

<script src="https://giscus.app/client.js"
        data-repo="sanix-darker/sanixdk.xyz"
        data-repo-id="R_kgDOJSF_ow"
        data-category="General"
        data-category-id="DIC_kwDOJSF_o84CXz0s"
        data-mapping="pathname"
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="0"
        data-input-position="top"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
<script>hljs.highlightAll(); </script>
<script>
// To handle TOC
(function() {
    const tocContainer = document.getElementById('toc-container');

    if(!tocContainer) {
        return;
    }
    const headingSelectors = ['h2', 'h3', 'h4', 'h5', 'h6'];
    const headings = Array.from(document.querySelectorAll(headingSelectors.join(', ')));

    let tocHtml = '<ul>';
    let currentLevel = 2;

    headings.forEach((heading, index) => {
        if (!heading.id) {
            heading.id = 'toc-heading-' + index;
        }
        const level = parseInt(heading.tagName.substring(1));

        while (level > currentLevel) {
            tocHtml += '<ul>';
            currentLevel++;
        }

        while (level < currentLevel) {
            tocHtml += '</ul></li>';
            currentLevel--;
        }

        tocHtml += `<li><a href="#${heading.id}">${heading.textContent}</a>`;
    });

    while (currentLevel > 2) {
        tocHtml += '</ul></li>';
        currentLevel--;
    }

    tocHtml += '</ul>';
    tocContainer.innerHTML += tocHtml;
})();
</script>


<hr/>

<center style="text-decoration: none">

[`feed`](https://sanixdk.xyz/feed.xml)  [`github`](https://github.com/sanix-darker "Where i sleep in term of projects.")  [`telegram`](https://t.me/sanixdarker "You can Conctact me here anytime.")  [`email`](mailto:s4nixdk@gmail.com?subject=Hello%20there "You can email me anytime.")

</center>
            </div>
        <br/>
        <script>
            const blogList = document.getElementById("blog-list");
            if (blogList) {
                const blogCount = document.querySelectorAll("h3").length;
                blogList.style.setProperty("--total-items", `${blogCount}`);
                console.log("Blog count :", blogCount);
            }

            document.addEventListener('DOMContentLoaded', function() {
                const searchInput = document.getElementById('search-input');
                const blogItems = document.querySelectorAll('.blog-item, article');

                if (!searchInput) return;

                searchInput.addEventListener('input', function() {
                    const searchTerm = this.value.toLowerCase().trim();

                    blogItems.forEach(function(item) {
                        const title = item.querySelector('h1, h2, h3, .blog-title');
                        const content = item.querySelector('p, .blog-excerpt, .content');
                        const tags = item.querySelectorAll('.tag, .tags span');

                        let searchableText = '';

                        // Add title text
                        if (title) {
                            searchableText += title.textContent.toLowerCase() + ' ';
                        }

                        // Add content text
                        if (content) {
                            searchableText += content.textContent.toLowerCase() + ' ';
                        }

                        // Add tags text
                        tags.forEach(function(tag) {
                            searchableText += tag.textContent.toLowerCase() + ' ';
                        });

                        // Show/hide based on search
                        if (searchTerm === '' || searchableText.includes(searchTerm)) {
                            item.style.display = 'block';
                            item.style.opacity = '1';
                        } else {
                            item.style.display = 'none';
                            item.style.opacity = '0';
                        }
                    });

                    // Show "no results" message if needed
                    const visibleItems = Array.from(blogItems).filter(item =>
                        item.style.display !== 'none'
                    );

                    let noResultsMsg = document.getElementById('no-results');
                    if (visibleItems.length === 0 && searchTerm !== '') {
                        if (!noResultsMsg) {
                            noResultsMsg = document.createElement('div');
                            noResultsMsg.id = 'no-results';
                            noResultsMsg.className = 'no-results';
                            noResultsMsg.innerHTML = '<p>// no posts found matching your search</p>';

                            const blogContainer = document.querySelector('.blog-list, .blog-posts, main');
                            if (blogContainer) {
                                blogContainer.appendChild(noResultsMsg);
                            }
                        }
                        noResultsMsg.style.display = 'block';
                    } else if (noResultsMsg) {
                        noResultsMsg.style.display = 'none';
                    }
                });
            });
        </script>
    </body>
</html>
