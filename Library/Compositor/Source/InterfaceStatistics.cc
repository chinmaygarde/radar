/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/InterfaceStatistics.h>

namespace rl {
namespace compositor {

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

}  // namespace compositor
}  // namespace rl
