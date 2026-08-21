<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png"/>

## HOW I HACKED MY TOKEN COSTS WITH MATHS

`2026-07-12 03:51PM` • 8 min read • **#ai** **#agents** **#rust** **#performance** **#opensource** **#cost**

---

<div id="toc-container"></div>

---

### TL;DR

- Coding agents burn tokens when they search for the same files after each task or context reset.
- [**Radar**](https://github.com/Sanix-Darker/radar) maps a repository locally and routes the agent to an exact `file#symbol` anchor.
- The default path uses no model, API, vector database, or network call.
- In the latest live gate, one `radar query` cut fresh input from `15,407` to `4,249` tokens and improved correct runs from `20/21` to `21/21`. Exact literal searches can still be cheaper without Radar.

If you only came for the numbers, you can stop here. I will not be mad. The rest explains the weird bill, how Radar works, and where it can make a query worse.

### THE BILL WAS MOSTLY ORIENTATION

Long time no see... I got busy with work, then with side projects that were apparently also work. One annoying pattern kept coming back. Before changing one function, the agent would reopen the README, list the tree, search several terms, inspect the wrong module, and only then reach the code.

After a context compaction, it often did the same tour again. README, tree, grep, wrong module... wait, did you not just do this? I was paying for amnesia, lol...

There are two ways to react. You can buy a suspicious "UNLIMITED TOKEN ACCOUNT" from a profile picture containing a Lamborghini, or you can stop sending the same useless context. [The grey market is real](https://www.tomshardware.com/tech-industry/artificial-intelligence/chinese-grey-market-sells-claude-api-access-at-90-percent-off-through-proxy-networks-that-harvest-user-data). I chose the boring option.

For a concrete example, imagine a 10,000-file repository and this question:

```text
Where is order persistence handled?
```

Without a route, an agent may do this:

```text
question -> list files -> read docs -> grep "order" -> open candidates
         -> follow imports -> find the storage function -> answer
```

Every file in that tour becomes input tokens. Radar moves the search before the agent:

```text
question -> radar query -> src/orders/store.rs#save_order -> verify source
```

The agent still reads the function and decides what to change. Radar only removes the repeated search.

That limit shaped the design.

### HOW RADAR BUILDS A ROUTE

The first spark came from my friend [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of making it rediscover the repository.

IMHO, one giant map can become another expensive context blob. Radar creates small maps close to the code. Each map covers a bounded part of the tree and points back to source.

```ascii
query
  -> verified learned route
  -> compact source index
  -> small repository overview
  -> bounded MAP.md metadata head
  -> real source#anchor
```

The committed `MAP.md` files are durable routers. Local files such as `.radar/ROUTES.md`, `.radar/SYMBOLS.md`, and `.radar/query.bin` are disposable fast paths. Radar rebuilds them if they disappear or become incompatible.

Radar does not write changes or replace source with summaries. One static local binary finds a source anchor and stops there.

With that narrow scope, the problem became: rank the most likely source anchors. I can measure that work directly.

### THE MATH, WITHOUT THE LECTURE

The default path combines four parts:

1. **Parsers extract source clues.** [Tree-sitter](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) finds definitions, signatures, calls, and imports. Radar stores those clues instead of whole function bodies.
2. **The graph ranks central code.** Files become nodes and references become edges. [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) gives more weight to code referenced by other important code.
3. **Rare terms improve the match.** A symbol such as `rotate_refresh_token` carries more information than `get`. Paths, callers, and package names add more evidence.
4. **Hashes reject stale routes.** BLAKE3 content hashes and Git tree identity show when an index or learned route is old. The exact-answer cache survives only while the source state matches.

A rough mental model is:

```text
route score = exact name + rare terms + path/caller clues + graph importance
```

This process ranks locations. A confident result returns one source anchor. An uncertain result returns at most three candidates. If the source index has no useful match, Radar reads only the metadata head of a relevant map.

I kept only steps that reduced context without hurting quality or latency. The raw reports and decisions are in [`OPTIMIZATIONS.md`](https://github.com/Sanix-Darker/radar/blob/main/OPTIMIZATIONS.md) for anyone who wants the ugly details.

You can run the result from one terminal.

### TRY IT IN ONE MINUTE

The repository is private for now, so you need access. Release `v0.3.0` has four archives: Linux and macOS, each for x86_64 and arm64. Linux archives use static musl binaries, and there is no Windows target. The [release instructions](https://github.com/Sanix-Darker/radar#install-from-a-release) cover the download, checksum verification, and that annoying macOS quarantine bit.

From source, this is the shortest useful path:

```bash
gh repo clone Sanix-Darker/radar
cd radar
cargo install --locked --path .

cd /path/to/your/repository
radar map
radar init
radar query "where is order persistence handled?"
```

A high-confidence response ends with a source anchor:

```text
FINAL SOURCE ANCHOR: src/orders/store.rs#save_order
```

Read that anchor first, then verify it in source. `radar refresh` updates changed areas, `radar status` reports freshness, and `radar check` validates committed maps. Direct `radar query` is the cheapest integration. A read-only MCP server is also available.

Mapping, querying, checking, and refreshing run locally. Radar does not upload source or make a hidden model call. External tools still have their own privacy rules.

I still needed to see if the shorter route survived real tasks.

### WHAT THE NUMBERS ACTUALLY SAY

The live gate used seven navigation tasks over repositories with 12, 300, and 800 files. Each task ran three times in a fresh isolated copy. Direct source search and one complete `radar query` received the same question. "Uncached input" is new context the model had to read, excluding text already in its cache.

<table>
  <thead>
    <tr><th>Navigation path</th><th>Uncached input</th><th>Correct runs</th><th>Reference cost</th></tr>
  </thead>
  <tbody>
    <tr><td>Direct source search</td><td>15,407</td><td>20/21</td><td>$0.155542</td></tr>
    <tr><td>One <code>radar query</code></td><td>4,249</td><td>21/21</td><td>$0.056409</td></tr>
    <tr><td><strong>Change</strong></td><td><strong>72.4% lower</strong></td><td><strong>one more correct run</strong></td><td><strong>63.7% lower</strong></td></tr>
  </tbody>
</table>

![Radar token reduction benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-reduction.png?v=79a12331)

The cost column applies a reproducible reference rate to the recorded token mix. It is not an invoice. The benchmark ledger contains the formula and raw rows.

One ugly row remains: an 800-file literal lookup used `4.1%` more input with Radar. Direct search already works well when the prompt contains the exact rare word, so Radar adds overhead there. The large gains came from subsystem orientation and semantic package questions.

![Radar reference token cost benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-cost.png?v=d900c304)

I then removed the model and asked a stricter question: did the router retrieve the expected source anchor?

<table>
  <thead>
    <tr><th>Router</th><th>Top-1</th><th>Expected anchor retrieved</th><th>Estimated response tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Previous Radar baseline</td><td>2/40</td><td>2/40</td><td>4,042</td></tr>
    <tr><td>TokenSave context command</td><td>2/40</td><td>5/40</td><td>8,421</td></tr>
    <tr><td>Current Radar router</td><td>37/40</td><td>40/40</td><td>847</td></tr>
  </tbody>
</table>

This is a narrow source-routing comparison on Radar's own 123-source quality corpus. Radar does not replace TokenSave's broader editing and repository-intelligence features.

I also timed the binary because saving tokens with a slow lookup would be a bad joke:

<table>
  <thead>
    <tr><th>Local path</th><th>p95 latency</th><th>Model tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Fresh quality query, 123 sources</td><td>9.67 ms</td><td>0</td></tr>
    <tr><td>Fresh lexical query, 10,000 sources</td><td>28.74 ms</td><td>0</td></tr>
    <tr><td>Fresh verified route, 10,000 sources</td><td>35.97 ms</td><td>0</td></tr>
    <tr><td>First resident lexical query</td><td>3.03 ms</td><td>0</td></tr>
    <tr><td>Repeated resident query or route</td><td>&le;0.040 ms</td><td>0</td></tr>
  </tbody>
</table>

![Radar local query latency benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-query-latency.png?v=c1063944)

Every measured path is below `50 ms`. One silly edge case was an unknown query against a root map with 500 child packages. Returning every child made a `9,102` byte hint. Radar now shows six useful names and the omitted count. The response fell to `244` bytes while the 40-query quality gate still passed.

No spreadsheet magic here. Python generates these charts from committed reports. [`BENCHMARKS.md`](https://github.com/Sanix-Darker/radar/blob/main/BENCHMARKS.md) has the prompts, repetitions, expected anchors, commands, cost maths, and raw paths.

### WHERE RADAR HELPS AND WHERE IT DOES NOT

Before somebody reads `72.4%` and applies it to every repository on earth, these results need limits:

- Radar uses graph maths, heuristics, and probabilistic ranking. It can return the wrong source anchor.
- It works best on large projects where orientation is expensive and repeated. A tiny repository or an exact rare-word search can be cheaper without Radar.
- Earlier agent-level development gates used Claude Haiku. The latest reproducible 21-run-per-arm table in this article uses the locked GPT-5.3-Codex-Spark navigator. Local quality and latency tables use no model at all.
- Token use, wall time, and cost vary with repository shape, prompt, hardware, cache state, host, and model.
- Unsupported languages receive less semantic detail, stale maps must be refreshed, and a probable route is never a substitute for reading the code.

### SO... ANOTHER AGENT FRAMEWORK?

Nope. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then another meeting about the council.

Radar is still alpha, and I want it to stay focused on routing.

Expensive agents should spend tokens changing and checking code, not rediscovering the same file four times.

---
