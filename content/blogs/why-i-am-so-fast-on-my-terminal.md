## WHY I'M SO FAST ON MY TERMINAL

`2025-01-22` • 15 min read • **#bash** **#productivity** **#terminal** **#git**

---

If you've ever watched me work in a terminal, you might wonder how I navigate so quickly between tasks, manage Git repositories, and execute complex workflows in seconds. The answer isn't magic—it's **ruthless optimization through aliases and shortcuts**.

After 5 years of working in terminals, I've accumulated a lot of aliases, custom functions, and Git shortcuts that make me exponentially faster. This post breaks down my most-used commands and shows you how to build your own velocity-enhancing toolkit.

On functions, i created my self, most start with an '_', some are then re-asigned to a valid alias, so this is not a convention, it's for me to differentiate custom aliases i made than real unix built-in commands.

### NAVIGATION & FILE MANAGEMENT

The standard `cd` command is slow. I've overridden it with `zoxide`, which learns from my navigation patterns:

```bash
# Instead of typing full paths
cd ~/projects/work/backend/api/services

# I just type:
cd services
```

My magic wrapper even runs `git status` automatically when entering a Git repo:

```bash
_magic_cd(){
    z $@ || cd $@
    _git_status_if_git_repo
}
alias cd='_magic_cd'
```

Why type `mkdir foo && cd foo` when you can just:

```bash
_md(){
    mkdir $1 && cd $1
}
alias md=_md
```


I replaced `ls` with `exa` for prettier output:

```bash
alias ls='exa'
alias ll='ls -alF'
alias la='ls -A'
alias ld='ls -d */'  # list only directories
```


I never use `rm` directly anymore. Everything goes to trash:

```bash
alias rf='trash-put'  # put in trash
alias rr='trash-restore'  # restore from trash
alias lf='trash-list | grep $(pwd)'  # list trash for current dir
```

### GIT WIZARDRY

This is where the real speed comes from. My Git workflow is turbocharged with over 100 custom aliases.

#### Branch Navigation

```bash
# Jump to any branch with fuzzy search
alias brr='git branch --sort=-committerdate | fzf --header "Checkout Recent Branch" --preview "git diff {1} --color=always | delta" --preview-window "right,80%" --pointer=">" | xargs git checkout'

# List branches by commit date
git branch-sorted  # alias for: git branch --sort=committerdate
```

#### Smart Commit Management

```bash
# Quick amend without opening editor
git add-amend-no-edit  # adds tracked files + amends

# Squash all commits on current branch
git squash  # interactive rebase from origin

# Cherry-pick with fuzzy finder
git pick-commit  # opens fzf to select any commit from history
```

#### Pull Request Workflow

I can create, view, and manage PRs without leaving the terminal:

```bash
# Create draft PR (GitHub)
git gh-pr  # opens editor with PR template

# Create PR to specific branch
git gh-pr-to dev-branch

# List all PRs with fuzzy search and actions
git_select_pull_request  # fzf menu with approve/merge/view options

# Open current branch's PR in browser
git open-pr
```

#### Diff & Log Navigation

```bash
# Browse commits with fuzzy search + delta preview
git browse  # alias: git-fuzzy-log

# See only commits on current branch
git changes  # commits not in origin

# View file history with fuzzy search
git log-for-file ./path/to/file

# Smart diff with file selection
git diff-search  # fzf to select files to diff
```

#### Worktree Management

I use Git worktrees extensively to work on multiple branches simultaneously:

```bash
# Checkout branch in new directory
git checkout-tree fix/bug-123 feat/new-feature

# This creates:
# ../project-fix-bug-123/
# ../project-feat-new-feature/
```

The function automatically:
- Fetches the remote branch
- Creates a new worktree directory
- Checks out the branch
- Handles branch name formatting

### DOCKER & DEVELOPMENT

