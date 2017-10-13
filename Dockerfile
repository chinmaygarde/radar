# Radar Development Environment for Linux
# =======================================
#
# Launch using:
# docker run --rm -it -v `pwd`:/Radar -w /Radar chinmaygarde/radar /bin/bash
#

FROM ubuntu:rolling
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
        wget

# Fix locale.
RUN rm -rf /var/lib/apt/lists/*
RUN localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
ENV LANG en_US.utf8
