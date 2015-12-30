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

cmake_minimum_required(VERSION 2.8)

################################
# RadarLove Library
################################

project(RadarLove)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(WIN32)
  if(NOT WINDOWS)
    set(WINDOWS TRUE)
  endif()
elseif(UNIX AND NOT APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    if(NOT RASPBERRY)
      set(LINUX TRUE)
    endif()
  elseif(CMAKE_SYSTEM_NAME MATCHES "kFreeBSD.*")
    set(FREEBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "kNetBSD.*|NetBSD.*")
    set(NETBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "kOpenBSD.*|OpenBSD.*")
    set(OPENBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*GNU.*")
    set(GNU TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*BSDI.*")
    set(BSDI TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "DragonFly.*|FreeBSD")
    set(FREEBSD TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "SYSV5.*")
    set(SYSV5 TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "Solaris.*")
    set(SOLARIS TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "HP-UX.*")
    set(HPUX TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "AIX.*")
    set(AIX TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES "Minix.*")
    set(MINIX TRUE)
  endif()
elseif(APPLE)
  if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*")
    set(DARWIN TRUE)
  elseif(CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
    set(MACOSX TRUE)
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "BeOS.*")
  set(BEOS TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Haiku.*")
  set(HAIKU TRUE)
endif()

if(NOT WINDOWS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # We add the no-unknown-warning-option because we want to add flags that are
  # only available on newer Clang versions without crapping out on older
  # defaults
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wpessimizing-move  -Wredundant-move")
endif()

file(GLOB_RECURSE RADARLOVE_SRC
  "Headers/*.h"
  "Source/*.h"
  "Source/*.cc"
  "Source/*.mm"
)

file(GLOB RADARLOVE_IMGUI_SRC
  "ThirdParty/imgui/imgui_draw.cpp"
  "ThirdParty/imgui/imgui.cpp"
  "ThirdParty/imgui/imgui_demo.cpp"
)

include_directories ("Headers")
include_directories ("ThirdParty")

add_library(RadarLove ${RADARLOVE_SRC} ${RADARLOVE_IMGUI_SRC})

if(LINUX)
  target_link_libraries(RadarLove rt GL)
endif()

if (FREEBSD)
  link_directories(
    "/usr/local/lib"
  )
  target_link_libraries(RadarLove GLESv2)
endif()

if (APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL -framework Foundation")
endif()

if(RASPBERRY)
  add_definitions("-D__raspberrypi__=1")
  target_link_libraries(RadarLove rt)
endif()

################################
# Google Test Library
################################

add_subdirectory (Test/GoogleTest)
enable_testing ()
include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests Target
################################

file(GLOB RADARLOVE_TEST_SRC
  "Test/*.h"
  "Test/*.cc"
  "Test/*.mm"
)

include_directories ( Source )
add_executable( RadarLoveTest ${RADARLOVE_TEST_SRC} )
target_link_libraries( RadarLoveTest RadarLove gtest gtest_main)
add_test( RadarLoveTestAll RadarLoveTest )

################################
# Linux Desktop Shell
################################

if(LINUX)
  file(GLOB RADARLOVE_LINUX_DESKTOP_SOURCES
    "Platforms/Linux/*.h"
    "Platforms/Linux/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  include_directories ("Samples")

  add_executable( RadarLoveLinuxDesktop ${RADARLOVE_LINUX_DESKTOP_SOURCES} )

  include(FindPkgConfig)

  pkg_search_module(SDL2 REQUIRED sdl2)

  include_directories(${SDL2_INCLUDE_DIRS})
  target_link_libraries(RadarLoveLinuxDesktop
    RadarLove
    pthread
    ${SDL2_LIBRARIES}
    rt
  )
endif()

################################
# Rasperry Pi Shell
################################

if(RASPBERRY)
  file(GLOB RADARLOVE_RASPBERRY_DESKTOP_SOURCES
    "Platforms/RaspberryPi/Source/*.h"
    "Platforms/RaspberryPi/Source/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  include_directories (
    "Samples"
    "/opt/vc/include"
    "/opt/vc/include/interface/vcos/pthreads"
    "/opt/vc/include/interface/vmcs_host/linux"
  )

  link_directories(
    "/opt/vc/lib"
  )

  add_executable( RadarLoveRaspberryPi ${RADARLOVE_RASPBERRY_DESKTOP_SOURCES} )

  include_directories(${SDL2_INCLUDE_DIRS})

  target_link_libraries(RadarLoveRaspberryPi
    RadarLove
    pthread
    rt
    EGL
    GLESv2
    openmaxil
    bcm_host
    vcos
    vchiq_arm
  )
endif()

################################
# FreeBSD Desktop Shell
################################

if(FREEBSD)
  file(GLOB RADARLOVE_BSD_DESKTOP_SOURCES
    "Samples/*.h"
    "Samples/*.cc"
  )
  include_directories ("Samples")

  add_executable( RadarLoveBSDDesktop ${RADARLOVE_BSD_DESKTOP_SOURCES} )

  include_directories("/usr/local/include")
  link_directories("/usr/local/lib")

  target_link_libraries(RadarLoveBSDDesktop
    RadarLove
    pthread
  )
endif()

################################
# Native Client (PNaCl) Shell
################################

if(RL_PLATFORM_NACL)

  file(GLOB RADARLOVE_NACL_SOURCES
    "Platforms/Nacl/Source/*.h"
    "Platforms/Nacl/Source/*.cc"
    "Samples/*.h"
    "Samples/*.cc"
  )

  # Generate the Executable

  include_directories ("Samples")

  add_executable( RadarLoveNacl ${RADARLOVE_NACL_SOURCES} )

  target_link_libraries(RadarLoveNacl
    RadarLove
    "ppapi"
    "ppapi_cpp"
    "nacl_io"
    "ppapi_gles2"
  )

  # Finalize PEXEs

  # Execuatable Shell
  add_custom_command(
    OUTPUT "RadarLoveNacl.pexe"
    COMMAND ${RL_PNACL_FINALIZE} -o RadarLoveNacl.pexe RadarLoveNacl
    DEPENDS RadarLoveNacl
    COMMENT "Finalizing Native Client PEXE" VERBATIM
  )

  add_custom_target(
    RadarLoveNaclPexe ALL
    DEPENDS "RadarLoveNacl.pexe"
  )

  # Test Target
  add_custom_command(
    OUTPUT "RadarLoveTest.pexe"
    COMMAND ${RL_PNACL_FINALIZE} -o RadarLoveTest.pexe RadarLoveTest
    DEPENDS RadarLoveTest
    COMMENT "Finalizing Native Client Test PEXE" VERBATIM
  )

  add_custom_target(
    RadarLoveNaclTest ALL
    DEPENDS "RadarLoveTest.pexe"
  )

  # Copy over the HTML harness and manifest

  configure_file("Platforms/Nacl/RadarLove.html" "RadarLove.html" COPYONLY)
  configure_file("Platforms/Nacl/RadarLove.nmf" "RadarLove.nmf" COPYONLY)
  configure_file("Platforms/Nacl/Server" "Server" COPYONLY)
  configure_file("Platforms/Nacl/GDBInit" "GDBInit" COPYONLY)
  configure_file("Platforms/Nacl/Debug" "Debug" COPYONLY)

endif()