```bash
# Clean Docker aggressively
alias clean_docker_images='docker rmi $(docker images -f "dangling=true" -q)'
alias clean_docker_container='docker rm -f $(docker ps -a -q)'

# Better container listing
alias ds="docker ps --format 'table {{.RunningFor}}\t{{.Status}}\t{{.Names}}'"

# LazyDocker for TUI management
alias lazydocker='lazydocker'
```

### PYTHON ENVIRONMENTS

```bash
# Quick virtual environment creation
alias v12='python3.12 -m venv env'
alias v11='python3.11 -m venv env'
alias v10='python3.10 -m venv env'

# Activate any virtual env
alias ee='source *env*/bin/activate'
alias de='deactivate'

# Python aliases
alias p='python3'
alias i='ipython3'
```

### CLIPBOARD INTEGRATION

```bash
# Copy/paste from command line
alias cb='xclip -selection clipboard'  # copy
alias co='xclip -o -selection clipboard'  # paste

# Copy current directory path
alias cpwd='pwd | cb'

# Send to termbin (with encryption!)
alias tbc='co | tb'  # paste clipboard to termbin
```

### SEARCH & TEXT PROCESSING

```bash
# Find text in files (ag = silver searcher)
# Just type 'ag searchterm'

# Search within Git history
git search "function_name"  # searches all commits

# Count lines of code by extension
alias loc='find . -name *.$@ | xargs wc -l'

# Example: loc py  # counts all .py files
```

### UTILITY FUNCTIONS

#### Confirmation Prompts

I built a confirmation system for dangerous operations:

```bash
_confirm(){
    args=("${@}")
    read -p "[?] ${args[0]} (Y/y): " -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        callback=${args[@]:1}
        $callback
        return 0
    fi
    return 1
}

# Usage:
_confirm "Install Docker?" _install_docker
```

#### Execute in Temporary Directory

```bash
_xm(){
    cd /tmp
    $@
    cd -
}

# Runs command in /tmp, then returns
```

#### Port Management

```bash
# Kill process on port
_killport(){
    sudo kill -9 $(sudo lsof -t -i:$1)
}
alias killport=_killport

# List processes on port
_lsport(){
    sudo ss -lptn "sport = :$1"
}
alias lsport=_lsport
```

### TMUX PRODUCTIVITY

```bash
# Quick session management
alias tn='tmux new -s'
alias ta='tmux attach-session -t'
alias tls='tmux ls'
alias tkill='tmux kill-server'

# I use custom tmux config at ~/.config/tmux/tmux.conf
alias tmux='tmux -f ~/.config/tmux/tmux.conf -2'
```

### GIT CONFIG HIGHLIGHTS

Beyond aliases, my `.git/config` has powerful features:

#### Delta for Beautiful Diffs

```ini
[core]
    pager = delta

[delta]
    navigate = true
    line-numbers = true
    features = decorations
    side-by-side = false
```

#### Conflict Resolution Memory

```ini
[rerere]
    enabled = true
```

This records conflict resolutions and auto-applies them in future rebases!

#### Smart Merge/Rebase Aliases

```ini
[alias]
    # Accept their changes during merge
    merge-take-theirs = !git merge --strategy-option=theirs

    # During rebase, accept current changes
    add-our = !git checkout --theirs "$@" && git add "$@"

    # During rebase, accept incoming changes
    add-their = !git checkout --ours "$@" && git add "$@"
```

#### Backporting Made Easy

```bash
# Backport a commit to another branch
git backport <commit-hash>

# This automatically:
# 1. Creates backport branch
# 2. Cherry-picks the commit
# 3. Opens PR with prefilled template
```

### INSTALLATION FUNCTIONS

I have functions to install entire development stacks:

```bash
_install_basics  # installs: tmux, nvim, fzf, delta, docker, etc.
_install_dev_stack  # full dev environment
_install_python_stuffs  # all Python versions + tools
```

Each installation function:
- Checks if already installed
- Prompts for confirmation
- Handles dependencies
- Provides verbose output

### THE DOTFILES SYSTEM

Everything is managed through my dotfiles:

