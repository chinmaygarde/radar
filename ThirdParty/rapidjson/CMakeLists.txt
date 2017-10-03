# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

add_library(rapidjson INTERFACE)

target_include_directories(rapidjson
  INTERFACE
    rapidjson/include
)
