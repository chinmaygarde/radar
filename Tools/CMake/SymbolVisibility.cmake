# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__symbol_visibility)
  return()
endif()
set(__symbol_visibility INCLUDED)

################################################################################
# Sets symbol visibility flags to recommended values
################################################################################

macro(SetRecommendedSymbolVisibility)

if (WINDOWS)
  return()
endif()

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS}     -fvisibility=hidden ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden ")

endmacro()
