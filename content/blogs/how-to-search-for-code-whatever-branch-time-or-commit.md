![image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/xo5l2448mxa8beq1l4o5.jpeg)

## GIT SEARCH FOR CODE WHATEVER THE BRANCH, THE TIME OR THE COMMIT
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
