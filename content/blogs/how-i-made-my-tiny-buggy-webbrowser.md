<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/the-weird-concept-of-branchless-programming/head.png" />

## HOW I MADE MY TINY BUGGY WEB BROWSER, xs
`2025-07-15 01:37` • 21+ min read • #c #web #curl


---

<div id="toc-container"></div>

---

You know... i adore the web, yeah yeah, i really do, but the modern browser feels like ordering espresso and getting a six-course prix-fixe, complete with telemetry mints and 400 MB of “just-in-case” JavaScript salad. One tab idles; my fans spin like I’ve fired up Blender on maximum ray tracing. Open DevTools and you can hear Chrome quietly ask for another gig of RAM like a Dickens orphan with better marketing.

After the third round of “why is my laptop branding cattle on my thighs?” I decided to rage-quit politely: **if today’s browsers insist on bench-pressing Electrons, I’ll build one that lifts toothpicks instead.** Enter **xs**, a glorified `main.c` held together by duct tape, libcurl, and the burning desire to read Hacker News faster than my CPU can throttle itself.

Make no mistake: **xs is ugly.** Fonts? Two. CSS? “Yes, ma’am, we have *padding*, would you like fries with that?” Dark mode? Only if you turn your monitor off. But at a svelte sub-1 MB binary, it launches before you can double-check the filename, fetches pages with all the grace of a 1998 dial-up modem, and renders them in a glorious retro pixel buffet. I call it a *pet project* because like any good pet, it occasionally chews the furniture (segfaults), yet I still show it off to friends.

So this intro is the confession: **I didn’t set out to disrupt anything**, I just wanted a browser that doesn’t write a novel to disk every time I type “\:wq” in Vim. What follows in the full article is a guided tour through the world’s tiniest front end stack, equal parts tutorial and therapy session. If your CPU fans are also screaming for mercy, or you just like seeing how far C can be stretched before the undefined behavior police arrive, grab a mug and read on.

---

# 0×01 PART 1 ,  NAKED & AFRAID: FETCH → PARSE → DOM

### 1.1 `main.c` ,  The Browser in 19 Lines (Now Commented to Death)

```c
#include "network.h"   // Internet plumbing
#include "parser.h"    // HTML tree surgery
#include "layout.h"    // Boxology 101
#include "render.h"    // Push pixels

#define BROWSER_NAME "xs"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <url>\n", BROWSER_NAME);
        return 1;                     // Rage-quit if no URL
    }

    char *html   = fetch_url(argv[1]);   // ① BYTES FROM THE CLOUD™
    DOMNode *dom = parse_html(html);     // ② TREE MADE OF TAGS
    LayoutBox *root = layout_dom(dom);   // ③ MATH HAPPENS HERE
    render_tree(root);                   // ④ THROW PAINT AT WALL
}
```

**Conversational walkthrough**

*“Imagine `main.c` as the maître d’ of a chaotic diner:*

1. **Takes your order** (`argv[1]`).
2. **Shouts into the kitchen** (`fetch_url`).
3. **The chef preps the ingredients** (`parse_html`).
4. **A line cook plates it neatly** (`layout_dom`).
5. **Wait-staff plops it on your table** (`render_tree`).

And when you complain the soup is cold, the maître d’ says, “works on my machine” and exits with status 1.”

> **Tiny example:**
> Change the order, call `render_tree` *before* `layout_dom`. Result: an empty black window, the graphical equivalent of a shrug emoji. Congratulations, you’ve invented “fail fast rendering.”

---

### 1.2 Networking ,  libcurl in 40 Lines, Explained Like a Podcast

```c
static size_t write_cb(void *p,size_t s,size_t n,void *u){
    struct Memory *m = u;
    size_t bytes = s * n;                 // packet payload
    char *bigger = realloc(m->data, m->size + bytes + 1);
    if (!bigger) return 0;                // OOM? Return 0 → libcurl aborts
    m->data = bigger;
    memcpy(m->data + m->size, p, bytes);  // copy new bytes
    m->size += bytes;
    m->data[m->size] = '\0';              // NUL-terminate
    return bytes;
}

char *fetch_url(const char *url){
    CURL *c = curl_easy_init();
    struct Memory buf = { malloc(1), 0 };     // start Schrödinger buffer
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_USERAGENT, "xs/0.1 (tiny, mighty)");
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &buf);
    curl_easy_perform(c);                     // 🚀 Dial-up noises here
    curl_easy_cleanup(c);
    return buf.data;                          // caller owns the leak
}
```

**What each line whispers at night**

