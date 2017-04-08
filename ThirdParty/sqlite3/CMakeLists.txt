# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

file(GLOB_RECURSE SQLITE3_SRC
  Headers/*.h
  Source/*.h
  Source/*.c
)

set_property(
  SOURCE
  ${SQLITE3_SRC}
  APPEND
  PROPERTY
  COMPILE_DEFINITIONS
  "SQLITE_OMIT_LOAD_EXTENSION=1"
)

add_library(sqlite3 ${SQLITE3_SRC})

if(APPLE)
  # For OSAtomicCompareAndSwapPtrBarrier
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} -Wno-deprecated-declarations ")
endif()

target_include_directories(sqlite3
  PUBLIC
    Headers
  PRIVATE
    Headers/sqlite3 Source
)
