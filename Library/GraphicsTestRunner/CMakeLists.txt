# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(GraphicsTestRunner)

target_link_libraries(GraphicsTestRunner
  PRIVATE
    libGLESv2
    libEGL
  PUBLIC
    TestRunner
    Core
    Image
  PRIVATE
    gtest
    DevSupport
)

target_include_directories(GraphicsTestRunner
  PUBLIC
    "../../ThirdParty/gtest/include"
    "../../ThirdParty/SwiftShader/include"
)

################################################################################
# Test
################################################################################

# The graphics test runner is tested using the standard test runner.
StandardRadarTest(GraphicsTestRunner)
