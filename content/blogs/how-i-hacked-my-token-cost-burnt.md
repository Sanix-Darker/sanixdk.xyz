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

That's basically it. If you only came for the numbers, you can stop here, i won't be mad. The rest explains the weird bill and where Radar actualy makes it worse.

### THE BILL WAS MOSTLY ORIENTATION

Long time no see... I got busy with work, then with side projects which were apparently also work. One annoying pattern kept coming back. Before touching one function, the agent would reopen the README, list the tree, search five terms, inspect the wrong module, and only then reach the code.

After a context compaction it often did the same tour again. README, tree, grep, wrong module... wait, didn't you just do this? I was paying for amnesia.

There are two ways to react. You can buy a suspicious "UNLIMITED TOKEN ACCOUNT" from a profile picture containing a Lamborghini, or you can stop sending the same useless context. [The grey market is real](https://www.tomshardware.com/tech-industry/artificial-intelligence/chinese-grey-market-sells-claude-api-access-at-90-percent-off-through-proxy-networks-that-harvest-user-data). I chose the boring option.

For a less abstract example, imagine a 10,000-file repository and this question:

```text
Where is order persistence handled?
```

Without a route, an agent may do this:

```text
question -> list files -> read docs -> grep "order" -> open candidates
         -> follow imports -> find the storage function -> answer
```

Every file in that little tour becomes input tokens. Radar moves the mechanical part before the agent:

```text
question -> radar query -> src/orders/store.rs#save_order -> verify source
```

The agent still reads the real function and makes the decision. It just starts on the right street instead of buying a map of the entire city on every trip.

### THE IDEA: MAPS ROUTE, SOURCE ANSWERS

The first spark came from my friend [**Yvan**](https://gtindo.dev/posts/gt-workflow-presentation/). His workflow starts an agent with a `MAP.md` instead of making it rediscover the whole repository. He was right about those first five minutes, honestly.

The catch is that one giant map can become another expensive context blob. So Radar creates small maps close to the code. Each one describes a bounded part of the tree and points back to source. A map answers "where should I look?" It never pretends to answer "what does this code do?"

```ascii
query
  -> verified learned route
  -> compact source index
  -> small repository overview
  -> bounded MAP.md metadata head
  -> real source#anchor
```

The committed `MAP.md` files are the durable routers. Local files such as `.radar/ROUTES.md`, `.radar/SYMBOLS.md` and `.radar/query.bin` are disposable fast paths. If one disappears or becomes incompatible, Radar rebuilds it from the repository.

I did not want another whole-repository packer or agent framework. Radar does not write changes or replace source with summaries. One static local binary finds a useful route, then gets out of the way.

Once i kept the scope that small, the problem changed from "make a model understand the repository" to "rank the most likely source anchors." The second one is much cheaper, and I can actualy measure it.

### THE MATH, WITHOUT THE LECTURE

No model is hiding behind the default path. Most of the useful behavior comes from four pretty ordinary ideas:

1. **Parsers give me street signs.** [Tree-sitter](https://tree-sitter.github.io/tree-sitter/using-parsers/queries/) extracts definitions, signatures, calls and imports. Radar keeps those small clues, not whole function bodies.
2. **The graph shows busy intersections.** Files become nodes and references become edges. [PageRank](https://snap.stanford.edu/class/cs224w-readings/Brin98Anatomy.pdf) gives more weight to code that other important code points toward.
3. **Rare words beat generic words.** A symbol such as `rotate_refresh_token` says a lot more than `get`. Paths, callers and package names add a few more clues.
4. **Hashes stop old answers hanging around.** BLAKE3 content hashes and Git tree identity tell Radar when an index or learned route is stale. The tiny exact-answer cache only survives while the source state still matches.

A rough mental model is:

```text
route score = exact name + rare terms + path/caller clues + graph importance
```

This is still ranking, not understanding. A confident result returns one source anchor. An uncertain one returns at most three candidates. If the source index has no useful answer, Radar reads only the metadata head of a relevant map instead of eating the whole file.

None of this is exotic, and that is the point. Each stage removes work before expensive context exists. If a clever idea hurt quality or latency, it went back out. The raw reports and decisions are in [`OPTIMIZATIONS.md`](https://github.com/Sanix-Darker/radar/blob/main/OPTIMIZATIONS.md) for anyone who wants the ugly details.

Anyway, the part that survived is small enough to try in a minute.

### TRY IT IN ONE MINUTE

The repository is private for now, so you need access. Release `v0.3.0` has four archives: Linux and macOS, each for x86_64 and arm64. Linux archives use static musl binaries, and there is no Windows target. The [release instructions](https://github.com/Sanix-Darker/radar#install-from-a-release) cover download, checksum verification and that annoying macOS quarantine bit.

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

Read that anchor first, then verify it in source. `radar refresh` updates changed areas, `radar status` reports freshness, and `radar check` validates committed maps. Direct `radar query` is the cheapest integration. The read-only MCP server is still there if needed.

Mapping, querying, checking and refreshing run locally. No source upload, no hidden model call. Explicitly launched external tools still have their own privacy rules.

Nice story, sure... but I wanted to see if the shorter route survived real tasks.

### WHAT THE NUMBERS ACTUALLY SAY

The live gate used seven navigation tasks over repositories with 12, 300 and 800 files. Each ran three times in a fresh isolated copy. Direct source search and one complete `radar query` got the same question. "Uncached input" means new context the model had to read, excluding text already in its cache.

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

The cost column is me applying a reproducible reference rate to the recorded token mix. It is not an invoice. The formula and raw rows are in the benchmark ledger.

There is one ugly row: an 800-file literal lookup used `4.1%` more input with Radar. Direct search is already excellent when the prompt contains the exact rare word, so Radar only adds overhead there. I left it in. The large gains came from subsystem orientation and semantic package questions, where the agent otherwise had to wander around.

![Radar reference token cost benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-token-cost.png?v=d900c304)

Then I removed the model entirely and asked a stricter question: did the router retrieve the expected source anchor?

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

To be clear, this is a narrow source-routing comparison. Radar does not replace TokenSave's broader editing and repository-intelligence features. On Radar's own 123-source quality corpus, it returned the expected anchor for all 40 questions with about one fifth of its old response volume.

I also timed the binary itself, because saving tokens with a slow lookup would be a pretty bad joke:

<table>
  <thead>
    <tr><th>Local path</th><th>p95 latency</th><th>Model tokens</th></tr>
  </thead>
  <tbody>
    <tr><td>Fresh quality query, 123 sources</td><td>9.67 ms</td><td>0</td></tr>
    <tr><td>Fresh lexical query, 10,000 sources</td><td>28.74 ms</td><td>0</td></tr>
    <tr><td>Fresh verified route, 10,000 sources</td><td>35.97 ms</td><td>0</td></tr>
    <tr><td>First resident lexical query</td><td>3.03 ms</td><td>0</td></tr>
    <tr><td>Repeated resident query or route</td><td>0.035 to 0.040 ms</td><td>0</td></tr>
  </tbody>
</table>

![Radar local query latency benchmark](/assets/how-i-hacked-my-token-cost-burnt/radar-query-latency.png?v=c1063944)

All six measured paths are now below `50 ms`. The silly edge case was an unknown query against a root map with 500 child packages: returning every child made a `9,102` byte hint. Radar now shows six useful names and the omitted count. Same result, `244` bytes, and the 40-query quality gate still passes.

No spreadsheet magic here. Python generates these charts from committed reports. [`BENCHMARKS.md`](https://github.com/Sanix-Darker/radar/blob/main/BENCHMARKS.md) has the prompts, repetitions, expected anchors, commands, cost math and raw paths.

Before somebody reads `72.4%` and applies it to every repository on earth, a few limits:

### DISCLAIMER: WHAT THESE RESULTS MEAN

- Radar is not a magical solution. It uses graph maths, heuristics and probabilistic ranking to suggest where to read. The source still has to confirm the answer.
- It works best on large projects where orientation is expensive and repeated. A tiny repository or an exact rare-word search can be cheaper without Radar.
- Earlier agent-level development gates used Claude Haiku. The latest reproducible 21-run-per-arm table in this article uses the locked GPT-5.3-Codex-Spark navigator. Local quality and latency tables use no model at all.
- Token use, wall time and cost vary with repository shape, prompt, hardware, cache state, host and model. The cost values above are a declared reference scenario, not a billing promise.
- Unsupported languages receive less semantic detail, stale maps must be refreshed, and a probable route is never a substitute for reading the code.

That is why Radar stays a navigation layer instead of pretending to be the agent itself.

### SO... ANOTHER AGENT FRAMEWORK?

Nope. The world has enough frameworks asking for seven folders, three philosophies, a council of agents, then another meeting about the council.

Radar has no daemon, API key or model on its default path, but it is still alpha. It can tell an agent where to read. It cannot decide what the code means for you, and honestly I don't want it to.

That's all I want from it: let expensive agents spend tokens on the change, review and tests, not on discovering the same source file four times.

`radar map` builds the dots. `radar query` finds the route. Source still answers.

---
