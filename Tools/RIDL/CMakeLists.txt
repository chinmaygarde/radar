# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(RIDL)

target_link_libraries(RIDL Core)

################################################################################
# Tests
################################################################################

StandardRadarTest(RIDL)
