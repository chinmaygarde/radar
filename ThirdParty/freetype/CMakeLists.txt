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

################################################################################
# Library
################################################################################

file(GLOB FREETYPE_LIB_SRC
  freetype/src/autofit/autofit.c
  freetype/src/base/ftbase.c
  freetype/src/base/ftbbox.c
  freetype/src/base/ftbdf.c
  freetype/src/base/ftbitmap.c
  freetype/src/base/ftcid.c
  freetype/src/base/ftfntfmt.c
  freetype/src/base/ftfstype.c
  freetype/src/base/ftgasp.c
  freetype/src/base/ftglyph.c
  freetype/src/base/ftgxval.c
  freetype/src/base/ftinit.c
  freetype/src/base/ftlcdfil.c
  freetype/src/base/ftmm.c
  freetype/src/base/ftotval.c
  freetype/src/base/ftpatent.c
  freetype/src/base/ftpfr.c
  freetype/src/base/ftstroke.c
  freetype/src/base/ftsynth.c
  freetype/src/base/ftsystem.c
  freetype/src/base/fttype1.c
  freetype/src/base/ftwinfnt.c
  freetype/src/bdf/bdf.c
  freetype/src/bzip2/ftbzip2.c
  freetype/src/cache/ftcache.c
  freetype/src/cff/cff.c
  freetype/src/cid/type1cid.c
  freetype/src/gzip/ftgzip.c
  freetype/src/lzw/ftlzw.c
  freetype/src/pcf/pcf.c
  freetype/src/pfr/pfr.c
  freetype/src/psaux/psaux.c
  freetype/src/pshinter/pshinter.c
  freetype/src/psnames/psnames.c
  freetype/src/raster/raster.c
  freetype/src/sfnt/sfnt.c
  freetype/src/smooth/smooth.c
  freetype/src/truetype/truetype.c
  freetype/src/type1/type1.c
  freetype/src/type42/type42.c
  freetype/src/winfonts/winfnt.c
)

add_library(freetype ${FREETYPE_LIB_SRC})

target_include_directories(freetype
  PUBLIC
    freetype/include
)

target_compile_definitions(freetype
  PRIVATE
    -DFT2_BUILD_LIBRARY
)
