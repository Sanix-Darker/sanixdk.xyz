#!/bin/bash

git submodule init && \
DOCKER_BUILDKIT=0 docker build --rm -t sanix-darker.com:latest -f ./Dockerfile .
