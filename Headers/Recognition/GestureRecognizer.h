// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H__
#define __RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H__

#include <Core/Core.h>
#include <Recognition/Equation.h>
#include <Recognition/Variable.h>

namespace rl {

class GestureRecognizer : public Serializable {
 public:
  explicit GestureRecognizer(Variable&& evaluationResult, Equation&& equation);

  explicit GestureRecognizer();

  GestureRecognizer(GestureRecognizer&& recognizer) = default;

  const Equation& equation() const;

  const Variable& evaluationResult() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

 private:
  Equation _equation;
  Variable _evaluationResult;

  RL_DISALLOW_COPY_AND_ASSIGN(GestureRecognizer);
};

}  // namespace rl

#endif  // __RADARLOVE_RECOGNITION_GESTURERECOGNIZER_H__
