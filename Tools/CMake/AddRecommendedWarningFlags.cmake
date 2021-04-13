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


# Enable Warnings.
AddCXXWarningIfSupported("-Werror"                         HAVE_ERRORS)
AddCXXWarningIfSupported("-Wall"                           HAVE_ALL_ERRORS)
AddCXXWarningIfSupported("-Wmost"                          HAVE_MOST)
AddCXXWarningIfSupported("-Wloop-analysis"                 HAVE_LOOP_ANALYSIS)
AddCXXWarningIfSupported("-Wpessimizing-move"              HAVE_PESSIMIZING_MOVE)
AddCXXWarningIfSupported("-Wredundant-move"                HAVE_REDUNDANT_MOVE)
AddCXXWarningIfSupported("-Wthread-safety"                 HAVE_THREAD_SAFETY_ANALYSIS)
AddCXXWarningIfSupported("-Wdelete-non-virtual-dtor"       HAVE_DELETE_NON_VIRTUAL_DTOR)
AddCXXWarningIfSupported("-Wsuggest-override"              HAVE_SUGGEST_OVERRIDE)
AddCXXWarningIfSupported("-Winconsistent-missing-override" HAVE_INCONSISTENT_MISSING_OVERRIDE)
AddCXXWarningIfSupported("-Wsign-compare"                  HAVE_SIGN_COMPARE)

# Disable warnings.
AddCXXWarningIfSupported("-Wno-unused-private-field"       HAVE_NO_UNUSED_PRIVATE_FIELD)
AddCXXWarningIfSupported("-Wno-suggest-override"           HAVE_NO_SUGGEST_OVERRIDE)
AddCXXWarningIfSupported("-Wno-subobject-linkage"          HAVE_NO_SUBOBJECT_LINKAGE)
AddCXXWarningIfSupported("-Wno-unused-but-set-variable"    HAVE_NO_UNUSED_BUT_SET_VARIABLE)
AddCXXWarningIfSupported("-Wno-misleading-indentation"     HAVE_NO_MISLEADING_INDENTATION)
AddCXXWarningIfSupported("-Wno-odr"                        HAVE_NO_ODR)
AddCXXWarningIfSupported("-Wno-thread-safety-reference"    HAVE_NO_THREAD_SAFETY_REFERENCE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Work around bug with -Werror on older GCC
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=36750
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  endif()
endif()

endmacro()
