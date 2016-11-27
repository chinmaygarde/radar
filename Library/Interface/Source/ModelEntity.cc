// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/ModelEntity.h>
#include <algorithm>

namespace rl {
namespace interface {

ModelEntity::ModelEntity(core::Name identifier, UpdateCallback updateCallback)
    : Entity(identifier, updateCallback) {}

void ModelEntity::addChild(ModelEntity::Ref child) {
  if (child == nullptr) {
    return;
  }

  if (std::find(_children.begin(), _children.end(), child) == _children.end()) {
    child->notifyInterfaceIfNecessary(Property::AddedTo, identifier());
    _children.emplace_back(std::move(child));
  }
}

void ModelEntity::removeChild(ModelEntity::Ref child) {
  if (child == nullptr) {
    return;
  }

  auto found = std::find(_children.begin(), _children.end(), child);
  if (found != _children.end()) {
    child->notifyInterfaceIfNecessary(Property::RemovedFrom, identifier());
    _children.erase(found);
  }
}

}  // namespace interface
}  // namespace rl
