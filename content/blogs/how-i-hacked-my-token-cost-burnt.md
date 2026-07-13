![Header Image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png)

## HOW I HACKED MY TOKEN/COST BURNT

`2026-07-12 03:51PM` • 8 min read • **#ai** **#agents** **#rust** **#performance** **#opensource** **#cost**

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
  -> .radar/ROUTES.md       learned or auto-seeded query routes
  -> .radar/SYMBOLS.md      exact public symbol -> file#line
  -> MAP.md -> unit/MAP.md  bounded knowledge nodes
  -> source#anchor          verify the answer
```

The `MAP.md` nodes are committed; `SYMBOLS.md` and `ROUTES.md` are local, gitignored fast paths. Agents grep the symbol index instead of loading it. Symbols stop at `5000` rows, routes at `500`, and `radar init` embeds the root router inside the agent contract to avoid one more file read. Plain `radar map` and `radar refresh` keep it synced.

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

### THE BORING ALGORITHMS SAVING THE MONEY

The pipeline is small enough to keep in my head:

```ascii
parallel walk -> parse -> graph -> place -> budget-pack -> hash -> emit
```

**Scan and parse.** Radar walks `.gitignore`-aware paths in parallel (defaulting to at most 64 workers), hashes changed contents with [BLAKE3](https://c2sp.org/BLAKE3), then runs [tree-sitter queries](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) for Rust, Python, JavaScript, TypeScript/TSX, Go, Java, C, C++, C#, PHP, Ruby and Bash. The parse cache key is `(language, content_hash)`, so moving unchanged bytes is a cache hit. Unsupported files are tracked, not magically understood.

**Graph and rank.** A file points to files defining the public names it references; ambiguous definitions split the edge mass. Radar computes [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) with `d = 0.85` over 30 iterations, although rendered order currently uses a simpler visible score: calls `3`, imports `1`, then stable path/line ties. The probability is there, it just does not run the government yet.

**Placement and packing.** Package manifests force nodes. Otherwise a directory splits above `30` public symbols; an owner above `35` promotes children carrying at least `8`. Root, inner and leaf bodies get `2400`, `1600` and `1200` byte budgets, with a `7200` byte maximum route chain. Symbols are ranked inside each node, then a [binary search](https://en.wikipedia.org/wiki/Binary_search_algorithm) keeps the largest prefix which renders inside the budget. Basically admission control, but for Markdown.

**Freshness.** On a clean checkout, Radar compares the stored root [`HEAD^{tree}`](https://git-scm.com/docs/gitrevisions) and can return without walking anything. Dirty or non-Git trees fall back to `mtime + size + inode`, then per-file BLAKE3. Each node gets an `api_hash` over canonical public signatures and a `kids_hash` over direct child API hashes. That borrows the [Merkle-tree idea](https://www.rfc-editor.org/rfc/rfc9162.html#name-merkle-trees) for one level; it is not a home-made recursive Merkle index.

### OKAY, SHOW ME THE COMMANDS

With repository access granted, install the alpha from checkout:

```bash
git clone git@github.com:sanix-darker/radar.git
cd radar && cargo install --path .
cd /path/to/your/huge-repo

