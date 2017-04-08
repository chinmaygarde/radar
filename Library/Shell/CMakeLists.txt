# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Shell)

target_link_libraries(Shell
  PUBLIC
    Interface
    InterfaceBuilder
    Coordinator
)

################################################################################
# Test
################################################################################

StandardRadarTest(Shell)
