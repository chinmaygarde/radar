// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Image/Image.h>

static void BenchDecodeJPG(benchmark::State& state) {
  rl::core::File file(rl::core::URI{"file://Beachball.jpg"});

  rl::image::Image image(std::move(file));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodePNG(benchmark::State& state) {
  rl::core::File file(rl::core::URI{"file://Beachball.png"});

  rl::image::Image image(std::move(file));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodeJPGFromAllocation(benchmark::State& state) {
  rl::core::File file(rl::core::URI{"file://Beachball.jpg"});
  auto map = file.map();

  rl::core::Allocation allocation;
  allocation.resize(map.size());
  memmove(allocation.data(), map.mapping(), map.size());

  rl::image::Image image(std::move(allocation));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

static void BenchDecodePNGFromAllocation(benchmark::State& state) {
  rl::core::File file(rl::core::URI{"file://Beachball.png"});
  auto map = file.map();

  rl::core::Allocation allocation;
  allocation.resize(map.size());
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
