# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

file(GLOB_RECURSE SHELL_SRC
  "Source/*.c"
  "Source/*.cc"
  "Source/*.h"
  "Source/*.m"
  "Source/*.mm"
)

file(GLOB_RECURSE SHELL_ASSETS
  "Assets/*"
)

add_executable(MacShell
  ${SHELL_SRC}
  ${SHELL_ASSETS}
)

find_library(APPKIT AppKit)

target_link_libraries(MacShell
  PRIVATE
    Shell
    Samples
    ${APPKIT}
)

set_target_properties(MacShell
  PROPERTIES
    MACOSX_BUNDLE               TRUE
    RESOURCE                    "${SHELL_ASSETS}"
    MACOSX_BUNDLE_INFO_PLIST    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Info.plist"
)
