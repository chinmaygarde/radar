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

file(GLOB HARFBUZZ_SRC
  Source/src/hb-blob.cc
  Source/src/hb-buffer-serialize.cc
  Source/src/hb-buffer.cc
  Source/src/hb-common.cc
  Source/src/hb-face.cc
  Source/src/hb-fallback-shape.cc
  Source/src/hb-font.cc
  Source/src/hb-ot-font.cc
  Source/src/hb-ot-layout.cc
  Source/src/hb-ot-map.cc
  Source/src/hb-ot-shape-complex-arabic.cc
  Source/src/hb-ot-shape-complex-default.cc
  Source/src/hb-ot-shape-complex-hangul.cc
  Source/src/hb-ot-shape-complex-hebrew.cc
  Source/src/hb-ot-shape-complex-indic-table.cc
  Source/src/hb-ot-shape-complex-indic.cc
  Source/src/hb-ot-shape-complex-myanmar.cc
  Source/src/hb-ot-shape-complex-thai.cc
  Source/src/hb-ot-shape-complex-tibetan.cc
  Source/src/hb-ot-shape-complex-use-table.cc
  Source/src/hb-ot-shape-complex-use.cc
  Source/src/hb-ot-shape-fallback.cc
  Source/src/hb-ot-shape-normalize.cc
  Source/src/hb-ot-shape.cc
  Source/src/hb-ot-tag.cc
  Source/src/hb-set.cc
  Source/src/hb-shape-plan.cc
  Source/src/hb-shape.cc
  Source/src/hb-shaper.cc
  Source/src/hb-unicode.cc
  Source/src/hb-warning.cc
  Source/src/hb-ucdn.cc
  Source/src/hb-ucdn/ucdn.c

  # Freetype backend.
  Source/src/hb-ft.cc
)

add_library(harfbuzz ${HARFBUZZ_SRC})

target_link_libraries(harfbuzz
  PUBLIC
    freetype
)

target_include_directories(harfbuzz
  PUBLIC
    Source/src
    Generated
  PRIVATE
    Source/src/hb-ucdn
)

target_compile_definitions(harfbuzz
  PRIVATE
    -DHAVE_OT=1
    -DHAVE_UCDN=1
)
