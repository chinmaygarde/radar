// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Image/Image.h>

#include <SampleJPG.h>
#include <SamplePNG.h>

static void BenchDecodeJPG(benchmark::State& state) {
  rl::core::Allocation allocation;

  RL_ASSERT(allocation.resize(SampleJPGLength));
  memcpy(allocation.data(), SampleJPG, SampleJPGLength);

  rl::image::Image image(std::move(allocation));

  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

BENCHMARK(BenchDecodeJPG);

static void BenchDecodePNG(benchmark::State& state) {
  rl::core::Allocation allocation;

  RL_ASSERT(allocation.resize(SamplePNGLength));
  memcpy(allocation.data(), SamplePNG, SamplePNGLength);

  rl::image::Image image(std::move(allocation));
  while (state.KeepRunning()) {
    auto res = image.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

BENCHMARK(BenchDecodePNG);
