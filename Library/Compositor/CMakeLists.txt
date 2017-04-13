# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Compositor)

target_link_libraries(Compositor
  PUBLIC
    Core
    Geometry
    Entity
    Image
    Typography
    GLFoundation
  PRIVATE
    imgui
    libtess2
)


################################################################################
# Graphics Test
################################################################################

StandardRadarGraphicsTest(Compositor)
