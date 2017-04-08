# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

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
