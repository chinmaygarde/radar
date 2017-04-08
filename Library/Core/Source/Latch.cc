/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Latch.h>

namespace rl {
namespace core {

Latch::Latch(size_t count) : _count(count) {}

void Latch::wait() {
  if (_count > 0) {
    std::unique_lock<std::mutex> lock(_conditionMutex);
    _condition.wait(lock, [&] { return _count == 0; });
  }
}

void Latch::countDown() {
  if (--_count == 0) {
    /*
     *  This lock is a pessimization but thread sanitizer seems to freak out
     *  without this. Figure out what is happening with this.
     */
    std::lock_guard<std::mutex> lock(_conditionMutex);
    _condition.notify_all();
  }
}

AutoLatch::AutoLatch(size_t count) : Latch(count) {}

AutoLatch::~AutoLatch() {
  wait();
}

AutoCountdown::AutoCountdown(Latch& latch) : _latch(latch) {}

AutoCountdown::~AutoCountdown() {
  _latch.countDown();
}

}  // namespace core
}  // namespace rl
