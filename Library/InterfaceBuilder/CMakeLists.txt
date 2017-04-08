# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(InterfaceBuilder)

target_link_libraries(InterfaceBuilder
  PUBLIC
    Interface
  PRIVATE
    pugixml
)

target_include_directories(InterfaceBuilder
  PRIVATE
    ../../ThirdParty/pugixml/src
)

################################################################################
# Test
################################################################################

StandardRadarTest(InterfaceBuilder)
