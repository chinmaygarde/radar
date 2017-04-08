# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Animation)

target_link_libraries(Animation
  PUBLIC
    Core
    Geometry
    Entity
)