radar scan              # optional stats; map also scans
radar map               # MAP.md tree + SYMBOLS.md + auto routes
radar init              # AGENTS/CLAUDE contract + repo-scoped skill
radar status            # freshness, slots, violations and route counts
```

This is real output from the current alpha mapping its own fresh checkout. No `src/auth/definitely_real.rs`, i promise:

```text
$ radar map
radar map — 10 written, 1 unchanged, 7 purpose slot(s) pending
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
radar serve                     # local web view on a free port
radar serve --mcp               # 7 stdio tools, same precomputed state
radar agent --prompt "fix token rotation"
```

`radar browse`:

![TUI](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-tui.png)

`radar serve`:

![BROWSER](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-web.png)

The MCP tools are `get_map`, `tree`, `find_symbol`, `refresh_status`, `route_find`, `route_add` and `fill_slot`. Useful when a host wants tools; entirely avoidable when plain files are cheaper.

The boring privacy bit: `scan`, `map`, `tree`, `status`, `refresh` and `check` run locally and do not upload source or call a model. `serve` binds to localhost by default. `slots --exec` and `agent` deliberately launch the external tool you choose, so its privacy rules apply. Radar edits maps and contracts, not application source. No tiny-font magic here.

### NUMBERS, BEFORE THE LINKEDIN CAROUSEL

The benchmark ledger runs identical agent tasks with billed tokens, turns, cost and correctness. It keeps ugly rounds too: before the placement fix, Radar lost the first N=5 run. Painful ? yes. Cheaper than lying to myself ? also yes.

The first four rows are headless Haiku agent comparisons, marked **unisolated**, over fixture or generated repositories. The last row is a separate synthetic engine benchmark. Measurements, not a blood oath:

<table>
  <thead>
    <tr><th>Measured slice</th><th>Radar</th><th>Bare / before</th><th>Honest result</th></tr>
  </thead>
  <tbody>
    <tr><td>Tiny repo, 12 files</td><td>75,842 input tokens</td><td>75,300</td><td>0.99x; no win, partial Radar answer</td></tr>
    <tr><td>Semantic hub, 300 files, N=10</td><td>62,836 / $0.0278</td><td>273,612 / $0.0803</td><td><strong>4.35x fewer input tokens</strong>, 65% lower cost, 10/10</td></tr>
    <tr><td>Deep monorepo hub, N=5</td><td>62,268 / $0.0275</td><td>515,905 / $0.1467</td><td><strong>8.3x fewer input tokens</strong>, 81% lower cost, 5/5</td></tr>
    <tr><td>Warm solved route, N=10</td><td>$0.0343</td><td>$0.0637</td><td>46% lower cost; Radar 10/10, bare 8/10</td></tr>
    <tr><td>10,000-file full parse</td><td>229 ms</td><td>1,299 ms</td><td>5.7x; engine time, not agent wall time</td></tr>
  </tbody>
</table>

At the measured 300-file hub, one navigation prompt triggered a multi-turn agent run averaging `$0.0278` with Radar: basically `$0.03`. That looks like spare change on a small project, because it is. But an intensive user does not send one prompt: across 100 comparable navigation prompts, the straight extrapolation is `$2.78` with Radar versus `$8.03` bare, or `$5.25` saved. That is arithmetic over the measured averages, not 100 extra benchmark runs.

What about something the size of Linux ? I have not benchmarked it, so here is **conjecture only**: sensitivity math for 100 navigation-heavy prompts using an arbitrary near-half reduction of `48%`. The first column is scenario input, not my estimate of a Linux query. Model, cache, query and tool choice can move every number.

<table>
  <thead>
    <tr><th>Bare scenario input / prompt</th><th>Bare, 100 prompts</th><th>Hypothetical at assumed -48%</th><th>Saved</th></tr>
  </thead>
  <tbody>
    <tr><td>$0.30</td><td>$30</td><td>$15.60</td><td>$14.40</td></tr>
    <tr><td>$1.00</td><td>$100</td><td>$52</td><td>$48</td></tr>
    <tr><td>$3.00</td><td>$300</td><td>$156</td><td>$144</td></tr>
  </tbody>
</table>

The formulas are just `saved = 100 x bare cost x 0.48` and `remaining = 100 x bare cost x 0.52`. Not a Linux benchmark. It only shows why a hypothetical gain near half stops looking negligeable after the hundredth prompt.

Raw token totals hide the different prices of fresh input, cache reads, cache creation and extra turns, so the ledger keeps dollars and correctness beside tokens. On a grep-friendly exact needle, Radar can lose turns. On a semantic hub query, bare navigation becomes a casino and the map does its job.

### SO... ANOTHER AGENT FRAMEWORK ?

Nope. Please no. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then a meeting about the council.

Radar needs no daemon, but it is alpha. Tiny repos gain almost nothing, unsupported languages get no semantic extraction, and purpose prose still needs a human or host agent. Better saying it here before someone opens an issue in ALL CAPS.

The goal is smaller: make expensive agents spend tokens on the change, review and tests, not on discovering `src/auth/jwt/handler.rs` four times. Yvan had the right first-five-minutes idea. I just broke the map into nodes, added hashes and routes, then got slightly too interested in the invoice.

The code lives in [**radar**](https://github.com/sanix-darker/radar). It is private on purpose; request access and i will add you to the project instead of giving every public AI scraper a free clone. `radar map` is the pitch. Everything after that is me refusing to pay for the same `grep` again.

---
