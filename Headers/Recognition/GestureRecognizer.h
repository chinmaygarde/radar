// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_
#define RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_

#include <Core/Core.h>
#include <Recognition/Polynomial.h>
#include <Recognition/Variable.h>
#include <Recognition/ActiveTouchSet.h>
#include <Compositor/PresentationEntity.h>

#include <set>
#include <vector>

namespace rl {
class GestureRecognizer : public Serializable {
 public:
  using Identifier = uint64_t;
  using Collection = std::vector<GestureRecognizer>;
  using ObservedEntities = std::set<Entity::Identifier>;

  enum class Continuation {
    Continue,
    End,
    Cancel,
  };

  explicit GestureRecognizer(Variable&& evaluationResult,
                             Polynomial&& polynomial);

  explicit GestureRecognizer();

  GestureRecognizer(GestureRecognizer&& recognizer) = default;

  Identifier identifier() const;

  const Polynomial& polynomial() const;

  const Variable& evaluationResult() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

  bool isSolvable() const;

  bool shouldBeginRecognition(
      const ActiveTouchSet& touches,
      const PresentationEntity::IdentifierMap& entities) const;

  Continuation stepRecognition(
      const ActiveTouchSet& touches,
      const PresentationEntity::IdentifierMap& entities) const;

 private:
  Identifier _identifier;
  Variable _evaluationResult;
  Polynomial _polynomial;
  size_t _touchCount;

  bool _preparedForUse;
  ObservedEntities _observedEntities;
  Variable::ProxySet _observedProxies;

  void prepareForUse();

  RL_DISALLOW_COPY_AND_ASSIGN(GestureRecognizer);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_
