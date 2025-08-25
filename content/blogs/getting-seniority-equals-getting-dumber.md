<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/getting-seniority-equals-getting-dumber/senior.png" />

## GETTING SENIORITY == GETTING DUMBER

`2025-08-25 11:03PM` • 5 min read • **#thoughts** **#career**

---

## DISCLAIMER: FEELINGS OVER FACTS

Everything you're about to read is purely how I'm feeling about this journey. This isn't some universal truth or career advice, it's just one developer's descent into madness, documented for your entertainment. Your mileage may vary. Side effects may include: excessive googling, imposter syndrome, and the sudden urge to read CPU documentation at 3 AM.

If you're a junior dev and this scares you: don't worry, this is just my personal flavor of existential crisis. You might get a completely different one, or even better, you might stay sane. The odds are in your favor if you stick to high-level languages and avoid asking "but why?" too often.

---

## THE DUNNING-KRUGER SPEEDRUN

Remember your first week coding? You were a god. You understood everything. Arrays? Easy. Functions? Child's play. You were basically the Neo of JavaScript, seeing the Matrix in perfectly formatted JSON.

Fast forward a few years, and now you're googling "what is a computer" at 3 AM while your CPU cache misses haunt your dreams. Congratulations, you've leveled up to knowing that you know nothing.

## THE RABBIT HOLE HAS RABBIT HOLES

Started with a simple bug in production. "It'll take 5 minutes," you said. Now you're 17 Stack Overflow tabs deep, reading Intel documentation about x86 instruction sets, and seriously considering whether electrons actually exist or if we've all been pranked by physicists.

My current project has me so deep in the implementation details that I'm literally counting CPU cycles like some kind of silicon accountant. Did you know your innocent array access is actually a complex negotiation between L1, L2, and L3 caches? No? Good for you. Sleep well.

## THE KNOWLEDGE PARADOX

Here's the thing about learning computers: it's like those Russian nesting dolls, except each doll you open reveals two more dolls, and those dolls are on fire, and also they're not dolls, they're abstract concepts that may or may not map to physical reality.

- **Junior Dev**: "I'll just use this library"
- **Mid-level Dev**: "I'll write my own implementation"
- **Senior Dev**: "But what IS memory, really?"
- **Staff Engineer**: *quietly sobbing while reading about quantum tunneling in transistors*

## THE MORE YOU KNOW, THE LESS YOU KNOW YOU KNOW

Every answer spawns three questions. Fix one bug, discover the entire codebase is held together by a comment that says `// DO NOT REMOVE, nobody knows why this works`.

You start questioning fundamental truths. Is UDP really connectionless, or is that just what it wants us to think? Why do we trust compilers? Who compiles the compiler? It's compilers all the way down until you hit assembly, and then it's just electrons doing whatever they want while we pretend we're in control.

## ENTER AI: THE CONFUSION MULTIPLIER

Just when you thought you'd mapped out the edges of your ignorance, AI arrived like that friend who shows up to your house party with 50 uninvited guests. Now we have neural networks making decisions we can't explain, writing code we don't understand, and confidently hallucinating facts with more conviction than a Wikipedia editor on a power trip.

```
Before AI:
┌─────────┐
│My Brain │ ──> "I don't understand computers"
└─────────┘

After AI:
┌─────────┐     ┌──────┐     ┌─────────────┐
│My Brain │ ──> │  AI  │ ──> │ ¯\_(ツ)_/¯  │
└─────────┘     └──────┘     └─────────────┘
                    │
                    └──> "I understand neither computers nor AI"
```

The best part? We're now debugging code written by machines that learned from Stack Overflow answers that were probably wrong to begin with. It's like playing telephone with a algorithm that has ADHD and a philosophy degree.

## THE GREAT ABSTRACTION TOWER OF BABEL

We've built this magnificent tower:

```
         ╔═══════════════╗
         ║      AI       ║  <- Nobody knows what happens here
         ╠═══════════════╣
         ║   Framework   ║  <- Abstraction of abstractions
         ╠═══════════════╣
         ║   Language    ║  <- High-level lies we tell ourselves
         ╠═══════════════╣
         ║   Compiler    ║  <- Dark magic translation layer
         ╠═══════════════╣
         ║   Assembly    ║  <- Where sanity goes to die
         ╠═══════════════╣
         ║   Hardware    ║  <- Angry electrons
         ╠═══════════════╣
         ║   Physics     ║  <- ¿¿¿Quantum something???
         ╚═══════════════╝
              │
              └──> You are here, questioning everything
```

