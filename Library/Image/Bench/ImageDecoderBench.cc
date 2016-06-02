// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <ImageDecoder/ImageDecoder.h>

#include <SampleJPG.h>
#include <SamplePNG.h>

static void BenchDecodeJPG(benchmark::State& state) {
  auto allocation = rl::core::make_unique<rl::core::Allocation>();
  RL_ASSERT(allocation->resize(SampleJPGLength));
  memcpy(allocation->data(), SampleJPG, SampleJPGLength);
  rl::image::ImageDecoder decoder(std::move(allocation));
  while (state.KeepRunning()) {
    auto res = decoder.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

BENCHMARK(BenchDecodeJPG);

static void BenchDecodePNG(benchmark::State& state) {
  auto allocation = rl::core::make_unique<rl::core::Allocation>();
  RL_ASSERT(allocation->resize(SamplePNGLength));
  memcpy(allocation->data(), SamplePNG, SamplePNGLength);
  rl::image::ImageDecoder decoder(std::move(allocation));
  while (state.KeepRunning()) {
    auto res = decoder.decode();
    RL_ASSERT(res.wasSuccessful());
  }
}

BENCHMARK(BenchDecodePNG);
