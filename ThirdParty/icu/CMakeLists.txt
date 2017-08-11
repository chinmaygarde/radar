# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# ICU Stub Data
################################################################################

AddRecommendedWarningFlags()

file(GLOB_RECURSE ICU_STUBDATA_SRC
  "icu/source/stubdata/*.cpp"
  "icu/source/stubdata/*.h"
)

add_library(icu_stubdata
  ${ICU_STUBDATA_SRC}
)

target_include_directories(icu_stubdata
  PRIVATE
    "icu/source/common"
)

################################################################################
# ICU Common
################################################################################

AddRecommendedWarningFlags()

file(GLOB_RECURSE ICU_COMMON_SRC
  "icu/source/common/*.cpp"
  "icu/source/common/*.h"
  "icu/source/common/unicode/*.h"
  "icu/source/common/unicode/*.cpp"
)

add_library(icu_common
  ${ICU_COMMON_SRC}
)

target_include_directories(icu_common
  PUBLIC
    "icu/source/common"
)

target_compile_definitions(icu_common
  PUBLIC
    -DU_DISABLE_RENAMING=1
  PRIVATE
    -DU_COMMON_IMPLEMENTATION=1
)

target_link_libraries(icu_common
  PRIVATE
    icu_stubdata
)

################################################################################
# ICU i18n
################################################################################

file(GLOB_RECURSE ICU_I18N_SRC
  "icu/source/i18n/*.cpp"
  "icu/source/i18n/*.h"
  "icu/source/i18n/unicode/*.h"
  "icu/source/i18n/unicode/*.cpp"
)

add_library(icu_i18n
  ${ICU_I18N_SRC}
)

target_include_directories(icu_i18n
  PUBLIC
    "icu/source/i18n"
)

target_compile_definitions(icu_i18n
  PRIVATE
    -DU_I18N_IMPLEMENTATION=1
)

target_link_libraries(icu_i18n
  PUBLIC
    icu_common
)
