# FEATURE MATRIX - sanixdk.xyz

This is the canonical feature inventory for the C static-site generator.

| Status | Meaning |
| --- | --- |
| ✅ | Implemented and covered by the E2E harness |
| 🟡 | Implemented with an external or intentional caveat |
| ⚪ | Not implemented |
| ❌ | Broken |

## A. Build pipeline

| ID | Scope | Contract | Status | Evidence or caveat |
| --- | --- | --- | --- | --- |
| B-01 | Compile | `make compile` builds `builder` with `gcc -Wall`. | ✅ | CI and `make e2e`. |
| B-02 | CLI | Missing, unknown, and unimplemented commands fail clearly. | ✅ | E2E checks all three failure paths. |
| B-03 | Blog list | Metadata produces exactly 25 complete blog cards. | ✅ | Exact-count HTTP fallback. |
| B-04 | Component injection | Top-level pages and nested post pages receive the correct templates. | ✅ | Path checks use exact content prefixes. |
| B-05 | Markdown rendering | Every content Markdown file produces HTML under `public/`. | ✅ | md4c build check. |
| B-06 | Static assets | Styles, favicon, robots file, and article assets are copied. | ✅ | Clean-build file checks. |
| B-07 | Minification | Only CSS is flattened; scripts remain untouched. | ✅ | Extension-gated minifier. |
| B-08 | RSS | RSS contains exactly 25 newest-first items and complete channel metadata. | ✅ | Final entry is flushed at EOF. |
| B-09 | Blog metadata head | Nine dynamic values populate each post head safely. | ✅ | Placeholder-count warning and bounded heap output. |
| B-10 | Metadata parser | All fields are bounded, terminated, and retained. | ✅ | Parser coverage in generated cards/feed. |
| B-11 | Output paths | Markdown paths are converted with checked buffers. | ✅ | Unbounded `replaceString` removed. |
| B-12 | Required templates | Missing templates stop the build. | ✅ | Intentional fail-fast build policy. |
| B-13 | Reproducibility | A build leaves tracked Markdown sources unchanged. | ✅ | `make build` restores `content/`. |

## B. Front-end

| ID | Scope | Contract | Status | Evidence or caveat |
| --- | --- | --- | --- | --- |
| F-01 | Home style | Terminal presentation and typing heading render from CSS. | ✅ | Generated home checks. |
| F-02 | Terminal input | One escaped, normalized command runs per Enter press. | ✅ | One `keydown` listener. |
| F-03 | Clear command | `clear` empties terminal output. | ✅ | Browser/fallback check. |
| F-04 | GitHub data | Profile and repository data load with a visible failure state. | 🟡 | GitHub API availability is external. |
| F-05 | WakaTime data | Profile README stats load with a visible failure state. | 🟡 | GitHub raw content and WakaTime markup are external. |
| F-06 | Command case | Echo and lookup use the same lowercase value. | ✅ | Uppercase-input check. |
| F-07 | Navigation | Home, blogs, projects, and about are linked globally. | ✅ | Four distinct link checks. |
| F-08 | Blog listing | All 25 cards render, with two columns on wide screens. | ✅ | Exact card count and CSS breakpoint. |
| F-09 | Search | Title/tag search, count, empty state, and Escape reset work. | ✅ | Footer search controller. |
| F-10 | About | Static about page renders. | ✅ | Generated-page check. |
| F-11 | Projects | Four lightweight project nodes render without runtime work. | ✅ | Exact node and destination checks. |
| F-12 | Table of contents | Post headings populate a nested TOC. | ✅ | Static controller plus browser story. |
| F-13 | Syntax highlighting | Code blocks use highlight.js and the GitHub-dark theme. | 🟡 | Rendering enhancement depends on a CDN. |
| F-14 | Comments | Giscus loads discussions by pathname. | 🟡 | Runtime service is external. |
| F-15 | Social metadata | Post title, URL, descriptions, and images populate share tags. | ✅ | Generated-head checks. |
| F-16 | Image loading | Article images use lazy loading and async decoding. | ✅ | Renderer and raw-image checks. |
| F-17 | Reduced motion | Decorative motion stops for reduced-motion users. | ✅ | CSS media-query check. |
| F-18 | Focus | Keyboard focus has a visible outline. | ✅ | CSS selector check. |
| F-19 | Skip link | Shared headers link to `<main id="main">`. | ✅ | Home and post checks. |

## C. Operations

| ID | Scope | Contract | Status | Evidence or caveat |
| --- | --- | --- | --- | --- |
| O-01 | Docker image | The complete generated tree is copied into nginx. | ✅ | Multi-stage image uses one recursive copy. |
| O-02 | Nginx | Dynamic gzip works without precompressed files. | ✅ | Config assertion rejects `gzip_static`. |
| O-03 | Compose | `make up` and `make down` manage port 3003. | ✅ | Compose configuration. |
| O-04 | Makefile | All documented targets use valid commands. | ✅ | E2E command assertions. |
| O-05 | CI | Pushes and pull requests run `make e2e`. | ✅ | Workflow trigger and command checks. |
| O-06 | Robots | Crawler directives contain no inline directive comments. | ✅ | Build assertion. |
| O-07 | Git tracking | Generated HTML, `public/`, and the builder are ignored. | ✅ | `.gitignore` and tracked-file checks. |
| O-08 | README | Local build, E2E, and container commands are current. | ✅ | README command assertions. |

## Remaining caveats

- Live GitHub and WakaTime data are best effort and expose a visible failure
  state when unavailable.
- Highlighting and comments rely on third-party browser services.
- `serve` remains intentionally unimplemented; local serving uses `make up`.
