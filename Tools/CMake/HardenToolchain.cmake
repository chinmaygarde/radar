# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__harden_toolchain)
  return()
endif()
set(__harden_toolchain INCLUDED)

################################################################################
# Hardens the C/C++ toolchain
################################################################################

macro(HardenToolchain)

if (WINDOWS)
  return()
endif()

################################################################################
# Fortify Source
################################################################################
add_definitions(-D_FORTIFY_SOURCE=2)

################################################################################
# Stack Smashing Protection
################################################################################
set(STACK_SMASHING_FLAGS "-fstack-protector-strong")

set(CMAKE_C_FLAGS   " ${CMAKE_C_FLAGS} ${STACK_SMASHING_FLAGS} ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${STACK_SMASHING_FLAGS} ")

################################################################################
# Position Independent Executables
################################################################################

if(APPLE)
  set(PIE_FLAGS "-fPIE")
  set(CMAKE_C_FLAGS   " ${CMAKE_C_FLAGS} ${PIE_FLAGS} ")
  set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${PIE_FLAGS} ")
  set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -Wl,-pie ")
endif()


################################################################################
# GOT/REL Protection
################################################################################

if(NOT APPLE)
  set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro,-z,now ")
endif()

################################################################################
# Non executable stack
################################################################################

if(NOT APPLE)
  set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,noexecstack ")
endif()

################################################################################
# Format String Issues
################################################################################

set(FORMAT_STRING_FLAGS "-Wformat -Wformat-security")
set(CMAKE_C_FLAGS   " ${CMAKE_C_FLAGS} ${FORMAT_STRING_FLAGS} ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} ${FORMAT_STRING_FLAGS} ")

endmacro()
