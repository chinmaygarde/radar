// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_POLYNOMIAL_H_
#define RADARLOVE_RECOGNITION_POLYNOMIAL_H_

#include <Core/Core.h>
#include <Recognition/Variable.h>
#include <Recognition/Term.h>

namespace rl {

class Polynomial : public Serializable {
 public:
  using Terms = std::vector<Term>;
  explicit Polynomial(Terms terms, double constant);

  explicit Polynomial();

  explicit Polynomial(Polynomial&& Polynomial) = default;

  const Terms& terms() const;

  Term::Degree degree() const;

  double constant() const;

  Variable::ValueType valueType() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

 private:
  Terms _terms;
  double _constant;

  RL_DISALLOW_COPY_AND_ASSIGN(Polynomial);
};

}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_POLYNOMIAL_H_