```bash
# Pull latest dotfiles
_pull_dot_files

# Apply dotfiles to system
_set_dot_files

# Push local changes
_push_dot_files  # commits + pushes to GitHub
```

My config includes:
- `.bashrc` and `.bash_aliases`
- `nvim` configuration
- `tmux` settings
- `i3wm` window manager config
- `git` config and templates
- `alacritty` terminal config

### ENCRYPTION FOR SHARING

When sharing sensitive content via termbin:

```bash
# Encrypt and send to termbin
tsend "secret data"
# https://termbin.com/xyz123

# Decrypt and retrieve
tget xyz123
# "secret data"
```

Uses AES-256 encryption with `gpg` before uploading.

### MORE GIT POWER MOVES

#### Advanced Git Aliases You'll Love

```bash
# Work with patches
git patch-put -1 HEAD  # Generate patch from last commit
git patch-get file.patch  # Apply patch from file

# Fork management
git fork-set-upstream git@github.com:original/repo
git fork-pull-upstream  # Sync your fork

# Stash with fuzzy search
git stash-search  # Interactive stash browser with preview

# Lost commits recovery
git lost-commits  # Show all dangling commits

# Smart reflog for current branch
git reflog-branch  # Only show reflog for current branch

# Branch comparison online
git compare origin  # Opens browser to compare branches

# Hide/unhide tracked files (like .gitignore but tracked)
git hide path/to/file  # Hide from git status
git unhide path/to/file  # Unhide
git hidden  # List all hidden files

# Intent to add (for diffing untracked files)
git add-intent file.py  # Add with zero-length blob

# Interactive adding with your editor
git add-interactive  # Opens editor to select hunks

# Cherry-pick with fuzzy search
git pick-commit  # fzf picker for any commit in history

# Clean Git directory completely
git clean-git-dir  # Prunes, gcs, and optimizes .git
```

#### Git Worktree Magic

Working on multiple branches simultaneously:

```bash
# Create multiple worktrees at once
git checkout-tree fix/bug-123 feat/new-feature refactor/cleanup

# This creates parallel directories:
# ../project-fix-bug-123/
# ../project-feat-new-feature/
# ../project-refactor-cleanup/

# List all worktrees
git tree-list

# Remove, move, lock/unlock worktrees
git tree-remove ../old-worktree
git tree-move ../old-path ../new-path
git tree-lock ../important-worktree
```

#### Git Search & History

```bash
# Search for text across entire Git history
git search "function_name"

# Open specific commit at line in file
git open-code abc123:file.py:42

# View file history with fuzzy search
git log-for-file ./path/to/file

# Log with time filters
git log-after 2024-01-01
git log-since "2 weeks ago"
git log-author "sanix-darker"

# Log specific line ranges
git log-line 10,20:main.py  # History of lines 10-20

# Count commits on branch
git count-commits

# Get branch that contains a tag
git get-branch-tag v1.2.3
```

#### Conflict Resolution Helpers

```bash
# During rebase/merge conflicts
git add-our file.py   # Keep my changes
git add-their file.py # Accept incoming changes

# Reuse recorded resolutions (rerere enabled)
# Git remembers how you resolved conflicts!

# Recreate a merge conflict
git checkout --conflict=merge src/file.py
```

### NEOVIM SPEED TRICKS

My text editor is supercharged with custom keymaps that make editing lightning-fast:

#### Quick Actions

```lua
-- Space is my leader key
vim.g.mapleader = ' '

-- Save and quit (instant)
'<leader>w'  -- :w (save)
'<leader>q'  -- :q (quit)
'<leader>Q'  -- Close all buffers

-- No more capital mistakes
:W  -- becomes :w
:Q  -- becomes :q
:WQ -- becomes :wq
```

#### Window & Split Management

