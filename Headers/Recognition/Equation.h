// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_RECOGNITION_EQUATION_H__
#define __RADARLOVE_RECOGNITION_EQUATION_H__

#include <Core/Core.h>
#include <Recognition/Variable.h>

namespace rl {

class Equation {
 public:
  class Term {
   public:
    using VariableDegree = std::pair<Variable&, double>;
    using Variables = std::vector<VariableDegree>;

    explicit Term(double coefficient, Variables variables = {});

    double coefficient() const;
    const Variables& variables() const;

   private:
    double _coefficient;
    std::vector<VariableDegree> _variables;
  };

  using Terms = std::vector<Term>;
  explicit Equation(Terms terms);

  const Terms& terms() const;

 private:
  Terms _terms;

  DISALLOW_COPY_AND_ASSIGN(Equation);
};

}  // namespace rl

#endif  // __RADARLOVE_RECOGNITION_EQUATION_H__
