# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

if(NOT RADAR_BENCHMARKING_ENABLED)
  return()
endif()

StandardRadarLibrary(BenchmarkRunner)

target_link_libraries(BenchmarkRunner
  PUBLIC
    DevSupport
    benchmark
)
