
install:
	apt-get install gcc

build:
	gcc -Wall main.c lib.c structs.h lib.h -lpthread -s -o web

build-website:
	./web build

run-website:
	./web run

.PHONY: install build build-website run-website
