# sanixdk.xyz

A small C static-site generator for my Markdown-based personal site.

## Local development

Requirements: GCC, Make, Bash, `jq`, `bc`, `curl`, and Python 3 for the
test HTTP server.

```console
make build
make e2e
```

`make build` compiles the generator and recreates `public/`. Generated HTML
is intentionally ignored by Git. `make e2e` rebuilds the site and checks the
generated pages with browser captures when available, or deterministic HTTP
fallback assertions.

## Container

```console
make up
```

The site is then available on <http://localhost:3003>. Stop it with:

```console
make down
```

Run `make` to list every supported target.