```lua
-- Resize windows with arrows
<Ctrl-Up>    -- Increase height
<Ctrl-Down>  -- Decrease height
<Ctrl-Left>  -- Decrease width
<Ctrl-Right> -- Increase width

-- Split navigation
'ss' -- Horizontal split
'sv' -- Vertical split
'sh' -- Move to left window
'sj' -- Move down
'sk' -- Move up
'sl' -- Move right
```

#### Visual Mode Superpowers

```lua
-- Move selected text up/down
<Shift-K> -- Move selection up
<Shift-J> -- Move selection down

-- Stay in visual mode when indenting
'<'  -- Indent left (stays selected)
'>'  -- Indent right (stays selected)

-- Paste without yanking
'p' in visual -- Paste without copying deleted text
```

#### Smart Line Movement

```lua
-- Move by visual lines when wrapped
'j' -- Moves down visual line (not file line)
'k' -- Moves up visual line

-- But with count, moves by actual lines
'5j' -- Moves 5 actual lines down
```

#### Quick Edits

```lua
-- Add semicolon/comma from insert mode
';;' -- Appends ; to end of line
',,' -- Appends , to end of line

-- Quick file opening
'gf' -- Opens file under cursor (creates if missing)

-- Reselect last paste
-- (commented in my config but useful)
```

### ADVANCED BASH FUNCTIONS

#### Git PR Review Workflow

I built an entire PR review system in bash:

```bash
git_select_pull_request  # Opens fzf menu with:
  # - List of all PRs
  # - Preview of PR description
  # - Preview of CI checks
  # Actions:
  #   - Approve PR
  #   - Merge PR
  #   - Squash & Merge
  #   - View changes
  #   - Approve & Squash
  #   - Close PR
```

Each action triggers the appropriate `gh` command and loops back to the menu.

#### Encryption for Terminal Sharing

```bash
# Full encryption workflow for termbin

export AES_KEY_EN_DE="your-secret-key"

# Encrypt and upload
tsend "secret data"
# https://termbin.com/xyz

# Download and decrypt
tget xyz
# "secret data"

# Works with files too
tsend ~/.ssh/config
tget xyz > recovered-config
```

#### Smart Command Execution

```bash
# Execute in /tmp then return
_xm ls -la  # Runs in /tmp, returns to current dir

# Execute in different repo
cdex ~/other-project git status
# Runs git status in other-project, returns here

# Confirmation wrapper
_confirm "Dangerous operation?" rm -rf /tmp/test

# Repeat until success
tryna wget unstable-url.com/file
# Keeps trying until succeeds

# Repeat until failure (for testing)
trynafail npm test
# Stops when tests start failing
```

#### Environment Management

```bash
# Random string generation
_random_string 16  # Generates 16-char random string

# Extract with regex
EXTRACT_REGEX "$string" '([0-9]+)'

# Environment variable with default
IS_ENV_SET $MY_VAR "default value"

# Check if installed
_installed nvim && echo "Neovim found"
```

#### Process & Port Management

```bash
# Find what's running on port
lsport 3000

# Kill process on port
killport 3000

# Better process search
running firefox  # Shows PID and command

# Graceful process killing
murder firefox  # Tries -15, -2, -1, then -9

# Wait for process to finish
waitfor 1234  # Waits for PID 1234 to exit
waitfor 1234 ; notify 'Done!'
```

### TIME & DATE UTILITIES

```bash
# Current date in ISO format
hoy  # 2025-01-22 (perfect for file prefixing)

# Timer with notification
timer 10m  # 10 minute timer
bb timer 5m  # Background 5 minute timer

# Display current time and calendar
rn  # Shows time + calendar

# Stopwatch
stopwatch  # Live counting stopwatch

# Uptime in readable format
_uptime  # "up 2 hours"

# Sleep with countdown
_sleep 60  # Shows countdown timer
```

### SYSTEM & HARDWARE

```bash
# Brightness control with xrandr
_xrandr ls  # List displays
_xrandr lu DP-1-0.1 0.8  # Set brightness to 80%

# WiFi management
wifi toggle  # Toggle wifi on/off
wifi on
wifi off

# Theme switching (dark/light)
theme 0  # Dark mode everywhere (vim, tmux, terminal)
theme 1  # Light mode everywhere

# Put system to sleep
sleepybear

# Fix SSH permissions
_fix_ssh_key  # chmod 600 ~/.ssh/id_ed25519
```

