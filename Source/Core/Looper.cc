// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Looper.h"
#include "Core/Utilities.h"

#include <pthread.h>
#include <mutex>

using namespace rl;

Looper* Looper::Current() {
  static std::once_flag once;
  static pthread_key_t LooperTLSKey;

  std::call_once(once, []() {
    pthread_key_create(&LooperTLSKey, [](void* looper) {
      delete static_cast<Looper*>(looper);
    });
  });

  auto currentLooper = static_cast<Looper*>(pthread_getspecific(LooperTLSKey));

  if (currentLooper == nullptr) {
    currentLooper = new Looper();
    pthread_setspecific(LooperTLSKey, currentLooper);
  }

  return currentLooper;
}

Looper::Looper()
    : _waitSet(),
      _trivialSource(nullptr),
      _shouldTerminate(false),
      _lock(),
      _pendingDispatches(Utils::make_unique<PendingBlocks>()),
      _beforeSleepObservers(),
      _afterSleepObservers() {
}

Looper::~Looper() {
}

bool Looper::addSource(std::shared_ptr<LooperSource> source) {
  return _waitSet.addSource(source);
}

bool Looper::removeSource(std::shared_ptr<LooperSource> source) {
  return _waitSet.removeSource(source);
}

void Looper::loop(std::function<void(void)> onReady) {
  if (_trivialSource.get() == nullptr) {
    /*
     *  A trivial source needs to be added to keep the loop idle
     *  without any other sources present.
     */
    _trivialSource = LooperSource::AsTrivial();
    addSource(_trivialSource);
  }

  if (onReady) {
    onReady();
  }

  beforeSleep();

  while (!_shouldTerminate) {
    LooperSource* source = _waitSet.wait();

    if (source == nullptr) {
      continue;
    }

    afterSleep();

    auto reader = source->reader();

    if (reader) {
      reader(source->readHandle());
    }

    source->onAwoken();

    beforeSleep();
  }

  _shouldTerminate = false;
}

void Looper::terminate() {
  _shouldTerminate = true;
  _trivialSource->writer()(_trivialSource->writeHandle());
}

void Looper::beforeSleep() {
  flushPendingDispatches();
  _beforeSleepObservers.invokeAll();
}

void Looper::afterSleep() {
  flushPendingDispatches();
  _afterSleepObservers.invokeAll();
}

void Looper::flushPendingDispatches() {
  std::unique_ptr<std::list<Block>> pending;

  {
    /*
     *  Hold the lock for as short of a time as possible. Release the lock while
     *  flushing dispatches
     */
    std::lock_guard<std::mutex> lock(_lock);
    if (_pendingDispatches->size() != 0) {
      pending.swap(_pendingDispatches);
      _pendingDispatches = Utils::make_unique<PendingBlocks>();
    }
  }

  if (pending != nullptr) {
    /*
     *  No lock is held here. So callouts can queue more dispatches
     */
    for (const auto& block : *pending) {
      block();
    }
  }
}

void Looper::dispatchAsync(std::function<void()> block) {
  assert(_trivialSource && "A trivial source must be present");

  std::lock_guard<std::mutex> lock(_lock);
  _pendingDispatches->push_back(block);
  _trivialSource->writer()(_trivialSource->writeHandle());
}

void Looper::addObserver(std::shared_ptr<LooperObserver> observer,
                         LooperObserver::Activity activity) {
  switch (activity) {
    case LooperObserver::Activity::BeforeSleep:
      _beforeSleepObservers.addObserver(observer);
      break;
    case LooperObserver::Activity::AfterSleep:
      _afterSleepObservers.addObserver(observer);
      break;
  }
}

void Looper::removeObserver(std::shared_ptr<LooperObserver> observer,
                            LooperObserver::Activity activity) {
  switch (activity) {
    case LooperObserver::Activity::BeforeSleep:
      _beforeSleepObservers.removeObserver(observer);
      break;
    case LooperObserver::Activity::AfterSleep:
      _afterSleepObservers.removeObserver(observer);
      break;
  }
}
