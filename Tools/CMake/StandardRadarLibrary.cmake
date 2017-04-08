# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__standard_radar_library)
  return()
endif()
set(__standard_radar_library INCLUDED)

include(AddRecommendedWarningFlags)

################################################################################
# Declares that the current source directory contains a library in the
# "standard" Radar library format.
#
# The directory format of a "standard" Radar library is as follows:
# - LibraryName
#     - Headers/          : Public library headers
#     - Source/           : Source files (includes private headers)
#     - Test/             : Test files (see `StandardRadarTest.cmake`)
#     - Bench/            : Benchmark files (see `StandardRadarBench.cmake`)
#     - CMakeLists.txt    : The file that calls this macro
#
################################################################################

macro(StandardRadarLibrary LIBRARY_NAME_ARG)

file(GLOB_RECURSE LIBRARY_SRC_CC
  "Headers/*.h"
  "Source/*.h"
  "Source/*.c"
  "Source/*.cc"
)

file(GLOB_RECURSE LIBRARY_SRC_MM
  "Source/*.mm"
)

TreatAsCXX("${LIBRARY_SRC_MM}")

add_library(${LIBRARY_NAME_ARG} ${LIBRARY_SRC_CC} ${LIBRARY_SRC_MM})

AddRecommendedWarningFlags()

target_include_directories(${LIBRARY_NAME_ARG} PUBLIC "Headers")

include_directories("Headers" "Source")

endmacro()
