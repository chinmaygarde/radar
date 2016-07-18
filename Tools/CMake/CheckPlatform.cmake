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

if(__check_platform)
  return()
endif()
set(__check_platform INCLUDED)

include(CheckIncludeFiles)

################################################################################
# Detects the current platform.
################################################################################

macro(CheckPlatform)

# The RaspberryPi has the bcm_host.h file at a specific location.
check_include_files("/opt/vc/include/bcm_host.h" RASPBERRY)

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

endmacro()
