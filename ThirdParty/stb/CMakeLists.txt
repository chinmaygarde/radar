# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

file(GLOB STB_LIB_SRC
  Source/STBImplementation.c
  stb/stb_image.h
)

add_library(stb ${STB_LIB_SRC})

target_include_directories(stb
  PUBLIC
    stb
  PRIVATE
    Source
)
