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

## HOW I MADE CI/CD RUN LOCALLY IN GO (AND STOPPED PUSHING TO TEST)

`2026-06-16 01:23PM` • 8 min read • **#go** **#cicd** **#cli** **#docker**

---

<div id="toc-container"></div>

---

Okay okay, I know what you're thinkin... another dev writin about CI/CD. But hear me out.

I hate waiting. Like, I **reallly** hate it.

And I think every developer knows this one specific pain: you make a tiny change, push to GitHub, wait 5 minutes for the CI pipeline to start, another 3 minutes for it to run... and then it fails because you forgot a semicolon.

**Five minutes. For a semicolon.**

And then you fix it, push again, and wait another 5 minutes. And somewhere in between you've completly lost your train of thought, your IDE is still open with that half-baked refactor, and your coffee is cold.

So I asked myself: what if I could just... run CI locally? Like, right here on my laptop, without pushing anything, without waiting for GitHub's servers to wake up, without any of that nonsense?

And then I built it. Because of course I did.

### THE PROBLEM WITH CI/CD (ITS THE FEEDBACK LOOP, STUPID)

Look, CI/CD pipelines are great in theory. They automate testing, linting, building, deploying... all the boring stuff we don't wanna do manualy.

But here's the thing nobody talks about: the feedback loop is **horrible**.

| You think | CI does |
|-----------|---------|
| "quick fix, push it" | queues for 3 minutes, runs lint, fails, you wait again |
| "add one more test" | triggers full pipeline (again), 7 minutes this time |
| "let me just check" | 15 commits later you're still waiting for the green check |

I did this dance for years. Push, wait, fail, fix, push, wait, fail again, curse softly, fix again, push again, wait again, finaly pass.

**There had to be a better way.**

And yeah, I know Docker Compose exists. I know you can run tests locally with `go test` or `npm test`. But that's not the same thing. CI pipelines have matrix builds, service containers (databases, caches), complex dependency graphs between jobs, environment variables, caching... running all of that manually is a nightmare.

So I decided to automate the automation. Meta, I know.

### ENTER GIT-CI: THE THING I PROBLY SHOULDNT HAVE BUILT

So here's what I built: **[git-ci](https://github.com/sanix-darker/git-ci)**. It's a CLI tool written in Go that runs CI/CD pipelines on your local machine.

The idea is stupid simple (the best kind of simple):

1. Point it at your `.github/workflows/ci.yml` or `.gitlab-ci.yml`
2. It parses the pipeline
3. It runs it locally using either Bash, Docker, or Podman

**No pushing. No waiting. No 3AM debugging sessions** because the CI broke something and you have no idea why.

Here's how it looks:

```bash
# Just run whatever CI file it finds
$ git ci run

# Run with Docker (so it's close to real CI env)
$ git ci run --docker

# Run a specific job
$ git ci run --job test

# Dry run to see what would happen
$ git ci run --dry-run --verbose
```

That's it. Three commands and your CI pipeline is running on your machine.

### THE ARCHITECTURE (BORING BUT NECESSARY)

Okay so I'm gonna get a little technical here but I promisse it's cool.