### DOCUMENTATION & LEARNING

```bash
# Quick reference from cheat.sh
h docker  # Docker cheatsheet
h git log  # Git log examples
h python  # Python examples

# HTTP status codes
httpstatus 404  # "404 Not Found"
httpstatus  # List all codes

# Alphabet (surprisingly useful)
alphabet  # abcdefghijklmnopqrstuvwxyz...

# NATO phonetic alphabet
nato bar  # "Bravo Alfa Romeo"

# Unicode lookup
u+ 2665  # ♥, HEART SYMBOL

# Emoji search
emoji cool  # 😎 😛 😒 🆒

# Lines of code counter
loc py  # Count lines in all .py files
```

### DOCKER WORKFLOW

```bash
# Better container listing
ds  # Table format: Running For | Status | Names

# Complete cleanup
clean_docker_images  # Remove dangling images
clean_docker_container  # Remove all containers
clean_docker_volume_prune  # Prune volumes

# Recursive node_modules cleanup
clean_nodemodules ~/projects
# Finds and deletes all node_modules directories
```

### INSTALLATION AUTOMATION

My entire dev stack installs with functions:

```bash
# Install categories
_install_basics  # Core tools (tmux, nvim, fzf, docker)
_install_dev_stack  # Full development environment
_install_python_stuffs  # All Python versions
_install_nvim_and_utils  # Neovim + LSPs + plugins
_install_i3  # i3 window manager + polybar + rofi
_install_path_browsing_utils  # zoxide, exa, bat

# Language-specific
_install_golang
_install_rust
_install_ruby
_install_java
_install_php
_install_composer

# Tools
_install_docker
_install_docker_compose
_install_lazydocker
_install_alacritty
_install_delta
_install_fzf
_install_tmux

# Each function:
# - Checks if already installed
# - Prompts for confirmation
# - Handles all dependencies
# - Provides verbose output
```

### PDF & FILE VIEWING

```bash
# View PDF in terminal
pdf document.pdf  # Uses pdftotext + less

# Image viewing
pix image.png  # Opens with mpv
vimiv  # Vim-like image viewer

# Video watching in terminal (cursed but cool)
tuivid video.mp4
```

### MATH & CALCULATIONS

```bash
# Quick math
m 2**10  # 1024
m sqrt(144)  # 12
m sin(pi/2)  # 1.0

# IP information
ipinfo  # Shows your IP and location

# Speed test
sp  # Runs speedtest
```

### THE REAL SECRET

The secret to my terminal speed isn't memorizing commands—it's building systems that:

1. **Reduce keystrokes**: Every common task has a 2-3 character alias
2. **Add context**: Functions show relevant info (git status, file previews)
3. **Use fuzzy finding**: `fzf` for selecting from lists instead of typing
4. **Automate confirmations**: Dangerous operations ask before running
5. **Learn from patterns**: Tools like `zoxide` adapt to my workflow
6. **Chain operations**: One command triggers multiple related actions
7. **Fail gracefully**: Always have fallbacks and helpful error messages
8. **Stay composable**: Small functions combine into complex workflows

### SPECIALIZED WORKFLOWS

#### Arduino Development

```bash
# Arduino CLI workflow
arduino-cli core update-index
arduino-cli board list
arduino-cli compile --fqbn esp32:esp32:esp32cam sketch.ino
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32 sketch

# Create new sketch
_new_arduino_sketch  # Creates + sets up serial

# Serial communication
picocom /dev/ttyUSB0 -b 115200
```

#### Android/Mobile

```bash
# Screen mirroring
scrcpy  # Mirror Android to desktop

# Custom with max size
scrcpy -m1024  # Limits to 1024px width
```

#### Network & API Testing

