/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Image/Image.h>

static void BenchDecodeJPG(benchmark::State& state) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.jpg"});

  rl::image::Image image(std::move(file));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodePNG(benchmark::State& state) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.png"});

  rl::image::Image image(std::move(file));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodeJPGFromAllocation(benchmark::State& state) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.jpg"});

  rl::core::FileMapping map(file);

  rl::core::Allocation allocation;
  RL_ASSERT(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodePNGFromAllocation(benchmark::State& state) {
  rl::core::FileHandle file(rl::core::URI{"file://Beachball.png"});

  rl::core::FileMapping map(file);

  rl::core::Allocation allocation;
  RL_ASSERT(allocation.resize(map.size()));
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

BENCHMARK(BenchDecodeJPG);
BENCHMARK(BenchDecodePNG);
BENCHMARK(BenchDecodeJPGFromAllocation);
BENCHMARK(BenchDecodePNGFromAllocation);
