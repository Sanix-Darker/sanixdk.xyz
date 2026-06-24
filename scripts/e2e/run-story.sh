#!/usr/bin/env bash
# run-story.sh <id> — run a single user-story test and append a TEST_LOG.md row.
# Exit code: 0 on PASS / SKIP, 1 on FAIL.

set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
# shellcheck disable=SC1091
source "${SCRIPT_DIR}/lib.sh"

if [[ $# -lt 1 ]]; then
    echo "usage: $0 <story-id>  e.g. $0 F-09" >&2
    exit 2
fi

ID="$1"
STORY="$(lookup_story "${ID}")"
if [[ -z "${STORY}" || "${STORY}" == "null" ]]; then
    echo "run-story: unknown id ${ID}" >&2
    exit 2
fi

TYPE="$(printf '%s' "${STORY}" | jq -r '.type')"
SECTION="$(printf '%s' "${STORY}" | jq -r '.section')"
PROMPT="$(printf '%s' "${STORY}" | jq -r '.prompt')"
NOTES="$(printf '%s' "${STORY}" | jq -r '.notes // ""')"

started_ms="$(date +%s%N)"
status="❌ FAIL"
result_notes="${NOTES}"

# --- skip: report SKIP, do nothing else ------------------------------------
if [[ "${TYPE}" == "skip" ]]; then
    status="⏭️ SKIP"
    result_notes="${NOTES} (skipped)"
fi

# --- build: eval the prompt as a bash script; capture exit + stdout --------
capture_path="${CAPTURE_DIR}/${ID}.log"
if [[ "${TYPE}" == "build" ]]; then
    if ( cd "${REPO_ROOT}" && eval "${PROMPT}" ) >"${capture_path}" 2>&1; then
        status="✅ PASS"
        result_notes="exit 0 — ${NOTES}"
    else
        status="❌ FAIL"
        result_notes="exit non-zero — ${NOTES}"
    fi
fi

# --- browser: prefer shell capture, otherwise fall back to a JS-free
#     curl+grep predicate (JSON `.fallback.checks`). SKIP only when
#     neither yield is available — i.e. the story has no `fallback` block.
if [[ "${TYPE}" == "browser" ]]; then
    capture_browser_path="${CAPTURE_DIR}/${ID}.json"
    if [[ -s "${capture_browser_path}" ]] && [[ "$(jq -r '.pass // false' "${capture_browser_path}" 2>/dev/null)" == "true" ]]; then
        status="✅ PASS"
        result_notes="${NOTES} — observed: $(jq -r '.observed // \"\"' "${capture_browser_path}" 2>/dev/null | head -c 160)"
    elif [[ -s "${capture_browser_path}" ]]; then
        status="❌ FAIL"
        result_notes="${NOTES} — failure: $(jq -r '.failure // \"unknown\"' "${capture_browser_path}" 2>/dev/null | head -c 160)"
    elif has_fallback "${ID}"; then
        fb_result="$(fallback_eval "${ID}")"
        fb_rc=$?
        if [[ ${fb_rc} -eq 0 ]]; then
            status="✅ PASS"
            result_notes="${NOTES} — fallback:$(printf '%s' "${fb_result}" | head -c 220)"
        else
            status="❌ FAIL"
            result_notes="${NOTES} — fallback_failed(rc=${fb_rc}):$(printf '%s' "${fb_result}" | head -c 220)"
        fi
    else
        status="⏭️ SKIP"
        result_notes="${NOTES} — no browser capture, no fallback"
    fi
fi

ended_ms="$(date +%s%N)"
duration_ms=$(( (ended_ms - started_ms) / 1000000 ))
duration_s=$(printf '%.1fs' "$(echo "${duration_ms}/1000" | bc -l)")

log_line "${ID}" "${SECTION}" "${TYPE}" "${status}" "${duration_s}" "${result_notes}"

if [[ "${status}" == "❌ FAIL" ]]; then
    exit 1
fi
exit 0
