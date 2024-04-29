.DEFAULT_GOAL := help

# $ git restore content/ && rm -rf public/ && make build && hs ./public/
SHELL := /bin/bash # Use bash syntax

## compile: Compile the app builder itself
compile:
	gcc -Wall -s main.c ./lib/*.c -o builder

# To concatenate header/footer components to all the pages
## build: Build the webpage that is going to be serve
build: compile
	./builder build

## serve: Serve the app (Not implemented/nor ready yet)
serve:
	./sdk serve

## docker-build: Docker build for the website
docker-build:
	docker build --rm -t sanixdk.xyz:latest -f ./Dockerfile .

## docker-run: Docker run for the website
docker-run: docker-build
	# --volume ./public/:/usr/share/nginx/html/
	docker run -it sanixdk.xyz:latest -p 3003:8080

## up: To build and deploy the website
up:
	docker compose -f ./docker-compose.yml up --build

## down: To get down the service
down:
	docker compose -f ./docker-compose.yml down

all: help
help: Makefile
	@echo " Choose a command..."
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'

.PHONY: compile build serve help docker-build docker-run
