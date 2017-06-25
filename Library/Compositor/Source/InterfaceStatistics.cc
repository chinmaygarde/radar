/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/InterfaceStatistics.h>
#include "Console.h"

namespace rl {
namespace compositor {

InterfaceStatistics::InterfaceStatistics(const std::string& tag)
    : _tag(tag), _interpolations(100), _transactionUpdateTimer(100) {}

const std::string InterfaceStatistics::tag() const {
  return _tag;
}

instrumentation::Stopwatch& InterfaceStatistics::interpolations() {
  return _interpolations;
}

instrumentation::Counter& InterfaceStatistics::interpolationsCount() {
  return _interpolationsCount;
}

instrumentation::Stopwatch& InterfaceStatistics::transactionUpdateTimer() {
  return _transactionUpdateTimer;
}

instrumentation::Counter& InterfaceStatistics::transactionsCount() {
  return _transactionsCount;
}

instrumentation::Counter& InterfaceStatistics::entitiesCount() {
  return _entitiesCount;
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
  RL_CONSOLE_DISPLAY_LABEL("Interpolators: %zu", _interpolationsCount.count());
  RL_CONSOLE_DISPLAY_VALUE("Animations", _interpolations);
  RL_CONSOLE_DISPLAY_LABEL("Transactions: %zu", _transactionsCount.count());
  RL_CONSOLE_DISPLAY_VALUE("Transactions", _transactionUpdateTimer);
  RL_CONSOLE_DISPLAY_LABEL("Entities: %zu", _entitiesCount.count());
  RL_CONSOLE_DISPLAY_LABEL("Constraints: %zu", _constraintsCount.count());
  RL_CONSOLE_DISPLAY_LABEL("Edit Vars: %zu", _editVariablesCount.count());
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
