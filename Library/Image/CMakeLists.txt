# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

StandardRadarLibrary(Image)

target_link_libraries(Image
  PUBLIC
    Core
    Geometry
  PRIVATE
    stb
)

################################################################################
# Test
################################################################################

StandardRadarTest(Image)
include_directories(ImageTest "Fixtures")

################################################################################
# Benchmark
################################################################################

StandardRadarBench(Image)
include_directories(ImageBenchmark "Fixtures")
