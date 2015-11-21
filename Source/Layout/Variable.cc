// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Variable.h>
#include <Layout/Utilities.h>
#include <Layout/Parameter.h>

namespace rl {
namespace layout {

Variable::Variable(Parameter* parameter) : _parameter(parameter) {
}

bool Variable::applyUpdate(double value) const {
  if (_parameter == nullptr) {
    return false;
  }
  return _parameter->setValue(value);
}

bool Variable::serialize(core::Message& message) const {
  auto success = true;
  // TODO: WIP
  return success;
}

bool Variable::deserialize(core::Message& message) {
  auto success = true;
  // TODO: WIP
  return success;
}

}  // namespace layout
}  // namespace rl
