SET(CMAKE_SYSTEM_NAME Nacl)
SET(CMAKE_SYSTEM_VERSION 1)

include_directories(BEFORE SYSTEM /Applications/nacl_sdk/pepper_45/include)
include_directories(BEFORE SYSTEM /Applications/nacl_sdk/pepper_45/include/newlib)

link_directories(/Applications/nacl_sdk/pepper_45/lib/pnacl/Release)

set(CMAKE_C_COMPILER      /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl/bin/pnacl-clang)
set(CMAKE_CXX_COMPILER    /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl/bin/pnacl-clang++)
set(CMAKE_AR              /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl/bin/pnacl-ar CACHE STRING "")
set(CMAKE_RANLIB          /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl/bin/pnacl-ranlib CACHE STRING "")
set(CMAKE_FIND_ROOT_PATH  /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl)

set(CMAKE_C_FLAGS               "-U__STRICT_ANSI__" CACHE STRING "")
set(CMAKE_CXX_FLAGS             "-U__STRICT_ANSI__" CACHE STRING "")

set(RL_PLATFORM_NACL BOOL ON)
set(RL_PNACL_FINALIZE /Applications/nacl_sdk/pepper_45/toolchain/mac_pnacl/bin/pnacl-finalize)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
