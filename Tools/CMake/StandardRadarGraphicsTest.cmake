# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

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

macro(StandardRadarGraphicsTest GRAPHICS_TEST_NAME_ARG)

if (NOT RADAR_GRAPHICS_TESTING_ENABLED)
  message(STATUS 
      "\tSkipping graphics test target for '${GRAPHICS_TEST_NAME_ARG}'.")
  return()
endif()

string(CONCAT GRAPHICS_TEST_TARGET_NAME ${GRAPHICS_TEST_NAME_ARG} "GraphicsTest")

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

AddRecommendedWarningFlags()

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

add_test(${GRAPHICS_TEST_TARGET_NAME} ${GRAPHICS_TEST_TARGET_NAME})

# Copy fixtures, if any.

file(GLOB_RECURSE GRAPHICS_TEST_FIXTURES
  "GraphicsFixtures/*"
)

file(COPY ${GRAPHICS_TEST_FIXTURES}
  DESTINATION "Fixtures"
  USE_SOURCE_PERMISSIONS
)

endmacro()
