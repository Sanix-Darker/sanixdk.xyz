# USER STORIES — sanixdk.xyz

Each user story is paired with an ID matching the row in
[`FEATURE_MATRIX.md`](./FEATURE_MATRIX.md). "Expected" describes the
post-fix behaviour the audit considers correct, not necessarily what the
code does today. Section headings are identical to the matrix.

---

## Persona recap
- **Visitor (V)** — anonymous reader browsing the site.
- **Author (A)** — site owner pushing a new blog post or fix.
- **Maintainer (M)** — someone running `make build` / `make up`.

---

## A · Build Pipeline

**B-01 / B-02 — Compile & CLI dispatch**
- *As an M, when I run `make compile`, the `builder` binary should compile
  silently (`gcc -pipe -Wall -s`) with zero warnings and be executable.*
- *As an M, `./builder build` should produce a complete `./public/` tree;
  `./builder serve` should print `NOT IMPLEMENTED YET !` (no plan to
  implement); any other argument should print `? Usage: ./builder
  build|serve` to stdout and exit non-zero.*

**B-03 — Metadata → blog-list HTML**
- *As a V, when I visit `/blogs/`, the page should show every entry in
  `content/metadatas.txt` as a card with:*
  - *a triangle-shaped background using the per-blog `image` URL as the
    `--bg-image` custom property,*
  - *the blog title linking to its permalink,*
  - *the publication date and the read-time string,*
  - *every tag rendered as its own pill (comma-separated in metadata).*
- Expected: 26 cards render today.

**B-04 — Component injection**
- *As an M, when I run a clean build, every `.md` under content should
  end up wrapped by the right header + footer for its directory:*
  - *Top-level (e.g. `index.md`, `about.md`) → `header.md` + content + `footer.md`.*
  - *Blog posts → `blog-header.md` (with template `%s` placeholders filled
    by per-blog metadata) + content + `comment-footer.md` + `footer.md`.*
  - *Any `.md` under `projects/` → header + content + footer (TODO: explicit
    projects support).*

**B-05 — Markdown → HTML**
- *As an M, after a build, every `.md` should have a corresponding
  `.html` in `public/` with the same path layout (`content/blogs/foo.md` →
  `public/blogs/foo.html`).*
- *As a V, the rendered HTML should match standard CommonMark (md4c
  parses), with no time spent on wrapping JS bundles.*

**B-06 — Static asset copy**
- *As an M, when I run `make build`, public should contain a full
  `assets/` directory mirror of `content/assets/` (recursive cp).*
- *As a V, every page's `<link rel="stylesheet" href="/assets/style.css">`
  should resolve with HTTP 200 (after **B-06** is fixed).*

**B-07 — Minification**
- Expected: only CSS is minified (newline/whitespace collapse outside
  rule blocks). JS is **left untouched** — script blocks in
  `comment-footer.md` rely on multi-line syntax to ship correctly.
- Today: minifier rewrites both `.js` and `.css` by removing every
  trailing `\n\r` → JavaScript sources shipped in `<script>` blocks are
  flattened and any parser/POSIX-edge becomes a SyntaxError.

**B-08 — RSS 2.0 feed**
- *As a V, hitting `/feed.xml` should return valid RSS 2.0 with the
  channel meta (`title`, `link`, `description`, `language`,
  `lastBuildDate`, `ttl=60`) and one `<item>` per blog post, newest
  first by `date`. Entities `<>&'\"` must be escaped. Each `<link>` and
  `<guid>` must resolve to the canonical absolute URL.*

**B-09 — Per-blog meta header**
- *As an M, after a build, every blog HTML head should show the
  per-blog metadata interpolated into 9 placeholders of
  `blog-header.md`:*
  - *title,*
  - *2× permalink usages (og:url, twitter:url),*
  - *2× title-tag usages (og:title, twitter:title),*
  - *2× image usages (og:image, twitter:card),*
  - *1× title used in <title> element.*
- Expected: zero memory leak, zero stack buffer overflow potential.

**B-10 — Metadata parser**
- *As an M, every blank-line-bounded entry in `metadata.txt` should
  populate ONE `EntryMap` slot with `path`, `link`, `title`, `image`,
  `date`, **`tags`**, **`time`**, plus the `filename` key.*
- Today: `tags` and `time` are written into `currentEntry` but never
  copied into `entryMap[*].entry`, so `entry.tags` and `entry.time`
  fields are always empty in lookups.

**B-11 / B-12 / B-13** — robustness.

---

## B · Front-end UX

