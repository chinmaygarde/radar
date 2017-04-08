# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Core)

target_link_libraries(Core
  PRIVATE
    base64
    sqlite3
    uriparser
)

if(NOT ANDROID)
  # pthread is implicit on Android.
  target_link_libraries(Core
    PRIVATE
      pthread
  )
endif()

if(APPLE)
  find_library(FOUNDATION Foundation)
  target_link_libraries(Core PRIVATE ${FOUNDATION})
endif()

if(LINUX)
  target_link_libraries(Core PRIVATE dl rt)
endif()

################################################################################
# Test
################################################################################

StandardRadarTest(Core)
