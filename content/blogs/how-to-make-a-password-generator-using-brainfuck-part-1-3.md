![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/gojo.jpg)

## HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 1/3)
`2025-01-05 14:10` • 10 min read • **#bash** **#brainfuck** **#terminal** **#c**

### DISCLAIMERS

- This is a Serie of 3 Parts of making this project happens, keep in touch ;)
- Am not going to go behond the basics of how brainfuck works to not loose track.
- Am not going to use ONLY brainfuck for my PassWord generator (i will make a C program alongside too).
- Am trying to not have too longs blog post, that's why i split this ongoing work into 3 parts, also, don't hesitate to ask questions in the comment section at the end.
- For people asking "WHY THE HECK DO YOU WANT TO DO THAT ?"... well... because i can ?
- Regarding the source code, am not going to explain EACH line, sorry, but happy to answer your questions.

### FUNKY INTRODUCTION

For those who don't know BrainFuck, it's an esoteric programming language with only 8 commands (<>+-[],.) created by Urban Müller in 1993.
Yeah, basically, a programming language invented to CAP in front of your dev friends/collegues (sorry for this bad joke).

But hey, it's a fun language i discovered 4 years ago without given too much attention on it, and in the recent weeks i challenged my self to give it a "REAL TRY" (just to cap...).

It's a Turing-complete language, so it has loops, iterations, shifts, you can even make arithmetics operations with it (CRAZY RIGHT ???).

This is an example of Brainfuck code :
```txt
>>+>>[<++>-]>+<<-
```

Yeah... much more like an ancient tribe sorcery summoning something not fun... lmao.

But, hey, you can even mimic `if-else` conditionals logic (that you can artificially create with loops and memory manipulation)

### HOW DOES THIS WEIRD THING ACTUALLY WORK ?

The idea behind brainfuck is JUST memory manipulation.

Basically you are given an array of 30,000 1byte memory blocks.
The array size is actually dependent upon the implementation used in the compiler or interpretor, but standard brainfuck states 30,000 (in my case i will deal first with 8 only)

Within this array, you can increase the memory pointer, increase the value at the memory pointer, etc, am not going to go to much in depth here how operators works in brainfuck :

```txt
> = increases memory pointer, or moves the pointer to the right 1 block.
< = decreases memory pointer, or moves the pointer to the left 1 block.
+ = increases value stored at the block pointed to by the memory pointer
- = decreases value stored at the block pointed to by the memory pointer
[ = like c while(cur_block_value != 0) loop.
] = if block currently pointed to's value is not zero, jump back to [
, = like c getchar(). input 1 character.
. = like c putchar(). print 1 character to the console
```

Don't worry if you didn't get any of what i just said up there, it's normal, i had the same facial expression as yours at this moment, when i was learning about it.

However, saying it's turing complete, doesn't means you can do anything with it, like sending people into Mars you know ?

