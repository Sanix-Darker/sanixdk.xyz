![Garou looking just as surprised as you are that this series still exists.](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-make-a-password-generator-using-brainfuck/garou.png)

## HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 2 / 3)

`2025-06-29  11:20 PM` • 22 min read • **#bash #brainfuck #terminal #c**

> **NOTE:** Before you dive in, make sure you’ve read [Part 1](https://sanixdk.xyz/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3), otherwise nothing here will make sense, and you’ll blame me, and I’ll blame coffee.

---

> **JEEEZZZZ, Seven. Months. Later hun...**
>
> Yes, yes, I know, that’s longer than a celebrity marriage.
>
> My laziness defeated my “weekly” motivation, *but* we’re back, so let’s fry our brains once more!

---

### OBJECTIVES *(a.k.a. “Why am I still doing this?”)*

1. Patch a few Brainfuck mysteries we glossed over in Part 1.
2. Fix our glitchy **BV** (Brainfuck Visualizer).
3. Summon **CHAOS**, a.k.a. randomness, for password fun.
4. Sneak-peek a shiny new Password Generator.

---

## BRIEF RECAP

Since i got some questions from the precedent part... let's answer some,

### “Can you explain Brain-fck in one sentence?”

> Imagine an infinitely long tape.
> `>` and `<` scoot your cursor along it,
> `+` and `-` raise or lower the current cell’s value,
> `[` and `]` loop like a hamster wheel,
> `,` and `.` handle input/output,
> **Voilà, that’s Brainfuck.**

(Okay, two sentences. Sue me.)

---

### “How do numbers become ‘Hello World’?”

Short answer: **ASCII sorcery**.

```bash
$ echo "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++." | bf /dev/stdin
Hello World!
```

When a cell hits, say, `72`, the interpreter **calls `putchar(72)`**, and … boom, `H` appears.

Need proof?

```bash
# Bash one-liner
$ echo $(printf \\$(printf '%03o' 72))
H
```

Or, if you still trust C:

```c
#include <stdio.h>
int main() {
    putchar(72);  // H
    putchar(101); // e
    putchar(121); // y
    putchar(10);  // \n
}
```

Compile, run, enjoy:

```bash
$ gcc hey.c -o hey && ./hey
Hey
```

So the Brainfuck tape `[72][101][10]` spells “Hey”, and the minimal BF to do that is:

```brainfuck
-[------->+<]>-.-[->+++++<]>++.[--->+<]>++.
```

Magic? Nope, just lovingly-abused pointers.

---

## WHAT WAS WRONG WITH OUR BV?

In Part 1 we heroically ignored `,` and `.` because *simplicity* (lazy demons in me).

We also assumed a **1-D tape** (boring) and left the code about as stable as a Jenga tower in an earthquake.

Run the classic “Hello World” through the old BV and it coughs up mismatched brackets and existential dread. Time for a refactor.

---

### Triage & Tweaks

1. **Shrink memory** to the sane-ish 30 000 cells.
2. **Dynamic stack**, no more hard-wiring the call depth.
3. **Cache `strlen`** once; your CPU thanks you.
4. **Show only the juicy cells** around the pointer.
5. **Detect bracket mismatches** before they detect you.
6. **Beam me up, Z-axis**, yes, we upgraded to a 3-D tape for extra madness.

---

### Key Surgery (highlight reel)

Below are *tiny* snippets, full code’s at the end if you’re a masochist.

```c
#define MEMORY_DEPTH 2    // New Z-axis
#define MEMORY_ROWS  7
#define MEMORY_COLS  8    // X-axis

// Dynamic stack because recursion isn’t the only way to segfault
typedef struct Stack {
    int *items;
    int  top;
    int  capacity;
} Stack;
```

```c
void display_memory(int mem[][MEMORY_ROWS][MEMORY_COLS],
                    int d, int r, int c) {
    fprintf(stderr, "----- Memory -----\n");
    for (int z = 0; z < MEMORY_DEPTH; z++) {
        fprintf(stderr, "\nLayer %d\n", z);
        for (int y = 0; y < MEMORY_ROWS; y++) {
            for (int x = 0; x < MEMORY_COLS; x++) {
                fprintf(stderr, "[%2d]", mem[z][y][x]);
            }
            if (z == d && y == r) {
                // cute arrow
                for (int x = 0; x < MEMORY_COLS; x++)
                    fprintf(stderr, x == c ? " ^ " : "   ");
            }
            fprintf(stderr, "\n");
        }
    }
}
```

```c
case '[':                      // Enter loop
    if (cell) push(&stack, i); // remember where we came from
    else       SKIP_TO_MATCH(); // jump over the loop
    break;
case ']':                      // Exit loop
    if (is_empty(&stack)) PANIC("Mismatched ]");
    if (cell) i = peek(&stack); // loop again
    else      pop(&stack);
    break;
```

Want the entire 350-line beast? keep scrolling to the appendix.

---

## SUMMONING CHAOS (Randomness 101)

A password generator without randomness is just **predictable sadness**.
Next time we’ll wire up entropy sources, sprinkle in `/dev/urandom`, and let Brainfuck dance.

*Teaser:*

```c
unsigned char random_byte(void) {
    FILE *f = fopen("/dev/urandom", "rb");
    unsigned char b;
    fread(&b, 1, 1, f);
    fclose(f);
    return b;
}
```

> *Yes, Windows folks, you’ll need another trick, stay tuned (and pray...).*

---

## FIRST GLIMPSE AT THE PASSWORD GENERATOR

Picture this:

```text
$ ./bf-pwgen 16
9Yf%kC+u@w$r4LzQ
```

* **Length**: user-choosable.
* **Character set**: printable ASCII 33-126, or tweak it.
* **Visualization**: our shiny 3-D BV lights up like a Christmas tree while the tape mutates.

More on that in Part 3, where we’ll:

1. Fold randomness into Brainfuck loops.
2. Export passwords to stdout *and* a QR code (because why not).
3. Benchmark against *real* generators, and maybe win?

---

## DEMO (SHOW TIIIIMME)

On a 3D Visualization for two grid, the bf visualizer works like this :

<video style="width: 100%;height: 80%;" controls>
  <source src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-make-a-password-generator-using-brainfuck/demo2D.mp4" type="video/mp4">
</video>

---

## FULL SOURCE (Appendix)

<details><summary>Click to expand 340 lines of weird C code written by a weird nerd.</summary>

> Warning: long code ahead. Skip if you value your retina and you have a life.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMORY_DEPTH 2   // New depth size for the z-axis
#define MEMORY_ROWS 7    // Define vertical size
#define MEMORY_COLS 8   // Define horizontal size

typedef struct Stack {
    int* items;
    int top;
    int capacity;
} Stack;

void init_stack(Stack* s, int capacity) {
    s->items = (int*)malloc(capacity * sizeof(int));
    if (!s->items) {
        fprintf(stderr, "Woups: Memory allocation failed for stack\n");
        exit(1);
    }
    s->top = -1;
    s->capacity = capacity;
}

void free_stack(Stack* s) {
    free(s->items);
}

int is_empty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, int value) {
    if (s->top >= s->capacity - 1) {
        fprintf(stderr, "Woups: Stack overflow\n");
        exit(1);
    }
    s->items[++s->top] = value;
}

