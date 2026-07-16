---
map: 1
scope: .
children: [lib/MAP.md]
uses: [lib]
fidelity: syntax
api_hash: 6d9a418204d06755
kids_hash: 03ba36ebabc792ea
tokens: ~234
stamped: 2026-07-16T20:18:04Z
---
# .

<!-- radar:slot purpose max=160 -->
Builds sanixdk.xyz from Markdown with a small C static-site generator, project pages, and shell-based end-to-end checks.
<!-- /radar:slot -->

## Routes
- lib/MAP.md · `lib/lib.c#buildComponentsIntoMarkdownsFiles`

## API
`main.c`
- `int main(int argc, char** argv)`
`scripts/e2e/lib.sh`
- `log_line()`
- `server_running()`
- `wait_for_server()`
- `start_server()`
- `stop_server()`
- `now_iso()`
- `header()`
- `has_fallback()`
- `fallback_fetch()`
- `fallback_eval()`
- `lookup_story()`
- `type_for()`
- `notes_for()`
- `section_for()`
- `log_init()`
`scripts/e2e/run.sh`
- `cleanup()`

## Tests
- `content/blogs/how-i-accidentally-created-the-fastest-csv-parser-ever-made.md`
- `content/blogs/unittest-any-codebase-with-comments.md`
