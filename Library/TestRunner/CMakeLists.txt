# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(TestRunner)

target_link_libraries(TestRunner
  PUBLIC
    Shell
    Geometry
  PRIVATE
    gtest
    DevSupport
)

target_include_directories(TestRunner
  PUBLIC
    "../../ThirdParty/gtest/include"
)
