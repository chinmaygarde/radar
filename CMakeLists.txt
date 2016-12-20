# Copyright 2015 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following disclaimer
#      in the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Google Inc. nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.4)

################################################################################
################################################################################
####################     ______          _               #######################
####################     | ___ \        | |              #######################
####################     | |_/ /__ _  __| | __ _ _ __    #######################
####################     |    // _` |/ _` |/ _` | '__|   #######################
####################     | |\ \ (_| | (_| | (_| | |      #######################
####################     \_| \_\__,_|\__,_|\__,_|_|      #######################
####################                                     #######################
################################################################################
################################################################################

project(Radar)

################################################################################
# Import CMake Utilities
################################################################################


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake")

include(AddCXXWarningIfSupported)
include(AddRecommendedWarningFlags)
include(CheckCompiler)
include(CheckPlatform)
include(HardenToolchain)
include(StandardRadarBench)
include(StandardRadarGraphicsTest)
include(StandardRadarLibrary)
include(StandardRadarTest)
include(SymbolVisibility)
include(TreatAsCXX)

CheckPlatform()

CheckCompiler()

HardenToolchain()

SetRecommendedSymbolVisibility()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# CCache
################################################################################

find_program(CCACHE_PROGRAM NAMES ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

################################################################################
# Ranlib Symbol Notifications on Mac
################################################################################

if(APPLE)
  SET(CMAKE_C_ARCHIVE_CREATE
      "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_CXX_ARCHIVE_CREATE
      "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_C_ARCHIVE_FINISH
      "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
  SET(CMAKE_CXX_ARCHIVE_FINISH
      "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

################################################################################
# Benchmarking
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_BENCHMARKING_ENABLED OFF CACHE BOOL "Disable benchmarking")
endif()

option(RADAR_BENCHMARKING_ENABLED "Enable benchmarking." ON)

if(RADAR_BENCHMARKING_ENABLED)
  # Build the benchmarking target with its tests disabled.
  add_custom_target(bench)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable tests on benchmarks")
  add_subdirectory("ThirdParty/googlebenchmark")
endif()

################################################################################
# Testing
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_TESTING_ENABLED OFF CACHE BOOL "Disable testing")
endif()

option(RADAR_TESTING_ENABLED "Enable testing." ON)

if(RADAR_TESTING_ENABLED)
  enable_testing()
endif()

################################################################################
# Graphics Testing
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_GRAPHICS_TESTING_ENABLED OFF CACHE BOOL "Disable graphics testing")
endif()

option(RADAR_GRAPHICS_TESTING_ENABLED "Enable graphics testing." ON)

if(RADAR_GRAPHICS_TESTING_ENABLED)
  enable_testing()
endif()

################################################################################
# Sanitizers (Require Custom Clang Setup)
# Usage:
#     $ cmake -DSANITIZE=address
#     Any supported Clang sanitizer can be specified
################################################################################

if(SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE} -g -O1")
endif()

################################################################################
# Force in-process variants.
################################################################################

if(DISABLE_XPC)
  add_definitions(-DRL_DISABLE_XPC=1)
endif()

################################################################################
################################################################################
###############     _     _ _                    _               ###############
###############    | |   (_) |                  (_)              ###############
###############    | |    _| |__  _ __ __ _ _ __ _  ___  ___     ###############
###############    | |   | | '_ \| '__/ _` | '__| |/ _ \/ __|    ###############
###############    | |___| | |_) | | | (_| | |  | |  __/\__ \    ###############
###############    \_____/_|_.__/|_|  \__,_|_|  |_|\___||___/    ###############
###############                                                  ###############
################################################################################
################################################################################

add_subdirectory("Library/Animation")
add_subdirectory("Library/BenchmarkRunner")
add_subdirectory("Library/Compositor")
add_subdirectory("Library/Coordinator")
add_subdirectory("Library/Core")
add_subdirectory("Library/DevSupport")
add_subdirectory("Library/Entity")
add_subdirectory("Library/Event")
add_subdirectory("Library/Geometry")
add_subdirectory("Library/GraphicsTestRunner")
add_subdirectory("Library/Image")
add_subdirectory("Library/Interface")
add_subdirectory("Library/InterfaceBuilder")
add_subdirectory("Library/Layout")
add_subdirectory("Library/Shell")
add_subdirectory("Library/TestRunner")
add_subdirectory("Library/Toolbox")
add_subdirectory("Library/Typography")

add_subdirectory("ThirdParty/base64")
add_subdirectory("ThirdParty/freetype")
add_subdirectory("ThirdParty/gtest")
add_subdirectory("ThirdParty/harfbuzz")
add_subdirectory("ThirdParty/imgui")
add_subdirectory("ThirdParty/libtess2")
add_subdirectory("ThirdParty/pugixml")
add_subdirectory("ThirdParty/sqlite3")
add_subdirectory("ThirdParty/stb")
add_subdirectory("ThirdParty/uriparser")

set(BUILD_EXTRA_PROGRAMS OFF CACHE BOOL "Disable extra programs." FORCE)
add_subdirectory("ThirdParty/SwiftShader")

################################################################################
################################################################################
############    ______ _       _    __                             #############
############    | ___ \ |     | |  / _|                            #############
############    | |_/ / | __ _| |_| |_ ___  _ __ _ __ ___  ___     #############
############    |  __/| |/ _` | __|  _/ _ \| '__| '_ ` _ \/ __|    #############
############    | |   | | (_| | |_| || (_) | |  | | | | | \__ \    #############
############    \_|   |_|\__,_|\__|_| \___/|_|  |_| |_| |_|___/    #############
############                                                       #############
################################################################################
################################################################################

################################################################################
# Mac OSX
################################################################################

# 1: `mkdir build`
# 2: `cd build`
# 3: `cmake ../ -G Xcode`
# 4: `open ../Platforms/Mac/RadarMac.xcodeproj`
#
# Xcode is your friend.

################################################################################
# Linux Desktop Shell
################################################################################

if(LINUX)
  add_subdirectory("Platforms/Linux")
endif()

################################################################################
# Rasperry Pi Shell
################################################################################

if(RASPBERRY)
  add_subdirectory("Platforms/RaspberryPi")
endif()

################################################################################
# Native Client (PNaCl Only) Shell
################################################################################

if(NACL)
  add_subdirectory("Platforms/Nacl")
endif()

################################################################################
# Android Shell
################################################################################

if(ANDROID)
  add_subdirectory("Platforms/Android")
endif()

################################################################################
# FreeBSD
################################################################################

# Everything should be automatic. There is no shell, only tests and benchmarks.

################################################################################
################################################################################
#######################    _____           _        ############################
#######################   |_   _|         | |       ############################
#######################     | | ___   ___ | |___    ############################
#######################     | |/ _ \ / _ \| / __|   ############################
#######################     | | (_) | (_) | \__ \   ############################
#######################     \_/\___/ \___/|_|___/   ############################
#######################                             ############################
################################################################################
################################################################################

add_subdirectory("Tools/RIDL")
