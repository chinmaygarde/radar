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
######################    ____ ____ ___  ____ ____    ##########################
######################    |__/ |__| |  \ |__| |__/    ##########################
######################    |  \ |  | |__/ |  | |  \    ##########################
######################                                ##########################
################################################################################
################################################################################

if(APPLE)
  # Avoid consulting cc which will always default to AppleClang. If a newer
  # version on Clang is in the path, that will be used over AppleClang.
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
endif()

project(RadarLove)

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
# Define the base library target
################################################################################

file(GLOB_RECURSE RADARLOVE_SRC
  "Headers/*.h"
  "Source/*.h"
  "Source/*.cc"
  "Source/*.mm"
)

if(NOT APPLE)
  # On non-Apple platforms, there is no actual Objective-C in '.mm' files.
  # Treat them as C++

  file(GLOB_RECURSE RADARLOVE_SRC_MM
    "Source/*.mm"
  )

  set_property(
    SOURCE
    ${RADARLOVE_SRC_MM}
    APPEND_STRING
    PROPERTY
    COMPILE_FLAGS
    " -x c++ "
  )
endif()

file(GLOB RADARLOVE_IMGUI_SRC
  "ThirdParty/imgui/imgui_draw.cpp"
  "ThirdParty/imgui/imgui.cpp"
  # "ThirdParty/imgui/imgui_demo.cpp"
)

file(GLOB RADARLOVE_SQLITE_SRC
  "ThirdParty/sqlite3/*.h"
  "ThirdParty/sqlite3/*.c"
)

set_property(
  SOURCE
  ${RADARLOVE_SRC}
  APPEND_STRING
  PROPERTY
  COMPILE_FLAGS
  " -Werror -Wextra "
)

if(DISABLE_XPC)
  # Usage:
  #     $ cmake -DDISABLE_XPC=1
  set_property(
    SOURCE
    ${RADARLOVE_SRC}
    APPEND
    PROPERTY
    COMPILE_DEFINITIONS
    "RL_DISABLE_XPC=1"
  )
endif()

set_property(
  SOURCE
  ${RADARLOVE_SQLITE_SRC}
  APPEND
  PROPERTY
  COMPILE_DEFINITIONS
  "SQLITE_OMIT_LOAD_EXTENSION=1"
)

include_directories ("Headers")
include_directories ("ThirdParty")

add_library(RadarLove
    ${RADARLOVE_SRC}
    ${RADARLOVE_IMGUI_SRC}
    ${RADARLOVE_SQLITE_SRC}
)

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
else()
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

if(LINUX)
  target_link_libraries(RadarLove dl rt GL)
endif()

if (FREEBSD)
  # The default Mesa install target dumps its headers in a slightly different
  # directory
  link_directories(
    "/usr/local/lib"
  )
  target_link_libraries(RadarLove GLESv2)
endif()

