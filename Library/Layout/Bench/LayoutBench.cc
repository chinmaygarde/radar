/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BenchmarkRunner/BenchmarkRunner.h>
#include <Layout/ConstraintCreation.h>
#include <Layout/Solver.h>

static void SolverCreation(benchmark::State& state) {
  rl::core::Namespace ns;
  while (state.KeepRunning()) {
    rl::layout::Solver solver(ns);
  }
}

BENCHMARK(SolverCreation)->Unit(benchmark::TimeUnit::kMicrosecond);

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

BENCHMARK(SolverAdd)->Unit(benchmark::TimeUnit::kMicrosecond);

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

BENCHMARK(SolverRemove)->Unit(benchmark::TimeUnit::kMicrosecond);

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

BENCHMARK(SolverAddRemove)->Unit(benchmark::TimeUnit::kMicrosecond);

static void SolverAddMany(benchmark::State& state) {
  state.SetComplexityN(state.range(0));

  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  while (state.KeepRunning()) {
    for (auto i = 0, count = state.range(0); i < count; i++) {
      rl::layout::Variable something(
          ns, rl::layout::Variable::Property::BoundsWidth);
      auto constraintX = something == 200.0;
      auto res = solver.addConstraints({constraintX});
      RL_ASSERT(res == rl::layout::Result::Success);
    }
  }
}

BENCHMARK(SolverAddMany)
    ->Unit(benchmark::TimeUnit::kMillisecond)
    ->RangeMultiplier(10)
    ->Range(1, 10000)
    ->Complexity();

static void SolverAddSingleToMany(benchmark::State& state) {
  state.SetComplexityN(state.range(0));

  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  for (auto i = 0, count = state.range(0); i < count; i++) {
    rl::layout::Variable something(ns,
                                   rl::layout::Variable::Property::BoundsWidth);
    auto constraintX = something == 200.0;
    auto res = solver.addConstraints({constraintX});
    RL_ASSERT(res == rl::layout::Result::Success);
  }

  while (state.KeepRunning()) {
    rl::layout::Variable final(ns, rl::layout::Variable::Property::BoundsWidth);
    auto constraintX = final == 200.0;
    auto res = solver.addConstraints({constraintX});
    RL_ASSERT(res == rl::layout::Result::Success);

    state.PauseTiming();
    res = solver.removeConstraint({constraintX});
    RL_ASSERT(res == rl::layout::Result::Success);
    state.ResumeTiming();
  }
}

BENCHMARK(SolverAddSingleToMany)
    ->Unit(benchmark::TimeUnit::kMicrosecond)
    ->RangeMultiplier(10)
    ->Range(1, 10000)
    ->Complexity();
