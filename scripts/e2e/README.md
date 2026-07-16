# E2E Browser Use Harness

Drives every user story in `docs/USER_STORIES.md` through Chrome via the
`browser-use` agent and writes a per-story PASS/FAIL row to
`docs/TEST_LOG.md`.

## Files

- `run.sh` - orchestrator. `./run.sh` runs every story; `./run.sh F-09`
  runs one.
- `run-story.sh` - single-story runner; reads `stories.json` by id and
  evaluates the test.
- `stories.json` - machine-readable manifest (one entry per user story).
- `lib.sh` - shared helpers (server lifecycle, time, JSON helpers).

## Lifecycle

```
make compile
./builder build         # produces ./public/
./scripts/e2e/run.sh    # starts server on :8123, runs every story, fills log
```

Per-browser-story chrome captures are written to `/tmp/e2e/<ID>.json`
by the browser-use agent (see `stories.json`).

## Exit codes

- `0` - all run stories PASS or SKIP.
- `1` - at least one story FAILED.

## CI integration

`make e2e` is the canonical pipeline. The harness performs one clean build
before iterating the stories; `make build` clears generated `public/` output
first, so repeated runs cannot duplicate page headers. CI uploads
`docs/TEST_LOG.md` as the `e2e-test-log` artifact via
`actions/upload-artifact@v7`; the upload step runs even when the
harness exits non-zero, so a partial log is still preserved for audit.

## Browser-use handoff

For browser-testable stories (type=`browser`), run.sh reads each per-story
result from `/tmp/e2e/<ID>.json` with shape:

```json
{ "pass": <bool>, "observed": "<text>", "failure": "<text?>" }
```

If the file is missing, the row is recorded as `⏭️ SKIP`. To inject fresh
captures (e.g. from a parent browser-use call) without the orchestrator
clobbering them, set:

```bash
E2E_NO_WIPE=1 ./scripts/e2e/run.sh
```

In that mode the orchestrator skips the stale-`*.json` wipe at start,
keeping anything that was already staged in `/tmp/e2e/` for the iterated
stories. Any non-empty, non-`0` value opts out (`1`, `yes`, `true`, ...).
