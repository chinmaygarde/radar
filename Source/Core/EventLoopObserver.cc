// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/EventLoopObserver.h>

namespace rl {
namespace core {

EventLoopObserver::EventLoopObserver(uint64_t priority, Callback callback)
    : _priority(priority), _callback(callback) {
  RL_ASSERT_MSG(_callback, "A callback must be provided to the loop observer");
}

uint64_t EventLoopObserver::priority() const {
  return _priority;
}

void EventLoopObserver::invoke() const {
  _callback();
}

bool EventLoopObserverComparer::operator()(
    std::shared_ptr<EventLoopObserver> a,
    std::shared_ptr<EventLoopObserver> b) const {
  return a->priority() < b->priority();
}

EventLoopObserverCollection::EventLoopObserverCollection() : _observers() {
}

void EventLoopObserverCollection::addObserver(
    std::shared_ptr<EventLoopObserver> observer) {
  std::lock_guard<std::mutex> lock(_lock);
  _observers.insert(observer);
}

void EventLoopObserverCollection::removeObserver(
    std::shared_ptr<EventLoopObserver> observer) {
  std::lock_guard<std::mutex> lock(_lock);
  _observers.erase(observer);
}

void EventLoopObserverCollection::invokeAll() {
  _lock.lock();

  if (_observers.size() == 0) {
    _lock.unlock();
    return;
  }

  EventLoopObserversSet copied(_observers);

  _lock.unlock();

  for (const auto& observer : copied) {
    observer->invoke();
  }
}

}  // namespace core
}  // namespace rl
