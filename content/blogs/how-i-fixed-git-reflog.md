<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-i-fixed-git-reflog/rezero.png" />

### HOW I ~FIXED~ GIT REFLOG
`2025-12-06 03:30PM` • 11 min read • **#git** **#golang** **#terminal**

---

<div id="toc-container"></div>

---

### DISCLAIMERS

Okay okay, this is a rage bait title... sorry all, my apologies Mr Linus.

What I mean by "fixed" is more like a "small add-on" I made for myself.

Don't get me wrong,

Git is perfect...

Git is beautiful...

BUT, git reflog is... granular... like too much granular (wink reflog...), to the point of pain sometimes... and cognitive complexity.

### MY ACTUAL PROBLEM

Let me paint you a small descriptive scenario...

You're working on a feature (or a fix, you call it).

You pull from origin (`git pull origin that-feature`).

BOUM, conflicts everywhere, you fix some..., miss others (of course... or worse, falsy resolving some), keep going.

You rebase.

You amend a commit.

You rebase AGAIN because you messed up the first one (yeah yeah, you're not alone...).

You stash something, forget about it, pop it back...


Two hours later, 3 cup of cofee and headaches, something is broken and you think: *"ah f*ck... i hate programming..."*, then after taking your 5 cup of cofee you ask yourself "i just want to undo everything I did this morning regarding that pull..."*

If you have trust yourself enought, you open `git reflog`:

TADAN !!!

```bash
$ git reflog
a3f2b1c HEAD@{0}: rebase (finish): returning to refs/heads/feature
8d4e9f2 HEAD@{1}: rebase (pick): fix auth
c7b3a5d HEAD@{2}: rebase (pick): add login
2e8f1c9 HEAD@{3}: rebase (start): checkout main
f4d2b7e HEAD@{4}: commit (amend): fix typo
9a1b2c3 HEAD@{5}: commit: fix typo
1d2e3f4 HEAD@{6}: checkout: moving from main to feature
# ... 47 more lines of this, that looks like a camerounian ancestral language...
```

you start counting backwards to find where "this morning" started...

Was it `HEAD@{12}`? `HEAD@{15}`?

Did you count the rebase steps individually or as a group ???

Do i need glasses ???

Why i didn't try something else in LIFE !!!!


**This is exactly MY problem**: `git reflog` tracks git's internal state changes, not your *intent*... and to be honnest, that is GOOD, extremly GOOD...

but not practical on Mondays... (IMHO) if you know, you know !

You didn't perform `15 operations`.

You performed `ONE logical action`: "Work on feature branch this morning."

### LET's DEFINE, WHAT I CALL an "ACTION" ?

An action, for me is what you were *trying* to do, not what git recorded.
if we can represent that more clearly, we can say :

```
| You think | Git records |
|-----------|-------------|
| "refactor auth module" | 12 commits, 3 rebases, 2 amends |
| "merge main into feature" | checkout, fetch, merge, conflict resolution, commit |
| "quick bugfix" | 1 commit (finally, git agrees) |
```

The mismatch is brutal when you need to undo things.

### MY WEIRD WRAPPER SOLUTION: `git action`

I built a tiny Go tool (~200 lines) that adds "checkpoint-based" undo/apply to git...

```bash
# Before risky work or computer explosion...
git action mark "before refactor"

# ... mass destruction ...
# ... 47 commits, mass destruction ...
# ... mass destruction ...

# Oh no.. nothing works
git action revert "before refactor"  # undo ALL of it
```

That's it. That's the whole thing.

### HOW IT WORKS

```bash
$ git action mark "before refactor"
✓ marked 'before refactor' at a3f2b1c

# do stuff...

$ git action ls
#   NAME                           MARKED AT        OPS SINCE   STATUS
───────────────────────────────────────────────────────────────────────────
1   before refactor                2 hours ago      23          revertable

$ git action revert 1
Reverting to 'before refactor'
  Hash: a3f2b1c
  Undoing: 23 operations

Proceed? [y/N]: y
✓ reverted to 'before refactor'
```

23 operations. One command to undo. No counting.

### THE COMMANDS

For now i just implemented these, that can change later... depending on my personnal needs,

```
| Command | What it does |
|---------|--------------|
| `git action mark "name"` | Create checkpoint at current HEAD |
| `git action ls` | List checkpoints + how many ops since each |
| `git action revert <id>` | Hard reset to checkpoint |
| `git action diff <id>` | See what changed since checkpoint |
| `git action drop <id>` | Delete a checkpoint |
```

### WAIT, ISN'T THIS JUST 'TAGS' ?

Yes and ... maybe no...

`git tag before-refactor` does create a reference point. But:

- **Tags don't show you ops count** — you don't know how much you're undoing
- **Tags pollute your tag namespace** — mixing releases with checkpoints
- **Tags don't have metadata** — when was it created? what branch?
- **Tags require you to remember names** — `git action ls` shows everything

you can read more about git tags [here](https://git-scm.com/book/en/v2/Git-Basics-Tagging)
It's a small quality-of-life improvement.
Absolutely Not revolutionary.
Just... less friction...

And to be honnest, tags are used most of the time for releases only... IMHO.

### MY TODAY WORKFLOW

**Before anything risky or problematic...:**
```bash
git action mark "before i mess up"
```

**When things break:**
```bash
git action ls      # see your checkpoints
git action diff 1  # see what you did
git action revert 1  # undo it all
```

**When you succeed:**
```bash
git action drop 1  # clean up, you don't need the checkpoint anymore
```

### REAL EXAMPLE

```bash
$ git action mark "before splitting user model"

$ # 2 hours of mass destruction
$ git add . && git commit -m "split user model"
$ git rebase -i HEAD~5   # squash some commits
$ git commit --amend     # fix typo
$ git rebase main        # sync with main
$ # oh no, conflicts everywhere, more fixing

$ git action ls
#   NAME                           MARKED AT        OPS SINCE   STATUS
───────────────────────────────────────────────────────────────────────────
1   before splitting user model    2 hours ago      17          revertable

$ git action diff 1
Changes since 'before splitting user model':

--- Commits ---
f4a2b1c Merge branch 'main' into feature
8d4e9f2 split user model
...

--- Diff ---
 src/models/user.py  | 145 +++----
 src/models/auth.py  |  89 ++++
 src/api/routes.py   |  23 +-
 3 files changed, 187 insertions(+), 70 deletions(-)

$ # Hmm, this is broken. Let me undo all of it.
$ git action revert 1
✓ reverted to 'before splitting user model'
```

### CONCLUSION

**What this solves:**

- "Undo everything I did in this work session" → IMHO yes, one command
- Reduces reflog mental math/ cognitive complexity → IMHO yes

**What this doesn't solve:**

- Complex git workflows → still need to learn git (sorry)
- Team collaboration → checkpoints are local only (and maybe that can change later ?)
- Everything else → git is still git... know the command lines

**Is it necessary ?**

To be fair, probably not.

You could use tags.

You could count reflog entries.

You could be more careful.

You could stop been lazy like me.


But I kept making the same mistake: doing risky stuff without a save point, then spending 10 minutes figuring out how to undo it by questioning my destiny !

Now I just type `git action mark "god help me on this conflicts"` and move on with my life.

Around ~200 lines of Go. Stores checkpoints in `.git/action-marks.json`. No extra dependencies needed,
yes i could have make it as a bash/sh alias (but these days i have sympathy with my felow windows friends...)
but still not a big fan of windows tho.

```go
type Action struct {
    Name      string    `json:"name"`
    Hash      string    `json:"hash"`
    Branch    string    `json:"branch"`
    Timestamp time.Time `json:"timestamp"`
}
```

That's the whole data model. It's stupid simple because the problem is stupid simple.
---

[Source code](https://github.com/sanix-darker/git-action)

---

**PS:** I started working on something quite in the same context, regarding CI/CD pipeline... so far, i am really not satisfy with what is offered today to run jobs pipeline locally regarding github, gitlab, Azure DevOps, etc... so i am working on a `git ci` alias to simplify enought that, you can have a look here : https://github.com/sanix-darker/git-ci, when i will be satisfy enought regarding the first stable version i will maybe consider realising the first betta version of it !

-----


