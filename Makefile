.DEFAULT_GOAL := help

SHELL := /bin/bash # Use bash syntax

## compile: Compile the app itself
compile:
	gcc -Wall -lpthread -s \
	main.c lib.c lib.h \
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

.PHONY: compile build help
