# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Expression)

target_link_libraries(Expression
  PUBLIC
    Core
    Entity
)

################################################################################
# Test
################################################################################

StandardRadarTest(Expression)
