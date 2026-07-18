<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-hacked-my-token-cost-burnt/rdr-icon.png"/>

## HOW I HACKED MY TOKEN/COST BURNT WITH MATHS

`2026-07-12 03:51PM` • 9 min read • **#ai** **#agents** **#rust** **#performance** **#opensource** **#cost**

---

<div id="toc-container"></div>

---

### TL;DR

- Coding agents waste tokens finding the same files again after every new task or context reset.
- [**Radar**](https://github.com/Sanix-Darker/radar) builds a small local map of a repository, ranks likely source locations, and sends the agent to an exact `file#symbol` anchor, meaning one file and one function.
- The default path uses no model, API, vector database or network call. It is a Rust binary using parsers, a graph, PageRank, term rarity, hashes and a tiny bounded cache.
- In the latest live gate, one `radar query` cut fresh input from `15,407` to `4,249` tokens while improving correct runs from `20/21` to `21/21`.
- It helps most on large projects where finding the right code is expensive. It is not a magic discount for every prompt.

That is the whole story. The rest explains why repository orientation costs money, how a few boring algorithms reduce it, and where the benchmarks say Radar does not help.

### THE BILL WAS MOSTLY ORIENTATION

Long time no see... I got busy with work, then with side projects which were apparently also work. One annoying pattern kept appearing in my agent sessions: before changing one function, the agent would reopen the README, list the tree, search five terms, inspect the wrong module, and only then reach the code.

After compaction it often did the same tour again. I was paying for amnesia.

There are two ways to react. You can buy a suspicious "UNLIMITED TOKEN ACCOUNT" from a profile picture containing a Lamborghini, or you can stop sending the same useless context. [The grey market is real](https://www.tomshardware.com/tech-industry/artificial-intelligence/chinese-grey-market-sells-claude-api-access-at-90-percent-off-through-proxy-networks-that-harvest-user-data). I chose the boring option.

Here is a simple example. Imagine a 10,000-file repository and this question:

```text
Where is order persistence handled?
```

Without a route, an agent may do this:

```text
question -> list files -> read docs -> grep "order" -> open candidates
         -> follow imports -> find the storage function -> answer
```

Every file in that chain becomes input tokens. Radar moves the mechanical part before the agent:

```text
question -> radar query -> src/orders/store.rs#save_order -> verify source
```

The agent still reads the real function and makes the decision. It simply starts on the right street instead of buying a map of the entire city on every trip.

### THE IDEA: MAPS ROUTE, SOURCE ANSWERS

The first spark came from my friend [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of making it rediscover the whole repository. He was right about the first five minutes.

My problem was that one giant map can become another expensive context blob. Radar therefore creates small maps close to the code. Each map describes a bounded part of the tree and points back to source. A map answers "where should I look?" It never pretends to answer "what does this code do?"

```ascii
query
  -> verified learned route
  -> compact source index
  -> small repository overview
  -> bounded MAP.md metadata head
  -> real source#anchor
```

The committed `MAP.md` files are the durable routers. Local files such as `.radar/ROUTES.md`, `.radar/SYMBOLS.md` and `.radar/query.bin` are disposable fast paths. If one disappears or becomes incompatible, Radar rebuilds it from the repository.

This is deliberately narrower than a whole-repository packer or an agent framework. Radar does not try to write the change, manage a council of agents or replace source code with summaries. Its advantage is focus: one static local binary finds a small answer-shaped route, then gets out of the way.

That narrow goal turns the problem from "make a model understand the repository" into "rank the most likely source anchors." The second problem is cheaper and easier to measure.

### THE MATH, WITHOUT THE LECTURE

Radar is LLM-free by default. Its useful behavior comes from four ordinary ideas:

1. **Parsers make street signs.** [Tree-sitter](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) extracts definitions, signatures, calls and imports from supported languages. Radar stores compact clues, not entire function bodies.
2. **A graph finds busy intersections.** Files become nodes and references become edges. [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) gives more weight to source that other important source points toward.
3. **Rare words carry more information.** An exact symbol such as `rotate_refresh_token` should matter more than a common word such as `get`. Path names, callers and package intent add more evidence.
4. **Hashes protect freshness.** BLAKE3 content hashes and Git tree identity tell Radar when an index or learned route is stale. A small exact-answer cache only survives while that source state remains valid.

A rough mental model is:

```text
route score = exact name + rare terms + path/caller clues + graph importance
```

That is ranking, not understanding. A confident result returns one source anchor. An uncertain result returns at most three bounded candidates. If the source index still has no useful answer, Radar reads only the metadata head of a relevant map instead of loading the whole file.

The important part is not that any one algorithm is exotic. The important part is that each stage removes work before expensive context is created.

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

Once the small deterministic path survived those tests, using it became simple.

### TRY IT IN ONE MINUTE

The private repository currently requires access. Release `v0.3.0` provides four archives: Linux and macOS, each for x86_64 and arm64. Linux archives use static musl binaries, and there is no Windows target. The [release instructions](https://github.com/Sanix-Darker/radar#install-from-a-release) cover download, checksum verification and macOS quarantine removal.

From source, the shortest useful path is:

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

Use that anchor as the first read, then verify the answer in source. `radar refresh` updates changed areas, `radar status` reports freshness, and `radar check` validates committed maps. The direct `radar query` command is the cheapest default integration; the read-only MCP server remains optional for hosts that need it.

The privacy boundary is equally small. Mapping, querying, checking and refreshing run locally without uploading source or calling a model. Commands that explicitly launch an external tool follow that tool's privacy rules.

Now the only useful question is whether this shorter route survives real tasks.

### WHAT THE NUMBERS ACTUALLY SAY

The latest live gate used seven navigation tasks over repositories with 12, 300 and 800 files. Each task ran three times in a fresh isolated copy. Direct source search and one complete `radar query` command received the same question. "Uncached input" below means new context the model had to read, excluding text already held in its cache.

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

![Radar token reduction benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-reduction.png)

The cost column is a reproducible reference scenario applied to the recorded token mix, not an invoice. The full rate formula and raw rows are in the benchmark ledger.

One 800-file literal lookup used `4.1%` more input with Radar. That result stays visible because direct search is already excellent when a prompt contains the exact rare word. The large gains came from subsystem orientation and semantic package questions, where the agent otherwise had to explore.

![Radar reference token cost benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-cost.png)

The second gate removes the model entirely and asks a stricter question: did the router retrieve the expected source anchor?

<table>
  <thead>
    <tr><th>Router</th><th>Top-1</th><th>Expected anchor retrieved</th><th>Estimated response tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Previous Radar baseline</td><td>2/40</td><td>2/40</td><td>4,042</td></tr>
    <tr><td>TokenSave context command</td><td>2/40</td><td>5/40</td><td>8,421</td></tr>
    <tr><td>Current Radar router</td><td>37/40</td><td>40/40</td><td>835</td></tr>
  </tbody>
</table>

This is a narrow source-routing comparison, not a claim that Radar replaces TokenSave's broader editing and repository-intelligence features. On this 10,000-file corpus, Radar returned the expected anchor for all 40 queries with about one fifth of its old response volume.

Finally, local latency is measured separately from agent wall time:

<table>
  <thead>
    <tr><th>Local path</th><th>p95 latency</th><th>Model tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Fresh 40-query quality corpus</td><td>39.56 ms</td><td>0</td></tr>
    <tr><td>Fresh verified route</td><td>35.75 ms</td><td>0</td></tr>
    <tr><td>First resident lexical query</td><td>3.76 ms</td><td>0</td></tr>
    <tr><td>Repeated resident query or route</td><td>0.038 to 0.044 ms</td><td>0</td></tr>
  </tbody>
</table>

![Radar local query latency benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-query-latency.png)

One isolated cold lexical stress probe still reaches `53.97 ms`; it is not hidden. The full quality corpus and verified routes remain below `50 ms`, while resident requests are far below it.

The charts are generated by Python from committed JSON/JSONL reports, not typed into a spreadsheet. [`BENCHMARKS.md`](https://github.com/Sanix-Darker/radar/blob/main/BENCHMARKS.md) documents prompts, repetitions, expected anchors, reproduction commands, cost math and raw report paths for readers with repository access.

Numbers need boundaries, especially when models and repositories behave differently.

### DISCLAIMER: WHAT THESE RESULTS MEAN

- Radar is not a magical solution. It uses graph mathematics, heuristics and probabilistic ranking to suggest where to read. The source still has to confirm the answer.
- It works best on large projects where orientation is expensive and repeated. A tiny repository or an exact rare-word search can be cheaper without Radar.
- Earlier agent-level development gates used Claude Haiku. The latest reproducible 21-run-per-arm table in this article uses the locked GPT-5.3-Codex-Spark navigator. Local quality and latency tables use no model at all.
- Token use, wall time and cost vary with repository shape, prompt, hardware, cache state, host and model. The cost values above are a declared reference scenario, not a billing promise.
- Unsupported languages receive less semantic detail, stale maps must be refreshed, and a probable route is never a substitute for reading the code.

Those limits are why Radar stays a navigation layer instead of trying to become the agent itself.

### SO... ANOTHER AGENT FRAMEWORK?

Nope. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then a meeting about the council.

Radar has no daemon, API key or model on its default path, but it is still alpha. Tiny repositories may not save enough orientation to matter. Unsupported languages receive less semantic detail. Purpose prose still needs a human or an external command chosen by the user.

The goal is smaller: let expensive agents spend tokens on the change, review and tests, not on discovering the same source file four times.

`radar map` builds the dots. `radar query` finds the route. Source still answers.

---