int pop(Stack* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Woups: Stack underflow\n");
        exit(1);
    }
    return s->items[s->top--];
}

void display_memory(int memory[][MEMORY_ROWS][MEMORY_COLS], int depth_pointer, int row_pointer, int col_pointer) {
    fprintf(stderr, "----- Memory Block Display ------\n");

    for (int depth = 0; depth < MEMORY_DEPTH; ++depth) {
        fprintf(stderr, "\n________________________________%d\n", depth); // depth

        for (int row = 0; row < MEMORY_ROWS; ++row) {
            fprintf(stderr, "|");  // Labeling rows

            for (int col = 0; col < MEMORY_COLS; ++col) {
                if (memory[depth][row][col] != 0) {
                    fprintf(stderr, "[%2d]", memory[depth][row][col]); // Proper spacing
                } else {
                    fprintf(stderr, "[  ]");  // For alignment of zero values
                }
            }
            fprintf(stderr, "\n");

            // Print the pointer only in relevant row and depth
            if (depth == depth_pointer && row == row_pointer) {
                for (int col = 0; col < MEMORY_COLS; ++col) {
                    if (col == col_pointer) {
                        // TODO: uncomment this if needed
                        fprintf(stderr, "   ^  ");  // Arrow pointing to the current cell
                    } else {
                        fprintf(stderr, "    ");  // Filler for alignment consistency
                    }
                }
                // TODO: uncomment this if needed
                fprintf(stderr, "\n");
            }
        }
    }
    fprintf(stderr, "---------------------------------\n");
}

