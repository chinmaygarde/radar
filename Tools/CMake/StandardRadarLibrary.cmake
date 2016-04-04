# Copyright 2016 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following disclaimer
#      in the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Google Inc. nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if(__standard_radar_library)
  return()
endif()
set(__standard_radar_library INCLUDED)

################################################################################
# Declares that the current source directory contains a library in the
# "standard" Radar library format.
#
# The directory format of a "standard" Radar library is as follows:
# - LibraryName
#     - Headers/LibraryName        : Public library headers
#     - Source                     : Source files (includes private headers)
#     - Test                       : Test files (see `StandardRadarTest.cmake`)
#     - CMakeLists.txt             : The file that calls this function
#
################################################################################

function(StandardRadarLibrary LIBRARY_NAME_ARG)

message(STATUS 
    "Found library '${LIBRARY_NAME_ARG}' with standard layout.")

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

target_include_directories(${LIBRARY_NAME_ARG} PUBLIC "Headers")

include_directories("Headers" "Source")

endfunction()
