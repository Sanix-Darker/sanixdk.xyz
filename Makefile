.DEFAULT_GOAL := help

SHELL := /bin/bash # Use bash syntax

## install: to install a bunch of stuffs + some libs
install:
	# install gcc and init submodule
	apt-get install git gcc && git submodule init
	# cd on externals and compile all the libs
	cd ./externals/ && ./libcyaml.sh
	# get back where we were.
	cd -

## compile: Compile the app itself
compile:
	gcc -Wall \
	-I/usr/local/cyaml/include -lpthread -s \
	main.c lib.c structs.h lib.h \
	-o web

## build: Build the webpage that are going to be serve
build:
	./web build

## serve: Serve the whole webSite
serve:
	./web serve

all: help
help: Makefile
	@echo " Choose a command: "
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'

.PHONY: install build build-website run-website help
