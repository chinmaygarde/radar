// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Action.h"

using namespace rl;

Action::Action()
    : _duration(0.125), _repeatCount(1), _autoReverses(false), _beginTime(0.0) {
}

double Action::duration() const {
  return _duration;
}

void Action::setDuration(double duration) {
  _duration = duration;
}

uint64_t Action::repeatCount() const {
  return _repeatCount;
}

void Action::setRepeatCount(uint64_t repeatCount) {
  _repeatCount = repeatCount;
}

bool Action::autoReverses() const {
  return _autoReverses;
}

void Action::setAutoReverses(bool autoReverses) {
  _autoReverses = autoReverses;
}

double Action::beginTime() const {
  return _beginTime;
}

void Action::setBeginTime(double beginTime) {
  _beginTime = beginTime;
}
