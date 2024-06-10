<!-- Template for blog header -->
<!DOCTYPE html>
<html data-color-scheme="dark">
<head>
    <meta charSet='utf-8' />
    <link href='/favicon.ico' rel='icon' />
    <link rel='canonical' href='https://sanixdk.xyz' />
    <meta content='initial-scale=1.0,width=device-width' name='viewport' />
    <meta content='#131516' name='theme-color' />
    <meta http-equiv='content-language' content='en-us,fr'>
    <link rel='stylesheet' href='https://matcha.mizu.sh/matcha.css'>
    <link rel='stylesheet' href='https://cdn.jsdelivr.net/gh/highlightjs/cdn-release@11.9.0/build/styles/github-dark.min.css'>
    <title>sanix | USE EXCLAMATIONS MARKS IN BASH COMMAND LINE LIKE A PRO.</title>
    <style>img{width: 100%;}pre{padding: 0px!important; border-radius: 7px;font-size: auto;box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);}</style>
    <meta property='og:url' content='https://sanixdk.xyz/blogs/how-to-use-exclamations-marks-in-bash-like-a-pro'>
    <meta property='og:type' content='website'>
    <meta property='og:title' content='dk's blog.'>
    <meta property='og:description' content='USE EXCLAMATIONS MARKS IN BASH COMMAND LINE LIKE A PRO.'>
    <meta property='og:image' content='https://dev-to-uploads.s3.amazonaws.com/uploads/articles/gh260avy87m5rzw4yhrh.png'>
    <meta name='twitter:card' content='https://dev-to-uploads.s3.amazonaws.com/uploads/articles/gh260avy87m5rzw4yhrh.png'>
    <meta property='twitter:url' content='https://sanixdk.xyz/blogs/how-to-use-exclamations-marks-in-bash-like-a-pro'>
    <meta property='twitter:domain' content='sanixdk.xyz'>
</head>
<body>
    <div class='container'>
        <br>
            <a href='/'><code>home</code></a> • <a href='/blogs/'><code>blogs</code></a> • <a href='/about'> <code>about</code></a>
        <hr/>

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
[<< blogs](/blogs/)

<script src="https://giscus.app/client.js"
        data-repo="sanix-darker/sanixdk.xyz"
        data-repo-id="R_kgDOJSF_ow"
        data-category="General"
        data-category-id="DIC_kwDOJSF_o84CXz0s"
        data-mapping="pathname"
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="0"
        data-input-position="top"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
<script>hljs.highlightAll();</script>

----

[`github`](https://github.com/sanix-darker "Where i sleep in term of projects.")
◦ [`linkedin`](https://www.linkedin.com/in/saa-djio-829399121/ "Not activelly maintained.")
◦ [`twitter`](https://twitter.com/sanixdarker "Time to time, posting stuff here.")
◦ [`telegram`](https://t.me/sanixdarker "You can Conctact me here anytime.")
◦ [`email`](mailto:s4nixdk@gmail.com?subject=Hello%20there "You can email me anytime.")
            </div>
        <br/>
    </body>
</html>
