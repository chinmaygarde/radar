Radar [![Documentation](https://img.shields.io/badge/documentation-latest-green.svg)](http://radar.run) [![Trello Board](https://img.shields.io/badge/trello-board-green.svg)](https://trello.com/b/9BYQ00JW/radar) [![Build Status](http://radar.run:8000/api/badges/chinmaygarde/radar/status.svg)](http://radar.run:8000/chinmaygarde/radar)
=================

Prerequisites
-------------
#### Mac
* Xcode 7.2 or newer.
* CMake
  * `brew install cmake`

#### Linux
* A C++ 11 compiler: `GCC 4.9` or above **or** `Clang 3.7` or newer.
* CMake, SDL 2, OpenGLES 2.
  * `apt-get install -y cmake libgles2-mesa-dev libsdl2-dev ninja-build`.
  * There is also a preconfigured `Vagrantfile` in source root if that is your style.
    * `vagrant up linux; vagrant ssh linux; cd /vagrant;`.
    * Limitation: In the Vagrant environment, you wont be able to run the desktop application, only build the tools, run tests, etc..

#### Native Client
* The [Native Client SDK](https://developer.chrome.com/native-client/sdk/download)
* Update to `Pepper 44` or above.
  * `naclsdk update; naclsdk update pepper_44;`

#### Windows 10 (Universal Application)
* Visual Studio Community 2015 or newer.
* CMake version 3.4 or newer.


Build Instructions
------------------

#### Building on Mac OSX
* Create a directory called `build` in the source root and move into it.
  * `mkdir build; cd build;`
* Generate the `Xcode` project for all targets.
  * `cmake ../ -G Xcode; cd ../`
* Open `Platforms/Mac/RadarMac.xcodeproj` and run the scheme you want.
  * The desktop shell has the `RadarMac` application scheme.

Note: Though the application target on Mac can be run via the `Platforms/Mac/RadarMac.xcodeproj` target, the test target must be launched via `Radar.xcodeproj` in `build`.

#### Building on Linux
* Create a directory and move into it.
  * `mkdir build; cd build;`
* Generate either the `Makefile` or `Ninja` project via `CMake`
  * For a `Makefile` project: `cmake ../`
  * For a `Ninja` project: `cmake ../ -G Ninja`
* Build. You can also selectively build a particular target by specifying its name after the command.
  * `make -j<jobs>`
  * `ninja`

#### Building for Native Client
* Create a directory and move into it.
  * `mkdir -p build/nacl; cd build/nacl;`
* Generate a `Makefile` project via `CMake` (`Ninja` works too) and ensure you pass in two additional variables: `CMAKE_TOOLCHAIN_FILE` and `NACL_SDK_ROOT`
  * `cmake ../../ -DCMAKE_TOOLCHAIN_FILE=../../Platforms/Nacl/Nacl.cmake -DNACL_SDK_ROOT=/path/to/nacl_sdk/pepper_44/`
* Build like the other targets
  * `make -j<jobs>`
  * Or, `ninja` if you specified `-G Ninja` to `CMake`
* To run in your browser, change into the `Platforms/Nacl` directory in your `build` directory and run the web server.
  * `cd Platforms/Nacl`
  * `./Server`
* Navigate the `Chrome Browser` to [the application](http://localhost:8000/Radar.html)

#### Building on Windows 10 (Universal)
* Ensure you have at least `CMake` version 3.4.
* Create a directory and move into it.
  * `mkdir build; cd build;`
* Generate a `Visual Studio` solution via `CMake` and ensure you specify the `CMAKE_SYSTEM_NAME` and `CMAKE_SYSTEM_VERSION` variables.
  * `cmake ../ -G "Visual Studio 14 2015" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0`
* Open the `Radar` solution generated and run any of the given targets.

Linters & Formatters
--------------------

All tools are run from the source root and require `rake`

#### Formatters
* Make sure `clang-format` is installed.
  * `rake format`

#### Linters
* Make sure `cpplint` is installed.
  * `rake lint`