The core of git-ci is written in **Go** (because Go compiles fast, runs everywhere, and doesn't need a runtime). It's structured in 3 main layers:

| Layer | What it does |
|-------|-------------|
| **Parsers** | Read CI config files (GitHub Actions, GitLab CI) and create universal Pipeline types |
| **Handlers** | Manage commands, validate pipelines, expand matrix jobs, resolve deps |
| **Runners** | Actualy execute the jobs. Bash, Docker, or Podman |

The cool part? Every CI provider parser outputs the **same** data structure. So a GitHub Actions pipeline and a GitLab CI pipeline both get converted into the same `Job`/`Step` types. The runners don't care where the pipeline came from.

```go
// pkg/types/types.go - The universal types
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

It's not rocket science. It's just... actualy reading the YAML files and doing what they say. Like a butler for your CI pipeline.

### THE MATRIX STUFF (MATH IS HARD LET ME TELL YOU)

One of the things I had to figure out was **matrix expansion**. You know when you see this in a GitHub Actions file:

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

That's a Cartesian product. 2×2 = 4 combinations, minus 1 exclude, plus 1 include = 4 actual jobs. Each one gets its own set of environment variables.

Implementing this was... humbling. The include/exclude logic especialy. Because `include` can add extra fields to existing combinations or create new ones entirely. And `exclude` needs to match existing combinations precisely.

**But I got it working eventualy.** Uses a map-based dedup strategy and injects matrix values as environment variables (both raw and `MATRIX_` prefixed, because why not both).

```bash
# After expansion, each job gets:
#   os=ubuntu-latest, go=1.22, experimental=true
#   MATRIX_OS=UBUNTU_LATEST, MATRIX_GO_VERSION=1.22
```

### DEPENDENCY RESOLUTION (I FELT SMART FOR A MINUTE)

The other thing I needed was topological sorting. Jobs have `needs` dependencies:

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

This creates a DAG (Directed Acyclic Graph). I used **Kahn's algorithm** for topological sorting. The logic is pretty simple:

1. Start with jobs that have no dependencies
2. Run them
3. Once a job finishes, unlock the jobs that depended on it
4. Repeat untill everything's done

Circular dependencies are detected and reported. **No more infinite loops.** You're welcome.

### DOCKER SERVICES (THE DATABASE PROBLEM)

Integration tests usualy need databases. Postgres, Redis, MySQL... the usual suspects.

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

git-ci creates a dedicated bridge network, starts each service container with DNS aliases, and attaches the job container to the same network. So your code can just connect to `postgres:5432` or `redis:6379` without any config changes.

And cleanup is automatic. Because I'm not a monster.

```bash
$ git ci run --docker --job integration
# Postgres and Redis are running. Tests connect to them. Everything cleans up after.
```

### THE THINGS THAT WENT WRONG

Okay I'm gonna be honnest with you. Not everything worked perfectly.

- **Image resolution**: GitHub Actions uses `runs-on: ubuntu-latest`. But locally you don't have GitHub's runners. So I had to map those to Docker images (like `catthehacker/ubuntu:act-latest`). Finding the right mappings was anoying.
- **Step parsing**: GitHub Actions has like 5 different ways to define steps. The `uses:`, the `run:`, the `with:`, the `if:`, the `env:` at step level... it's a lot.
- **Shell escaping**: Passing commands through Docker exec with proper escaping is a nightmare. I probly got it 90% right.
- **Windows support**: Yeah... about that. Let's just say it works on Linux and macOS for now.

### THE VERDICT

So after all this work... does it actualy work?

**Yes. SURPRISINGLY yes.**

I use it every day now. Before pushing anything to GitHub, I run:

```bash
$ git ci run --docker
```

If it passes locally, it passes on CI. And I don't have to wait 5 minutes to find out I forgot a stupid semicolon.

Is it perfect? Hell no. There are edge cases I haven't hit yet. There are providers I don't support (Azure, Bitbucket, CircleCI, Travis... the list goes on). But it works for the things I need it to work for.

And honestly, even if it only saved me 20 minutes a day, that's 20 minutes I can spend building actual stuff instead of waiting for pipelines.

### LESSONS LEARNED

1. **Go is awesome for CLI tools.** Single binary, cross-compilation, fast startup. Can't beat it.
2. **YAML is evil.** Seriously, the amount of YAML edge cases in CI config files is insane. Indentation matters, anchors are confusing, and don't even get me started on multi-line strings.
3. **CI/CD is actualy complex.** When I started this project I thought "how hard can it be to read a YAML file and run commands?" The answer is: harder than you think (like, WAY harder).
4. **Local CI is a game changer.** The feedback loop goes from 5-10 minutes to 5-10 seconds. That changes how you work.

### WHATS NEXT

I'm planning to add more providers (CircleCI is next, then maybe Drone and Travis). Also want to add a K8s runner at some point because why not.

But for now, it does what I need it to do. And maybe it'll help you too.

Check it out on **[GitHub](https://github.com/sanix-darker/git-ci)** if you're curious. PRs welcome. Issues welcome. Rants about my code style? Also welcome, I guess.

---

**PS:** I initially mentiond this project at the end of my [HOW I FIXED GIT REFLOG](https://sanixdk.xyz/blogs/how-i-fixed-git-reflog) post, back when it was just an idea. Now it's actualy workin and i use it daily. Funny how these things evolve when you stop just thinking and start coding.

### FURTHER READING

- **[git-ci on GitHub](https://github.com/sanix-darker/git-ci)** - The actual repo
- **[GitHub Actions Docs](https://docs.github.com/en/actions)** - What I had to reverse-engineer
- **[Kahn's Algorithm](https://en.wikipedia.org/wiki/Topological_sorting#Kahn's_algorithm)** - The algorithm behind dependency resolution
- **[catthehacker/ubuntu](https://github.com/catthehacker/docker_images)** - Docker images that mimic GitHub runners

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
