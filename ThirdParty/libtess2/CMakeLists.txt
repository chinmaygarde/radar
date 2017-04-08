# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

file(GLOB LIBTESS2_SRC
  Source/Source/bucketalloc.c
  Source/Source/bucketalloc.h
  Source/Source/dict.c
  Source/Source/dict.h
  Source/Source/geom.c
  Source/Source/geom.h
  Source/Source/mesh.c
  Source/Source/mesh.h
  Source/Source/priorityq.c
  Source/Source/priorityq.h
  Source/Source/sweep.c
  Source/Source/sweep.h
  Source/Source/tess.c
  Source/Source/tess.h
)

add_library(libtess2 ${LIBTESS2_SRC})

target_include_directories(libtess2
  PUBLIC
    Headers
  PRIVATE
    Headers/libtess2 Source/Source
)
