# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Executable
################################################################################

file(GLOB_RECURSE LIBRARY_SRC_CC
  "Source/*.h"
  "Source/*.c"
  "Source/*.cc"
)

add_executable(PasswordChecker ${LIBRARY_SRC_CC})

AddRecommendedWarningFlags()

target_link_libraries(PasswordChecker
  PRIVATE
    Toolbox
)
