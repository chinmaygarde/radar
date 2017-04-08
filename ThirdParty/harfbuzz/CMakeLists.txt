# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

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
    -DHB_NO_MT=1
    -DHAVE_OT=1
    -DHAVE_UCDN=1
)
