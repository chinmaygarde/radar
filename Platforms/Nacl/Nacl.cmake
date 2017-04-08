# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

set(NACL BOOL ON)

include_directories(BEFORE SYSTEM ${NACL_SDK_ROOT}/include)
include_directories(BEFORE SYSTEM ${NACL_SDK_ROOT}/include/newlib)

link_directories(${NACL_SDK_ROOT}/lib/pnacl/Release)

if(CMAKE_HOST_APPLE)
  set(PNACL_TOOLCHAIN "mac_pnacl")
elseif(CMAKE_HOST_UNIX)
  set(PNACL_TOOLCHAIN "linux_pnacl")
else()
  message(FATAL_ERROR "PNACL_TOOLCHAIN variable not set for this platform")
endif()

set(CMAKE_C_COMPILER
    ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN}/bin/pnacl-clang)
set(CMAKE_CXX_COMPILER
    ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN}/bin/pnacl-clang++)
set(CMAKE_AR
    ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN}/bin/pnacl-ar CACHE STRING "")
set(CMAKE_RANLIB
    ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN}/bin/pnacl-ranlib CACHE STRING "")
set(CMAKE_FIND_ROOT_PATH
    ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN})

set(CMAKE_C_FLAGS    "-U__STRICT_ANSI__" CACHE STRING "")
set(CMAKE_CXX_FLAGS  "-U__STRICT_ANSI__" CACHE STRING "")

set(RL_PNACL_FINALIZE ${NACL_SDK_ROOT}/toolchain/${PNACL_TOOLCHAIN}/bin/pnacl-finalize)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
