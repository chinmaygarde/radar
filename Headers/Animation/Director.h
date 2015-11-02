// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_DIRECTOR_H_
#define RADARLOVE_ANIMATION_DIRECTOR_H_

#include <Core/Core.h>
#include <Coordinator/PresentationEntity.h>
#include <Animation/Interpolator.h>
#include <Instrumentation/Stopwatch.h>

#include <unordered_map>

namespace rl {
namespace animation {

class Director {
 public:
  struct Key {
    interface::Entity::Identifier entityIdentifier;
    interface::Entity::Property entityProperty;
    Key(interface::Entity::Identifier ident, interface::Entity::Property prop)
        : entityIdentifier(ident), entityProperty(prop) {}
  };

  explicit Director();

  template <typename T>
  void setInterpolator(Key key,
                       std::unique_ptr<Interpolator<T>> interpolator,
                       const core::ClockPoint& startTime) {
    auto result = collection<T>().emplace(key, std::move(interpolator));
    if (result.second) {
      (*(result.first)).second->start(startTime);
    }
  }

  size_t stepInterpolations(instrumentation::Stopwatch& stopwatch);

 private:
  struct KeyHash {
    std::size_t operator()(const Key& key) const {
      return std::hash<interface::Entity::Identifier>()(key.entityIdentifier) ^
             ((key.entityProperty) << 1);
    }
  };

  struct KeyEqual {
    bool operator()(const Key& lhs, const Key& rhs) const {
      return lhs.entityIdentifier == rhs.entityIdentifier &&
             lhs.entityProperty == rhs.entityProperty;
    }
  };

  template <typename T>
  using Interpolators = std::
      unordered_map<Key, std::unique_ptr<Interpolator<T>>, KeyHash, KeyEqual>;

  Interpolators<double> _numberInterpolators;
  Interpolators<geom::Point> _pointInterpolators;
  Interpolators<geom::Size> _sizeInterpolators;
  Interpolators<geom::Rect> _rectInterpolators;
  Interpolators<geom::Matrix> _matrixInterpolators;
  Interpolators<coordinator::Color> _colorInterpolators;

  template <typename T>
  Interpolators<T>& collection();

  RL_DISALLOW_COPY_AND_ASSIGN(Director);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_DIRECTOR_H_