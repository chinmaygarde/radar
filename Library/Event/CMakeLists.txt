# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Event)

target_link_libraries(Event
  PUBLIC
    Core
    Geometry
)
