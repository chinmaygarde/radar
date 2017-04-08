# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__add_cxx_warning_if_supported)
  return()
endif()
set(__add_cxx_warning_if_supported INCLUDED)

include(CheckCXXCompilerFlag)

################################################################################
# Declares that the current source directory contains a benchmark in the
# "standard" Radar library format. This generates a standalone benchmark runner.
#
# For a description of the "standard" library format, see
# `StandardRadarLibrary.cmake`
################################################################################

macro(AddCXXWarningIfSupported WARNING_FLAG WARNING_LABEL)

check_cxx_compiler_flag(${WARNING_FLAG} ${WARNING_LABEL})

if(${WARNING_LABEL})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAG} ")
endif()

endmacro()
