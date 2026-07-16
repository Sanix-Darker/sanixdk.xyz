![Header Image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png)

## HOW I HACKED MY TOKEN/COST BURNT

`2026-07-12 03:51PM` • 11 min read • **#ai** **#agents** **#rust** **#performance** **#opensource** **#cost**

---

<div id="toc-container"></div>

---

### THE TOKEN DISCOUNT UNDERGROUND

Long time no see... i got busy with work, then with a lot of big side projects which were apparently also work. During all that, one annoying line kept showing up in my AI bills: repository orientation, paid again and again.

There are currently two ways to reduce an AI bill:

1. Make the agent read less useless stuff.
2. Buy a "100% LEGIT Claude/Codex UNLIMITED TOKEN ACCOUNT" from `token_wizard_94`, whose profile picture is a Lamborghini containing an NFT ape.

[Grey-market Claude proxies are real](https://www.tomshardware.com/tech-industry/artificial-intelligence/chinese-grey-market-sells-claude-api-access-at-90-percent-off-through-proxy-networks-that-harvest-user-data): pooled promos, stolen credentials, model swapping and operators keeping your prompts. My less-proven theory, after [ThePrimeagen covered Mini Shai-Hulud](https://www.youtube.com/watch?v=Ws-Nc9S8i_Y), is that supply-chain worms feed part of this market: poison package, scrape keys, hide access behind proxy. [The worm is documented](https://www.akamai.com/blog/security-research/mini-shai-hulud-worm-returns-goes-public); the resale hop is me connecting dots. Please don't cite me in court.

What if, instead of buying questionable tokens, we stopped sending the same useless context in the first place ?

Why should an agent spend 15 minutes doing `grep -> read -> wrong file -> grep again` when the repository already contains enough structure to route the query?

As a software engineer working on huge codebases, i had this idea stuck in my head for weeks: after compaction, agents read the same 14 files again. This is not optimal... you are actualy paying for amnesia.

So, on a Friday night and two Heinekens later, i started building [**radar**](https://github.com/sanix-darker/radar). The repository is intentionally private for now: ask me for access and i will invite you into the project. The door is closed to public AI bots scraping the internet, not to actual humans. This is access control, not fake scarcity.

### NOT MY ORIGINAL IDEA, SORRY YVAN

The first spark came from my friend [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of letting it rediscover the whole house. He was right about the first five minutes.

But what if one giant mapping file is still too heavy/FAT ? It gets loaded, cached, forgotten, loaded again... congratulations, we compressed the repo into a second bill.

My missing piece was a set of small knowledge units: dots connected by routes, each cheap to read and each pointing back to real source. Maps route; source answers. The map must never cosplay as the code.

```ascii
query
  -> radar query "<task>"     one local resolver, zero model tokens
     -> .radar/ROUTES.md      verified learned or auto-seeded routes
     -> .radar/SYMBOLS.md     exact public symbol -> file#line
     -> MAP.md metadata head  bounded fallback, never the whole map
  -> source#anchor            verify the answer
```

The `MAP.md` nodes are committed; `SYMBOLS.md` and `ROUTES.md` are local, gitignored fast paths. `radar query` checks a current verified route before loading the full index, then exact public symbols, caller/call context and finally bounded `MAP.md` frontmatter. The fallback reads metadata heads, not whole map files. Symbols stop at `5000` rows, routes at `500`, and `radar init` embeds the root router inside the agent contract to avoid one more file read. Plain `radar map` and `radar refresh` keep it synced.

Radar places maps at the root, package manifests, public-API-heavy directories and children promoted by budget overflow. Nodes carry bounded signatures, routes, `uses`, `Jump` hubs, tests, hashes and one optional purpose sentence, never function bodies. Learned routes must point to a real file and any named symbol anchor. An owner `api_hash` makes old routes stale hints; cold repos get deterministic routes from up to three ranked public symbols per scope. No vector database was harmed.

The difference from existing tools is mostly **where the cost lives**:

<table>
  <thead>
    <tr><th>Existing shape</th><th>Good at</th><th>Radar does differently</th></tr>
  </thead>
  <tbody>
    <tr><td><a href="https://github.com/yamadashy/repomix">Repomix</a> / whole-repo packs</td><td>One portable context blob</td><td>Keeps small routers; reads only the route chain</td></tr>
    <tr><td><a href="https://aider.chat/docs/repomap.html">Aider repo maps</a></td><td>Runtime ranked context for Aider</td><td>Commits tool-agnostic maps; regenerates local indices</td></tr>
    <tr><td><a href="https://github.com/oraios/serena">Serena</a> / live semantic tools</td><td>Rich query-time navigation</td><td>Precomputes an offline read path; MCP stays optional</td></tr>
  </tbody>
</table>

I did not benchmark those tools head-to-head. The difference is simpler: Radar leaves its map in Git, so it survives a clone, a compaction and a dead process.

> **Disclaimer:** Radar is not a magical solution. It is graph mathematics, ranking, hashes, thresholds and probabilistic evaluation wrapped in deterministic code. It works best on big projects where orientation is expensive and repeated. On a tiny repository, a direct search can still be cheaper.

### THE BORING ALGORITHMS SAVING THE MONEY

The pipeline is small enough to keep in my head:

```ascii
parallel walk -> parse -> graph -> place -> budget-pack -> hash -> emit
```

**Scan and parse.** Radar walks `.gitignore`-aware paths in parallel (defaulting to at most 64 workers), hashes changed contents with [BLAKE3](https://c2sp.org/BLAKE3), then runs [tree-sitter queries](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) for Rust, Python, JavaScript, TypeScript/TSX, Go, Java, C, C++, C#, PHP, Ruby and Bash. The parse cache key is `(language, content_hash)`, so moving unchanged bytes is a cache hit. Unsupported files are tracked, not magically understood.

**Graph and rank.** A file points to files defining the public names it references; ambiguous definitions split the edge mass. Radar computes [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) with `d = 0.85` over 30 iterations, although rendered order currently uses a simpler visible score: calls `3`, imports `1`, then stable path/line ties. The probability is there, it just does not run the government yet.

**Placement and packing.** Package manifests force nodes. Otherwise a directory splits above `30` public symbols; an owner above `35` promotes children carrying at least `8`. Root, inner and leaf bodies get `2400`, `1600` and `1200` byte budgets, with a `7200` byte maximum route chain. Symbols are ranked inside each node, then a [binary search](https://en.wikipedia.org/wiki/Binary_search_algorithm) keeps the largest prefix which renders inside the budget. Basically admission control, but for Markdown.

**Freshness.** On a clean checkout, Radar compares the stored root [`HEAD^{tree}`](https://git-scm.com/docs/gitrevisions) and can return without walking anything. Dirty or non-Git trees fall back to `mtime + size + inode`, then per-file BLAKE3. Each node gets an `api_hash` over canonical public signatures and a `kids_hash` over direct child API hashes. That borrows the [Merkle-tree idea](https://www.rfc-editor.org/rfc/rfc9162.html#name-merkle-trees) for one level; it is not a home-made recursive Merkle index.

### THE IDEAS THAT LOST THE BENCHMARK

I also tried the clever-looking pile before deciding boring was faster. The rule was simple: benchmark one proposal, keep it only if latency and answer quality improve, otherwise roll it back.

<table>
  <thead>
    <tr><th>Proposal</th><th>Measured result</th><th>Decision</th></tr>
  </thead>
  <tbody>
    <tr><td>Int8 MiniLM + HNSW</td><td>36.534 ms p95 and +385,256 KiB RSS; verified route was 0.339 ms p95</td><td>Rejected</td></tr>
    <tr><td>Integer-ID graph vectors</td><td>PageRank fell from 209.785 ms to 31.927 ms with byte-identical maps</td><td><strong>Kept</strong></td></tr>
    <tr><td>SimHash dedup</td><td>Merged opposite allow/deny code and ran 11.7x slower than exact hashing</td><td>Rejected</td></tr>
    <tr><td>Exact same-scan parse coordination</td><td>10,000 identical files fell from 84.353 ms to 41.252 ms</td><td><strong>Kept</strong></td></tr>
    <tr><td>Serialized 2.05 MB query view</td><td>Added 4.54 s to map generation and raised startup from 71.392 ms to 105.506 ms</td><td>Rejected</td></tr>
    <tr><td>Local 1.8B to 2.6B models</td><td>1.67 to 1.98 s warm, with the worst quality on multi-fact questions</td><td>Rejected</td></tr>
  </tbody>
</table>

Bloom and MinHash filters dropped valid semantic candidates. Memory mapping was slower than reading the tiny integer graph. spaCy extracted prose triples but scored `0/6` on source-code calls. The tiny decision tree memorized repeated queries and generalized to `1/12` paraphrases. None entered production. The full receipts, including raw reports and rollback reasons, live in [`OPTIMIZATIONS.md`](https://github.com/sanix-darker/radar/blob/main/OPTIMIZATIONS.md).

### OKAY, SHOW ME THE COMMANDS

With repository access granted, the `v0.2.1` release has exactly four archives: Linux and macOS, each for x86_64 and arm64. Linux uses static musl binaries, so the artifact does not inherit the build runner's glibc version. The [release README](https://github.com/sanix-darker/radar#install-from-a-release) has target detection, checksum verification, extraction and the macOS quarantine step.

For example, an authenticated GitHub CLI can fetch the Linux x86_64 bundle and checksum:

```bash
gh release download v0.2.1 --repo sanix-darker/radar \
  --pattern 'radar-v0.2.1-linux-x86_64.tar.gz*'
sha256sum -c radar-v0.2.1-linux-x86_64.tar.gz.sha256
tar -xzf radar-v0.2.1-linux-x86_64.tar.gz
./radar --version
```

Or install the alpha from checkout:

```bash
git clone git@github.com:sanix-darker/radar.git
cd radar && cargo install --path .
cd /path/to/your/huge-repo

radar scan              # optional stats; map also scans
radar map               # MAP.md tree + SYMBOLS.md + auto routes
radar init              # AGENTS/CLAUDE contract + repo-scoped skill
radar status            # freshness, slots, violations and route counts
radar query "where does order persistence happen?"
```

This is real output from the current alpha mapping its own fresh checkout. No `src/auth/definitely_real.rs`, i promise:

```text
$ radar map
radar map: 10 written, 1 unchanged, 7 purpose slot(s) pending
fill slots with your agent (Phase 3: radar slots --exec)

$ radar ls --ascii --depth 1
.                           syntax      ~312  ok
|-- example                 syntax      ~331  ok
|-- examples                syntax      ~335  ok
|-- scripts                 syntax      ~328  ok
|-- src                     syntax      ~332  ok
`-- tests/corpus/valid      syntax      ~456  ok

$ radar check
radar check: 11 map(s) OK
```

`tree` produces the same topology; `ls` is its alias because i also enjoy typing two letters.

For navigation and freshness:

```bash
rg '^rotate_token ' .radar/SYMBOLS.md
radar route find "where is token rotation handled?"
radar route add "token rotation" "src/auth/tokens.rs#rotate_token"

radar refresh               # maps + local indices + embedded root
radar refresh --since HEAD~1 # targeted map re-emission for changed files
radar refresh --deep        # optional offline LSP verification
radar watch --interval 2    # refresh loop
radar check                 # CI gate; exits 0/1/2/3
```

And for humans or agents who want more furniture:

```bash
radar slots --prompt
radar slots --exec "claude -p"  # optional 160-char purpose sentences
radar browse                    # read-only TUI
radar export --html radar-map.html
radar serve --web 127.0.0.1:8080
radar serve --mcp /path/to/repo # one read-only stdio tool
radar agent --prompt "fix token rotation"
```

`radar browse`:

![TUI](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-tui.png)

`radar serve`:

![BROWSER](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-web.png)

The MCP surface is now one read-only `navigate` tool. Its schema is `387` bytes, down from `2,261` bytes for the previous seven-tool version. It checks verified routes and the source index before a metadata-head fallback. Useful when a host wants a resident tool; entirely avoidable when one `radar query` command or plain files are cheaper.

The boring privacy bit: `scan`, `map`, `query`, `tree`, `status`, `refresh` and `check` run locally and do not upload source or call a model. `serve` binds to localhost by default. `slots --exec` and `agent` deliberately launch the external tool you choose, so its privacy rules apply. Radar edits maps and contracts, not application source. No tiny-font magic here.

### NUMBERS, BEFORE THE LINKEDIN CAROUSEL

The benchmark ledger keeps the ugly rounds too. A failed prompt, a wrong anchor or a slower prototype stays visible instead of becoming a motivational LinkedIn number. Painful ? yes. Cheaper than lying to myself ? also yes.

The latest consumer gate runs the same seven tasks five times with and without Radar. Each run gets a fresh isolated repository copy, a read-only sandbox, ignored user configuration, low reasoning and the locked `gpt-5.3-codex-spark` navigator. `Before` lets it search source. `After` gives it one complete shell-quoted `radar query` command. The table reports median **uncached** input tokens, not totals inflated by cache reads.

<table>
  <thead>
    <tr><th>Project and task</th><th>Before</th><th>With <code>radar query</code></th><th>Reduction</th><th>Before wall</th><th>After wall</th></tr>
  </thead>
  <tbody>
    <tr><td>12 files, exact location</td><td>2,162</td><td>666</td><td>3.25x</td><td>8.033 s</td><td>15.662 s</td></tr>
    <tr><td>12 files, caller trace</td><td>934</td><td>713</td><td>1.31x</td><td>6.213 s</td><td>11.184 s</td></tr>
    <tr><td>12 files, orientation</td><td>4,525</td><td>659</td><td>6.87x</td><td>16.430 s</td><td>11.278 s</td></tr>
    <tr><td>300 files, literal lookup</td><td>2,047</td><td>585</td><td>3.50x</td><td>5.607 s</td><td>9.976 s</td></tr>
    <tr><td>300 files, semantic hub</td><td>5,073</td><td>638</td><td>7.95x</td><td>19.185 s</td><td>7.830 s</td></tr>
    <tr><td>300 files, deep semantic hub</td><td>8,442</td><td>646</td><td>13.07x</td><td>23.701 s</td><td>8.830 s</td></tr>
    <tr><td>800 files, literal lookup</td><td>2,273</td><td>600</td><td>3.79x</td><td>11.678 s</td><td>7.531 s</td></tr>
  </tbody>
</table>

![Radar token reduction benchmark](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-token-reduction.png)

All `35/35` final runs were correct and all `35/35` used exactly one command. The sum of the seven medians fell from `25,456` to `4,507` uncached input tokens: **82.3% less**. Radar's Rust process uses zero model calls and zero model tokens. The remaining `585` to `713` tokens belong to the navigator choosing that one command and formatting the result. Call `radar query` directly and that wrapper cost does not exist.

The wall-time column is the honesty tax. Orientation, semantic hubs and the 800-file task got faster. Tiny exact and trace tasks, plus the 300-file literal lookup, used fewer tokens but took longer because model startup dominates cheap searches. This is why i keep latency beside tokens instead of declaring one universal victory.

The local engine has a separate gate because model wall time is not Radar processing time:

<table>
  <thead>
    <tr><th>Scale and path</th><th>p95 local latency</th><th>Model tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>841 files, fresh lexical query</td><td>11.617 ms</td><td>0</td></tr>
    <tr><td>841 files, resident lexical query</td><td>1.800 ms</td><td>0</td></tr>
    <tr><td>10,201 files, fresh verified route</td><td>2.795 ms</td><td>0</td></tr>
    <tr><td>10,201 files, resident exact route</td><td>10.898 ms</td><td>0</td></tr>
    <tr><td>10,201 files, resident lexical query</td><td>12.847 ms</td><td>0</td></tr>
    <tr><td>10,201 files, fresh uncached lexical query</td><td>137.308 ms</td><td>0</td></tr>
  </tbody>
</table>

![Radar local query latency benchmark](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-query-latency.png)

Verified routes and every resident query stay below the `50 ms` p95 target at 10,201 files. A brand-new process doing an uncached lexical lookup is the explicit exception: it loads the full index and reaches `137.308 ms` p95. Still zero tokens, but not sub-50 ms, so it does not get painted blue in the chart.

The charts are generated by Python from committed JSON/JSONL reports, not typed from a spreadsheet. [`BENCHMARKS.md`](https://github.com/sanix-darker/radar/blob/main/BENCHMARKS.md) documents the prompts, repetitions, expected anchors, hardware-facing local runs and raw report paths. It is the place to inspect the measurement instead of trusting a pink bar because i made it pretty.

### SO... ANOTHER AGENT FRAMEWORK ?

Nope. Please no. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then a meeting about the council.

Radar needs no daemon, but it is alpha. Tiny repos can save input while still losing wall time to model startup, unsupported languages get no semantic extraction, and purpose prose still needs a human or host agent. Better saying it here before someone opens an issue in ALL CAPS.

The goal is smaller: make expensive agents spend tokens on the change, review and tests, not on discovering `src/auth/jwt/handler.rs` four times. Yvan had the right first-five-minutes idea. I just broke the map into nodes, added hashes and routes, then got slightly too interested in the invoice.

The code lives in [**radar**](https://github.com/sanix-darker/radar). It is private on purpose; request access and i will add you to the project instead of giving every public AI scraper a free clone. `radar map` is the pitch. Everything after that is me refusing to pay for the same `grep` again.

---
