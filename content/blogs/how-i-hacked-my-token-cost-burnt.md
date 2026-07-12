![Header Image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png)

## HOW I HACKED MY TOKEN/COST BURNT

`2026-07-12 03:51PM` • 6 min read • **#ai** **#agents** **#rust** **#performance** **#opensource**

---

<div id="toc-container"></div>

---

### THE TOKEN DISCOUNT UNDERGROUND

There are currently two ways to reduce an AI bill:

1. Make the agent read less useless stuff.
2. Buy a "100% LEGIT Claude/Cortex UNLIMITED TOKEN ACCOUNT" from `token_wizard_94`, whose profile picture is a Lamborghini containing an NFT ape.

What if, instead of buying questionable tokens, we stopped sending the same useless context in the first place ?

Why should an agent spend 15 minutes doing `grep -> read -> wrong file -> grep again` when the repository already contains enough structure to route the query?

As a software engineer i use to work on huge codebases, and i had this idea stuck in my head for weeks... agents repeatedly read the same 14 files after context compaction between prompts... for me this is not optimal, you are actualy paying for amnesia.

So, on a Friday night, between 2 drinks of Heineken, i started working on [**radar**](https://github.com/sanix-darker/radar), a small Rust tool which compiles a repository into a committed navigation layer. No cloud, embeddings, database, API key or model required for the useful part.

### NOT MY INITIAL ORIGINAL IDEA

The first spark came from a friend of mine [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of letting it rediscover the whole house. This is the correct instinct.

But what if one giant mapping file is still too heavy/FAT ? It gets loaded, cached, forgotten, loaded again... congratulations, we compressed the repo into a second bill.

My missing piece was a set of small knowledge units: dots connected by routes, each cheap to read and each pointing back to real source. Maps route; source answers. The map must never cosplay as the code.

```ascii
query
  -> .radar/ROUTES.md       learned or auto-seeded query routes
  -> .radar/SYMBOLS.md      exact public symbol -> file#line
  -> MAP.md -> unit/MAP.md  bounded knowledge nodes
  -> source#anchor          verify the answer
```

The `MAP.md` nodes are the committed layer. `SYMBOLS.md` and `ROUTES.md` are local, gitignored fast paths which Radar generates and maintains. Agents grep the symbol index instead of loading it; symbols are capped at `5000` rows and routes at `500`. `radar init` also embeds the current root router inside the agent contract, so the first hop can cost zero reads; later `radar map` and plain `radar refresh` calls keep that copy synced.

Radar writes `MAP.md` only where knowledge has enough mass: the root, package manifests and public-API-heavy directories. A node can carry routes, public signatures, up to six cross-scope `uses`, three `Jump` hubs, tests, freshness hashes and one tiny optional purpose sentence. It never copies function bodies.

A learned route is accepted only if Radar can validate its file and any named `file#symbol` anchor. It stores an eight-character owner `api_hash`; after refresh, a mismatched route becomes a hint instead of a fact. Cold repositories also get deterministic auto-routes from the top three symbol hubs per scope. No vector database was harmed during this lookup.

The difference from existing tools is mostly **where the cost lives**:

<table>
  <thead>
    <tr><th>Existing shape</th><th>Good at</th><th>Radar does differently</th></tr>
  </thead>
  <tbody>
    <tr><td><a href="https://github.com/yamadashy/repomix">Repomix</a> / whole-repo packs</td><td>One portable context blob</td><td>Keeps small routers; reads only the route chain</td></tr>
    <tr><td><a href="https://aider.chat/docs/repomap.html">Aider repo maps</a></td><td>Runtime ranked context for Aider</td><td>Commits tool-agnostic maps; regenerates local indices</td></tr>
    <tr><td><a href="https://github.com/oraios/serena">Serena</a> / live semantic tools</td><td>Rich query-time navigation</td><td>Precomputes an offline read path; MCP stays optional</td></tr>
    <tr><td>Vector or <a href="https://doi.org/10.1561/1500000019">BM25</a> retrieval</td><td>Fuzzy chunk search</td><td>Uses exact symbols and graph facts; no index server</td></tr>
  </tbody>
</table>

Those tools are not bad. Radar is just making a different bet: the durable orientation layer should survive `git clone`, process death and context compaction without paying a standing tool-definition tax.

### THE BORING ALGORITHMS SAVING THE MONEY

The pipeline is small enough to keep in my head:

```ascii
parallel walk -> parse -> graph -> place -> budget-pack -> hash -> emit
```

**Scan and parse.** Radar walks `.gitignore`-aware paths in parallel (defaulting to at most 64 workers), hashes changed contents with the [BLAKE3 specification](https://c2sp.org/BLAKE3), then runs [tree-sitter queries](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) for Rust, Python, JavaScript, TypeScript/TSX, Go, Java, C, C++, C#, PHP, Ruby and Bash. The parse cache key is `(language, content_hash)`, so a moved file with identical bytes is a cache hit. Unsupported files are tracked, but not magically understood. Honnesty is also a feature.

**Graph and probability.** A file points to files defining the public names it references; ambiguous definitions split the edge mass. Radar computes classic [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) with `d = 0.85` for 30 power iterations:

```text
rank_next(v) = (1-d)/N + d * (incoming_mass(v) + dangling_mass/N)
```

That is the probability bit: a boring random walk, not AI confidence wearing glasses. In the current alpha, emitted API rows, `Jump` hubs and auto-routes use an even simpler visible score: calls count `3`, imports count `1`, then deterministic path/line tie-breaks. PageRank is computed in the graph layer, but it does not secretly decide the rendered order. I prefer an awkward truth over a magical decimal.

**Placement and packing.** Package manifests force nodes. Otherwise a directory splits above `30` public symbols; an owner above `35` promotes children carrying at least `8`. Root, inner and leaf bodies get `2400`, `1600` and `1200` byte budgets, with a `7200` byte maximum route chain. Symbols are ranked inside each node, then a [binary search](https://github.com/sanix-darker/radar/blob/main/src/mapfile.rs#L449-L478) keeps the largest prefix which renders inside the budget. Basically admission control, but for Markdown.

**Freshness.** On a clean Git checkout, Radar compares the stored root [`HEAD^{tree}` object](https://git-scm.com/docs/gitdatamodel) and can return without walking anything. Dirty or non-Git trees fall back to `mtime + size + inode`, then per-file BLAKE3. Each node also gets an `api_hash` over sorted canonical public signatures and a `kids_hash` over its direct child API hashes. That borrows the [Merkle-tree idea](https://www.rfc-editor.org/rfc/rfc9162.html#name-merkle-trees) for one level; it is not a home-made recursive Merkle index. Private-only edits usually avoid API churn, unless they change the reference graph.

### OKAY, SHOW ME THE COMMANDS

Radar is still alpha, so install it from the checkout:

```bash
git clone https://github.com/sanix-darker/radar
cd radar && cargo install --path .
cd /path/to/your/huge-repo

radar scan --json       # files, languages, defs, refs and cache hits
radar map               # MAP.md tree + SYMBOLS.md + auto routes
radar init              # AGENTS/CLAUDE contract + repo-scoped skill
radar status            # freshness, slots, violations and route counts
```

`tree` shows the map cost before an agent reads it. `ls` is the same command because i also enjoy typing two letters:

```bash
radar tree --depth 2 --ascii
radar ls --depth 2 --ascii
```

```text
.                           syntax      ~312  ok
|-- example                 syntax      ~331  ok
|-- examples                syntax      ~335  ok
|-- scripts                 syntax      ~328  ok
|-- src                     syntax      ~332  ok
`-- tests/corpus/valid      syntax      ~456  ok
```

For navigation and freshness:

```bash
rg '^rotate_token ' .radar/SYMBOLS.md
radar route find "where is token rotation handled?"
radar route add "token rotation" "src/auth/tokens.rs#rotate_token"

radar refresh               # maps + local indices + embedded root
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

The MCP tools are `get_map`, `tree`, `find_symbol`, `refresh_status`, `route_find`, `route_add` and `fill_slot`. Useful when a host wants tools; entirely avoidable when plain files are cheaper.

### NUMBERS, BEFORE THE LINKEDIN CAROUSEL

The [benchmark ledger](https://github.com/sanix-darker/radar/blob/main/BENCHMARKS.md) runs identical agent tasks and records billed tokens, turns, cost and correctness. It also keeps the ugly rounds: the first N=5 300-file run regressed to `0.51x` before a map-placement bug was fixed. Painful ? yes. Cheaper than lying to myself ? also yes.

The agent rows below use generated subjects, headless Haiku and are marked **unisolated**. The last row is a separate synthetic engine benchmark. Measurements, not a blood oath:

<table>
  <thead>
    <tr><th>Measured slice</th><th>Radar</th><th>Bare / before</th><th>Honest result</th></tr>
  </thead>
  <tbody>
    <tr><td>Tiny repo, 12 files</td><td>75,842 input tokens</td><td>75,300</td><td>0.99x; no win, partial Radar answer</td></tr>
    <tr><td>Semantic hub, 300 files, N=10</td><td>62,836 / $0.0278</td><td>273,612 / $0.0803</td><td><strong>4.35x fewer input tokens</strong>, 65% lower cost, 10/10</td></tr>
    <tr><td>Deep monorepo hub, N=5</td><td>62,268 / $0.0275</td><td>515,905 / $0.1467</td><td><strong>8.3x fewer input tokens</strong>, 81% lower cost, 5/5</td></tr>
    <tr><td>Warm solved route, N=10</td><td>$0.0343</td><td>$0.0637</td><td>46% lower cost, 10/10; repeated-query path</td></tr>
    <tr><td>10,000-file full parse</td><td>229 ms</td><td>1,299 ms</td><td>5.7x; engine time, not agent wall time</td></tr>
  </tbody>
</table>

Raw token sums are not the whole bill. Cache reads, cache creation, fixed harness context and extra turns have different prices. This is why the ledger keeps dollars beside tokens and correctness. On a grep-friendly exact needle, Radar can lose turns. On a semantic hub query, bare navigation becomes a casino and the map does its job.

### SO... ANOTHER AGENT FRAMEWORK ?

Nope. Please no. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then a meeting about the council.

Radar does not edit code, send code elsewhere or require a daemon. It is alpha; tiny repos gain almost nothing, unsupported languages do not get semantic extraction, and purpose slots still need a human or host agent if you want prose. These are limitations, not roadmap-shaped features.

The goal is smaller: make expensive agents spend tokens on the change, review and tests, not on discovering `src/auth/jwt/handler.rs` four times. Yvan had the right first-five-minutes idea. I just broke the map into nodes, added hashes and routes, then got slightly too interested in the invoice.

The code is in [**radar**](https://github.com/sanix-darker/radar). `radar map` is the pitch. Everything after that is me refusing to pay for the same `grep` again.

### BONUS

radar offers a browse mode on TUI and on web-server with interactive graph to navigate, using either `radar browse` or `radar serve`

#### ON TERMINAL TUI

![TUI](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-tui.png)

#### ON BROWSER

![BROWSER](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/radar-web.png)

---
