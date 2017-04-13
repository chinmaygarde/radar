# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(GLFoundation)

target_link_libraries(GLFoundation
  PUBLIC
    Core
    Geometry
)

if (APPLE)
  find_library(OPENGL_FRAMEWORK OpenGL)
  target_link_libraries(GLFoundation PUBLIC ${OPENGL_FRAMEWORK})
endif()

if (LINUX)
  find_library(GLES_LIBRARY GLESv2)
  target_link_libraries(GLFoundation PUBLIC ${GLES_LIBRARY})
endif()

if (NACL)
  target_link_libraries(GLFoundation PUBLIC ppapi_gles2)
endif()