| Line        | Voice in your head        | One-liner analogy                                                  |
| ----------- | ------------------------- | ------------------------------------------------------------------ |
| `malloc(1)` | “Start small, dream big.” | Buying a thimble to catch a waterfall.                             |
| `realloc`   | “Need more room!”         | Expanding the thimble to a bucket mid-pour.                        |
| `memcpy`    | “No packet left behind.”  | Scooping water from the pipe to the bucket.                        |
| `'\0'`      | “End of story.”           | Putting a period at the end of a sentence so Gumbo doesn’t wander. |

> **Quick demo:**
> Replace `curl_easy_setopt(c, CURLOPT_USERAGENT, ...)` with `"Mozilla/5.0 (flip-phone)"`. Visit google.com. Google will still answer, turns out they welcome retro devices.

**Why not async?**
Because our target audience likes things *small & synchronous*. Add epoll and you’ll burn 400 LOC on error handling alone. We’d rather spend those lines on ASCII art.

---

### 1.3 Parsing With Gumbo ,  Turning Tag Soup Into Tree House

```c
static void walk(GumboNode *g, DOMNode *parent){
    if(g->type == GUMBO_NODE_ELEMENT){
        const char *tag = gumbo_normalized_tagname(g->v.element.tag);
        DOMNode *n = create_dom_node(tag, NULL);
        append_child(parent, n);
        for(size_t i=0;i<g->v.element.children.length;i++)
            walk(g->v.element.children.data[i], n);
    } else if(g->type == GUMBO_NODE_TEXT){
        append_child(parent, create_dom_node("#text", g->v.text.text));
    }
}
```

**Small easy example**

Input HTML:

```html
<h1>Hello <em>xs</em>!</h1>
```

`walk` produces a DOM that looks like:

```
h1
 ├── #text  "Hello "
 └── em
     └── #text  "xs"
```

> Print with:
>
> ```c
> void dump(DOMNode *n,int depth){
>     for(int i=0;i<depth;i++) putchar(' ');
>     printf("%s \"%s\"\n", n->tag, n->text ?: "");
>     for(auto c=n->first;c;c=c->next) dump(c, depth+2);
> }
> ```
>
> **Try it:** the console output is more satisfying than Fortune cookies.

**Why recursion?**
Because HTML is as deeply nested as your emotional issues. And yes, you *could* swap to an explicit stack to avoid blowing the real one, but then where’s the drama?

---

### 1.4 From DOM to “How It Looks” ,  The World’s Shortest Layout Pass

```c
static void layout_box(LayoutBox *b){
    // 1. Content size ,  trivial for text
    b->content.w = strlen(b->dom->text ?: "") * FONT_W;
    b->content.h = FONT_H;

    // 2. Add glorified whitespace
    int p = b->style->padding;
    b->box.w = b->content.w + p*2;
    b->box.h = b->content.h + p*2;

    // 3. Stack children vertically
    int cursor = b->y + p;
    for(LayoutBox *c=b->first;c;c=c->next){
        c->x = b->x + p;
        c->y = cursor;
        layout_box(c);
        cursor += c->box.h + p;
    }
}
```

**Mini exercise**

Change `p = 0`. Voilà, *quirks mode*. Fonts jam together like bumper cars, but hey, you shaved four pixels!

---

### 1.5 Part 1 Recap With Cartoon

```
┌────────┐      ┌────────┐      ┌────────┐      ┌─────────┐
│  bytes │─▶──▶│  DOM   │─▶──▶│ layout │─▶──▶│  pixels  │
└────────┘      └────────┘      └────────┘      └─────────┘
  network         parsing          math            art
```

If you can draw that on a napkin, you can debug 80 % of rendering bugs.

---

# 0×02 PART 2 ,  BOXES → PIXELS → “OH NO, IT SCROLLED”

### 2.1 Layout Again, but Slower: Why You Sometimes Need Two Passes

Real browsers go *measure → place → stretch → re-measure* (a.k.a. layout yoga).
**xs** cheats: one top-down pass plus a retroactive height fix.

> **Simple experiment:**
> Add a long paragraph:
>
> ```html
> <p>Lorem ipsum... (800 chars)</p>
> ```
>
> Watch the window grow vertically until it escapes your monitor. No scrollbars, just pure existential dread.

### 2.2 Painting With SDL ,  Single Loop, Single Beer

```c
for (int y = b->y; y < b->y+b->h; ++y)
    for (int x = b->x; x < b->x+b->w; ++x)
        fb[y*WIDTH + x] = b->style->bg;
```

**Tiny math-class example**

* Window 320 × 200.
* One `<div>` 50 × 20.
* Inner loop writes 1,000 pixels.
* At 60 FPS that’s 60,000 writes, your 1998 Pentium II is smiling.

Want an outline? Add:

