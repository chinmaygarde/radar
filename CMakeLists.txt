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

cmake_minimum_required(VERSION 3.0.2)

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
################################################################################
##################     _____ _               _            ######################
##################    /  __ \ |             | |           ######################
##################    | /  \/ |__   ___  ___| | _____     ######################
##################    | |   | '_ \ / _ \/ __| |/ / __|    ######################
##################    | \__/\ | | |  __/ (__|   <\__ \    ######################
##################     \____/_| |_|\___|\___|_|\_\___/    ######################
##################                                        ######################
################################################################################
################################################################################

include(CheckIncludeFiles)

# The RaspberryPi has the bcm_host.h file at a specific location.
check_include_files("/opt/vc/include/bcm_host.h" RASPBERRY)

if(WIN32)
  if(NOT WINDOWS)
    set(WINDOWS TRUE)
  endif()
elseif(UNIX AND NOT APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    if(NOT RASPBERRY)
      set(LINUX TRUE)
    endif()
  elseif(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD.*")
    set(FREEBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
    set(NETBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "kOpenBSD.*|OpenBSD.*")
    set(OPENBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*GNU.*")
    set(GNU TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*BSDI.*")
    set(BSDI TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "DragonFly.*|FreeBSD")
    set(FREEBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "SYSV5.*")
    set(SYSV5 TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "Solaris.*")
    set(SOLARIS TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "HP-UX.*")
    set(HPUX TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "AIX.*")
    set(AIX TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "Minix.*")
    set(MINIX TRUE)
  endif()
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*")
    set(DARWIN TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
    set(MACOSX TRUE)
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "BeOS.*")
  set(BEOS TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Haiku.*")
  set(HAIKU TRUE)
endif()

################################################################################
# Tweaks for the various platforms and compilers
################################################################################

if(NOT WINDOWS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Basic version checks. GCC >= 4.9 and Clang >= 3.6 or AppleClang.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Require at least GCC 4.9
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "GCC version must be at least 4.9!")
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Require at least Clang 3.6
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.6)
    message(FATAL_ERROR "Clang version must be at least 3.6!")
  endif()
elseif (NOT NACL AND NOT WINDOWS)
  message(FATAL_ERROR "Unsupported Compiler!")
endif()

# Work around issues/missing features in older but still supported compilers.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Work around bug with -Wextra on older GCC
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36750
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  endif()
endif()

if (FREEBSD)
  # The default Mesa install target dumps its headers in a slightly different
  # directory
  link_directories("/usr/local/lib")
endif()

if(RASPBERRY)
  add_definitions("-D__raspberrypi__=1")
endif()

if(ANDROID)
  set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

enable_testing()

################################################################################
# Import CMake Modules
################################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake")

include(AddCXXWarningIfSupported)
include(TreatAsCXX)
include(StandardRadarLibrary)
include(StandardRadarTest)
include(StandardRadarBench)

################################################################################
################################################################################
################     _____       _   _                      ####################
################    |  _  |     | | (_)                     ####################
################    | | | |_ __ | |_ _  ___  _ __  ___      ####################
################    | | | | '_ \| __| |/ _ \| '_ \/ __|     ####################
################    \ \_/ / |_) | |_| | (_) | | | \__ \     ####################
################     \___/| .__/ \__|_|\___/|_| |_|___/     ####################
################          | |                               ####################
################          |_|                               ####################
################                                            ####################
################################################################################
################################################################################

################################################################################
# Control benchmarking
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_BENCHMARKING_ENABLED OFF CACHE BOOL "Disable benchmarking")
endif()

option(RADAR_BENCHMARKING_ENABLED
    "If benchmarking targets will be generated" ON)

################################################################################
# Control testing
################################################################################

if(NOT (APPLE OR LINUX OR WINDOWS))
  set(RADAR_TESTING_ENABLED OFF CACHE BOOL "Disable testing")
endif()

option(RADAR_TESTING_ENABLED
    "If testing targets will be generated" ON)

################################################################################
# Enable Sanitizers (Requires Custom Clang Setup)
# Usage:
#     $ cmake -DSANITIZE=address
#     Any supported Clang sanitizer can be specified
################################################################################

if(SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE} -g -O1")
endif()

if (NOT WINDOWS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror ")
endif()

AddCXXWarningIfSupported("-Wpessimizing-move" HAVE_PESSIMIZING_MOVE)
AddCXXWarningIfSupported("-Wredundant-move" HAVE_REDUNDANT_MOVE)
AddCXXWarningIfSupported("-Wloop-analysis" HAVE_LOOP_ANALYSIS)

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

if(RADAR_BENCHMARKING_ENABLED)
  # Build the benchmarking target with its tests disabled.
  add_custom_target(bench)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable tests on benchmarks")
  add_subdirectory("ThirdParty/googlebenchmark")
endif()

add_subdirectory("ThirdParty/gtest")
add_subdirectory("ThirdParty/imgui")
add_subdirectory("ThirdParty/sqlite3")
add_subdirectory("ThirdParty/uriparser")
add_subdirectory("ThirdParty/stb")

add_subdirectory("Library/Animation")
add_subdirectory("Library/BenchmarkRunner")
add_subdirectory("Library/Compositor")
add_subdirectory("Library/Coordinator")
add_subdirectory("Library/Core")
add_subdirectory("Library/DevSupport")
add_subdirectory("Library/Entity")
add_subdirectory("Library/Event")
add_subdirectory("Library/Geometry")
add_subdirectory("Library/Image")
add_subdirectory("Library/Interface")
add_subdirectory("Library/Layout")
add_subdirectory("Library/Shell")
add_subdirectory("Library/TestRunner")
add_subdirectory("Library/Toolbox")

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
# 3: `cmake ../ -G Ninja`
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
