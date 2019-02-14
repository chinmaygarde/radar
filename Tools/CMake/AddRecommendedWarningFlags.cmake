# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__warnings)
  return()
endif()
set(__warnings INCLUDED)

include(AddCXXWarningIfSupported)

################################################################################
# Sets recommended warning flags.
################################################################################

macro(AddRecommendedWarningFlags)

if (WINDOWS)
  return()
endif()

AddCXXWarningIfSupported("-Werror"                   HAVE_ERRORS)
AddCXXWarningIfSupported("-Wall"                     HAVE_ALL_ERRORS)
AddCXXWarningIfSupported("-Wmost"                    HAVE_MOST)
AddCXXWarningIfSupported("-Wloop-analysis"           HAVE_LOOP_ANALYSIS)
AddCXXWarningIfSupported("-Wpessimizing-move"        HAVE_PESSIMIZING_MOVE)
AddCXXWarningIfSupported("-Wredundant-move"          HAVE_REDUNDANT_MOVE)
AddCXXWarningIfSupported("-Wthread-safety"           HAVE_THREAD_SAFETY_ANALYSIS)
AddCXXWarningIfSupported("-Wno-unused-private-field" HAVE_UNUSED_PRIVATE_FIELD)
AddCXXWarningIfSupported("-Wdelete-non-virtual-dtor" HAVE_DELETE_NON_VIRTUAL_DTOR)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Work around bug with -Werror on older GCC
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36750
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  endif()
endif()

endmacro()
