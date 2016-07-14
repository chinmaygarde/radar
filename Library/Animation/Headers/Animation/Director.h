// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_DIRECTOR_H_
#define RADARLOVE_ANIMATION_DIRECTOR_H_

#include <Core/Core.h>

#include <Animation/Interpolator.h>
#include <Core/Stopwatch.h>

#include <unordered_map>

namespace rl {
namespace animation {

class Director {
 public:
  struct Key {
    core::Name entityIdentifier;
    entity::Entity::Property entityProperty;
    Key(core::Name ident, entity::Entity::Property prop)
        : entityIdentifier(ident), entityProperty(prop) {}
  };

  explicit Director();

  template <typename T, typename... InterpolatorArgs>
  void setInterpolator(Key key,
                       const core::ClockPoint& startTime,
                       InterpolatorArgs&&... args) {
    /*
     *  Create a fresh interpolator and attempt to insert it into the
     *  collection. If one doesn't already exist, the emplace will succeed and
     *  we can start the interpolation.
     *
     *  Note: This logic will be augmented once interpolation replacement
     *  behavior is settled upon.
     */
    auto& interpolators = collection<T>();
    auto interpolator = core::make_unique<Interpolator<T>>(
        std::forward<InterpolatorArgs>(args)...);

    auto result = interpolators.emplace(key, std::move(interpolator));
    if (result.second) {
      (*(result.first)).second->start(startTime);
    }
  }

  size_t stepInterpolations(instrumentation::Stopwatch& stopwatch);

 private:
  struct KeyHash {
    std::size_t operator()(const Key& key) const {
      size_t seed = key.entityIdentifier.hash();
      core::HashCombine(seed, static_cast<uint64_t>(key.entityProperty));
      return seed;
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
  Interpolators<geom::Matrix::Decomposition> _matrixInterpolators;
  Interpolators<entity::ColorHSB> _colorInterpolators;

  template <typename T>
  Interpolators<T>& collection();

  RL_DISALLOW_COPY_AND_ASSIGN(Director);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_DIRECTOR_H_
