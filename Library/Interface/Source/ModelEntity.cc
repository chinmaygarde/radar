/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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

const std::vector<ModelEntity::Ref>& ModelEntity::children() const {
  return _children;
}

void ModelEntity::visitHierarchy(std::function<bool(ModelEntity&)> visitor) {
  if (visitor == nullptr) {
    return;
  }

  if (!visitor(*this)) {
    return;
  }

  for (const auto& entity : _children) {
    entity->visitHierarchy(visitor);
  }
}

}  // namespace interface
}  // namespace rl
