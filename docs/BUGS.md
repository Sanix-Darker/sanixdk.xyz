# BUGS - sanixdk.xyz

Audit findings are retained here as a compact resolution log. Current
behavior is defined in [`FEATURE_MATRIX.md`](./FEATURE_MATRIX.md).

| Status | Meaning |
| --- | --- |
| 🔴 | Open |
| 🟠 | Fix in progress |
| 🟢 | Fixed and covered by the E2E harness |
| ⚫ | Accepted behavior with a documented reason |

## Phase 1 findings

| ID | Area | Finding | Status | Resolution |
| --- | --- | --- | --- | --- |
| BG-01 | Assets | Clean builds omitted `content/assets/`. | 🟢 | Recursively copy the complete asset tree. |
| BG-02 | Minifier | Newline stripping could break JavaScript. | 🟢 | Restrict minification to CSS. |
| BG-03 | Metadata head | Fixed stack output could overflow and leaked memory. | 🟢 | Allocate bounded output and free both buffers. |
| BG-04 | Metadata | Tags and read time were not retained consistently. | 🟢 | Parse all fields into the stored entry. |
| BG-05 | Terminal | Two listeners duplicated command output. | 🟢 | Use one `keydown` listener. |
| BG-06 | Terminal | Echo case differed from command lookup. | 🟢 | Echo the normalized, escaped command. |
| BG-07 | Live data | Failed fetches left permanent loading text. | 🟢 | Render a visible retry hint on invalid or failed responses. |
| BG-08 | Search | Search code existed without a usable input. | 🟢 | Ship title/tag filtering, result count, empty state, and Escape reset. |
| BG-09 | Templates | Substring path checks could misclassify pages. | 🟢 | Match exact `content/blogs/` and `content/projects/` prefixes. |
| BG-10 | Images | Article images lacked lazy loading. | 🟢 | Add lazy loading and async decoding in the renderer and raw hero. |
| BG-11 | Motion | Decorative animation ignored reduced-motion preferences. | 🟢 | Add a global reduced-motion override. |
| BG-12 | Focus | Keyboard focus was not visible. | 🟢 | Add `:focus-visible` outlines. |
| BG-13 | Navigation | Pages had no skip-to-content route. | 🟢 | Add shared skip links and a main target. |
| BG-14 | Docker | Several brittle copy statements could omit files. | 🟢 | Copy the generated tree recursively in one step. |
| BG-15 | Makefile | `serve` called a nonexistent binary. | 🟢 | Call `./builder serve`. |
| BG-16 | Makefile | `prod` used the nonexistent `git update` command. | 🟢 | Pull with rebase/autostash, then build. |
| BG-17 | Robots | Inline comments made crawler directives invalid. | 🟢 | Keep only valid, explicit directives. |
| BG-18 | Projects | The projects page was an unstyled raw list. | 🟢 | Add a four-node CSS-only project reel. |
| BG-19 | README | Setup referenced nonexistent `make compose`. | 🟢 | Document `make build`, `make e2e`, and `make up`. |
| BG-20 | Assets | The stylesheet source could be lost after generation. | 🟢 | Keep the source under `content/assets/` and ignore `public/`. |
| BG-21 | Paths | `replaceString` used an unbounded 1000-byte buffer. | 🟢 | Remove it and derive output paths with checked `snprintf`. |
| BG-22 | Builder | Missing required templates terminate the build. | ⚫ | Fail-fast is intentional for incomplete generated sites. |
| BG-23 | Blog layout | Wide screens still showed a single narrow card column. | 🟢 | Add a two-column breakpoint at 1000 px. |
| BG-24 | Nginx | `gzip_static` expected files the build never produced. | 🟢 | Use dynamic gzip. |
| BG-25 | Build | Header/footer injection temporarily mutates Markdown. | 🟢 | `make build` restores tracked content after generation. |
| BG-26 | Terminal | An empty `staticSkills` loop did nothing. | 🟢 | Remove the dead loop. |
| BG-27 | Metadata head | Template placeholder drift was silent. | 🟢 | Warn unless exactly nine dynamic slots are present. |
| BG-28 | CLI | The unimplemented `serve` command returned success. | 🟢 | Return failure with `make up` guidance. |

## Current status

- Open findings: 0
- Accepted findings: 1 (`BG-22`)
- Automated regression entry point: `make e2e`
