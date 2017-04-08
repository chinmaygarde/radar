# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

if(__treat_as_cxx)
  return()
endif()
set(__treat_as_cxx INCLUDED)

################################################################################
# Treats the specified sources as C++ files
################################################################################

function(TreatAsCXX SOURCES_ARGS_LIST)

if(NOT APPLE)
  if(UNIX OR NACL OR ANDROID)
    set_property(
      SOURCE
      ${SOURCES_ARGS_LIST}
      APPEND_STRING
      PROPERTY
      COMPILE_FLAGS
      " -x c++ "
    )
  else()
    set_source_files_properties(${SOURCES_ARGS_LIST}
      PROPERTIES LANGUAGE CXX
    )
  endif()
endif()

endfunction()
