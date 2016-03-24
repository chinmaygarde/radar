// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/EventLoopObserver.h>

namespace rl {
namespace core {

EventLoopObserver::EventLoopObserver(Callback callback, int64_t priority)
    : _priority(priority), _callback(callback) {
  RL_ASSERT_MSG(_callback, "A callback must be provided to the loop observer");
}

int64_t EventLoopObserver::priority() const {
  return _priority;
}

void EventLoopObserver::invoke(Activity activity) const {
  _callback(activity);
}

bool EventLoopObserverComparer::operator()(
    std::shared_ptr<EventLoopObserver> a,
    std::shared_ptr<EventLoopObserver> b) const {
  return a->priority() < b->priority();
}

EventLoopObserverCollection::EventLoopObserverCollection(
    EventLoopObserver::Activity activity)
    : _activity(activity) {}

bool EventLoopObserverCollection::addObserver(
    std::shared_ptr<EventLoopObserver> observer) {
  std::lock_guard<std::mutex> lock(_observersMutex);
  auto result = _observers.insert(observer);
  return result.second;
}

bool EventLoopObserverCollection::removeObserver(
    std::shared_ptr<EventLoopObserver> observer) {
  std::lock_guard<std::mutex> lock(_observersMutex);

  auto found = _observers.find(observer);
  if (found == _observers.end()) {
    return false;
  }
  _observers.erase(found);
  return true;
}

void EventLoopObserverCollection::invokeAll() {
  EventLoopObserversSet copied;

  {
    std::lock_guard<std::mutex> lock(_observersMutex);

    if (_observers.size() == 0) {
      return;
    }

    copied = _observers;
  }

  for (const auto& observer : copied) {
    observer->invoke(_activity);
  }
}

}  // namespace core
}  // namespace rl