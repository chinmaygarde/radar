# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.


include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)

file(GLOB_RECURSE LINUX_PLATFORM_SRC
  "../../Samples/*.cc"
  "../../Samples/*.h"
  "Source/*.cc"
  "Source/*.h"
)

add_executable(RadarLinux ${LINUX_PLATFORM_SRC})

target_link_libraries(RadarLinux
  ${SDL2_LIBRARIES}
  Shell
)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  "Source"
  "../../Samples"
)
