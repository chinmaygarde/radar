# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Coordinator)

target_link_libraries(Coordinator
  PUBLIC
    Core
    Geometry
    Animation
    Layout
    Compositor
)
