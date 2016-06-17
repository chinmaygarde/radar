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

if(__add_cxx_warning_if_supported)
  return()
endif()
set(__add_cxx_warning_if_supported INCLUDED)

################################################################################
# Declares that the current source directory contains a benchmark in the
# "standard" Radar library format. This generates a standalone benchmark runner.
#
# For a description of the "standard" library format, see
# `StandardRadarLibrary.cmake`
################################################################################

include(CheckCXXCompilerFlag)

macro(AddCXXWarningIfSupported WARNING_FLAG WARNING_LABEL)

check_cxx_compiler_flag(${WARNING_FLAG} ${WARNING_LABEL})

if(${WARNING_LABEL})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAG} ")
endif()


endmacro(AddCXXWarningIfSupported)
