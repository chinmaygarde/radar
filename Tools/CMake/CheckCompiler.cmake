# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__check_compiler)
  return()
endif()
set(__check_compiler INCLUDED)

################################################################################
# Sets recommended warning flags.
################################################################################

macro(CheckCompiler)

set (CMAKE_CXX_STANDARD 14)

# Basic version checks. GCC >= 4.9 and Clang >= 3.6 or AppleClang.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Require at least GCC 4.9
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "GCC version must be at least 4.9!")
  endif()

  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fdiagnostics-color=always")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Require at least Clang 3.6
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.6)
    message(FATAL_ERROR "Clang version must be at least 3.6!")
  endif()
elseif (NOT NACL AND NOT WINDOWS)
  message(FATAL_ERROR "Unsupported Compiler!")
endif()

if (FREEBSD)
  # The default Mesa install target dumps its headers in a slightly different
  # directory
  link_directories("/usr/local/lib")
endif()

if(RASPBERRY)
  add_definitions("-D__raspberrypi__=1")
endif()

if(ANDROID)
  set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
endif()

endmacro()
