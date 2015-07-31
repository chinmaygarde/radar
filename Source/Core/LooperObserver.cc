// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/LooperObserver.h"

#include <cassert>

using namespace rl;

LooperObserver::LooperObserver(uint64_t priority, Callback callback)
    : _priority(priority), _callback(callback) {
  assert(_callback && "A callback must be provided to the looper observer");
}

uint64_t LooperObserver::priority() const {
  return _priority;
}

void LooperObserver::invoke() const {
  _callback();
}

bool LooperObserverComparer::operator()(
    std::shared_ptr<LooperObserver> a,
    std::shared_ptr<LooperObserver> b) const {
  return a->priority() < b->priority();
}

LooperObserverCollection::LooperObserverCollection() : _observers(){};

void LooperObserverCollection::addObserver(
    std::shared_ptr<LooperObserver> observer) {
  AutoLock lock(_lock);
  _observers.insert(observer);
}

void LooperObserverCollection::removeObserver(
    std::shared_ptr<LooperObserver> observer) {
  AutoLock lock(_lock);
  _observers.erase(observer);
}

void LooperObserverCollection::invokeAll() {
  _lock.lock();

  if (_observers.size() == 0) {
    _lock.unlock();
    return;
  }

  LooperObserversSet copied(_observers);

  _lock.unlock();

  for (const auto& observer : copied) {
    observer->invoke();
  }
}
