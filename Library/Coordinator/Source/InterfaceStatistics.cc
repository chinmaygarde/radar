// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/InterfaceStatistics.h>

namespace rl {
namespace coordinator {

InterfaceStatistics::InterfaceStatistics(const std::string& tag) : _tag(tag) {}

const std::string InterfaceStatistics::tag() const {
  return _tag;
}

instrumentation::Stopwatch& InterfaceStatistics::interpolations() {
  return _interpolations;
}

instrumentation::Stopwatch& InterfaceStatistics::transactionUpdateTimer() {
  return _transactionUpdateTimer;
}

instrumentation::Counter& InterfaceStatistics::interpolationsCount() {
  return _interpolationsCount;
}

instrumentation::Counter& InterfaceStatistics::constraintsCount() {
  return _constraintsCount;
}

instrumentation::Counter& InterfaceStatistics::editVariablesCount() {
  return _editVariablesCount;
}

void InterfaceStatistics::start() {
  _interpolations.start();
  _interpolationsCount.reset();
}

void InterfaceStatistics::stop() {
  _interpolations.stop();
}

InterfaceStatisticsFrame::InterfaceStatisticsFrame(InterfaceStatistics& stats)
    : _stats(stats) {
  _stats.start();
}

InterfaceStatisticsFrame::~InterfaceStatisticsFrame() {
  _stats.stop();
}

}  // namespace coordinator
}  // namespace rl