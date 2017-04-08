/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <DevSupport/DevSupport.h>
#include <gtest/gtest.h>

int main(int argc, char** argv) {
  rl::dev::UpdateWorkingDirectoryForFixtures();

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
