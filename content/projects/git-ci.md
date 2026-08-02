<img src="https://cdn.jsdelivr.net/gh/sanix-darker/git-ci@master/asset.png" />

# git-ci

Run CI/CD workflows locally from your terminal. `git-ci` parses popular pipeline formats and executes jobs locally with Bash, Docker, or Podman runners.

## Highlights

- Parse multiple CI providers: GitHub, GitLab, CircleCI, Drone, Travis
- Run jobs with filtering (`--job`, `--only`, `--except`, `--stage`)
- Matrix strategy expansion and dependency-aware scheduling
- Parallel execution with `--parallel` / `--max-parallel`
- Dry-run mode for safe validation before execution
- Built-in `validate`, `init`, `env`, `config`, and `discover` helpers

## Install

```bash
# from source

go install github.com/sanix-darker/git-ci/cmd@latest

# or download a release binary
curl -L https://github.com/sanix-darker/git-ci/releases/latest/download/gci-$(uname -s)-$(uname -m) -o gci
chmod +x gci
sudo mv gci /usr/local/bin/

# optional: keep parity with git aliases
git config --global alias.ci '!gci'
```

## Quick start

```bash
git ci run -f .github/workflows/ci.yml

git ci run --only "test-*"
git ci run --docker

git ci validate --strict

git ci init --provider github
```

## Links

- [GitHub repository](https://github.com/sanix-darker/git-ci)
- [project homepage](https://git-ci.sanixdk.xyz)
- [docs and reference](https://github.com/sanix-darker/git-ci#readme)