Each layer promises to abstract away the complexity below it. Plot twist: it just adds its own special flavor of chaos. Now I'm not just wondering how my code works, I'm wondering if the AI that helped write it was having an existential crisis too.

## THE COMFORT IN CONFUSION

But here's the beautiful part: once you accept that computers are basically magic rocks we tricked into thinking, and that we've now taught those rocks to teach themselves, everything becomes easier. That imposter syndrome? That's not a bug, it's a feature. It means you've learned enough to know how much you don't know.

So next time someone asks you a "simple" question and you respond with "well, it depends on what you mean by 'variable'", know that you're not getting dumber. You're just seeing the matrix for what it really is: a beautiful, terrifying, incomprehensible mess that somehow, miraculously, occasionally works.

And if it doesn't work? Have you tried turning it off and on again? Because honestly, at this point, that's as scientific as any other solution.

---

## THE SENIORITY PARADOX EQUATION

```
Knowledge accumulated = K
Understanding = U
Time in industry = T

Junior:    K ↑  U ↑  Confidence = 100%
Mid-level: K ↑↑ U ↓  Confidence = 50%
Senior:    K ↑↑↑ U ↓↓ Confidence = undefined
Staff:     K ↑↑↑↑ U = NULL, Confidence = NaN

With AI:   All previous metrics ÷ Math.random()
```

The more senior you get, the more you realize every technical decision is just choosing which flavor of future regret you prefer. And now with AI, we're not even choosing anymore, we're letting the machines choose their own regrets and hoping they align with ours.

## PLOT TWIST: YOU DON'T ACTUALLY NEED THIS PAIN

Here's the thing though, and this is important, **most developers don't need to understand everything, and that's perfectly fine**.

Python doesn't ask you about cache lines. PHP doesn't care if you know what a memory fence is. JavaScript was literally created in 10 days, do you think it wants you to suffer? These languages exist specifically so you DON'T have to think about whether your struct is aligned to a 64-byte boundary for optimal cache performance.

```
Normal Developer Workflow:
┌──────────┐
│Write Code│ ──> "It works!" ──> Ship it !!
└──────────┘

My Workflow (because I hate myself):
┌──────────┐
│Write Code│ ──> "But WHY does it work?"
└──────────┘         │
                     v
              [3 AM rabbit hole]
                     │
                     v
            "What even is a computer?"
```

The reason I'm having an existential crisis about CPU cycles and instruction sets? Because I made the questionable life choice of doing low-level programming as a *hobby*. That's right, while normal people collect stamps or watch Netflix, I'm out here writing C and assembly, playing with embedded systems, wondering why my interrupt handler is causing cache thrashing.

When you're working with languages where a single misplaced pointer can summon nasal demons, where you need to manually manage memory like you're playing Tetris with bytes, where undefined behavior lurks around every corner, yeah, you NEED to know what's actually happening under the hood. Otherwise, you're not programming, you're just gambling with segfaults.

But if you're building web apps in React? Making REST APIs with Django? You don't need this knowledge. The languages were designed to protect you from it. That's not weakness, that's efficiency. That's standing on the shoulders of giants who already suffered so you don't have to.

## THE SELF-INFLICTED WOUND

My problem isn't seniority making me dumber. It's that I voluntarily chose to peek behind the curtain, and now I can't unsee the wizard frantically pulling levers while the whole of Oz runs on hopes and undefined behavior. I'm basically that person who reads the ingredient list on hot dogs, some knowledge just makes life harder.

So if you're feeling dumb because you don't understand CPU architecture, remember: you probably don't need to. Unless, like me, you've decided that writing bootloaders is a "fun weekend project." In which case, welcome to the club. We meet at 3 AM in the debugger, crying over assembly code that should work but doesn't because the CPU has "opinions."

---

*P.S., If you understood why I used `==` instead of `===`, congratulations, you're already too far gone. Welcome to the club. We have undefined cookies and AI-generated coffee that might be sentient.*

*P.P.S, Yes, I spent 20 minutes perfecting those ASCII diagrams instead of fixing that production bug. Priorities.*

*P.P.P.S, To all the Python devs living their best life without knowing what a stack pointer is: you made the right choice. Stay pure. Don't look into the void. The void has pointers, and they're all null.*


-----------
