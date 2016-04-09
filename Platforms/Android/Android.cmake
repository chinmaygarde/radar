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

set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

set(ANDROID BOOL ON)
set(ANDROID_PLATFORM_VERSION 16)
set(ANDROID_ARCH arm)
set(ANDROID_TOOLCHAIN arm-linux-androideabi)
set(ANDROID_GCC_VERSION 4.9)

################################################################################
# Setup the toolchain on the host
################################################################################

set(ANDROID_ROOT "${ANDROID_NDK_ROOT}" CACHE STRING "Android NDK Root")

if(CMAKE_HOST_APPLE)
  set(TOOLCHAIN_HOST_DIR "darwin-x86_64")
else()
  message(FATAL_ERROR "TOOLCHAIN_HOST_DIR not set for this platform")
endif()

set(TOOLCHAIN
  "${ANDROID_ROOT}/toolchains/${ANDROID_TOOLCHAIN}-${ANDROID_GCC_VERSION}/prebuilt/${TOOLCHAIN_HOST_DIR}/bin/${ANDROID_TOOLCHAIN}")

set(CMAKE_C_COMPILER   "${TOOLCHAIN}-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}-g++")
set(CMAKE_AR           "${TOOLCHAIN}-ar" CACHE STRING "")
set(CMAKE_RANLIB       "${TOOLCHAIN}-ranlib" CACHE STRING "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

################################################################################
# Setup sysroot
################################################################################

set(ANDROID_SYSROOT "${ANDROID_NDK_ROOT}/platforms/android-${ANDROID_PLATFORM_VERSION}/arch-${ANDROID_ARCH}")

set(CMAKE_FIND_ROOT_PATH "${ANDROID_SYSROOT}")
set(CMAKE_SYSROOT "${ANDROID_SYSROOT}")

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

include_directories("${ANDROID_NDK_ROOT}/sources/android/support/include")
include_directories("${ANDROID_NDK_ROOT}/sources/cxx-stl/llvm-libc++/libcxx/include")
include_directories("${ANDROID_NDK_ROOT}/sources/cxx-stl/llvm-libc++abi/libcxxabi/include")
