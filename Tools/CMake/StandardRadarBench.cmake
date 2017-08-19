# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__standard_radar_bench)
  return()
endif()
set(__standard_radar_bench INCLUDED)

include(AddRecommendedWarningFlags)

################################################################################
# Declares that the current source directory contains a benchmark in the
# "standard" Radar library format. This generates a standalone benchmark runner.
#
# For a description of the "standard" library format, see
# `StandardRadarLibrary.cmake`
################################################################################

macro(StandardRadarBench BENCH_NAME_ARG)

if (NOT RADAR_BENCHMARKING_ENABLED)
  message(STATUS 
      "\tSkipping benchmark target for '${BENCH_NAME_ARG}'.")
  return()
endif()

string(CONCAT BENCH_TARGET_NAME ${BENCH_NAME_ARG} "Benchmark")

file(GLOB_RECURSE BENCH_SRC_CC
  "Bench/*.h"
  "Bench/*.c"
  "Bench/*.cc"
)

file(GLOB_RECURSE BENCH_SRC_MM
  "Bench/*.mm"
)

TreatAsCXX("${BENCH_SRC_MM}")

add_executable(${BENCH_TARGET_NAME} ${BENCH_SRC_CC} ${BENCH_SRC_MM})

add_test(${BENCH_TARGET_NAME} ${BENCH_TARGET_NAME})

AddRecommendedWarningFlags()

include_directories("Headers" "Source" "Bench")

target_link_libraries(${BENCH_TARGET_NAME} BenchmarkRunner ${BENCH_NAME_ARG})

if(LINUX)
  target_link_libraries(${BENCH_TARGET_NAME} rt)
endif()

add_dependencies(bench ${BENCH_TARGET_NAME})

endmacro()
