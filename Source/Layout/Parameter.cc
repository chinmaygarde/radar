// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Parameter.h>

namespace rl {
namespace layout {

Parameter::Parameter(double value) : _value(value) {
}

bool Parameter::setValue(double value) {
  if (_value == value) {
    return false;
  }

  _value = value;

  return true;
}

double Parameter::value() const {
  return _value;
}

Variable Parameter::asVariable() {
  return this;
}

}  // namespace layout
}  // namespace rl