```c
if (x==b->x || x==b->x+b->w-1 ||
    y==b->y || y==b->y+b->h-1)
        fb[y*WIDTH+x] = 0x000000ff;   // black border
```

Boom, CSS `border: 1px solid black` in two if-statements.

### 2.3 JavaScript via Duktape ,  One Include, Instant ES2018

```c
duk_eval_string(ctx,
    "document.body.appendChild("
    "  document.createTextNode('JS says hi!') );");
```

After page load, run that string with `duk_peval_string`. A new text node appears, the DOM mutates, `dirty=true` and we repaint the last 100 px of the window. No V8, no ICU, no service workers, just instant gratification.

> **Debug trick:**
> Compile with `-DDUK_F_DEBUG`. Duktape will log *every* property lookup. Terminal scrolls faster than Netflix credits.

### 2.4 Event Loop in 7 Steps (Kid-Friendly Version)

1. Ask SDL, “Got input?”
2. If key pressed → run JS handler.
3. JS mutates DOM.
4. Mark region dirty.
5. Call `layout_dom` on that subtree.
6. Overwrite those pixels.
7. **goto 1** forever or until ^C.

> **Small example:**
> Bind `onkeypress="this.remove()"`. Press any key and the element deletes itself, the parent shrinks, and you see instant *cascading collapse*, the fun kind.

### 2.5 Fonts & Friends ,  Quality of Life in Two Bullet Points

* Ship **DejaVuSans.ttf** (≈300 KB). Load once, reuse glyph metrics for every `<p>`.
* Print `printf("%c", 219);` before each paint, in text mode you get a real-time bar chart showing how many boxes you drew. Totally useless, oddly soothing.

---

## 0×FE SPEEDRUN TIPS (Because Attention Spans)

| “Feature”      | 10-Dollar Fix                                                               |
| -------------- | --------------------------------------------------------------------------- |
| **Links**      | On click, call `system("xs %s &");` ,  tabs done.                            |
| **Images**     | Use `stb_image.h`, blit raw RGB. PNG? Sure. GIF? Only if you hate yourself. |
| **CSS Colors** | Parse `#RRGGBB` with `strtol(ptr+1, NULL, 16)`. Done.                       |
| **Scrolling**  | Move everything up by `scrollOffset`, repaint. Whee!                        |

---

# 0×FF EPILOGUE ,  `return EXIT_SUCCESS;`

Congratulations, you’ve just taken a whirlwind tour through:

1. **Networking** (tcpdump optional, caffeine mandatory)
2. **HTML Parsing** (the part where mistakes become “quirks mode”)
3. **Layout Math** (Pythagoras meets Comic Sans)
4. **Software Rasterising** (writing every pixel by hand, grandpa would be proud)
5. **JavaScript Glue** (enough to `alert('It works!')`)

The whole adventure fits in a binary smaller than most JavaScript frameworks’ *logo*.
Hacker News will still ask, “But does it run WebGPU?”, you can smile and reply, “Sure, copy-pasté me the spec and a time machine.”

---

## ∎ CONCLUSION ,  `return EXIT_SUCCESS /* and maybe a pizza */`

And that’s the whole circus tent: a browser the size of a floppy disk, painted with the aesthetic charm of a late-90 s shareware installer, yet scrappy enough to load the very page you’re reading. **xs** doesn’t minify your carbon footprint or re-invent UX; it just bootstraps a slice of the web without demanding a blood sacrifice from your RAM. Every launch reminds me that performance isn’t magic, it’s math, sweat, and the stubborn refusal to ship more bytes than necessary.

If you measure success in kilobytes saved and giggle-inducing `printf`s, then congratulations, you’re already part of this misguided success story. Fork it, break it, port it to your smart toaster. Swap SDL for raw frame-buffer writes, or rename `fetch_url()` to `fetch_bacon()` and call it a dietary browser. Nobody’s stopping you; that’s the beauty of tiny code and tinier expectations.

Will **xs** ever handle WebGL, DRM, or the 74th synonym for “cookie banner”? Probably not, those specs alone weigh more than its entire executable. But as a daily driver for text-first sites, docs, and the occasional XKCD binge, it’s shockingly enough. And every time Chrome politely recommends 423 pending background updates, I smile, type `./xs https://textise dot something`, and savor the silence of a fan that never spins up.

So here’s the final commit message: *“Feature: contentment. Size: unchanged.”* Keep hacking, keep trimming off the bloat, and remember, ugly code that works is still prettier than the perfect idea that never shipped.

**Until Part 3 (“GPU shenanigans & speculative tabbed browsing”), happy hacking, and may your NULL checks be plentiful and your segfaults poetic.**




-----------
