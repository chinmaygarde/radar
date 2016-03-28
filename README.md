Radar [![Documentation](https://img.shields.io/badge/documentation-latest-green.svg)](http://radar.run)
=================

Prerequisites
-------------
#### Mac
* Xcode 7.2 or newer.
* CMake
  * `brew install cmake`

#### Linux
* CMake, SDL 2, OpenGLES 2.
  * `apt-get install -y cmake libgles2-mesa-dev libsdl2-dev ninja-build`.
  * There is also a preconfigured `Vagrantfile` in source root if that is your style.
    * `vagrant up linux; vagrant ssh linux; cd /vagrant;`.
    * In the Vagrant environment, you wont be able to run the desktop application, only build the tools, run tests, etc..

Build Instructions
------------------

#### Building on a Mac
* Create a directory called `build` in the source root and move into it. This directory is ignored by Git.
  * `mkdir build; cd build;`
* Generate the Xcode project for all targets.
  * `cmake ../ -G Xcode; cd ../`
* Open `Platforms/Mac/RadarMac.xcodeproj` and run the scheme you want.
  * The desktop shell has the `RadarMac` application scheme.

#### Building on a Mac
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
