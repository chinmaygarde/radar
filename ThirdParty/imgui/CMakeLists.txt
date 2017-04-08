# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

file(GLOB IMGUI_SRC
  Source/imgui_draw.cpp
  Source/imgui.cpp
)

add_library(imgui ${IMGUI_SRC})

target_include_directories(imgui
  PUBLIC
    Headers
  PRIVATE
    Headers/imgui
    Source
)
