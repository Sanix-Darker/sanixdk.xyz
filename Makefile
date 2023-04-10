.DEFAULT_GOAL := help

SHELL := /bin/bash # Use bash syntax

## compile: Compile the app builder itself
compile:
	gcc -Wall -s main.c \
	./md4c/entity.c ./md4c/entity.h \
	./md4c/md4c-html.c ./md4c/md4c-html.h \
	./md4c/md4c.c ./md4c/md4c.h -o sdk

## build: Build the webpage that is going to be serve
build:
	./sdk build

## serve: Serve the app (Not implemented/nor ready yet)
serve:
	./sdk serve

all: help
help: Makefile
	@echo " Choose a command: "
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'

.PHONY: compile build help
