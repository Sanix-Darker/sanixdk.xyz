![image](https://dev-to-uploads.s3.amazonaws.com/uploads/articles/epjs5yraqmidvxv1e7ih.jpg)

## HOW TO CLONE A SUB DIRECTORY OF A GIT PROJECT (NOT A ZIP)
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