So CHILL Mr ROBOT, ["we don't do that here"](https://imgflip.com/memegenerator/212259281/We-dont-do-that-here).

It's most-likely memory blocks manipulation only.

YES, there are some freaks out there doing `game engines` with it... am a human trying to have a normal life okay ? am not going to explain how they did that.

### LET's TEST THIS THING, SHALL WE ?

You will need an intepreator to test your bf code, it looks like, the most popular is [*bf*](https://github.com/fabianishere/bf), even tho it looks a little old,

you can install on your OS (for these blog posts, am working on Linux, the best OS By The Way), specially Ubuntu.

```bash
# so, on DEB distro, you can just
$ sudo apt update
$ sudo apt install bf

# Or better, you can compile it yourself (if you're a real CHAD):
$ git clone https://github.com/fabianishere/bf && cd ./bf
$ make
$ sudo make install # to install on system-wide
$ bf -h
```

Let's test some "random bf code":
```bash
$ echo "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++." | bf /dev/stdin
Hello World!
```

Look... i can explain, i had the same facial expression as you right now, you may probably asking yourself "ALL THIS TO PRINT 'Hello Word' ? ARE YOU CRAZY".

Yes... that's true... BUT WAIT, don't go yet please XD, i have some good news for you down there.

If you're still there, it's a good news, you want to know more, thanks for that mate.

We're going to breakdown that weird ALIEN bash command line written up there :
- `echo "+++..."` to print to the standard output the given string in param.
- `|` to get the standard output of the left command and put inside the right command.
- `bf /dev/stdin`, while testing `bf`, i discovered it take as param the path of a file containing the code, since i wanted to do a lot of comand line test, it was easier, to just pass `/dev/stdin` which will contain what the `|` is sending in the standard output.

However i really needed to see how i was manipulating memory blocks, i mean, a kind of memory representation of how the rules i just written were actually affecting the memory.

### FIRST STEP (a BRAINFUCK MEMORY VISUALIZER)

I choosed to use a high level programming language to achieve that, the C programming language, sorry JS devs, ["not today"](https://imgflip.com/memegenerator/37010978/Not-Today).

But first, we need to undestand some few stuffs... like standard I/O streams.


#### WHAT ARE `/dev/stdin`, `/dev/stdout`, `/dev/stderr` ?

The following listed, are standard I/O stream, you can get to know more [HERE](https://en.wikipedia.org/wiki/Standard_streams).

Programs use them to get input, send output after some execution and also , error output to another stream.

So, since i wanted to still "test out" my code using *bf*, i had to use in my code `fprintf` and `printf`, with `fprintf` printing the built memory representation into the standard error stream with `fprintf(stderr, "to stderr stream");` and since `printf` automatically send to the standard output stream, i had everything on hands.

For a better overview of what am talking about in this example:
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    fprintf(stderr, "\nto stderr");
    printf("\nto stdout");

    return 0;
}
```

And as you can test out :
```bash
$ gcc ./test.c -o ./test

$ ./test 2> error.txt # getting the stderr into error.txt
to stdout

$ ./test 1> out.txt # getting the stdout into out.txt
to stderr

$ cat ./out.txt
to stdout

$ cat ./error.txt
to stderr
```

#### THE PLAN I HAD IN MIND

I wanted a simple program, so that at the end i could be able to do :
```bash
$ ./bv ">>+<-" | bf /dev/stdin
# Have the memory representation of those operations (in the stderr),
# then bf intepreator running the actual code (took from bv stdout)
```

I knew at some point i would have to use a [Stack Datastructure(LIFO)](https://www.codecademy.com/learn/linear-data-structures/modules/cspath-stacks/cheatsheet),

Let's recap, what i wanted to achieve for `bv` :

- Loop over the incomming brainfuck code string (one line FOR NOW).
- Update and represent the memory blocks at the final state.
- Represent the "cursor position" to the last block touched.
- Print in the StdErr the final Ascii representation of the memory manipulated.
- Print (forward) the brainfuck code to the standard out stream for any other program that may take it from the `/dev/stdin` to go on.


#### LET's CODE

- Regarding the Stack Datastructure, something extremly simple like :

```c
typedef struct Stack {
    int items[MAX_STACK_SIZE];
    int top;
} Stack;

void init_stack(Stack* s) {
    s->top = -1;
}

int is_empty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, int value) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Woups: Stack overflow\n");
        exit(1);
    }
    s->items[++s->top] = value;
}

