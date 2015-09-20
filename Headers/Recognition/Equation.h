// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_RECOGNITION_EQUATION_H__
#define __RADARLOVE_RECOGNITION_EQUATION_H__

#include <Core/Core.h>
#include <Recognition/Variable.h>
#include <Recognition/Term.h>

namespace rl {

class Equation : public Serializable {
 public:
  using Terms = std::vector<Term>;
  explicit Equation(Terms terms, double constant);

  explicit Equation(Equation&& equation) = default;

  const Terms& terms() const;

  Term::Degree degree() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

 private:
  Terms _terms;
  double _constant;

  RL_DISALLOW_COPY_AND_ASSIGN(Equation);
};

}  // namespace rl

#endif  // __RADARLOVE_RECOGNITION_EQUATION_H__
