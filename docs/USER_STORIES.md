# USER STORIES - sanixdk.xyz

Each story maps to the matching ID in
[`FEATURE_MATRIX.md`](./FEATURE_MATRIX.md). The contracts below describe the
current intended behavior and are exercised by `make e2e`.

## Personas

- **Visitor (V)** reads and navigates the generated site.
- **Author (A)** adds or edits Markdown content.
- **Maintainer (M)** builds, tests, or deploys the site.

## A. Build pipeline

**B-01 / B-02 - Builder CLI**

- As an M, `make compile` should build `./builder` with no compiler warnings.
- As an M, missing arguments, unknown commands, and the unimplemented `serve`
  command should fail with useful guidance.

**B-03 - Blog index**

- As a V, `/blogs/` should show one card for each of the 25 entries in
  `content/metadatas.txt`.
- Each card should include its image, permalink, publication date, read time,
  and individual tag pills.

**B-04 / B-05 - Components and rendering**

- As an A, top-level Markdown should receive the shared header and footer.
- Blog posts and nested project pages should receive the metadata-aware blog
  header, comment footer, and shared footer.
- As an M, each content Markdown file should produce a matching HTML file
  under `public/`.

**B-06 / B-07 - Assets and minification**

- As a V, `/assets/style.css`, the favicon, and content assets should be
  present after a clean build.
- CSS may be flattened. JavaScript and inline scripts must keep their
  line structure.

**B-08 - RSS**

- As a V, `/feed.xml` should be valid RSS 2.0 with exactly 25 items, newest
  first.
- Channel metadata and item URLs should be complete, absolute, and XML
  escaped.
- The final metadata entry must be emitted even when the input file has no
  trailing blank line.

**B-09 / B-10 - Metadata**

- As an M, each blog head should populate exactly nine dynamic values:
  the HTML title, two canonical post URLs, four title or description values,
  and two image values. `twitter:card` remains the static
  `summary_large_image` value.
- As an A, each metadata entry should preserve path, link, title, image, date,
  tags, time, and filename using bounded, null-terminated fields.

**B-11 / B-12 / B-13 - Reliability**

- As an M, content-to-public path conversion should reject truncation instead
  of writing past a fixed buffer.
- Missing required templates should stop the build with a non-zero exit.
- A completed build should leave tracked files under `content/` unchanged.

## B. Front-end

**F-01 - Home presentation**

- As a V, the home page should keep its lightweight terminal style and typing
  heading without loading a client framework.

**F-02 / F-03 / F-06 - Terminal**

- As a V, Enter should execute one command through one `keydown` listener.
- Command echoes should be HTML escaped and normalized to the same lowercase
  value used for lookup.
- `clear` should empty the output.
- `help` should list local and live-data commands.

**F-04 / F-05 - Live data**

- As a V, GitHub and WakaTime data should load when their external endpoints
  are available.
- If either endpoint fails or returns an invalid payload, its section should
  replace the loading copy with a visible retry hint.

**F-07 - Navigation**

- As a V, every shared header should expose home, blogs, projects, and about
  links.

**F-08 / F-09 - Blog discovery**

- As a V, `/blogs/` should show all 25 cards, with one card per row at every
  viewport width.
- Typing in search should filter by title and tags, update an accessible
  counter, show an empty state when needed, and reset on Escape.

**F-10 / F-11 - Static pages**

- As a V, `/about` should render the author profile and links.
- As a V, `/projects` should show ten lightweight nodes: four deployed projects
  and six public repositories, without screenshots, JavaScript, or runtime
  fetches.

**F-12 / F-13 / F-14 - Blog enhancements**

- Blog posts should use a slightly wider reading column than index pages so
  data tables have room on desktop while retaining horizontal overflow on
  small screens.
- As a V, posts with `#toc-container` should receive a generated nested table
  of contents whose links remain white in idle and hover states.
- Code blocks should use the GitHub-dark highlight.js theme when its CDN is
  reachable.
- Giscus comments should map discussions by pathname.

**F-15 / F-16 - Sharing and images**

- As a crawler, each post should expose populated Open Graph and Twitter
  metadata.
- As a V, Markdown images and raw article images should use lazy loading and
  asynchronous decoding where appropriate.

**F-17 / F-18 / F-19 - Accessibility**

- Reduced-motion preferences should disable decorative animation and
  transitions.
- Keyboard focus should remain visible.
- The first focusable link should skip directly to `<main id="main">`.

## C. Operations

**O-01 / O-02 / O-03 - Container serving**

- As an M, the multi-stage image should copy the complete generated `public/`
  tree into nginx.
- Nginx should use dynamic gzip and should not depend on absent `.gz` files.
- `make up` should expose the site at `localhost:3003`; `make down` should stop
  it.

**O-04 / O-08 - Local commands**

- As an M, the Makefile and README should document only commands that exist.
- `prod` should update with a rebase and then run a clean build.

**O-05 - CI**

- As a contributor, pushes and pull requests should run the complete E2E
  harness and upload `docs/TEST_LOG.md` as an artifact.

**O-06 / O-07 - Generated content**

- As a crawler, `robots.txt` should contain valid directives with no inline
  comments inside `Allow` or `Disallow` values.
- As a contributor, generated HTML and `public/` should remain unversioned.

## Passing a story

A story passes when:

1. Its generated behavior matches this document.
2. Its matrix row is green or has an explicit external-service caveat.
3. `make e2e` records PASS, or SKIP only for a documented external/manual
   dependency.
