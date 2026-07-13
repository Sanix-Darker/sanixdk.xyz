#!/usr/bin/env bash
# run.sh — orchestrator for the browser-use E2E harness.
#
# Usage:
#   ./scripts/e2e/run.sh           # run ALL stories (builds + browser + skips)
#   ./scripts/e2e/run.sh F-09      # run a single story by id
#   ./scripts/e2e/run.sh browser   # run only browser-testable stories
#   ./scripts/e2e/run.sh build     # run only build-testable stories
#
# Exit codes:
#   0  — all run stories PASS or SKIP
#   1  — at least one story FAILED

set -u
set -o pipefail
# We deliberately do NOT set -e — a single broken story shouldn't abort
# the loop. Counter bookkeeping reads `rc=$?` after each story runner;
# the tally itself comes from the tail-of-log grep (no need for `set -e`
# toggle dance inside the loop).
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
# shellcheck disable=SC1091
source "${SCRIPT_DIR}/lib.sh"

TARGET="${1:-ALL}"
FILTER="${TARGET^^}"  # uppercase filter

# 1. ---------- Build site ----------------------------------------------------
echo "==> clean build + git restore"
if ! ( cd "${REPO_ROOT}" && make build >"${CAPTURE_DIR}/build.log" 2>&1 ); then
    cat "${CAPTURE_DIR}/build.log" >&2
    exit 2
fi

# 2. ---------- Reset stale browser captures (BEFORE server starts) ---------
# Stale /tmp/e2e/*.json from a previous run could leak into today's verdict.
# Wipe here — server's httpd.log is freshly created via `>` redirect,
# so we don't touch it (avoids the race vs the shortly-spawned server).
# Opt-out: E2E_NO_WIPE=1 keeps existing *.json in place. Use case: the
# parent (a browser-use-agent-driven run) has already staged captures
# before invoking run.sh, so wiping them would defeat the handoff.
# Any non-empty, non-"0" value of E2E_NO_WIPE opts out. The strict
# `!= "1"` would surprise `E2E_NO_WIPE=yes`; permissive truthy check covers
# `1/yes/true/on/anything-non-empty-not-0`.
if [[ -n "${E2E_NO_WIPE:-}" && "${E2E_NO_WIPE}" != "0" ]]; then
    echo "==> keeping pre-existing browser captures (E2E_NO_WIPE=${E2E_NO_WIPE})"
else
    echo "==> clearing stale browser captures"
    find "${CAPTURE_DIR}" -type f -name '*.json' -delete 2>/dev/null || true
fi

# 3. ---------- Start HTTP server (fail-fast on bind failure) ----------------
echo "==> starting http.server on :${PORT}"
start_server "${PORT}" "public" || {
    echo "FATAL: server bind failed; aborting run." >&2
    exit 2
}

# Cleanup on exit (interrupts as well).
cleanup() { stop_server "${PORT}"; }
trap cleanup EXIT INT TERM

# 4. ---------- Init TEST_LOG.md ----------------------------------------------
log_init "target=${TARGET}"

# 4. ---------- Iterate stories -----------------------------------------------
total=0
pass=0
fail=0
skip=0
ids=$(jq -r '.[].id' "${STORIES_JSON}")

for id in ${ids}; do
    type="$(type_for "${id}")"
    case "${FILTER}" in
        ALL)        keep=1 ;;
        BROWSER)    [[ "${type}" == "browser" ]] && keep=1 || keep=0 ;;
        BUILD)      [[ "${type}" == "build" ]]   && keep=1 || keep=0 ;;
        SKIP)       [[ "${type}" == "skip" ]]    && keep=1 || keep=0 ;;
        *)          [[ "${id}" == "${TARGET}" ]] && keep=1 || keep=0 ;;
    esac
    [[ "${keep}" == "1" ]] || continue

    total=$((total+1))
    echo "==> ${id} (${type})"
    before_lines=$(wc -l < "${LOG_FILE}" 2>/dev/null || echo 0)
    # set -e is intentionally off at script top — no per-iter toggle.
    "${SCRIPT_DIR}/run-story.sh" "${id}"
    rc=$?
    after_lines=$(wc -l < "${LOG_FILE}" 2>/dev/null || echo 0)

    # Increment counters based on the row we just wrote.
    if (( after_lines > before_lines )); then
        last_row=$(tail -n 1 "${LOG_FILE}")
        if   [[ "${last_row}" == *"✅ PASS"* ]]; then pass=$((pass+1))
        elif [[ "${last_row}" == *"❌ FAIL"* ]]; then fail=$((fail+1))
        elif [[ "${last_row}" == *"⏭️ SKIP"*  ]]; then skip=$((skip+1))
        fi
    fi

    # Surface failure immediately for human-in-the-loop, but keep going so
    # we capture the full log on CI artifacts.
    if [[ ${rc} -ne 0 ]]; then
        echo "    → FAILED (rc=${rc})"
    fi
done

# 5. ---------- Summary line ---------------------------------------------------
{
    echo
    echo "_Summary:_ total=${total} pass=${pass} fail=${fail} skip=${skip}"
} >> "${LOG_FILE}"

echo
echo "=== E2E summary: total=${total} pass=${pass} fail=${fail} skip=${skip} ==="

if (( fail > 0 )); then exit 1; fi
exit 0
