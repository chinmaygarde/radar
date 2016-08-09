FROM ubuntu:16.04
MAINTAINER Chinmay Garde <chinmaygarde@gmail.com>
RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y libgles2-mesa-dev
RUN apt-get install -y libsdl2-dev
RUN apt-get install -y ninja-build
RUN apt-get install -y gdb
RUN apt-get install -y git
RUN apt-get install -y man
