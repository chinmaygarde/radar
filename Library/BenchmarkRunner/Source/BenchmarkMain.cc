/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <DevSupport/DevSupport.h>

int main(int argc, char** argv) {
  rl::dev::UpdateWorkingDirectoryForFixtures();
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
