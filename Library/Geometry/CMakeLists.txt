# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Geometry)

target_link_libraries(Geometry
  PUBLIC
    Core
)

################################################################################
# Test
################################################################################

StandardRadarTest(Geometry)
