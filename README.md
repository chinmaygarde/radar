Radar [![CircleCI](https://circleci.com/gh/chinmaygarde/radar.svg?style=svg&circle-token=5df84d6f43aa1ed0e7b0299e21a4bbdc5872133c)](https://circleci.com/gh/chinmaygarde/radar)
=================

Prerequisites
-------------
#### Mac
* Xcode 7.2 or newer.
* CMake
  * `brew install cmake`

#### Linux
* A C++ 11 compiler: `GCC 4.9` or above **or** `Clang 3.6` or newer.
* CMake, SDL 2, OpenGLES 2.
  * `apt-get install -y cmake libgles2-mesa-dev libsdl2-dev ninja-build`.
  * There is also a preconfigured `Vagrantfile` in source root if that is your style.
    * `vagrant up linux; vagrant ssh linux; cd /vagrant;`.
    * Limitation: In the Vagrant environment, you wont be able to run the desktop application, only build the tools, run tests, etc..

Build Instructions
------------------

#### Building on Mac OSX
* Create a directory called `build` in the source root and move into it.
  * `mkdir build; cd build;`
* Generate the `Xcode` project for all targets.
  * `cmake ../ -G Xcode; cd ../`
* Open `build/Radar.xcodeproj` and run the scheme you want.

#### Building on Linux
* Create a directory and move into it.
  * `mkdir build; cd build;`
* Generate either the `Makefile` or `Ninja` project via `CMake`
  * For a `Makefile` project: `cmake ../`
  * For a `Ninja` project: `cmake ../ -G Ninja`
* Build. You can also selectively build a particular target by specifying its name after the command.
  * `make -j<jobs>`
  * `ninja`

Linters & Formatters
--------------------

All tools are run from the source root and require `rake`

#### Formatters
* Make sure `clang-format` is installed.
  * `rake format`

#### Linters
* Make sure `cpplint` is installed.
  * `rake lint`
