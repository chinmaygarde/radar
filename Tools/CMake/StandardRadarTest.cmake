# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__standard_radar_test)
  return()
endif()
set(__standard_radar_test INCLUDED)

include(AddRecommendedWarningFlags)

################################################################################
# Declares that the current source directory contains a test in the
# "standard" Radar library format. This generates a standalone test runner.
#
# For a description of the "standard" library format, see
# `StandardRadarLibrary.cmake`
################################################################################

macro(StandardRadarTest TEST_NAME_ARG)

if (NOT RADAR_TESTING_ENABLED)
  message(STATUS 
      "\tSkipping test target for '${TEST_NAME_ARG}'.")
  return()
endif()

string(CONCAT TEST_TARGET_NAME ${TEST_NAME_ARG} "Test")

# Find all test files and mark ObjC files as C++ on Linux.

file(GLOB_RECURSE TEST_SRC_CC
  "Test/*.h"
  "Test/*.c"
  "Test/*.cc"
)

file(GLOB_RECURSE TEST_SRC_MM
  "Test/*.mm"
)

TreatAsCXX("${TEST_SRC_MM}")

# Add the test executable target.

add_executable(${TEST_TARGET_NAME} ${TEST_SRC_CC} ${TEST_SRC_MM})

AddRecommendedWarningFlags()

# Tests can include private headers directly.

include_directories("Headers" "Source" "Test")

# Link against the test runner containing the program entry point.

target_link_libraries(${TEST_TARGET_NAME}
  PUBLIC
    TestRunner
    ${TEST_NAME_ARG}
)

if(LINUX)
  target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
      rt
  )
endif()

# Add the target to CTest

add_test(${TEST_NAME_ARG} ${TEST_TARGET_NAME})

# Copy fixtures, if any.

file(GLOB_RECURSE TEST_FIXTURES
  "Fixtures/*"
)

file(COPY ${TEST_FIXTURES}
  DESTINATION "Fixtures"
  USE_SOURCE_PERMISSIONS
)

# Copy the ICU Data file.

file(GLOB TEST_ICU_DATA
  "${PROJECT_SOURCE_DIR}/ThirdParty/icu/icu/source/data/in/icudt59l.dat"
)

file(COPY ${TEST_ICU_DATA}
  DESTINATION "Fixtures"
  USE_SOURCE_PERMISSIONS
)

endmacro()
