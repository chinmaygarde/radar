# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

set(ANDROID BOOL ON)
set(ANDROID_PLATFORM_VERSION 21)
set(ANDROID_ARCH arm)
set(ANDROID_CXX_LIB_ARCH armeabi)
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

link_directories("${ANDROID_NDK_ROOT}/sources/cxx-stl/llvm-libc++/libs/${ANDROID_CXX_LIB_ARCH}")
