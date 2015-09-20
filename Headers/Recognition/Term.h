// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_RECOGNITION_TERM_H__
#define __RADARLOVE_RECOGNITION_TERM_H__

#include <Core/Core.h>
#include <Recognition/Variable.h>

namespace rl {

class Term : public Serializable {
 public:
  using Degree = uint32_t;
  using VariableDegree = std::pair<Variable&, Degree>;
  using Variables = std::vector<VariableDegree>;

  explicit Term(double coefficient, Variables&& variables);
  explicit Term();

  double coefficient() const;

  Degree degree() const;

  const Variables& variables() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message) override;

 private:
  double _coefficient;
  std::vector<VariableDegree> _variables;
};

}  // namespace rl

#endif  // __RADARLOVE_RECOGNITION_TERM_H__
