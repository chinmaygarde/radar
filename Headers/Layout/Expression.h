// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_EXPRESSION_H_
#define RADARLOVE_LAYOUT_EXPRESSION_H_

#include <Core/Core.h>
#include <Layout/Term.h>

#include <vector>

namespace rl {
namespace layout {

class Expression : public core::Serializable {
 public:
  using Terms = std::vector<Term>;

  Expression();

  Expression(const Terms& terms, double constant);

  Expression(const Term& term);

  Expression(const Variable& variable);

  Expression(double constant);

  core::Namespace* ns() const;

  const Terms& terms() const;

  double constant() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

 private:
  Terms _terms;
  double _constant;
  core::Namespace* _inferredNamespace;

  void inferNamespace();

  RL_DISALLOW_ASSIGN(Expression);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_EXPRESSION_H_
