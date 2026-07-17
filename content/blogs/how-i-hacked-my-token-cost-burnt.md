<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png"/>

## HOW I HACKED MY TOKEN/COST BURNT WITH MATHS

`2026-07-12 03:51PM` • 13 min read • **#ai** **#agents** **#rust** **#performance** **#opensource** **#cost**

---

<div id="toc-container"></div>

---

### THE TOKEN DISCOUNT UNDERGROUND

Long time no see... i got busy with work, then with a lot of big side projects which were apparently also work. During all that, one annoying line kept showing up in my AI bills: repository orientation, paid again and again.

There are currently two ways to reduce an AI bill:

1. Make the agent read less useless stuff.
2. Buy a "100% LEGIT Claude/Codex UNLIMITED TOKEN ACCOUNT" from `token_wizard_94`, whose profile picture is a Lamborghini containing an NFT ape.

[Grey-market Claude proxies are real](https://www.tomshardware.com/tech-industry/artificial-intelligence/chinese-grey-market-sells-claude-api-access-at-90-percent-off-through-proxy-networks-that-harvest-user-data): pooled promos, stolen credentials, model swapping and operators keeping your prompts. My less-proven theory, after [ThePrimeagen covered Mini Shai-Hulud](https://www.youtube.com/watch?v=Ws-Nc9S8i_Y), is that supply-chain worms feed part of this market: poison package, scrape keys, hide access behind proxy. [The worm is documented](https://www.akamai.com/blog/security-research/mini-shai-hulud-worm-returns-goes-public); the resale hop is me connecting dots. Please don't cite me in court.

What if, instead of buying questionable tokens, we stopped sending the same useless context in the first place?

Why should an agent spend 15 minutes doing `grep -> read -> wrong file -> grep again` when the repository already contains enough structure to route the query?

As a software engineer working on huge codebases, i had this idea stuck in my head for weeks: after compaction, agents read the same 14 files again. This is not optimal... you are actualy paying for amnesia.

So, on a Friday night and two Heinekens later, i started building [**radar**](https://github.com/Sanix-Darker/radar). It is now released as a small Rust binary and deliberately LLM-free by default. The repository remains private, so source and release downloads require access.

### NOT MY ORIGINAL IDEA, SORRY YVAN

The first spark came from my friend [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of letting it rediscover the whole house. He was right about the first five minutes.

But what if one giant mapping file is still too heavy/FAT? It gets loaded, cached, forgotten, loaded again... congratulations, we compressed the repo into a second bill.

My missing piece was a set of small knowledge units: dots connected by routes, each cheap to read and each pointing back to real source. Maps route; source answers. The map must never cosplay as the code.

```ascii
query
  -> radar query "<task>"       one local resolver, zero model tokens
     -> .radar/ROUTES.md        verified learned or auto-seeded route
     -> .radar/query.bin        compact public + private source index
     -> repository overview     bounded orientation answer
     -> MAP.md metadata head    bounded fallback, never the full map
  -> source#anchor              verify the answer
```

The `MAP.md` nodes are committed. `ROUTES.md`, `SYMBOLS.md` and `query.bin` are local, disposable fast paths. The compact snapshot is rebuilt from canonical scan state when it is missing, corrupt or incompatible.

`radar query` checks a verified route first, then an exact and lexical source index containing public and internal definitions. It scores symbol names, signatures, bounded function-body fingerprints, calls, callers, package terms and path intent. Posting-list rarity keeps common words from winning. Caller, path, package-entry and exact-symbol questions have separate intent gates. Only after those paths fail does Radar return an overview or read bounded `MAP.md` frontmatter.

That last detail mattered. Reading only metadata heads was cheaper, but it failed an exact location task when used alone. The production order keeps the cheap metadata fallback without pretending it can replace source indexing.

Radar places maps at the root, package manifests, public-API-heavy directories and children promoted by budget overflow. Nodes carry bounded signatures, routes, `uses`, `Jump` hubs, tests, hashes and one optional purpose sentence, never function bodies. Learned routes must point to a real file and any named symbol anchor. An owner `api_hash` makes old routes stale hints. No vector database was harmed.

The difference from existing tools is mostly **where the cost lives**:

<table>
  <thead>
    <tr><th>Existing shape</th><th>Good at</th><th>Radar does differently</th></tr>
  </thead>
  <tbody>
    <tr><td><a href="https://github.com/yamadashy/repomix">Repomix</a> / whole-repo packs</td><td>One portable context blob</td><td>Keeps small routers and reads only the route chain</td></tr>
    <tr><td><a href="https://aider.chat/docs/repomap.html">Aider repo maps</a></td><td>Runtime ranked context for Aider</td><td>Commits tool-agnostic maps and regenerates local indexes</td></tr>
    <tr><td><a href="https://github.com/oraios/serena">Serena</a> / live semantic tools</td><td>Rich query-time navigation</td><td>Precomputes an offline read path; MCP stays optional</td></tr>
    <tr><td><a href="https://github.com/aovestdipaperino/tokensave">TokenSave</a></td><td>Broad local repository intelligence, context and editing workflows</td><td>Uses one static binary, committed routers and exact source anchors with no database, embedding or model runtime</td></tr>
  </tbody>
</table>

TokenSave inspired useful questions about local persistence, ranked context, language coverage and repository state. I tested the strongest candidates instead of copying the architecture. On the same 40-query source-anchor corpus, TokenSave retrieved the expected anchor in `5/40` cases and returned an estimated `8,421` tokens. Radar retrieved `40/40` in `835` estimated tokens. That comparison is intentionally narrow: TokenSave is a broader product, while Radar is optimized for repository routing.

> **Disclaimer:** Radar is not a magical solution. It is graph mathematics, ranking, hashes, thresholds and probabilistic evaluation wrapped in deterministic code. It works best on big projects where orientation is expensive and repeated. On a tiny repository, a direct search can still be cheaper.

### THE BORING ALGORITHMS SAVING THE MONEY

The indexing pipeline is small enough to keep in my head:

```ascii
parallel walk -> parse -> graph -> place -> budget-pack -> hash -> emit
```

**Scan and parse.** Radar walks `.gitignore`-aware paths in parallel, hashes changed contents with [BLAKE3](https://c2sp.org/BLAKE3), then runs [tree-sitter queries](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) for Rust, Python, JavaScript, TypeScript/TSX, Go, Java, C, C++, C#, PHP, Ruby and Bash. The parse cache key is `(language, content_hash)`, so identical bytes share one extraction even when scanner workers discover them at the same time.

**Graph and rank.** A file points to files defining the names it references; ambiguous definitions split the edge mass. Stable integer IDs and contiguous adjacency vectors keep the graph compact. Radar computes [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) with `d = 0.85` over 30 iterations, then reuses that ranking for maps, symbols and automatic routes.

**Placement and packing.** Package manifests force nodes. Otherwise a directory splits above `30` public symbols; an owner above `35` promotes children carrying at least `8`. Root, inner and leaf bodies get `2400`, `1600` and `1200` byte budgets, with a `7200` byte maximum route chain. Symbols are ranked inside each node, then a binary search keeps the largest prefix which renders inside the budget.

**Query retrieval.** Each extracted definition stores at most 64 compact 32-bit fingerprints from bounded body and comment text. The query index combines those fingerprints with exact names, signatures, call edges, paths and term rarity. The result is one final source anchor when confidence is high, otherwise at most three bounded candidates.

**Freshness and caching.** On a clean checkout, Radar compares the stored root `HEAD^{tree}` and can return without walking anything. Dirty or non-Git trees fall back to file stamps and BLAKE3. The resident server keeps at most 256 normalized exact answers and clears them when source state or verified routes change. There is no wall-clock TTL that can return a stale answer just because the timer has not expired yet.

### THE IDEAS THAT LOST THE BENCHMARK

The rule was simple: test one proposal, keep it only if latency and answer quality improve, otherwise roll it back.

<table>
  <thead>
    <tr><th>Proposal</th><th>Measured result</th><th>Decision</th></tr>
  </thead>
  <tbody>
    <tr><td>Int8 MiniLM + HNSW</td><td>47.62 ms end-to-end p95, 59.13 ms embedding p95, 91.7% semantic recall and +444,716 KiB RSS</td><td>Rejected</td></tr>
    <tr><td>Bloom + MinHash candidate filters</td><td>Bloom lost 7/12 semantic cases or pruned nothing; MinHash semantic recall was 16.7%</td><td>Rejected</td></tr>
    <tr><td>Memory-mapped adjacency</td><td>0.0536 ms versus 0.0498 ms for an ordinary read on the same tiny graph</td><td>Rejected</td></tr>
    <tr><td>SimHash deduplication</td><td>32.99 ms versus 2.88 ms for BLAKE3 and it merged opposite allow/deny behavior</td><td>Rejected</td></tr>
    <tr><td>Compact query snapshot</td><td>Startup 139.57 to 21.72 ms, RSS 28,220 to 14,296 KiB, route p95 311.19 to 34.48 ms</td><td><strong>Kept</strong></td></tr>
    <tr><td>Bounded exact-answer cache</td><td>Repeated lexical p95 4.442 to 0.038 ms; exact route 3.025 to 0.044 ms</td><td><strong>Kept</strong></td></tr>
    <tr><td>Local 1.8B to 2.6B models</td><td>1.67 to 1.98 s warm and worst quality on multi-fact questions</td><td>Rejected</td></tr>
  </tbody>
</table>

spaCy loaded in `515.94 ms`, added `134,144 KiB` RSS and scored `0/6` on source-code call extraction. The tiny decision tree generalized to `1/12` paraphrases. Semantic caching inherited the rejected embedding stack. Async added a dependency around a resident path already near `0.04 ms` p95. Time-based invalidation weakened correctness. Merkle triple patches duplicated Git's tree identity without discovering changed files.

The full receipts, raw JSON/JSONL reports and rollback reasons live in [`OPTIMIZATIONS.md`](https://github.com/Sanix-Darker/radar/blob/main/OPTIMIZATIONS.md) for readers with repository access.

### OKAY, SHOW ME THE COMMANDS

With repository access, the `v0.3.0` release has exactly four archives:

- Linux x86_64
- Linux arm64
- macOS x86_64
- macOS arm64

Linux uses static musl binaries, so the artifact does not inherit the build runner's glibc version. There is no Windows target. The [release README](https://github.com/Sanix-Darker/radar#install-from-a-release) includes target detection, checksum verification, extraction, user-local installation and the macOS quarantine step.

For example, an authenticated GitHub CLI can fetch Linux x86_64:

```bash
gh release download v0.3.0 --repo Sanix-Darker/radar \
  --pattern 'radar-v0.3.0-linux-x86_64.tar.gz*'
sha256sum -c radar-v0.3.0-linux-x86_64.tar.gz.sha256
tar -xzf radar-v0.3.0-linux-x86_64.tar.gz
./radar --version
```

Or install from source:

```bash
gh repo clone Sanix-Darker/radar
cd radar
cargo install --locked --path .
cd /path/to/your/huge-repo

radar map
radar init
radar status
radar query "where does order persistence happen?"
```

This is real output from Radar mapping its own fresh checkout:

```text
$ radar map
radar map - 0 written, 11 unchanged, 7 purpose slot(s) pending
fill slots with `radar slots --exec`

$ radar tree --ascii --depth 1
.                           syntax      ~304  ok
|-- bench                   syntax      ~330  ok
|-- example                 syntax      ~330  ok
|-- examples                syntax      ~333  ok
|-- src                     syntax      ~330  ok
`-- tests/corpus/valid      syntax      ~454  ok

$ radar check
radar check: 11 map(s) OK
```

For navigation and freshness:

```bash
radar route find "where is token rotation handled?"
radar route add "token rotation" "src/auth/tokens.rs#rotate_token"
radar refresh
radar refresh --since HEAD~1
radar refresh --deep
radar watch --interval 2
radar check
```

And for humans or hosts who want more furniture:

```bash
radar slots --prompt
radar slots --exec "<your command>"
radar browse
radar export --html radar-map.html
radar serve --web 127.0.0.1:8080
radar serve --mcp /path/to/repo
radar agent --prompt "fix token rotation"
```

`radar browse`:

![TUI](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-tui.png)

`radar serve`:

![BROWSER](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-web.png)

The MCP surface is one read-only `navigate` tool with a `387` byte schema. Locally it is extremely fast, but the live consumer used much more context around MCP responses than around one direct command. So `radar query` is the default integration and MCP remains optional.

The boring privacy bit: `scan`, `map`, `query`, `tree`, `status`, `refresh` and `check` run locally and do not upload source or call a model. `serve` binds to localhost by default. `slots --exec` and `agent` deliberately launch the external tool you choose, so its privacy rules apply.

### NUMBERS, BEFORE THE LINKEDIN CAROUSEL

The benchmark ledger keeps the ugly rounds too. A failed prompt, a wrong anchor or a slower prototype stays visible instead of becoming a motivational LinkedIn number.

The latest consumer gate runs the same seven tasks three times in fresh isolated repository copies. It uses a read-only sandbox, ignored user configuration, low reasoning and the locked `gpt-5.3-codex-spark` navigator. `Before` lets it search source. `After` gives it one complete shell-quoted `radar query` command. Input below means median uncached input tokens.

<table>
  <thead>
    <tr><th>Project scale and task</th><th>Before</th><th>With <code>radar query</code></th><th>Change</th><th>Before wall</th><th>After wall</th></tr>
  </thead>
  <tbody>
    <tr><td>12 files, exact storage location</td><td>994</td><td>592</td><td>1.68x fewer</td><td>7.13 s</td><td>16.99 s</td></tr>
    <tr><td>12 files, caller trace</td><td>900</td><td>657</td><td>1.37x fewer</td><td>7.14 s</td><td>15.69 s</td></tr>
    <tr><td>12 files, subsystem orientation</td><td>3,935</td><td>634</td><td>6.21x fewer</td><td>17.61 s</td><td>9.31 s</td></tr>
    <tr><td>300 files, cold literal lookup</td><td>766</td><td>611</td><td>1.25x fewer</td><td>7.55 s</td><td>7.16 s</td></tr>
    <tr><td>300 files, semantic package hub</td><td>3,733</td><td>574</td><td>6.50x fewer</td><td>19.00 s</td><td>9.30 s</td></tr>
    <tr><td>300 files, deep semantic package hub</td><td>4,517</td><td>596</td><td>7.58x fewer</td><td>17.78 s</td><td>9.40 s</td></tr>
    <tr><td>800 files, cold literal lookup</td><td>562</td><td>585</td><td>4.1% higher</td><td>7.61 s</td><td>6.49 s</td></tr>
    <tr><td><strong>Sum of task medians</strong></td><td><strong>15,407</strong></td><td><strong>4,249</strong></td><td><strong>72.4% lower</strong></td><td></td><td></td></tr>
  </tbody>
</table>

![Radar token reduction benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-reduction.png)

The direct-query arm was correct in `21/21` runs. Direct source search was correct in `20/21`. The one 800-file literal task is `4.1%` higher and stays visible because direct search is already cheap when the prompt contains the exact rare word.

The MCP arm was also correct in `21/21`, but its sum of uncached-input medians was `34,677`. The direct command used `87.7%` less. A microsecond local tool does not guarantee a cheap consumer if the host wraps it in a large protocol context.

#### WHAT THOSE TOKENS COST UNDER ONE DECLARED RATE

Tokens are not dollars, especially here. Spark is still a research preview and its public credit rate is not final in the [Codex rate card](https://help.openai.com/en/articles/20001106-codex-rate-card). So this is a reproducible reference scenario, not a Spark invoice: price every recorded run with the published [GPT-5.3-Codex API rates](https://developers.openai.com/api/docs/models/gpt-5.3-codex), then take the median for each task.

The rate is `$1.75/M` uncached input, `$0.175/M` cached input and `$14/M` output. Reasoning tokens already sit inside output tokens and are not charged twice.

<table>
  <thead>
    <tr><th>Project scale and task</th><th>Before reference cost</th><th>With <code>radar query</code></th><th>Change</th></tr>
  </thead>
  <tbody>
    <tr><td>12 files, exact storage location</td><td>$0.008820</td><td>$0.008184</td><td>7.2% lower</td></tr>
    <tr><td>12 files, caller trace</td><td>$0.010137</td><td>$0.009500</td><td>6.3% lower</td></tr>
    <tr><td>12 files, subsystem orientation</td><td>$0.038593</td><td>$0.008286</td><td>78.5% lower</td></tr>
    <tr><td>300 files, cold literal lookup</td><td>$0.007730</td><td>$0.008120</td><td>5.0% higher</td></tr>
    <tr><td>300 files, semantic package hub</td><td>$0.038024</td><td>$0.007131</td><td>81.2% lower</td></tr>
    <tr><td>300 files, deep semantic package hub</td><td>$0.044884</td><td>$0.007729</td><td>82.8% lower</td></tr>
    <tr><td>800 files, cold literal lookup</td><td>$0.007353</td><td>$0.007458</td><td>1.4% higher</td></tr>
    <tr><td><strong>Sum of task medians</strong></td><td><strong>$0.155542</strong></td><td><strong>$0.056409</strong></td><td><strong>63.7% lower</strong></td></tr>
  </tbody>
</table>

![Radar reference token cost benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-cost.png)

The two higher-cost literal rows stay in the chart. The result is not "Radar wins every prompt." The result is that repository orientation and semantic routing dominate the savings, while already-trivial literals define the no-win boundary.

#### ROUTING QUALITY BEFORE SPEED

The deterministic quality corpus contains 40 exact, semantic, caller, body-vocabulary, package and ambiguity queries over 10,000 source files:

<table>
  <thead>
    <tr><th>Router</th><th>Top-1</th><th>Expected anchor retrieved</th><th>Estimated response tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Previous Radar public-symbol baseline</td><td>2/40</td><td>2/40</td><td>4,042</td></tr>
    <tr><td>TokenSave context command</td><td>2/40</td><td>5/40</td><td>8,421</td></tr>
    <tr><td>Current Radar lexical router</td><td>37/40</td><td>40/40</td><td>835</td></tr>
  </tbody>
</table>

Compared with the old Radar baseline, response volume fell `79.3%` while expected-anchor retrieval rose from `5%` to `100%`. The fresh-process quality run is `39.56 ms` p95.

#### THE LOCAL ENGINE

Model wall time is not Radar processing time, so the local binary has a separate 10,000-file gate:

<table>
  <thead>
    <tr><th>Path</th><th>p95 local latency</th><th>Model tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Fresh process, 40-query quality corpus</td><td>39.56 ms</td><td>0</td></tr>
    <tr><td>Fresh process, verified route</td><td>35.75 ms</td><td>0</td></tr>
    <tr><td>Fresh process, isolated lexical stress probe</td><td>53.97 ms</td><td>0</td></tr>
    <tr><td>First resident lexical query</td><td>3.76 ms</td><td>0</td></tr>
    <tr><td>Repeated resident lexical query</td><td>0.038 ms</td><td>0</td></tr>
    <tr><td>Repeated resident exact route</td><td>0.044 ms</td><td>0</td></tr>
    <tr><td>Resident metadata fallback</td><td>0.105 ms</td><td>0</td></tr>
  </tbody>
</table>

![Radar local query latency benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-query-latency.png)

The isolated fresh lexical stress probe is the only latest p95 above `50 ms`. The full 40-query quality corpus, verified routes, first resident requests and repeated resident requests are below the target. Server startup is `24.39 ms`, resident RSS is `14,156 KiB`, and the complete tool schema is `387` bytes.

The charts are generated by Python from committed JSON/JSONL reports, not typed from a spreadsheet. [`BENCHMARKS.md`](https://github.com/Sanix-Darker/radar/blob/main/BENCHMARKS.md) documents the prompts, repetitions, expected anchors, hardware-facing local runs, exact cost formula, reproduction commands and raw report paths for readers with repository access.

### SO... ANOTHER AGENT FRAMEWORK?

Nope. Please no. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then a meeting about the council.

Radar needs no daemon, no API key and no model, but it is still alpha. Tiny repos can save input while losing wall time to model startup. Unsupported languages get no semantic extraction. A rare cold lexical stress case still crosses `50 ms`. Purpose prose still needs a human or the external command you explicitly choose.

The goal is smaller: make expensive agents spend tokens on the change, review and tests, not on discovering the same source file four times. The code and benchmark receipts live in [**radar**](https://github.com/Sanix-Darker/radar); ask me for repository access if you want to inspect or test it.

`radar map` builds the dots. `radar query` finds the route. Source still answers.

---