```bash
# gRPC clients
grpcurl list  # List services
evans  # Interactive gRPC client

# HTTP debugging
httpstat https://example.com  # Beautiful HTTP stats

# Static file server
serveit  # Starts server on :8000 (with fallbacks)

# URL parsing
url 'https://evil.com/track?url=https%3A%2F%2Freal.com'
# Shows protocol, hostname, path, query params parsed

# mkcert for local HTTPS
mkcert localhost 127.0.0.1
```

#### Download & Media

```bash
# YouTube/SoundCloud downloads
getsong https://youtube.com/watch?v=...  # Audio only
getpod https://youtube.com/watch?v=...   # Podcast format
getsubs https://video.com/...  # Download subtitles

# Aria2c (fast downloader)
a -x16 https://example.com/bigfile.zip  # 16 connections

# Video compression
shrinkvid input.mp4  # Compresses with ffmpeg

# EXIF removal
removeexif photo.jpg
```

#### Clipboard History

```bash
# Greenclip (clipboard manager)
# Triggered via rofi in i3

# Copy/paste from clipboard
cb  # xclip copy
co  # xclip paste

# Stream clipboard changes
pastas  # Prints on every clipboard change
pastas | wget -i -  # Download every copied link
```

#### Search & Find

```bash
# Silver searcher (ag)
ag "search term"  # Fast code search

# FZF integrations
<Ctrl-T>  # Fuzzy file search
<Ctrl-R>  # Fuzzy history search
<Alt-C>   # Fuzzy directory change

# Git fuzzy integrations (from fzf-git.sh)
git diff **<Tab>  # Fuzzy select files to diff
git add **<Tab>   # Fuzzy select files to add
```

#### Text Processing

```bash
# Specific line from output
cat file.txt | line 10  # Get line 10

# Markdown quote formatter
echo "text" | markdownquote  # Prepends > to each line

# JSON formatting
echo '{"a":1}' | jsonformat  # Pretty prints

# Case conversion
echo "hello" | uppered  # HELLO
echo "WORLD" | lowered  # world

# Straight quotes (for code)
straightquote file.txt  # Converts "smart" to "straight"

# Length of string
length "hello"  # 5

# Scratch buffer in vim
scratch  # Opens temp file in vim for quick notes
```

#### Screen & Display

```bash
# Screen layout management
~/.screenlayout/dual-monitor.sh

# Screenshot & recording
peek  # GIF recorder

# Image viewing
nsxiv image.png  # Vim-like image viewer
```

#### Project Management

```bash
# Lines search in file with nvim
vf "search term" file.py
# Opens nvim with :Lines search

# Create directory and cd
md new-project  # mkdir + cd

# Touch with directory creation
touchd /path/that/doesnt/exist/file.txt
# Creates all dirs + file

# Remove current directory
rd  # Confirms then deletes pwd

# Copy with directory creation
cpd source.txt /new/path/dest.txt
# Creates /new/path/ if needed
```

#### ISO & VM Management

```bash
# Download ISOs with fuzzy search
_search_isos  # fzf menu of common Linux ISOs

# Vagrant
_install_vagrant
_install_virtualbox

# Balena Etcher for USB boot
balena-etcher --no-sandbox
```

#### Security & Secrets

```bash
# GPG key generation for Git signing
_generate_gpg_keys

# Git commit signing
git commit-signed

# ggshield (GitGuardian)
ggshield auth login
ggshield install --mode local  # Prevents secret leaks
```

#### Package Management Helpers

```bash
# Cargo update all
_cargo_update

# NVM shortcuts
nvm use 20
nvm install 16

# Pip with --break-system-packages
pip install pandas --break-system-packages

# Poetry & debugging
ee_install_debuglibs  # ipdb, debugpy for venv
```

### TMUX MASTERY

```bash
# Session management
tn work      # New session "work"
ta work      # Attach to "work"
tls          # List sessions

# My custom FZF session switcher
<Ctrl-F>     # Fuzzy session search (custom binding)

# Popup fzf inside tmux
fzfp         # FZF in tmux popup

# Config location
tmux -f ~/.config/tmux/tmux.conf -2
```

