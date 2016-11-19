# Copyright 2016 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following disclaimer
#      in the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Google Inc. nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

AddRecommendedWarningFlags()

include_directories("Headers" "Source" "Bench")

target_link_libraries(${BENCH_TARGET_NAME} BenchmarkRunner ${BENCH_NAME_ARG})

if(LINUX)
  target_link_libraries(${BENCH_TARGET_NAME} rt)
endif()

add_dependencies(bench ${BENCH_TARGET_NAME})

endmacro()
