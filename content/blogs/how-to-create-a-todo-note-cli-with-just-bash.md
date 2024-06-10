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

![note-taking](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-create-a-todo-note-cli-with-just-bash/preview.jpg)

## HOW TO CREATE A TODO NOTE CLI APP WITH BASH

-----------
`2024-05-11 12:23PM` **#fzf** **#bash** **#cli**

A TODO app, the most 'basic' app we (as dev) always build, or found on Tutorials when we're trying
 to learn a new stack or a new programming language (after a 'hello world' ofc).

The most simple app that covers a lot in terms of dealing memory, manipulating basic data-structure and store data.

## REQUIREMENTS

- bash (>5 recommended) (should work on all their variants `sh`, `zsh`...).
- nvim : the code editor, could be replaced by `nano`, `vim` or `code`.
- grep: should be installed by default on your OS(for the filtering or `ag`).
- [fzf](https://github.com/junegunn/fzf) : the fuzz
    ```bash
    # Clone the repository
    $ git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf && cd ~/.fzf/
    # Install it
    $ bash ~/.fzf/install
    ```

## THE NOTE CLI

The bash script is straighforward and is basically easy to understand what it does.
- First the script will create a directory on your `home` path called *notes*.
- Then, Depending on the day, the note is taken, the filename will take that value date.
- We have the filtering steps, that we wrapped in `_ag_filter` and `_grep_filter`, depending on your requirements, you can decide to use one or another.
- At the end we have the fzf that opens when no search/filter context is provided to let you browse on your notes.
- If you select one the note file, the code editor will be opened for you to update or delete some stuffs.

```bash

note(){
    local code_editor="nvim"
    local notes_dir=$HOME/notes/
    local content_view="cat $notes_dir/{1}-notes.md"
    local today_note_file="$notes_dir/$(date '+%Y-%m-%d')-notes.md"
	local enter_command="$code_editor $notes_dir/{1}-notes.md"

    function _grep_filter(){
        cat $notes_dir* | grep "$@"
    }
    function _ag_filter(){
        ag "$@" $notes_diraa
    }

    # We create the notes directory if it doesn't exist
    if [ ! -d "$notes_dir" ]; then mkdir -p $notes_dir; fi;

    # We browse context (topic) elements if only one argument is passed
    if [ "$#" = "1" ]; then ls $notes_dir | _grep_filter $1 ; fi;

    # We browse the content of notes if no arguments are passed
    if [ -z $1 ]; then
        # if fzf is installed, use it as a live browser, otherwise,
        # cat the list of note from today
        if [! command -v fzf &> /dev/null ]; then cat $today_note_file; else
            ls $notes_dir | sed 's/-notes.md//g' | fzf --header "NOTES LIST" \
                --preview "${content_view}" --preview-window "right:98" \
                --bind "enter:execute:${enter_command}" \
                --bind "ctrl-d:preview-down,ctrl-u:preview-up" \
                --tac; # for the reverse order
        fi
    else
        local content_message="${@:2}"
        # We only save a > 3 note
        if [ ${#content_message} -ge 3 ]; then
            echo -e "- **$(date '+%H:%M:%S')** > [$1] $content_message \n" >> $today_note_file;
        fi;
    fi
}
```

Example of Uses cases :

```bash
# the fist arg been the topic by default.
$ note topic note to my self... # will add a new note for the <topic>
$ note topic # will seach for notes on this <topic>
$ note # will list for you all notes taken globally using fzf
```

![demo-gif](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-create-a-todo-note-cli-with-just-bash/demo.gif)

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
