# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(BodymovinParser)

target_link_libraries(BodymovinParser
  PUBLIC
    Core
    Geometry
)

################################################################################
# Test
################################################################################

StandardRadarTest(BodymovinParser)
