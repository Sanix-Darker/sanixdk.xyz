
path: ./content/blogs/how-to-make-a-password-generator-using-brainfuck-part-2-3.md
link: /blogs/how-to-make-a-password-generator-using-brainfuck-part-2-3
title: HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 2/3)
image: https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-make-a-password-generator-using-brainfuck/garou.png
date: 2025-01-19 09:20PM
tags: #bash #brainfuck #terminal #c
time: 22 min

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-make-a-password-generator-using-brainfuck/garou.png)

## HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 2/3)
`2025-01-19 09:20PM` • 22 min read • **#bash** **#brainfuck** **#terminal** **#c**

**NOTE:* Before reading this, make sure to check [The First Part](https://sanixdk.xyz/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3)*

Last time we did nothing really interesting so far, just a buggy brainfuck visualizer in C, in this article we're going to try our best to be a little more inteligent... you know ? together.

<gif generation matrix>

### OBJECTIVES

- Explain some missing points from the First Part related to bf,
- Fix our buggy bv (a.k.a brainfuck memory visualizer),
- Add the CHAOS (not like what Thanos did, but something a little bit similar in our context),
- First overview of our PassWord Generator

### BRIEF RECAP

I got some interesting feedbacks/questions about things mentioned/explained in the previous article, that am going to clarify here :

- > "Are you insane ?"

    yes.

_
- > "Can you again explain in a single sentence what the hell is brain fuck ?"

    Imagine you have an array, and `>`, `<` are what you use to move between each block,
        `+`, `-` are what you use to increase/decrease the integer value of each block,
        you have the bare minimum to start coding using brainFuck.

    So *`>>>++<-`* actually means:

        - ">>>" move 3 times on the right,
        - "++" increase 2 times the current block,
        - "<" go on left one time,
        - "-" decrease one time.

    Now, you can then add `[`, `]` that represents loops, `,`, `.` for stdin/stdout.

_
- > I don't get how that brain fuck code became, integers to "hello world".

    Just to recap, it's linked to the section, i was mentioning that this brain fuck code is inteprated as "Hello World"

    >
    > Let's test some "random bf code":
    > ```bash
    > $ echo "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++." | bf /dev/stdin
    > Hello World!
    > ```

    You see, when we reach a certain number in a memory block, the interprator try to call [`putchar`](https://en.cppreference.com/w/c/io/putchar) that is responsible for printing the [ASCII](https://en.wikipedia.org/wiki/ASCII) letter representing the actual number in the alphabet.

    And you can test that by yourself by using this bash command for example :
    ```console
    $ echo $(printf \\$(printf '%03o' <valid-ascii-number>))

    # So for example, 72 -> H
    $ echo $(printf \\$(printf '%03o' 72))
    H
    ```

    This means, to print "Hey" with BrainFuck, we just have to (in bash) use 72 101 and 121, which each represents "H", "e" and "y":

    ```console
    $ echo $(printf \\$(printf '%03o' 72))$(printf \\$(printf '%03o' 101))$(printf \\$(printf '%03o' 121))
    Hey
    ```

    or in C  :
    ```c
    #include <stdio.h>

    int main() {
        putchar(72);
        putchar(101);
        putchar(121);
        putchar(10);
        return 0;
    }
    ```

    if you compile and try to execute that you have :
    ```console
    $ gcc ./hey.c -o ./hey && ./hey
    Hey
    ```

    So, the brainfuck memory representing that word of three letter is just : `[ 72] [101] [ 10]`.

    which is actually : `-[------->+<]>-.-[->+++++<]>++.[--->+<]>++.`, and you can test that by yourself with bf:

    ```console
    $ echo "-[------->+<]>-.-[->+++++<]>++.[--->+<]>++." | bf /dev/stdin
    Hey
    ```

    And that's how the code below got transformed to "Hello World", maggic right ?


### WHAT WAS WRONG WITH our bv ?

If you noticed on the part 1, we didn't handle yet `,` and `.` on  the brainfuck visualizer, and that was intentional, because for now, in order to keep things extremly simple, we're going to not use those to have linear code for manipulation.

If we try to run the Hello World code with our previous bv;

-----------
