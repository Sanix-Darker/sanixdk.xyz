#!/usr/bin/env bash
# lib.sh - shared helpers for the browser-use E2E harness.
# Source from run.sh / run-story.sh. Never executed directly.

# Helpers only - do NOT bring `set -e`. Callers decide whether to
# fail-fast or carry on. We DO enable -u and -o pipefail so callers
# inherit strictness on undefined vars + pipeline failures.
set -u
set -o pipefail

E2E_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${E2E_DIR}/../.." && pwd)"
LOG_FILE="${REPO_ROOT}/docs/TEST_LOG.md"
CAPTURE_DIR="${E2E_CAPTURE_DIR:-/tmp/e2e}"
STORIES_JSON="${E2E_STORIES_JSON:-${E2E_DIR}/stories.json}"
PORT="${E2E_PORT:-8123}"

mkdir -p "${CAPTURE_DIR}"

# log_line <id> <section> <type> <status_emoji_word> <duration_s> <notes...>
# Writes a single TEST_LOG.md row, NFC-safe markdown pipe characters.
log_line() {
    local id="$1" section="$2" type="$3" status="$4" duration="$5"; shift 5
    local notes="${*:-}"
    # Replace any literal '|' inside notes so the row stays single-line.
    notes="${notes//|/\\|}"
    printf '| %s | %s | %s | %s | %s | %s |\n' \
        "${id}" "${section}" "${type}" "${status}" "${duration}" "${notes}" \
        >> "${LOG_FILE}"
}

# server_running <port>: returns 0 if a python http.server is bound to <port>.
# Uses both lsof/ss port binding check + pgrep on a deterministic pattern
# (no GNU-only \b word-boundary; portable across Linux/macOS/BusyBox).
server_running() {
    local port="$1"
    if ss -ltn 2>/dev/null | grep -qE ":${port}[[:space:]]"; then return 0; fi
    if command -v lsof >/dev/null 2>&1 && lsof -iTCP:"${port}" -sTCP:LISTEN >/dev/null 2>&1; then
        return 0
    fi
    pgrep -f "http.server ${port}" >/dev/null 2>&1
}

# wait_for_server <port> [timeout_s=8]: busy-loops curl + backoff until it
# answers 200 or times out. Returns 0 on ready, 1 otherwise.
wait_for_server() {
    local port="$1" timeout="${2:-8}"
    local i=0 url="http://127.0.0.1:${port}/"
    while (( i < timeout )); do
        if curl -fsS -o /dev/null --max-time 1 "${url}" 2>/dev/null; then
            return 0
        fi
        sleep 1
        i=$((i+1))
    done
    return 1
}

# start_server <port> <directory>: idempotent; only starts if not already up.
# After binding, does a real curl health probe to make sure it's serving.
# When a fresh server is spawned, we truncate the log via the `>` redirect
# inside the subshell - leave any existing httpd.log alone so we never
# delete a file the previous (already-killed) server was writing to.
start_server() {
    local port="$1" dir="$2"
    if server_running "${port}" && wait_for_server "${port}" 5; then
        echo "lib: server already up on ${port}"
        return 0
    fi
    (
        cd "${REPO_ROOT}/${dir}"
        # `>` truncates-or-creates atomically via the kernel; safe even when
        # an old server held the inode: our new fd points to a fresh entry.
        setsid nohup python3 -m http.server "${port}" --bind 127.0.0.1 \
            > "${CAPTURE_DIR}/httpd.log" 2>&1 < /dev/null &
    )
    disown -a 2>/dev/null || true
    if wait_for_server "${port}" 10; then
        echo "lib: server up on ${port}, root=${dir}"
        return 0
    fi
    echo "lib: failed to bind ${port} (see ${CAPTURE_DIR}/httpd.log)" >&2
    return 1
}

# stop_server <port>: abruptly terminates any http.server on <port>.
stop_server() {
    local port="$1"
    pkill -f "http.server ${port}\b" 2>/dev/null || true
    for pid in $(pgrep -f "http.server ${port}\b" 2>/dev/null); do
        kill -9 "${pid}" 2>/dev/null || true
    done
    sleep 0.2
}

