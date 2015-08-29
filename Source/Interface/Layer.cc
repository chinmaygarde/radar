// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Layer.h>
#include <Interface/Interface.h>

namespace rl {

Layer::Layer() : Entity(true), _sublayers(), _superlayer(nullptr) {
}

Layer::~Layer() {
}

void Layer::addSublayer(Layer::Ref layer) {
  if (layer == nullptr) {
    return;
  }

  /**
   *  Make sure this layer has no parent to begin with
   */
  if (layer->_superlayer != nullptr) {
    layer->_superlayer->removeSublayer(layer);
  }
  RL_ASSERT(layer->_superlayer == nullptr);

  /*
   *  Assign the parent and add the same to the sublayers collections
   */
  layer->_superlayer = this;
  layer->notifyInterfaceIfNecessary(AddedTo, identifier());
  _sublayers.push_back(layer);
}

void Layer::removeSublayer(Layer::Ref layer) {
  if (layer == nullptr) {
    return;
  }

  auto found = std::find(_sublayers.begin(), _sublayers.end(), layer);

  if (found != _sublayers.end()) {
    _sublayers.erase(found);
    layer->_superlayer = nullptr;
    layer->notifyInterfaceIfNecessary(RemovedFrom, identifier());
  }
}

void Layer::makeRootInInterface() const {
  notifyInterfaceIfNecessary(MakeRoot);
}

const std::list<Layer::Ref>& Layer::sublayers() const {
  return _sublayers;
}

const Layer* Layer::superlayer() const {
  return _superlayer;
}

}  // namespace rl
