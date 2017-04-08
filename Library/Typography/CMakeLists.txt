# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Typography)

target_link_libraries(Typography
  PUBLIC
    Core
  PRIVATE
    harfbuzz
)

StandardRadarTest(Typography)

# Since we are going to test library internals thats directly import PRIVATE
# dependency headers.
target_link_libraries(TypographyTest
  PRIVATE
    harfbuzz
)
