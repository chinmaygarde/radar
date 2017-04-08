# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Layout)

target_link_libraries(Layout
  PUBLIC
    Core
    Event
    Entity
)

################################################################################
# Test
################################################################################

StandardRadarTest(Layout)

################################################################################
# Benchmark
################################################################################

StandardRadarBench(Layout)