**F-01 — Home animation**
- *As a V, on first paint I should see the dark terminal-themed home
  with the H1 "HI, AM SANIX" rendered with a `#` prefix and a typing
  animation. Headings should pick up the funny `/*...*/`, `// `, `~ `,
  `--- ` decorative prefixes from CSS.*

**F-02 / F-06 — Terminal input**
- *As a V, focusing the input and pressing Enter once should:*
  - *echo the typed command prefixed by `you@sanixdk:~$`,*
  - *print `command not found: X` if X is not registered,*
  - *clear the input field.*
- *As a V, hitting Enter on a real command should call the async
  handler **exactly once** — no duplicate output.*
- Today: handlers pile up because there are two
  `addEventListener('keypress', …)` blocks running without
  `removeEventListener` succeeding on the first one, so the second
  `addEventListener` is added in addition to the first.

**F-03 — `clear`**
- *As a V, after typing `clear` and Enter, `#terminalOutput` should be
  empty.*

**F-04 / F-05 — Live data**
- *As a V, on DOMContentLoaded the GitHub and WakaTime fetchers start
  with "Loading …" placeholders, then swap in real numbers once the
  JSON / WakaTime-section parse succeeds.*
- Failure mode today: any API hiccup leaves the persistent "Loading
  GitHub data…" copy because there's no error replacement.

**F-08 — Blogs list**
- *As a V, all 26 cards visible at `/blogs/`, newest first, clickable
  with smooth hover transitions.*

**F-09 — Search**
- *Decision required:*
  - **Revive**: enable a search input on `/blogs/`, filter cards live
    on `input`, support both title and tag search, show "no posts
    found matching your search" empty state.
  - **Remove**: delete the dead JS in `footer.md`, delete the dead
    comment in `lib.c`, delete the empty `search-script.md`.

**F-10 — About**
- *As a V, hitting `/about` returns a small page with the developer's
  dev-stack image and pointer back to a Substack article.*

**F-11 — Projects**
- *As a V, hitting `/projects` returns a card grid (post-fix) of all
  the projects with name, description, language tag.*

**F-12 — TOC on blog posts**
- *As a V, on any blog post page that contains a
  `<div id="toc-container">`, the JS should populate it with a nested
  UL pointing at the headings via anchor links.*

**F-13 — Syntax highlighting**
- *As a V, every `<pre><code class="language-*">` block should be
  highlighted using the github-dark theme after the network loads
  `highlight.min.js@11.9.0`.*

**F-14 — Giscus comments**
- *As a V, the `[<< blogs]` backlink appears, then the giscus iframe
  loads at the bottom of the post. Discussions should map to pathname
  so each post has a unique thread.*

**F-15 — Per-post meta tags**
- *As a V (or a search engine), embed cards and twitter:card share
  previews should embed the post's title and cover image.*

**F-16 — Lazy-loaded images**
- *As a V on a slow connection, hero images should not block first
  paint; everything except the LCP candidate should be `loading="lazy"`
  with `decoding="async"`. Apply throughout blog bodies and code blocks
  that include screenshots.*

**F-17 — Reduced-motion support**
- *As a V with `prefers-reduced-motion: reduce`, decorative animations
  and transitions should collapse to ≤ 0 ms. Critical-UI motion
  (typing) should continue, or be replaced by an instant-render
  fallback.*

**F-18 — Focus rings**
- *As a keyboard V, focus indicators on links / form inputs should
  remain visible — never `outline: 0` without an alternative.*

**F-19 — Skip-to-content link**
- *As a screen-reader V on the home page, the first focusable element
  should jump me past the navigation block to the main content.*

---

## C · Operational

**O-01 — Dockerfile**
- *As an M, `make docker-build` should yield an image that runs nginx
  on 80 and serves a fully-styled site even on a clean clone.*

**O-02 — nginx**
- *As a V, blog pages should be served gzip-precompressed where the
  build emits `.gz` siblings. If we ship without precompression, drop
  `gzip_static on`.*

**O-04 / O-08** — Makefile + README consistency.

**O-05 — CI**
- *On each push, the workflow installs gcc + make, compiles, and
  builds. Build artefacts are not deployed (intentional).*

**O-06 — robots.txt**
- *`robots.txt` should be valid: each `Disallow:` directive takes a
  path or nothing; comments must be on their own lines.*

---

## Standards for "PASS"

A user story is **PASS** when:
1. The behaviour matches the *Expected* line.
2. The relevant row in `FEATURE_MATRIX.md` shows ✅.
3. An E2E browser run recorded the behaviour in `docs/TEST_LOG.md`.

Failure to satisfy any of the above means **PASS = false**.
