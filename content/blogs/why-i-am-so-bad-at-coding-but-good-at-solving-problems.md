<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/why-i-am-so-bad-at-coding-but-good-at-solving-problems/good-at-programming.png" />

## WHY I AM SO BAD AT CODING BUT GOOD AT SOLVING PROBLEMS

`2026-07-29 08:01AM` • 7 min read • **#dev** **#career** **#debug** **#ai** **#engineering**

---

<div id="toc-container"></div>

---

I have never felt like one of those mythical engineers who open a file, type 40 perfect lines in one shot, sip cold coffee like it is a reward, then move on to the next ticket as if bugs are just shy little creatures waiting to be noticed. That has never been me. I get stuck, i read too much, i open ten tabs, i mumble at stack traces, and sometimes my first draft looks like it was written by a guy trying to escape a fire alarm. But when something is broken, my brain does something else entirely: it slows down and starts building a map. Not a fix yet, just a map. Where did the thing start lying, what is touching it, what changed, and which innocent file is getting blamed for a crime it did not commit.

Funny enough, that habit did not come from software first. It is how i deal with normal life too. If a plan collapses, if something stops working, if i end up in one of those dumb situations where every option looks equally annoying, i do not usualy jump straight to action. I try to understand the shape of the mess first. What are the moving parts, what is fixed, what is noise, what only looks urgent because my brain is panicking a little bit. Then i reduce the problem until it stops feeling like a wall and starts feeling like a badly documented system. Same reflex, different battlefield. Code just happens to be the place where this method pays rent.

That is why i keep saying i am bad at coding, but not bad at solving problems. Solving is not typing fast. Solving is understanding what the problem actualy is, and to get there you need to know how things work deep inside, not just at the shiny demo layer. You need to know where the data comes from, which function mutates it, why the cache stayed stale, why the retry loop changed the timing, why a contract was broken six calls earlier and only exploded here. That dry nerdy stuff is where the truth lives. Call graphs, parser behavior, queue semantics, invariants, process boundaries, state transitions, all those boring little gears people skip when they are in a hurry. Most of the time the real bug is hidden in those gears, quietly laughing.

So my personal technique has become very simple over time: first rebuild the story, then isolate the moving parts, then delete everything that is lying to you. If a problem still feels huge after that, it usually means i still do not understand it well enough. This is also exactly how i approach Jira tickets in my day to day job, except with more pressure, more people in CC, and less room for romantic nonsense. I rewrite the ticket in plain language, define the technical boundary, reproduce the issue with the smallest path i can find, then only touch code when the causal chain is clear in my head. That approach is slower for the first ten minutes and much faster for the next two days, which is a trade i will take every single time.

AI made this even more obvious. A lot of people use it like a slot machine for diffs: prompt in, confidence out, merge request in thirty seconds, and then everybody acts surprised when the patch solves the symptom and quietly keeps the disease. I cannot work like that. If i do not understand the frame first, AI mostly helps me produce faster confusion. But when i already know the entry point, the broken contract, the expected state, and the possible blast radius, then it becomes incredibly useful. It can brute-force variants, draft ugly first passes, compare options, suggest commands, and help me inspect a narrow part of the system without dragging the whole repo into the prompt like a hostage situation.

That same reflex is all over the public things i build too, even when the projects look unrelated from the outside. `shhx.dev`, `zzollo`, `ector`, `s2c`, `gitMark`, `tchaka`, `git-ci`, `cisv-core`, `prev`, `git-action`, they all came from the same internal habit: understand first, reduce second, build third. So yes, maybe i am not a beautiful coder, maybe some of my first drafts look slightly cursed, and maybe i will never be the guy doing live coding backflips on stage. But i know how to sit with a problem until it stops being fog and starts becoming structure. And once it becomes structure, i can usualy beat it.
