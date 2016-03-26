// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/ModelEntity.h>

namespace rl {
namespace interface {

ModelEntity::ModelEntity(core::Name identifier, UpdateCallback updateCallback)
    : Entity(identifier, updateCallback) {}

void ModelEntity::addChild(const ModelEntity& child) {
  child.notifyInterfaceIfNecessary(Property::AddedTo, identifier());
}

void ModelEntity::removeChild(const ModelEntity& child) {
  child.notifyInterfaceIfNecessary(Property::RemovedFrom, identifier());
}

}  // namespace interface
}  // namespace rl
