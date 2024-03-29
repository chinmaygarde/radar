# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(GraphicsTestRunner)

target_link_libraries(GraphicsTestRunner
  PUBLIC
    Core
    Image
    TestRunner
  PRIVATE
    DevSupport
    GLFoundation
    gtest
)

target_include_directories(GraphicsTestRunner
  PUBLIC
    "../../ThirdParty/gtest/include"
)

if (APPLE)
  find_library(APPKIT_FRAMEWORK AppKit)
  target_link_libraries(GraphicsTestRunner
    PRIVATE
      ${APPKIT_FRAMEWORK}
  )
endif()


################################################################################
# Test
################################################################################

# The graphics test runner is tested using the standard test runner.
StandardRadarTest(GraphicsTestRunner)
