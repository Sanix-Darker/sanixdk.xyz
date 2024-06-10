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
    <title>sanix | HOW TO CREATE A TODO NOTE CLI APP WITH BASH</title>
    <style>img{width: 100%;}pre{padding: 0px!important; border-radius: 7px;font-size: auto;box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);}</style>
    <meta property='og:url' content='https://sanixdk.xyz/blogs/how-to-create-a-todo-note-cli-with-just-bash'>
    <meta property='og:type' content='website'>
    <meta property='og:title' content='dk's blog.'>
    <meta property='og:description' content='HOW TO CREATE A TODO NOTE CLI APP WITH BASH'>
    <meta property='og:image' content='https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-create-a-todo-note-cli-with-just-bash/preview.jpg'>
    <meta name='twitter:card' content='https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-create-a-todo-note-cli-with-just-bash/preview.jpg'>
    <meta property='twitter:url' content='https://sanixdk.xyz/blogs/how-to-create-a-todo-note-cli-with-just-bash'>
    <meta property='twitter:domain' content='sanixdk.xyz'>
</head>
<body>
    <div class='container'>
        <br>
            <a href='/'><code>home</code></a> • <a href='/blogs/'><code>blogs</code></a> • <a href='/about'> <code>about</code></a>
        <hr/>

![image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/xo5l2448mxa8beq1l4o5.jpeg)

## Git search for code whatever the branch, the time or the commit
-----------
`2023-10-02 04:03AM` **#cli** **#terminal**

Git has a nice feature called `grep`, when you want to get a commit for an old code, and it's a kind of search, but not enough for me !
I wrote a bash script to search for a "piece of code" inside a git project history whatever the branch and time.

It can :
- Search a word and return the list of commits where the word was found.
- Switch the project to the target commit, open the file at the specified line, for you, to copy your code, then restore yourself to your working branch !

If we save the file as `./gf.sh`, we give execution rights `chmod +x ./gf.sh` and we add it in our `.bashrc` as an alias : `alias gf=${HOME}/gf.sh`.

We can wrote interesting command easily.

Example :
```bash
$ gf search 'vieux code'
$ gf open 'commit-id:fichier:ligne'
```

This is how the code looks :
```bash
#!/bin/bash
# by d4rk3r

# A smart way to search for a code
# inside your git history whatever the branch you're

# -- Setup :
# -- After getting the bash script

# chmod +x /path/to/gf.sh
# -- to add the execute right to the script
# -- You can add it to your .bashrc env

# gf s 'pattern' -- to search for some code
# -- then after copy the line of the commit,

# gf o commit_id:line:file_name....
# -- gf will switch to that commit and open the line where the code is,
# -- after quitting, gf will restore your repo to the precedent branch where you were working

_git_search(){
    # a smart git command to get commit, file and line where
    # the string is available
    git rev-list --all | (
        while read revision; do
            git grep -n -F ${1} $revision
        done
    ) > ${HOME}/.gf-out
}

_search(){
    echo "[-] Searching for '${1}' in this repo..."
    sleep 2
    # We less in the list of results
    _git_search "${1}"
    cat ${HOME}/.gf-out | less
}

_open_code(){
    # getting your current branch
    branch=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')

    # A smart split from the incomming $1 parameter
    arr=($(echo "$1" | tr ':' '\n'))
    commit=${arr[0]}
    file=${arr[1]}
    line=${arr[2]}

    # some verbose
    echo "[-] Moving on commit : $commit"
    sleep 1
    echo "[-] Moving on file : $file"
    sleep 1
    echo "[-] Moving on line : $line"
    sleep 1

    # A checkout on the commit then a less on the line of the file
    git checkout "$commit"
    less +"$line" "$file"

    sleep 2
    echo "\n[-] Rolling back to your precedent branch..."
    # comming back to reality
    git checkout "$branch"
}

_help_commands(){
    echo "[-] gf help center:"
    echo "[-] gf s 'search-kwy-word'"
    echo "[-] gf o 'commit-id:line:file-name'"
}

main(){
    echo "[-] gf started..."

    if [ "$1" == "search" ] || [ "$1" == "s" ]; then
        _search $2
    elif [ "$1" == "open" ] || [ "$1" == "o" ]; then
        _open_code $2
    else
        echo "[x] Error: Bad parameter provided..."
        _help_commands
    fi
}

main $1 $2
```

Source code : [Gist-Code](https://gist.github.com/Sanix-Darker/e571c1fdc2d0a5ecdb9959fecaa3416a)

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
