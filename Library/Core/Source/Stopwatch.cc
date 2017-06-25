/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Stopwatch.h>
#include <limits>

namespace rl {
namespace instrumentation {

Stopwatch::Stopwatch(size_t samples)
    : _currentLapIndex(0), _currentLapStartPoint(core::ClockPoint::min()) {
  _laps.resize(std::max<size_t>(1, samples));
}

void Stopwatch::start() {
  _currentLapStartPoint = core::Clock::now();
}

void Stopwatch::stop() {
  _laps[_currentLapIndex] = core::Clock::now() - _currentLapStartPoint;
  _currentLapIndex = (_currentLapIndex + 1) % _laps.size();
}

core::ClockDuration Stopwatch::currentLap() const {
  return core::Clock::now() - _currentLapStartPoint;
}

size_t Stopwatch::samples() const {
  return _laps.size();
}

core::ClockDuration Stopwatch::lastLap() const {
  const auto samples = _laps.size();
  return _laps[(_currentLapIndex + samples - 1) % samples];
}

core::ClockDuration Stopwatch::lapDuration(size_t lap) const {
  return _laps[lap % _laps.size()];
}

AutoStopwatchLap::AutoStopwatchLap(Stopwatch& stopwatch)
    : _stopwatch(stopwatch) {
  _stopwatch.start();
}

AutoStopwatchLap::~AutoStopwatchLap() {
  _stopwatch.stop();
}

}  // namespace instrumentation
}  // namespace rl