int memory_shift_printer(const char *input) {
    int memory[MEMORY_DEPTH][MEMORY_ROWS][MEMORY_COLS] = {{{0}}}; // Initialize 3D array
    int depth_pointer = 0;
    int row_pointer = 0;
    int col_pointer = 0;

    Stack stack;
    init_stack(&stack, 1024);
    size_t input_length = strlen(input);

    for (size_t i = 0; i < input_length; ++i) {
        switch (input[i]) {
            case '>':
                if (col_pointer < MEMORY_COLS - 1) col_pointer++;
                break;
            case '<':
                if (col_pointer > 0) col_pointer--;
                break;
            case '+':
                memory[depth_pointer][row_pointer][col_pointer]++;
                break;
            case '-':
               if (memory[depth_pointer][row_pointer][col_pointer] > 0) memory[depth_pointer][row_pointer][col_pointer]--;
                break;
            case '^':  // Move up in the memory matrix
                if (row_pointer > 0) row_pointer--;
                break;
            case 'v':  // Move down in the memory matrix
                if (row_pointer < MEMORY_ROWS - 1) row_pointer++;
                break;
            case '~':  // Move between depths in the memory matrix
                if (depth_pointer < MEMORY_DEPTH - 1) depth_pointer++;
                break;
            case '%':  // Move backward between depths in the memory matrix
                if (depth_pointer > 0) depth_pointer--;
                break;
            case '[':
                if (memory[depth_pointer][row_pointer][col_pointer] != 0) {
                    push(&stack, i);
                } else {
                    int loop = 1;
                    while (loop > 0) {
                        if (++i == input_length) {
                            fprintf(stderr, "Woups: Mismatched brackets\n");
                            free_stack(&stack);
                            return 1;
                        }
                        if (input[i] == '[') loop++;
                        if (input[i] == ']') loop--;
                    }
                }
                break;
            case ']':
                if (is_empty(&stack)) {
                    fprintf(stderr, "Woups: Mismatched brackets\n");
                    free_stack(&stack);
                    return 1;
                }
                if (memory[depth_pointer][row_pointer][col_pointer] != 0) {
                    i = stack.items[stack.top];
                } else {
                    pop(&stack);
                }
                break;
        }

        system("clear");
        display_memory(memory, depth_pointer, row_pointer, col_pointer);
        usleep(40000);
    }

    free_stack(&stack);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <brainfuck_code>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    memory_shift_printer(input);

    printf("%s\n", input);

    return 0;
}
```

(If the scroll wheel starts smoking, you’ve found the end.)

</details>

---

## WRAP-UP

You survived Part 2, WAOUW !

We fixed the visualizer, dipped our toes into randomness, and prepped the battlefield for a full-blown Brainfuck password generator.

**Next stop:** Part 3, where entropy meets insanity.

*Until then, may your brackets always match and your pointers never segfault.*


-----------
