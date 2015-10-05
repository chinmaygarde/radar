// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_
#define RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_

#include <Core/Core.h>
#include <Recognition/Equation.h>
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

  explicit GestureRecognizer(Variable&& evaluationResult, Equation&& equation);

  explicit GestureRecognizer();

  GestureRecognizer(GestureRecognizer&& recognizer) = default;

  Identifier identifier() const;

  const Equation& equation() const;

  const Variable& evaluationResult() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

  bool shouldBeginRecognition(
      const ActiveTouchSet& touches,
      const PresentationEntity::IdentifierMap& entities) const;

  bool shouldContinueRecognition(
      const ActiveTouchSet& touches,
      const PresentationEntity::IdentifierMap& entities) const;

  struct Less {
    bool operator()(const GestureRecognizer& lhs,
                    const GestureRecognizer& rhs) const {
      return lhs._identifier < rhs._identifier;
    }
  };

 private:
  Identifier _identifier;
  Variable _evaluationResult;
  Equation _equation;
  size_t _touchCount;

  bool _preparedForUse;
  ObservedEntities _observedEntities;
  Variable::ProxySet _observedProxies;

  void prepareForUse();

  RL_DISALLOW_COPY_AND_ASSIGN(GestureRecognizer);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H_