int pop(Stack* s) {
    if (!is_empty(s)) {
        return s->items[s->top--];
    }

    fprintf(stderr, "Woups: Stack underflow\n");
    exit(1);
}
```
was enought,no need to over-complicated things for now.


- Regarding the displayer, i made this small method

```c
void display_memory(int memory[], int mem_pointer, int size) {
    fprintf(stderr, "\n-------------\nmemory blocks\n-------------\n");
    for (int i = 0; i < size; ++i) {
        fprintf(stderr, "[%d]", memory[i]);
    }
    fprintf(stderr, "\n");

    // fr pointer position
    for (int i = 0; i < mem_pointer; ++i) {
        fprintf(stderr, "    ");
    }
    fprintf(stderr, "^\n");
    fprintf(stderr, "memory pointer\n--------------\n\n");
}
```
that will display the memory blocks and also the memory pointer(cursor) position.


- The biggest part, looping over each character, increment the memory table, shift, understand loops, and use the stack to store instructions :
```c
int memory_shift_printer(const char *input) {

    int memory[MAX_MEMORY_SIZE] = {0};
    int mem_pointer = 0;

    Stack stack;
    init_stack(&stack);

    for (size_t i = 0; i < strlen(input); ++i) {
        // yes, that's a lot of if/else... i don't care, what you gonna do ?
        if (input[i] == '>' && mem_pointer < MAX_MEMORY_SIZE - 1) {
            mem_pointer++;
        } else if (input[i] == '<' && mem_pointer > 0) {
            mem_pointer--;
        } else if (input[i] == '+') {
            memory[mem_pointer]++;
        } else if (input[i] == '-' && memory[mem_pointer] > 0) {
            memory[mem_pointer]--;
        } else if (input[i] == '[') {
            if (memory[mem_pointer] != 0) {
                push(&stack, i);
                continue;
            }

            int loop = 1;
            while (loop > 0) {
                i++;
                if (i == strlen(input)) {
                    fprintf(stderr, "Woups: Mismatched brackets\n");
                    return 1;
                }
                if (input[i] == '[') loop++;
                if (input[i] == ']') loop--;
            }
        } else if (input[i] == ']') {
            if (memory[mem_pointer] == 0) {
                pop(&stack);
            }
            if (is_empty(&stack)) {
                fprintf(stderr, "Woups: Mismatched brackets\n");
                return 1;
            }
            // to be sure move to the command after the matching [
            i = stack.items[stack.top] - 1;
        }

        // for verbose purpose... i can uncomment this to see, what happens at each step.
        // display_memory(memory, mem_pointer, MAX_MEMORY_SIZE);
    }
    display_memory(memory, mem_pointer, MAX_MEMORY_SIZE);
    return 0;
}
```
pretty simple right ?

I added some comments where it can be confusing to get what's going on.
this is not complicated YET, for what's comming in future parts.


- Now the entrypoint, the LORD function `main` :
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEMORY_SIZE 50

// code below...

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <brainfuck_code>\n", argv[0]);
        return 1;
    }

    char *input = argv[1];
    memory_shift_printer(input);

    return 0;
}
```

and we have our brainfuck code VISUALIZER ready, just last step, compile and run :
```bash
$ gcc -std=c11 ./bv.c -o ./bv
dk:~
$ ./bv ">>>++[<+++>-]+<++<"

-------------
memory blocks
-------------
[0][0][8][1][0][0][0][0][0][0]
   ^
memory pointer
--------------

```

***NICE !!!***

We can see our memory blocks array, the memory pointer(cursor) on the last operation too.


And since it' turing complete, you can also do mathematical operation with it :
```bash
$ ./bv ">+++[-<+>]>>." | bf /dev/stdin

-------------
memory blocks
-------------
[3][0][0][0][0][0][0][0][0][0][0][0][0]
         ^
memory pointer
--------------

```

### FLAWS (AS A DISCLAIMER)

This C code is not optimal on Part 1, and will be optimized in next future parts, for instance, you can not do :
```bash
$ ./bv "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++." | bf /dev/stdin
Woups: Stack overflow
```
And that is the input code for "Hello World!" message.

Maybe some of you already found the flaws and ways to optimize the code (but it's the part of the game ;)).

You can get the full first version of the source code [HERE](https://gist.github.com/Sanix-Darker/e9e2438a2333b3e3589884b91b27ec60)


## CONCLUSION | END OF PART 1

I hope you enjoyed this first part of a serie of 3 about the brainfuck password generator am building.

In the Next step, we will do two main things :
- Optimize the brainfuck VISUALIZER c code.
- Write brainfuck code to get some input and actually generate stuffs.

So keep in touch ;)

## RESOURCES USED

- [https://gist.github.com/roachhd/dce54bec8ba55fb17d3a (I HIGHTLY RECOMMEND)](https://gist.github.com/roachhd/dce54bec8ba55fb17d3a)
- [https://www.codecademy.com/learn/linear-data-structures/modules/cspath-stacks/cheatsheet](https://www.codecademy.com/learn/linear-data-structures/modules/cspath-stacks/cheatsheet)
- [https://en.wikipedia.org/wiki/Standard_streams](https://en.wikipedia.org/wiki/Standard_streams)


-----------