if (APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -framework OpenGL                                   \
    -framework Foundation")
endif()

if(RASPBERRY)
  add_definitions("-D__raspberrypi__=1")
  target_link_libraries(RadarLove rt)
endif()

add_subdirectory("Library/Core")
add_subdirectory("Library/Animation")
add_subdirectory("Library/Event")
add_subdirectory("Library/Layout")

target_link_libraries(RadarLove Animation Layout)

################################################################################
# Google Test Library
################################################################################

add_subdirectory (Test/GoogleTest)
enable_testing ()
include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################################################################
# Unit Tests Target
################################################################################

file(GLOB RADARLOVE_TEST_SRC
  "Test/*.h"
  "Test/*.cc"
  "Test/*.mm"
)

if(NOT APPLE)
  # Since we want to run either the XCTest or GTest harness to run all unit
  # tests, all test files have the ".mm" extension. On non-Apple platforms,
  # there is no actual Objective-C in these files. So treat them as C++

  file(GLOB RADARLOVE_TEST_SRC_MM
    "Test/*.mm"
  )

  set_property(
    SOURCE
    ${RADARLOVE_TEST_SRC_MM}
    APPEND_STRING
    PROPERTY
    COMPILE_FLAGS
    " -x c++ "
  )
endif()

if(DISABLE_XPC)
  # Certain tests are disabled based on which backend is being tested.
  set_property(
    SOURCE
    ${RADARLOVE_TEST_SRC}
    APPEND
    PROPERTY
    COMPILE_DEFINITIONS
    "RL_DISABLE_XPC=1"
  )
endif()

include_directories ( Source )
add_executable( RadarLoveTest ${RADARLOVE_TEST_SRC} )
target_link_libraries( RadarLoveTest RadarLove gtest gtest_main)
add_test( RadarLoveTestAll RadarLoveTest )

################################################################################
# Linux Desktop Shell
################################################################################

if(LINUX)
  file(GLOB RADARLOVE_LINUX_DESKTOP_SOURCES
    "Platforms/Linux/*.h"
    "Platforms/Linux/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  include_directories ("Samples")

  add_executable( RadarLoveLinuxDesktop ${RADARLOVE_LINUX_DESKTOP_SOURCES} )

  include(FindPkgConfig)

  pkg_search_module(SDL2 REQUIRED sdl2)

  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries(RadarLoveLinuxDesktop
    RadarLove
    pthread
    ${SDL2_LIBRARIES}
    rt
  )
endif()

################################################################################
# Rasperry Pi Shell
################################################################################

if(RASPBERRY)
  file(GLOB RADARLOVE_RASPBERRY_DESKTOP_SOURCES
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

  add_executable( RadarLoveRaspberryPi ${RADARLOVE_RASPBERRY_DESKTOP_SOURCES} )

  include_directories(${SDL2_INCLUDE_DIRS})

  target_link_libraries(RadarLoveRaspberryPi
    RadarLove
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
  file(GLOB RADARLOVE_BSD_DESKTOP_SOURCES
    "Samples/*.h"
    "Samples/*.cc"
  )
  include_directories ("Samples")

  add_executable( RadarLoveBSDDesktop ${RADARLOVE_BSD_DESKTOP_SOURCES} )

  include_directories("/usr/local/include")
  link_directories("/usr/local/lib")

  target_link_libraries(RadarLoveBSDDesktop
    RadarLove
    pthread
  )
endif()

################################################################################
# Native Client (PNaCl Only) Shell
################################################################################

if(RL_PLATFORM_NACL)

  file(GLOB RADARLOVE_NACL_SOURCES
    "Platforms/Nacl/Source/*.h"
    "Platforms/Nacl/Source/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  # Generate the Executable

  include_directories ("Samples")

  add_executable( RadarLoveNacl ${RADARLOVE_NACL_SOURCES} )

  target_link_libraries(RadarLoveNacl
    RadarLove
    "ppapi"
    "ppapi_cpp"
    "nacl_io"
    "ppapi_gles2"
  )

  # Finalize PEXEs

  # Execuatable Shell
  add_custom_command(
    OUTPUT "RadarLoveNacl.pexe"
    COMMAND ${RL_PNACL_FINALIZE} -o RadarLoveNacl.pexe RadarLoveNacl
    DEPENDS RadarLoveNacl
    COMMENT "Finalizing Native Client PEXE" VERBATIM
  )

  add_custom_target(
    RadarLoveNaclPexe ALL
    DEPENDS "RadarLoveNacl.pexe"
  )

  # Test Target
  add_custom_command(
    OUTPUT "RadarLoveTest.pexe"
    COMMAND ${RL_PNACL_FINALIZE} -o RadarLoveTest.pexe RadarLoveTest
    DEPENDS RadarLoveTest
    COMMENT "Finalizing Native Client Test PEXE" VERBATIM
  )

  add_custom_target(
    RadarLoveNaclTest ALL
    DEPENDS "RadarLoveTest.pexe"
  )

  # Copy over the HTML harness and manifest

  configure_file("Platforms/Nacl/RadarLove.html" "RadarLove.html" COPYONLY)
  configure_file("Platforms/Nacl/RadarLove.nmf" "RadarLove.nmf" COPYONLY)
  configure_file("Platforms/Nacl/Server" "Server" COPYONLY)
  configure_file("Platforms/Nacl/GDBInit" "GDBInit" COPYONLY)
  configure_file("Platforms/Nacl/Debug" "Debug" COPYONLY)

endif()

################################################################################
# Tools
################################################################################

add_subdirectory("Tools/RIDL")
