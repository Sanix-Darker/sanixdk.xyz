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
    <title>sanix | HOW TO CLONE SUB DIRECTORIES.</title>
    <style>img{width: 100%;}pre{padding: 0px!important; border-radius: 7px;font-size: auto;box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);}</style>
    <meta property='og:url' content='https://sanixdk.xyz/blogs/how-to-clone-sub-directory-from-a-served-git-repository'>
    <meta property='og:type' content='website'>
    <meta property='og:title' content='dk's blog.'>
    <meta property='og:description' content='HOW TO CLONE SUB DIRECTORIES.'>
    <meta property='og:image' content='https://dev-to-uploads.s3.amazonaws.com/uploads/articles/epjs5yraqmidvxv1e7ih.jpg'>
    <meta name='twitter:card' content='https://dev-to-uploads.s3.amazonaws.com/uploads/articles/epjs5yraqmidvxv1e7ih.jpg'>
    <meta property='twitter:url' content='https://sanixdk.xyz/blogs/how-to-clone-sub-directory-from-a-served-git-repository'>
    <meta property='twitter:domain' content='sanixdk.xyz'>
</head>
<body>
    <div class='container'>
        <br>
            <a href='/'><code>home</code></a> • <a href='/blogs/'><code>blogs</code></a> • <a href='/about'> <code>about</code></a>
        <hr/>

![image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/epjs5yraqmidvxv1e7ih.jpg)

## How to clone a sub directory of a git project (not a zip)
-----------
`2023-11-20 06:03AM` **#git** **#bash** **#github** **#gitlab**

Have you ever tried to get a folder from a git repository either on github, gitlab or bitbucket ?

![Final Demo](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/m93toqmbg8cgm1me7qg5.png)

hop hop hop, am not talking about Downloading a ZIP folder (like DownGit does), but only cloning a sub-directory with the git clone... command !

I got that question from a friend, and i wrote a small and interesting bash script to do so !

We're going to do that in 2 steps.

### WRITE A BASH SCRIPT FUNCTION

We're going to write a function in our `.bashrc` that will update our [sparse-checkout](https://git-scm.com/docs/git-sparse-checkout) like this :
```bash
_git_clone_sub ()
{
    REPO_NAME="$(echo $2 | grep -oE '[^/]+$')";
    git clone --filter=blob:none --no-checkout $2
    cd $REPO_NAME;
    git sparse-checkout set --no-cone "$1/*"
    if [ -n "$3" ]; then
        git pull origin $3;
        git checkout $3;
    else
        git fetch origin;
        git checkout main
        [[ $? != 0 ]] && git checkout master;
    fi
}
```
This script is quite simple, this is what it does:
- Extract the repo name from the link (github.com/author/repo-name).
- Create a git project with the repo name + add the origin from the link.
- Then update the tree configuration of our project.
- As an optional parameter, we have the branch, if it's provide, the script will clone a sub directory from that branch, otherwise, it will clone from master (you can change that with `main` if you want).

Now we can easily clone sub directories like this (after a `source ~/.bashrc`) :
```bash
_git_clone_sub subDir1 https://github.com/auth/repo
```
or by specifying the branch name :
```bash
_git_clone_sub subDir1 https://github.com/auth/repo dev-branch
```

### WRITE A GIT ALIAS

So far so good... but we can do better !
We could call that bash function from git directly by adding an alias in our `~/.gitconfig` file like this :
```config
[alias]
    clone-sub = !bash -i -c '_git_clone_sub "$@"' -s
```
Yeah yeah wait a minute, i know, that one looks weird, but let me explain... it's actually simple !

First, since we are going to run a bash function, we embrace that with 'bash -c'; then we call our function.
The `$@` is to get all arguments got using the `-s` flag at the end of the alias.

That's been said, you can now do :
```bash
git clone-sub subDir1 https://github.com/auth/repo
```
```bash
git clone-sub subDir1 https://github.com/auth/repo dev-branch
```

### DEMO

![DEMO](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/nulatd2q852objx9f7zo.gif)

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