### MONITORING & SYSTEM INFO

```bash
# Neofetch alternative
neofetch

# Process monitoring
htop   # CPU monitor
nvtop  # GPU monitor (NVIDIA)

# Disk usage
size /path/to/dir  # du -sh wrapper

# System services
services  # List all systemd services

# Watch for file changes
echo file.txt | entr command
# Runs command when file.txt changes
```

### MAIL & COMMUNICATION

```bash
# Thunderbird (email)
thunderbird

# Slack
slack

# Telegram
telegram-desktop
```

### DEVELOPMENT SERVERS

```bash
# PHP server
php -S localhost:8000

# Python server
serveit  # python -m http.server wrapper

# Node server (http-server)
hs  # http-server -o (opens browser)
```

### OVERLOOKED POWER MOVES

#### Boop - Audio Feedback for Long Commands

```bash
# Get audio notification on command completion
run_long_test ; boop
# Plays happy sound on success, sad sound on failure

# Perfect for:
npm install ; boop
cargo build ; boop
pytest ; boop
```

#### Smart Git Functions I Use Daily

```bash
# Coworker's branch checkout
git pull-coworker author:branch-name
# Automatically adds their remote, fetches, and checks out

# Update any branch without switching
git update-branch dev-branch origin
# Stashes, fetches, updates, switches back, unstashes

# Open PR link in browser
git open-pr  # Finds PR number and opens in firefox

# Get link to last commit
git last-commit-link

# Compare branch online
git compare origin  # Opens browser to GitHub compare view
```

#### The "_xm" Pattern - Temporary Directory Execution

```bash
# Execute in /tmp then return
_xm wget https://example.com/installer.sh
_xm tar -xzf archive.tar.gz
_xm python3 test-script.py

# All run in /tmp, automatic cleanup
```

#### Confirmation System for Safety

Every dangerous operation uses `_confirm`:

```bash
_confirm "Delete production database?" drop_db
_confirm "Install 50GB of dependencies?" install_all

# Set NOTINTERACTIVE=1 to skip all prompts
NOTINTERACTIVE=1 _install_dev_stack
```

#### Color Output Functions

```bash
_echo_green "Success message"
_echo_red "Error message"
_echo_blue "Info message"
_echo_yellow "Warning message"
```

#### File Listing with Trash

```bash
# List trash contents for current directory
lf  # Shows deleted items from pwd

# Restore specific item
trash-restore

# Empty specific patterns
trash-rm "*.tmp"

# Complete trash wipe
rrff  # trash-empty
```

#### Git Squashing Strategies

```bash
# Squash everything on branch
git squash  # Interactive from origin

# Squash to specific branch
git squash-to dev

# Squash exact number of commits
git squash-count 5

# Revert all branch commits
git revert-all  # Reverts all commits not in origin
```

#### Advanced Git Status

```bash
# Only modified files
git status-modified

# Only deleted files
git status-deleted

# Only untracked files
git status-created

# Only staged files
git status-staged
```

#### Git Tree Operations

```bash
# Checkout multiple branches as worktrees
git checkout-tree feat/a feat/b feat/c
# Creates 3 parallel directories

# List all worktrees with details
git tree-list

# Prune deleted worktrees
git tree-prune

# Lock important worktree
git tree-lock ../critical-work
```

#### Smart Dotfiles Management

```bash
# Copy from system to repo
_copy_to_dotfiles
# Automatically copies:
# - nvim config
# - tmux config
# - bash configs
# - i3/polybar/rofi configs
# - git config

# Push changes to GitHub
_push_dot_files
# Runs: copy → diff → commit → push

# Apply dotfiles to system
_set_dot_files
# Copies everything from repo to ~/.config
```

#### Installation Checks

