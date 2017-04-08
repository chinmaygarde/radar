# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Entity)

target_link_libraries(Entity
  PUBLIC
    Core
    Geometry
    Image
)
