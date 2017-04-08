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
  PRIVATE
    imgui
    libtess2
)

if (APPLE)
  find_library(OPENGL_FRAMEWORK OpenGL)
  target_link_libraries(Compositor PRIVATE ${OPENGL_FRAMEWORK})
endif()

if (LINUX)
  find_library(GLES_LIBRARY GLESv2)
  target_link_libraries(Compositor PRIVATE ${GLES_LIBRARY})
endif()

if (NACL)
  target_link_libraries(Compositor PRIVATE ppapi_gles2)
endif()

################################################################################
# Graphics Test
################################################################################

StandardRadarGraphicsTest(Compositor)
