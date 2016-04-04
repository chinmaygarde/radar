// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Layout/Solver.h>
#include <Layout/ConstraintCreation.h>

static void SolverCreation(benchmark::State& state) {
  rl::core::Namespace ns;
  while (state.KeepRunning()) {
    rl::layout::Solver solver(ns);
  }
}

BENCHMARK(SolverCreation);

static void SolverAdd(benchmark::State& state) {
  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  rl::layout::Variable width(ns, rl::layout::Variable::Property::BoundsWidth);
  rl::layout::Variable height(ns, rl::layout::Variable::Property::BoundsHeight);

  auto constraintX = width == 200.0;
  auto constraintY = height == 900.0;

  while (state.KeepRunning()) {
    solver.addConstraints({constraintX, constraintY});
    state.PauseTiming();
    solver.removeConstraints({constraintX, constraintY});
    state.ResumeTiming();
  }
}

BENCHMARK(SolverAdd);

static void SolverRemove(benchmark::State& state) {
  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  rl::layout::Variable width(ns, rl::layout::Variable::Property::BoundsWidth);
  rl::layout::Variable height(ns, rl::layout::Variable::Property::BoundsHeight);

  auto constraintX = width == 200.0;
  auto constraintY = height == 900.0;

  while (state.KeepRunning()) {
    state.PauseTiming();
    solver.addConstraints({constraintX, constraintY});
    state.ResumeTiming();
    solver.removeConstraints({constraintX, constraintY});
  }
}

BENCHMARK(SolverRemove);

static void SolverAddRemove(benchmark::State& state) {
  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  rl::layout::Variable width(ns, rl::layout::Variable::Property::BoundsWidth);
  rl::layout::Variable height(ns, rl::layout::Variable::Property::BoundsHeight);

  auto constraintX = width == 200.0;
  auto constraintY = height == 900.0;

  while (state.KeepRunning()) {
    solver.addConstraints({constraintX, constraintY});
    solver.removeConstraints({constraintX, constraintY});
  }
}

BENCHMARK(SolverAddRemove);
