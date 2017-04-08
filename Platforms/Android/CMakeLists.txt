# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

file(GLOB RADAR_ANDROID_SOURCES
  "Source/*.h"
  "Source/*.c"
  "Source/*.cc"
  "../../Samples/*.h"
  "../../Samples/*.cc"
)

include_directories(
  "../../Samples"
  "Source"
  "Source/native_app_glue"
)

add_library(RadarAndroid SHARED ${RADAR_ANDROID_SOURCES})

target_link_libraries(RadarAndroid
  Shell
)
