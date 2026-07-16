.DEFAULT_GOAL := help

# $ git restore content/ && rm -rf public/ && make build && hs ./public/
SHELL := /bin/bash # Use bash syntax

## compile: Compile the app builder itself
compile:
	gcc -pipe -Wall -s main.c ./lib/*.c -o builder

# To concatenate header/footer components to all the pages
## build: Build the webpage that is going to be serve
build: compile
	rm -rf ./public/
	./builder build
	git restore ./content/

## e2e: Run the browser-use E2E harness against ./public/ (writes docs/TEST_LOG.md, exits non-zero on any FAIL)
e2e:
	./scripts/e2e/run.sh

## e2e-story ID=<story>: Run a single user story (overwrites docs/TEST_LOG.md with one row). Useful for CI triage.
e2e-story:
ifndef ID
	$(error ID=<story> is required (e.g. `make e2e-story ID=F-09` or `make e2e-story ID=BROWSER`))
endif
	./scripts/e2e/run.sh "$(ID)"

## serve: Explain how to run the site locally
serve: compile
	./builder serve

## docker-build: Docker build for the website
docker-build:
	docker build --rm -t sanixdk.xyz:latest -f ./Dockerfile .

## docker-run: Docker run for the website
docker-run: docker-build
	docker run --rm -it -p 3003:8080 sanixdk.xyz:latest

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

prod:
	git pull --rebase --autostash
	$(MAKE) build

.PHONY: compile build serve e2e e2e-story help docker-build docker-run up down prod
