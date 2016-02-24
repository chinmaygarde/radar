# Copyright 2015 The Chromium Authors. All rights reserved.
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

SET(CMAKE_SYSTEM_NAME Generic)

include_directories(BEFORE SYSTEM ${NACL_SDK_ROOT}/include)
include_directories(BEFORE SYSTEM ${NACL_SDK_ROOT}/include/newlib)

link_directories(${NACL_SDK_ROOT}/lib/pnacl/Release)

set(CMAKE_C_COMPILER
    ${NACL_SDK_ROOT}/toolchain/mac_pnacl/bin/pnacl-clang)
set(CMAKE_CXX_COMPILER
    ${NACL_SDK_ROOT}/toolchain/mac_pnacl/bin/pnacl-clang++)
set(CMAKE_AR
    ${NACL_SDK_ROOT}/toolchain/mac_pnacl/bin/pnacl-ar CACHE STRING "")
set(CMAKE_RANLIB
    ${NACL_SDK_ROOT}/toolchain/mac_pnacl/bin/pnacl-ranlib CACHE STRING "")
set(CMAKE_FIND_ROOT_PATH
    ${NACL_SDK_ROOT}/toolchain/mac_pnacl)

set(CMAKE_C_FLAGS    "-U__STRICT_ANSI__" CACHE STRING "")
set(CMAKE_CXX_FLAGS  "-U__STRICT_ANSI__" CACHE STRING "")

set(RL_PLATFORM_NACL BOOL ON)
set(RL_PNACL_FINALIZE ${NACL_SDK_ROOT}/toolchain/mac_pnacl/bin/pnacl-finalize)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
