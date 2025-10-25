<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/vibe-coding-is-a-good-poison/vibe-code.png" />

## VIBE CODING IS A GOOD POISON

*2025-08-15 07:45AM • 7 min read • #ai #dev #legacy*

---

<div id="toc-container"></div>

---

> Look, let me be brutally honest with you. I'm a backend engineer. I live in the world of databases, APIs, and server logic. My natural habitat is the terminal, not the browser. And a few days ago, I "redesigned" my website... and it was painful as hell.
>
> You see, I have a love...hate toxic relationship with CSS. Actually, scratch that, it's mostly hate.
>
> The first time I tried to center a `<div>`, I spent one full hour on it. This was back in late 2016, and I remember thinking, "With that amount of time, I could've finished a project… or at least fixed some of the bugs haunting me in PHP5." I'm a simple man: if something does the job, I'm good with it. That's why my original website was basically just centered text and some CSS for code snippets. No fancy animations, no sparkles... just pure, uncut functionality.
>
> Oh, and YES, I asked ChatGPT for help. Don't judge me. Some alignments are just evil.
>
> So when I started this current project and used AI to generate CSS, the fucking thing works. Works brilliantly, actually. Better than anything I've ever written when my brain feels like it's running on dial-up internet trying to understand flexbox.
>
> But here's the thing that's keeping me up at night: **I have absolutely no fucking clue how it did it.**

This up there, was from a linkedIn post i made fews days ago,
Welcome to vibe coding, folks. The good poison that's so good at being poison, it makes heroin look like a gateway drug.

### VIBE CODING VS AUTO-COMPLETION: THE GREAT DIVIDE

My big brother and a good friend of mine [Boris Mbarga](https://github.com/elhmn) and I fight about this constantly (the fight started ~2 years when the AI era began).
For him, Copilot is just an advanced auto-completion tool. "It's like having IntelliSense on steroids,"... "You're still writing the code, it's just helping you type faster."

And you know what? He has a point. Traditional auto-completion suggests the next line based on what you're typing, the functions defined... the classes or attributes depening on the context.
so... you're still driving; it's just predicting where you want to turn.

And from my point of view... it's not the same, why ? because it gets between me and my brain.
And that's where vibe coding gets dangerous IMHO.

There's a massive difference between:
- **AUTO-COMPLETION**: You type `for` and it suggests `for (let i = 0; i < array.length; i++)`.
- **VIBE CODING(Copilot/Claudette/chat-G-Pididi)**: You say "create a responsive card layout" and it spits out 50 lines of CSS Grid magic (from another dimension).

With auto-completion, you're still thinking through the logic. With vibe coding, you're outsourcing the thinking itself.
It's more like passing your time reviewing someone code... instead of thinking for yourself at first.

[`Andrej Karpathy`](https://karpathy.ai/) coined "vibe coding" as a kind of AI-assisted coding where you "forget that the code even exists." That's not auto-completion  that's code generation with a side of willful ignorance... well, kind of.

Think about it this way: you join a new company tomorrow. They hand you a codebase. Most of it is legacy code that nobody understands anymore. You're told to maintain and support this digital Frankenstein's monster.
yeah... that, right there is a form of vibe coding. Except instead of inheriting someone else's legacy nightmare, you're creating your own. In real time... (faster...)

## EXAMPLE : My CSS CATASTROPHE

Sooo, let's back to me now,
Here's what happened when I asked AI to create a responsive card layout:

```css
.card-container {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 2rem;
  padding: clamp(1rem, 5vw, 3rem);
}

.card {
  backdrop-filter: blur(10px);
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
  transform: translateY(-4px);
}
```

As a noob and who still thinks tables are a valid layout solution, this might as well be written in ancient Sumerian. Can I explain why it chose `cubic-bezier(0.4, 0, 0.2, 1)` over a simple `ease`? Fuck no. Do I understand why it's using `clamp()` instead of just hardcoding padding? Not a chance.

It's like having a Ferrari but not knowing how to pop the hood. Sure, it gets you where you need to go, but the moment something breaks, you're absolutely cooked/~fucked~.

During this anknowledgment, i noticed, there is 3 major steps when code is spitted like that from a maggic source,

- **First : Euphoria**
"Holy shit, this AI just created better CSS in 30 seconds than I could in 30 minutes! Finally, I can build frontend stuff without wanting to throw my laptop out the window!"

- **Then : Uncertainty**
"Okay, so this works great, but what happens when I need to modify it? Maybe I should... nah, I'll just ask the AI to change it. What could go wrong?"

- **And finally: Terror** (most people don't get to the 2 step by the way)
"The client wants to change the hover animation. The AI's suggestion broke everything. I can't debug this because I don't understand the original code AND I'm terrible at CSS to begin with. Send help."

## WHY IT'S SUCH A GOOD POISON

Because it fucking works... (well, most of the time) first try...For simple stuff. And for someone like me who broke into a cold sweat the last time I had to debug a CSS alignment issue  the speed is intoxicating.

Why spend hours fighting with CSS Grid when AI can give you a working layout in seconds? Why suffer through the pain of centering divs when you can just ask ChatGPT for help?

Boris is right that there's value in AI assistance. But he's wrong about where the line is. Auto-completion enhances your thinking. Vibe coding replaces it.

And I'm right that it gets between you and your brain. But I'm probably wrong to completely dismiss the productivity gains.

The truth? We're both partially right and dangerously wrong.

## THE LEGACY CODE FACTORY

We already have a phrase for code that nobody understands: legacy code. And legacy code is universally despised for good reason.

Vibe coding is essentially a legacy code factory. When you vibe code, you are incurring tech debt as fast as the LLM can spit it out. Every line of AI-generated CSS you don't understand is a potential problem waiting to explode.

25% of startup companies in Y Combinator's Winter 2025 batch had codebases that were 95% AI-generated. These companies are building their entire technical foundation on code that their developers fundamentally don't understand.

What happens when they need to scale? When they need to fix bugs? If you don't understand the code, your only recourse is to ask AI to fix it for you, which is like paying off credit card debt with another credit card.

## THE BOTTOM LINE

Vibe coding is poison. Good poison. The kind that tastes amazing going down but slowly kills you from the inside.

As a backend engineer who's terrible at CSS, AI feels like salvation. But salvation that comes with a hidden cost isn't salvation at all.

Boris and I will keep fighting about this. He'll argue for the productivity gains. I'll argue about the cognitive costs. We're both right. We're both wrong.

But here's what we agree on: never ship code you don't understand. Because when that code breaks, and it will break (yeah, 100% sure lol),  you'll be the one staring at the screen at 3 AM (with 3000mg of caffeine in your blog system), wondering what the fuck you've gotten yourself into.

And trust me, that's not a vibe you want to be coding in.

---

## FURTHER READING

- **[Vibe-coding is a horrible idea. So is dismissing AI-assisted coding.](https://waleedk.medium.com/vibe-coding-is-a-horrible-idea-so-is-dismissing-ai-assisted-coding-d6288b288af7)** - A balanced take on the middle ground.

- **[CSS and "vibe coding": Why good frontend devs still matter](https://medium.com/@karstenbiedermann/css-and-vibe-coding-why-good-frontend-devs-still-matter-in-the-age-of-ai-09797a7f1287)** - Why CSS is challenging for AI.

- **[Top 5 Problems with Vibe Coding](https://www.glideapps.com/blog/vibe-coding-risks)** - Practical risks and limitations.


-----------

