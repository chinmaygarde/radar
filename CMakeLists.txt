# Copyright 2014 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
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

cmake_minimum_required(VERSION 2.8)

################################
# RadarLove Library
################################
project(RadarLove)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE RADARLOVE_SRC
  "Headers/*.h"
  "Source/*.h"
  "Source/*.cc"
)

include_directories ("Headers")
include_directories ("ThirdParty")

add_library(RadarLove ${RADARLOVE_SRC} ${RADARLOVE_SRC_PLATFORM})

if(UNIX AND NOT APPLE)
  target_link_libraries(RadarLove rt GL)
endif()

################################
# Google Test
################################

add_subdirectory (Test/GoogleTest)
enable_testing ()
include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################

file(GLOB RADARLOVE_TEST_SRC
  "Test/*.h"
  "Test/*.cc"
)

include_directories ( Source )
add_executable( RadarLoveTest ${RADARLOVE_TEST_SRC} )
target_link_libraries( RadarLoveTest RadarLove gtest gtest_main)
add_test( RadarLoveTestAll RadarLoveTest )

################################
# Linux Desktop Shell
################################

if(UNIX AND NOT APPLE)
  file(GLOB RADARLOVE_LINUX_DESKTOP_SOURCES
    "Platforms/Linux/*.h"
    "Platforms/Linux/*.cc"
  )
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
