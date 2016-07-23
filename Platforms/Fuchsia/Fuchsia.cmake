# Copyright 2016 The Chromium Authors. All rights reserved.
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

################################################################################
################################################################################
##################   ______          _         _          ######################
##################   |  ___|        | |       (_)         ######################
##################   | |_ _   _  ___| |__  ___ _  __ _    ######################
##################   |  _| | | |/ __| '_ \/ __| |/ _` |   ######################
##################   | | | |_| | (__| | | \__ \ | (_| |   ######################
##################   \_|  \__,_|\___|_| |_|___/_|\__,_|   ######################
##################                                        ######################
################################################################################
################################################################################
########                                                               #########
######## Usage: cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/this/file> \     #########
########              -DFUCHSIA_SDK=<path/to/sdk>                \     #########
########              -DFUCHSIA_TARGET=x86_64-or-aarch64         \     #########
########              SRC_ROOT                                         #########
########                                                               #########
################################################################################
################################################################################

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(FUCHSIA BOOL ON)

add_definitions(-D__fuchsia__=1)

################################################################################
# Setup the toolchain on the host
################################################################################

if(CMAKE_HOST_APPLE)
  set(_TOOLCHAIN_BIN "${FUCHSIA_SDK}/toolchains/clang+llvm-x86_64-darwin/bin")
else()
  message(FATAL_ERROR "Toolchain could not be determined for this platform.")
endif()

# Setup compilers.
set(CMAKE_C_COMPILER   "${_TOOLCHAIN_BIN}/clang")
set(CMAKE_CXX_COMPILER "${_TOOLCHAIN_BIN}/clang++")
set(CMAKE_AR           "${_TOOLCHAIN_BIN}/llvm-ar"      CACHE STRING "")
set(CMAKE_RANLIB       "${_TOOLCHAIN_BIN}/llvm-ranlib"  CACHE STRING "")

set(CMAKE_C_COMPILER_WORKS   TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

################################################################################
# Setup sysroot
################################################################################

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)

set(FUCHSIA_SYSROOT       "${FUCHSIA_SDK}/platforms/${FUCHSIA_TARGET}-fuchsia")

set(CMAKE_FIND_ROOT_PATH  "${FUCHSIA_SYSROOT}")
set(CMAKE_SYSROOT         "${FUCHSIA_SYSROOT}")

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS}   --target=${FUCHSIA_TARGET}-fuchsia "  CACHE STRING "")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} --target=${FUCHSIA_TARGET}-fuchsia "  CACHE STRING "")
