
path: ./content/blogs/how-to-make-a-password-generator-using-brainfuck-part-2-3.md
link: /blogs/how-to-make-a-password-generator-using-brainfuck-part-2-3
title: HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 2/3)
image: https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/fujigoru.png
date: 2025-01-12 11:22
tags: #bash #brainfuck #terminal #c
time: 17 min

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/fujigoru.png)

## HOW TO MAKE A PASSWORD GENERATOR USING BRAINFUCK (PART 2/3)
`2025-01-12 11:22` • 17 min read • **#bash** **#brainfuck** **#terminal** **#c**


**NOTE:* Before reading this, make sure to check [The First Part](https://sanixdk.xyz/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3)*

Last time we did nothing really interesting so far, just a buggy brainfuck visualizer, in this article we're going to try our best feel a little inteligent... you know ? together.

### SPOILERS ALERT

<gif generation matrix>

### OBJECTIVES

- Explain some missing points from the First Part,
- Fix our buggy bv (a.k.a brainfuck memory visualizer),
- Explain why exactly we need a bv,
- Add Interesting updates to our bv (a.k.a brainfuck memory visualizer),
- Add the CHAOS (not like what Thanos did, but something a little bit similar in our context),
- First overview of our PassWord Generator

### MISSING POINTS

I got some interesting feedbacks about points mentioned in the previous article, that am going to clarify here :

- 1.) ARE YOU A HUMAN ?

    huh... yeah ? ( i mean... i hope so ? )


- 2.) WHY DOING THAT ?

    Taken from Part 1 :

    > - For people asking "WHY THE HECK DO YOU WANT TO DO THAT ?"... well... because i can ?


- 3.) I DON'T GET HOW THAT BF CODE BECAME "HELLO WORLD"

    Just to recap, it's linked to the section, i was mentioning that this brain fuck code is inteprated as "Hello World"

    >
    > Let's test some "random bf code":
    > ```bash
    > $ echo "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++." | bf /dev/stdin
    > Hello World!
    > ```

    You see, when we reach a certain number in a memory block, the interprator try to call `putchar` that is responsible for printing the ASCII letter representing the actual number

    And you can test that by yourself by using this bash command for example :
    ```
    $ echo $(printf \\$(printf '%03o' <valid-ascii-number>))

    $ echo $(printf \\$(printf '%03o' 72))
    H
    ```

-----------