```bash
# Check and confirm reinstall
_confirm_install_again docker
# Prompts only if already installed

# Check if command exists
_installed nvim && echo "Found" || echo "Missing"
```

#### Process Management Tricks

```bash
# Run truly in background (no STDOUT/STDERR)
bb ollama serve  # Never hear from it again
bb timer 5m      # Background timer

# Monitor and wait
waitfor 12345 ; notify "Process done"

# Search processes
psaux firefox  # ps aux | grep firefox

# Running processes by name
running node   # Shows PID + command
```

#### Network Utilities

```bash
# Check port usage
lsport 3000    # What's listening on 3000?
killport 3000  # Kill process on 3000

# WiFi control
wifi toggle
wifi on
wifi off

# IP info
ipinfo  # Your IP + location
```

#### Path Manipulation

```bash
# Pretty print $PATH
prettypath  # One entry per line

# Add to $PATH easily (in .bashrc)
export PATH="$PATH:/new/path"
```

#### Math & Text Length

```bash
# Quick calculations
m 2**16  # 65536
m sqrt(144)  # 12

# String length
length "hello world"  # 11
```

#### Process Helpers

```bash
# Kill gracefully (tries -15, -2, -1, -9)
murder firefox

# Run until succeeds
tryna wget unstable-url.com

# Run until fails (for testing)
trynafail npm test

# Each loop with replacements
ls | each 'echo "File: {}"'
# Better than xargs for simple tasks
```

#### Timer & Stopwatch

```bash
# Countdown with notification
timer 10m

# Background countdown
bb timer 5m

# Live stopwatch
stopwatch

# Visible countdown
_sleep 60  # Shows countdown
```

#### Git Lost & Found

```bash
# Find dangling commits
git lost-commits

# Reflog for current branch only
git reflog-branch

# Revive lost work
git checkout <sha>  # From lost-commits output
```

#### Branch Operations

```bash
# Checkout with search
git checkout-search  # fzf picker

# Recent branches
git brr  # Fuzzy recent branch picker

# Branch details with dates
git branch-details  # Full info sorted by date

# Branch sorted by commit date
git branch-sorted
```

#### File Status with FZF

```bash
# Fuzzy file picker for git status
git status-search  # Pick files to stage

# Smart add with preview
git add-search  # FZF with diff preview
```

#### Amend Shortcuts

```bash
# Quick amend variations
git add-amend              # add -u + amend
git add-amend-no-edit      # add -u + amend --no-edit
git add-amend-no-verify    # add -u + amend --no-verify
git amend-no-edit-no-verify  # fastest amend

# Change commit author
git amend-author "New Name <email@example.com>"
```

#### Log Filtering

```bash
# After specific date
git log-after 2024-01-01

# By author
git log-author sanix-darker

# Specific line range in file
git log-line 10,50:main.py

# From specific branch
git log-from-branch dev
```

#### Ignore File Management

```bash
# List what would be deleted
git list-ignore-files

# Actually delete ignored files
git remove-ignore-files
```

### BUILDING YOUR OWN

Start small:

1. **Track what you type**: Run `history | awk '{print $2}' | sort | uniq -c | sort -rn | head -20`
2. **Alias the top 20**: Create shortcuts for your most-used commands
3. **Add fzf gradually**: Replace manual selection with fuzzy finding
4. **Build confirmation wrappers**: Protect yourself from mistakes
5. **Version control everything**: Keep your dotfiles in Git

### THE PAYOFF

With these optimizations:
- I navigate repos **10x faster** than using GUI tools
- Creating PRs takes **5 seconds** instead of minutes
- I never lose work to accidental deletions
- Complex Git operations become one-liners
- I can work effectively over SSH with high latency

The investment in building these tools pays off every single day. After thousands of hours in terminals, these optimizations have saved me weeks of time.

**My complete dotfiles**: https://github.com/Sanix-Darker/dotfiles

---

*Want to level up your terminal game? Start by aliasing your five most-used commands. Then build from there. In a year, you'll wonder how you ever worked without them.*
