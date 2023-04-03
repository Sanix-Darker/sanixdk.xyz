#!/bin/bash

set -e

echo "> Building libcyaml <cyaml/cyaml.h>";

# we need this binding unfortunately
apt-get install libyaml-dev -y

# we cd in the lib and we build it
cd ./libcyaml/ && \
    make install VARIANT=release && \
        [ "$?" == "0" ] && echo "Built successfully !" && \
            exit 0;

echo "< ERROR:: Something went wrong !";
exit 1;
