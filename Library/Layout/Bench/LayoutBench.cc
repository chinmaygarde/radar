// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Layout/Solver.h>

static void SolverCreation(benchmark::State& state) {
  rl::core::Namespace ns;
  while (state.KeepRunning())
    rl::layout::Solver solver(ns);
}

BENCHMARK(SolverCreation);
