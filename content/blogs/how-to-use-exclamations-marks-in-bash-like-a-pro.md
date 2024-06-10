![Example](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/gh260avy87m5rzw4yhrh.png)

## Bash and exclamation marks !!!!
----------------
`2023-11-08 10:03AM` **#bash** **#cli** **#terminal**

In case you didn't know, exclamation marks play an important role in the bash workflow, especially for your command history!

ok... let me explain that weird screenshot on top now. Actually it's simple :
- The first part of the command will take the first command item from the second most recent command you   wrote, then concatenate to "add" and the second item of the most recent command, `git add second_folder`.

- For the second part, it's a substitution command of "status" by "checkout" for the second command that i wrote, in reverse order on my bash history; it will take as arguments, those that i gave to my second command in the same order.
That said, the output of this command will be:
`git checkout second_folder`

**Pretty NEAT right !?**

So let's talk about it, the `!` invokes history expansion, a feature that originally appeared in the C shell, back in the days before you could count on terminals to have arrow keys. It's especially useful if you add the current command number to the prompt `(PS1="\!$ ")` so you can quickly look at your screen to get numbers for past commands.

So, am not going to go deep inside possibilities here but just give some cool cheat-sheets from exclamations marks when you're on your terminal !

This is a pretty short list of the cool ones :
- `!!` run the latest command,
       ```bash
     # Ex :
     $ apt-get update
     $ sudo !!
       ```

- `!-3` run the third command in the reverse order from my bash history; this is also available for positive values such as the first command, from your `~/.bash_history` -> `!1`


- `!*` get arguments passed to the most recent command ran,
       ```bash
     # Ex :
     $ touch filex, test4, example
     $ chmod 777 !*
       ```

- `!ls` to run a command from your history starting with `ls`

With these 4 items, you can basically play/mix around with your bash history and get great stuffs like the one up there with a substitution string.

-----------
