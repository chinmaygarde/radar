# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__enable_lto)
  return()
endif()
set(__enable_lto INCLUDED)

################################################################################
# Enable Link Time Optimizations (Only in Release)
################################################################################

macro(EnableLTO)

if (WINDOWS)
  return()
endif()

set(CMAKE_EXE_LINKER_FLAGS_RELEASE    " ${CMAKE_EXE_LINKER_FLAGS_RELEASE}    -flto ")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE " ${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -flto ")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE " ${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -flto ")
set(CMAKE_C_FLAGS_RELEASE             " ${CMAKE_C_FLAGS_RELEASE}             -flto ")
set(CMAKE_CXX_FLAGS_RELEASE           " ${CMAKE_CXX_FLAGS_RELEASE}           -flto ")

endmacro()
