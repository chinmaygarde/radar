# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Samples)

target_link_libraries(Samples
  PUBLIC
    Interface
    InterfaceBuilder
)
