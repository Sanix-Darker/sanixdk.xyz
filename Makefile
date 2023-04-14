.DEFAULT_GOAL := help

SHELL := /bin/bash # Use bash syntax

## compile: Compile the app builder itself
compile:
	gcc -Wall -s main.c main.h\
	./md4c/entity.c ./md4c/entity.h \
	./md4c/md4c-html.c ./md4c/md4c-html.h \
	./md4c/md4c.c ./md4c/md4c.h -o sdk

## build: Build the webpage that is going to be serve
# TODO: find a way to match components inside the final renderer page
build:
	mkdir ./public && mkdir ./public/blogs && \
    mkdir ./public/components \
    mkdir ./public/projects && ./sdk build

## polish: To concatenate header/footer components to all the pages
polish:
	# we compress the css (yeah i need that part too unfortunatelly)
	cat ./content/style.css | tr -d '[:space:]' >  ./public/style.css;
	# ./content/projects/*.md since there is no project for now
	for f in ./content/*.md ./content/blogs/*.md; \
	do \
		content=`cat $$f`; \
		cat ./content/components/header.md > $$f; \
		echo -ne "$$content" >> $$f; \
		cat ./content/components/footer.md >> $$f; \
	done;

## serve: Serve the app (Not implemented/nor ready yet)
serve:
	./sdk serve

## docker-build: Docker build for the website
docker-build:
	DOCKER_BUILDKIT=0 docker build --rm -t sdk.xyz:latest -f ./Dockerfile .

## docker-run: Docker run for the website
docker-run:
	docker run -it sdk.xyz:latest -p 3003:8080

all: help
help: Makefile
	@echo " Choose a command: "
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'

.PHONY: compile build serve help docker-build
