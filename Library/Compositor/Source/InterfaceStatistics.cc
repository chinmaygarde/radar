/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/InterfaceStatistics.h>
#include "Console.h"

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

void InterfaceStatistics::present() const {
  RL_CONSOLE_SECTION(_tag.c_str());
  RL_CONSOLE_DISPLAY_VALUE("Interpolations (%zu): %.2f ms",
                           _interpolationsCount.count(),
                           _interpolations.lastLap().count() * 1e3);
  RL_CONSOLE_DISPLAY_VALUE("Last Transaction: %.2f ms",
                           _transactionUpdateTimer.lastLap().count() * 1e3);
  RL_CONSOLE_DISPLAY_VALUE("Constraints: %zu", _constraintsCount.count());
  RL_CONSOLE_DISPLAY_VALUE("Edit Vars: %zu", _editVariablesCount.count());
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
