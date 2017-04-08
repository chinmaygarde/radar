# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Interface)

target_link_libraries(Interface
  PUBLIC
    Coordinator
    Image
    Toolbox
)

################################################################################
# Test
################################################################################

StandardRadarTest(Interface)

if(RADAR_TESTING_ENABLED)
  # The test harness requires the shell as well.
  target_link_libraries(InterfaceTest
    PUBLIC
      Shell
  )
endif()
