// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_RECOGNITION_TERM_H_
#define RADARLOVE_RECOGNITION_TERM_H_

#include <Core/Core.h>
#include <Recognition/Variable.h>

namespace rl {
namespace recognition {

class Term : public core::Serializable {
 public:
  using Degree = uint32_t;
  struct VariableDegree : public Serializable {
    Variable variable;
    Degree degree;

    explicit VariableDegree() : degree(1) {}
    explicit VariableDegree(const Variable& v, Degree d)
        : variable(v), degree(d) {}

    bool serialize(core::Message& message) const override;

    bool deserialize(core::Message& message) override;
  };

  using Variables = std::vector<VariableDegree>;

  explicit Term(double coefficient, Variables&& variables);
  explicit Term();

  double coefficient() const;

  Degree degree() const;

  const Variables& variables() const;

  template <typename T>
  T solve(const ActiveTouchSet& touches,
          const coordinator::PresentationEntity::IdentifierMap& entities) const;

  Variable::ValueType valueType() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

 private:
  double _coefficient;
  std::vector<VariableDegree> _variables;
};

}  // namespace recognition
}  // namespace rl

#endif  // RADARLOVE_RECOGNITION_TERM_H_
