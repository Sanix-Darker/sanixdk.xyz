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
