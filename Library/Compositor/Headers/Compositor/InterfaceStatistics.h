// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Instrumentation.h>

namespace rl {
namespace compositor {

class InterfaceStatistics {
 public:
  InterfaceStatistics(const std::string& tag);

  const std::string tag() const;

  instrumentation::Stopwatch& interpolations();

  instrumentation::Stopwatch& transactionUpdateTimer();

  instrumentation::Counter& interpolationsCount();

  instrumentation::Counter& constraintsCount();

  instrumentation::Counter& editVariablesCount();

  void start();

  void stop();

 private:
  std::string _tag;
  instrumentation::Stopwatch _interpolations;
  instrumentation::Stopwatch _transactionUpdateTimer;
  instrumentation::Counter _interpolationsCount;
  instrumentation::Counter _constraintsCount;
  instrumentation::Counter _editVariablesCount;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceStatistics);
};

class InterfaceStatisticsFrame {
 public:
  InterfaceStatisticsFrame(InterfaceStatistics& stats);

  ~InterfaceStatisticsFrame();

 private:
  InterfaceStatistics& _stats;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceStatisticsFrame);
};

}  // namespace compositor
}  // namespace rl
