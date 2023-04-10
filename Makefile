.DEFAULT_GOAL := help

SHELL := /bin/bash # Use bash syntax

## install: to install a bunch of stuffs + some libs
install:
	# should be run before the docker commands
	# install gcc and init submodule
	# or git submodule update --init
	apt-get install git gcc && git submodule init
	# cd on externals and compile all the libs
	cd ./externals/ && ./libcyaml.sh
	# get back where we were.
	cd -

## compile: Compile the app itself
compile:
	gcc -Wall -L/usr/local/lib -lcyaml \
    -I/usr/local/cyaml/include -lpthread -s \
	main.c \
		lib.c \
		lib.h \
		structs.h \
		./md4c/entity.c \
		./md4c/entity.h \
		./md4c/md4c-html.c \
		./md4c/md4c-html.h \
		./md4c/md4c.c \
		./md4c/md4c.h -o web

## build: Build the webpage that are going to be serve
build:
	./web build

all: help
help: Makefile
	@echo " Choose a command: "
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'

.PHONY: install build build-website run-website help
