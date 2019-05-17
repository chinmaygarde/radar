# Radar Development Environment for Linux
# =======================================
#
# Launch using:
# docker run --rm -it -v `pwd`:/Radar -w /Radar chinmaygarde/radar:v2 /bin/bash
#
# Build new Docker image using:
# docker build -t chinmaygarde/radar:v3 .
#
# Publish to Docker Hub using:
# docker push chinmaygarde/radar:v3
#
# Dont forget to update the CI endpoints.

FROM ubuntu:latest
MAINTAINER Chinmay Garde <chinmaygarde@gmail.com>

# Update dependencies.
RUN apt-get update

# Install dependencies.
RUN apt-get install -y       \
        build-essential      \
        ccache               \
        cmake                \
        gdb                  \
        git                  \
        libgles2-mesa-dev    \
        libsdl2-dev          \
        locales              \
        man                  \
        ninja-build          \
        python-pip           \
        python2.7            \
        python2.7-dev        \
        ssh                  \
        unzip                \
        valgrind             \
        wget                 \
        ruby-full

# Install CPP Lint
RUN pip install cpplint

# Fix locale.
RUN rm -rf /var/lib/apt/lists/*
RUN localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG en_US.utf8
