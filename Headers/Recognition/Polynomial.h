// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_POLYNOMIAL_H_
#define RADARLOVE_RECOGNITION_POLYNOMIAL_H_

#include <Core/Core.h>
#include <Recognition/Variable.h>
#include <Recognition/Term.h>

namespace rl {

class Polynomial : public core::Serializable {
 public:
  using Terms = std::vector<Term>;
  explicit Polynomial(Terms terms, double constant);

  explicit Polynomial();

  explicit Polynomial(Polynomial&& Polynomial) = default;

  const Terms& terms() const;

  Term::Degree degree() const;

  double constant() const;

  Variable::ValueType valueType() const;

  template <typename T>
  T solve(const ActiveTouchSet& touches,
          const PresentationEntity::IdentifierMap& entities) const {
    auto solution = T{};

    for (auto const& term : _terms) {
      solution = solution + term.solve<T>(touches, entities);
    }

    return solution;
  }

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

 private:
  Terms _terms;
  double _constant;

  RL_DISALLOW_COPY_AND_ASSIGN(Polynomial);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_POLYNOMIAL_H_
