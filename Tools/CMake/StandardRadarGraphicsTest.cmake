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

if(__standard_radar_graphics_test)
  return()
endif()
set(__standard_radar_graphics_test INCLUDED)

include(AddRecommendedWarningFlags)

################################################################################
# Declares that the current source directory contains a graphics test in the
# "standard" Radar library format. This generates a standalone test runner.
#
# For a description of the "standard" library format, see
# `StandardRadarLibrary.cmake`
################################################################################

function(StandardRadarGraphicsTest GRAPHICS_TEST_NAME_ARG)

if (NOT RADAR_GRAPHICS_TESTING_ENABLED)
  message(STATUS 
      "\tSkipping graphics test target for '${GRAPHICS_TEST_NAME_ARG}'.")
  return()
endif()

string(CONCAT GRAPHICS_TEST_TARGET_NAME ${GRAPHICS_TEST_NAME_ARG} "GraphicsTest")

message(STATUS 
    "\tAdding graphics test '${GRAPHICS_TEST_TARGET_NAME}' for '${GRAPHICS_TEST_NAME_ARG}'.")

AddRecommendedWarningFlags()

# Find all test files and mark ObjC files as C++ on Linux.

file(GLOB_RECURSE GRAPHICS_TEST_SRC_CC
  "GraphicsTest/*.h"
  "GraphicsTest/*.c"
  "GraphicsTest/*.cc"
)

file(GLOB_RECURSE GRAPHICS_TEST_SRC_MM
  "GraphicsTest/*.mm"
)

TreatAsCXX("${GRAPHICS_TEST_SRC_MM}")

# Add the test executable target.

add_executable(${GRAPHICS_TEST_TARGET_NAME} ${GRAPHICS_TEST_SRC_CC} ${GRAPHICS_TEST_SRC_MM})

# Tests can include private headers directly.

include_directories("Headers" "Source" "GraphicsTest")

# Link against the test runner containing the program entry point.

target_link_libraries(${GRAPHICS_TEST_TARGET_NAME}
  PUBLIC
    GraphicsTestRunner
    ${GRAPHICS_TEST_NAME_ARG}
)

if(LINUX)
  target_link_libraries(${GRAPHICS_TEST_TARGET_NAME}
    PRIVATE
      rt
  )
endif()

# Add the target to CTest

add_test(${GRAPHICS_TEST_NAME_ARG} ${GRAPHICS_TEST_TARGET_NAME})

# Copy fixtures, if any.

file(GLOB_RECURSE GRAPHICS_TEST_FIXTURES
  "GraphicsFixtures/*"
)

file(COPY ${GRAPHICS_TEST_FIXTURES}
  DESTINATION "Fixtures"
  USE_SOURCE_PERMISSIONS
)

endfunction()
