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

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

################################################################################
# Try to detect the platform we are on
################################################################################

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
# Enable Sanitizers (Requires Custom Clang Setup)
# Usage:
#     $ cmake -DSANITIZE=address
#     Any supported Clang sanitizer can be specified
################################################################################

if(SANITIZE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE} -g -O1")
endif()

################################################################################
# Tweaks for the various platforms and compilers
################################################################################

if(NOT WINDOWS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Basic version checks. GCC >= 4.9 and Clang >= 3.7 or AppleClang.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Require at least GCC 4.9
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "GCC version must be at least 4.9!")
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Require at least Clang 3.7
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.7)
    message(FATAL_ERROR "Clang version must be at least 3.7!")
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
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
        NOT CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  # When these flags are added to -Wall or -Wextra, this check can be removed.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wpessimizing-move                    \
    -Wredundant-move")
endif()

if (FREEBSD)
  # The default Mesa install target dumps its headers in a slightly different
  # directory
  link_directories(
    "/usr/local/lib"
  )
  target_link_libraries(Radar GLESv2)
endif()

if (APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -framework OpenGL                                   \
    -framework Foundation")
endif()

if(RASPBERRY)
  add_definitions("-D__raspberrypi__=1")
  target_link_libraries(Radar rt)
endif()

################################################################################
# Import CMake Modules
################################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake")

include(TreatAsCXX)
include(StandardRadarLibrary)
include(StandardRadarTest)

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
add_subdirectory("Library/Coordinator")
add_subdirectory("Library/Core")
add_subdirectory("Library/Entity")
add_subdirectory("Library/Event")
add_subdirectory("Library/Geometry")
add_subdirectory("Library/Interface")
add_subdirectory("Library/Layout")
add_subdirectory("Library/Shell")
add_subdirectory("Library/TestRunner")
add_subdirectory("Library/Toolbox")

add_subdirectory("ThirdParty/gtest")
add_subdirectory("ThirdParty/imgui")
add_subdirectory("ThirdParty/sqlite3")

################################################################################
################################################################################
######################     _____         _            ##########################
######################    |_   _|       | |           ##########################
######################      | | ___  ___| |_ ___      ##########################
######################      | |/ _ \/ __| __/ __|     ##########################
######################      | |  __/\__ \ |_\__ \     ##########################
######################      \_/\___||___/\__|___/     ##########################
######################                                ##########################
################################################################################
################################################################################

enable_testing ()

file(GLOB RADAR_TEST_SRC
  "Test/*.h"
  "Test/*.cc"
  "Test/*.mm"
)

file(GLOB RADAR_TEST_SRC_MM
  "Test/*.mm"
)

TreatAsCXX("${RADAR_TEST_SRC_MM}")

add_executable(RadarTest ${RADAR_TEST_SRC})
target_link_libraries(RadarTest TestRunner Core Interface)

if(LINUX)
  target_link_libraries(RadarTest rt)
endif()

add_test(RadarTestAll RadarTest)

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
# Linux Desktop Shell
################################################################################

if(LINUX)
  add_subdirectory("Platforms/Linux")
endif()

################################################################################
# Rasperry Pi Shell
################################################################################

if(RASPBERRY)
  file(GLOB RADAR_RASPBERRY_DESKTOP_SOURCES
    "Platforms/RaspberryPi/Source/*.h"
    "Platforms/RaspberryPi/Source/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  include_directories (
    "Samples"
    "/opt/vc/include"
    "/opt/vc/include/interface/vcos/pthreads"
    "/opt/vc/include/interface/vmcs_host/linux"
  )

  link_directories(
    "/opt/vc/lib"
  )

  add_executable( RadarRaspberryPi ${RADAR_RASPBERRY_DESKTOP_SOURCES} )

  include_directories(${SDL2_INCLUDE_DIRS})

  target_link_libraries(RadarRaspberryPi
    Radar
    pthread
    rt
    EGL
    GLESv2
    openmaxil
    bcm_host
    vcos
    vchiq_arm
  )
endif()

################################################################################
# FreeBSD Desktop Shell
################################################################################

if(FREEBSD)
  file(GLOB RADAR_BSD_DESKTOP_SOURCES
    "Samples/*.h"
    "Samples/*.cc"
  )
  include_directories ("Samples")

  add_executable( RadarBSDDesktop ${RADAR_BSD_DESKTOP_SOURCES} )

  include_directories("/usr/local/include")
  link_directories("/usr/local/lib")

  target_link_libraries(RadarBSDDesktop
    Radar
    pthread
  )
endif()

################################################################################
# Native Client (PNaCl Only) Shell
################################################################################

if(NACL)
  add_subdirectory("Platforms/Nacl")
endif()

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