# now_iso: ISO-8601 UTC timestamp for the log header.
now_iso() { date -u +"%Y-%m-%dT%H:%M:%SZ"; }

# header <run_note...>: writes the TEST_LOG.md preamble.
header() {
    local note="${*:-fresh run}"
    {
        echo "# TEST LOG - E2E browser-use run @ $(now_iso)"
        echo
        echo "Run note: ${note}"
        echo
        echo "| ID | Section | Type | Status | Duration | Notes |"
        echo "|----|---------|------|--------|----------|-------|"
    } > "${LOG_FILE}"
}

# has_fallback <id>: returns 0 if the story has a non-empty `fallback` block.
# Used by run-story.sh to decide between ⏭️ SKIP and the curl+grep fallback path.
has_fallback() {
    local id="$1"
    local fb
    fb="$(lookup_story "${id}" | jq -r '.fallback // empty | if type=="object" then "yes" else empty end' 2>/dev/null)"
    [[ "${fb}" == "yes" ]]
}

# fallback_eval <id>: runs the fallback curling + grep checks for the story.
# Echoes a short "[PASS]/[FAIL]" summary on stdout; returns 0 if all checks
# pass, 1 if any failed, 2 if the fallback block is malformed / empty.
#
# Each check object in stories.json has the schema:
#   { "name": "<label>", "url": "<relative path>", "regex": "<ERE pattern>",
#     "min": <int(default 1)> }
# Success ⇒ we observe ≥ `min` matches of `regex` in `curl url`
# Failure ⇒ `<min` matches, curl rc != 0, or response body empty.
fallback_eval() {
    local id="$1"
    local base_url="http://127.0.0.1:${PORT}"
    local checks_json n i check name url regex min body rc count
    local joined_parts=() fails=0

    checks_json="$(lookup_story "${id}" | jq -c '.fallback.checks // []' 2>/dev/null)"
    n="$(printf '%s' "${checks_json}" | jq 'length')"
    if [[ -z "${n}" || "${n}" == "null" || "${n}" == "0" ]]; then
        printf 'no-checks'
        return 2
    fi

    for ((i = 0; i < n; i++)); do
        check="$(printf '%s' "${checks_json}" | jq -c ".[${i}]")"
        name="$(printf '%s' "${check}"  | jq -r '.name  // "unnamed"')"
        url="$(printf '%s' "${check}"   | jq -r '.url   // "/"')"
        regex="$(printf '%s' "${check}" | jq -r '.regex // "^$"')"
        min="$(printf '%s' "${check}"   | jq -r '.min   // 1')"
        # Bail out of arithmetic if min isn't a valid integer.
        if ! [[ "${min}" =~ ^[0-9]+$ ]]; then min=1; fi

        body="$(curl -fsS --max-time 5 "${base_url}${url}" 2>/dev/null)"; rc=$?
        if (( rc != 0 )); then
            joined_parts+=("[FAIL] ${name}:curl ${url} rc=${rc}")
            fails=$((fails + 1))
            continue
        fi

        count="$(printf '%s' "${body}" | grep -oE -- "${regex}" 2>/dev/null | wc -l)"
        if (( count >= min )); then
            joined_parts+=("[PASS] ${name}=${count}≥${min}")
        else
            joined_parts+=("[FAIL] ${name}=${count}<${min} regex=${regex}")
            fails=$((fails + 1))
        fi
    done

    local sep=";"
    local joined="${joined_parts[0]}"
    local p
    for p in "${joined_parts[@]:1}"; do joined="${joined}${sep}${p}"; done
    printf '%s' "${joined}"
    [[ ${fails} -eq 0 ]]
}

# lookup_story <id>: echos the matched JSON object via jq (must exist).
lookup_story() {
    local id="$1"
    jq -c ".[] | select(.id == \"${id}\")" "${STORIES_JSON}"
}

# type_for <id>: echos the type string.
type_for() {
    lookup_story "$1" | jq -r '.type'
}

# notes_for <id>: echos the static notes field.
notes_for() {
    lookup_story "$1" | jq -r '.notes // ""'
}

# section_for <id>: echos the section letter.
section_for() {
    lookup_story "$1" | jq -r '.section'
}

# log_init <note>: convenience initializer.
log_init() { header "$*"; }
