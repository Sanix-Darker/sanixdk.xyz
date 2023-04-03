## EXTERNALS

## WHY

Because C lang doesn't support a light and simple package manager.

## HOW

This folder will contains all external libs use in this project but linked as git-submodules
So you need to `git submodule init` (which is inside the Makefile as a target on 'make install') to get all of those.

For the libs, we're going to have:
- *libcyaml* : a Schema-based YAML parsing and serialisation. (the script is ./libcyaml.sh)


Each libs are going to have a script at the root with the same name as the repo,
the script will contains all steps to build the lib.
