// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_ANIMATIONDIRECTOR_H_
#define RADARLOVE_COMPOSITOR_ANIMATIONDIRECTOR_H_

#include <Core/Core.h>
#include <Compositor/PresentationEntity.h>
#include <Compositor/Interpolator.h>
#include <Instrumentation/Stopwatch.h>

#include <unordered_map>

namespace rl {

class AnimationDirector {
 public:
  struct Key {
    Entity::Identifier entityIdentifier;
    Entity::Property entityProperty;
    Key(Entity::Identifier ident, Entity::Property prop)
        : entityIdentifier(ident), entityProperty(prop) {}
  };

  struct KeyHash {
    std::size_t operator()(const Key& key) const {
      return std::hash<Entity::Identifier>()(key.entityIdentifier) ^
             ((key.entityProperty) << 1);
    }
  };

  struct KeyEqual {
    bool operator()(const Key& lhs, const Key& rhs) const {
      return lhs.entityIdentifier == rhs.entityIdentifier &&
             lhs.entityProperty == rhs.entityProperty;
    }
  };

  explicit AnimationDirector();

  template <typename T>
  void setInterpolator(Key key,
                       std::unique_ptr<Interpolator<T>> interpolator,
                       const ClockPoint& startTime) {
    auto result = collection<T>().emplace(key, std::move(interpolator));
    if (result.second) {
      (*(result.first)).second->start(startTime);
    }
  }

  size_t stepInterpolations(Stopwatch& stopwatch);

 private:
  template <typename T>
  using Interpolators = std::
      unordered_map<Key, std::unique_ptr<Interpolator<T>>, KeyHash, KeyEqual>;

  Interpolators<double> _numberInterpolators;
  Interpolators<Point> _pointInterpolators;
  Interpolators<Size> _sizeInterpolators;
  Interpolators<Rect> _rectInterpolators;
  Interpolators<Matrix> _matrixInterpolators;
  Interpolators<Color> _colorInterpolators;

  template <typename T>
  Interpolators<T>& collection();

  RL_DISALLOW_COPY_AND_ASSIGN(AnimationDirector);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_ANIMATIONDIRECTOR_H_
