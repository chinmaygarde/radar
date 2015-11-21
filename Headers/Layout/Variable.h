// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_VARIABLE_H_
#define RADARLOVE_LAYOUT_VARIABLE_H_

#include <Core/Core.h>
#include <Interface/Entity.h>

namespace rl {
namespace layout {

class Parameter;
class Variable : public core::Serializable {
 public:
  Variable(Parameter* parameter);

  Variable(const Parameter& parameter);

  bool applyUpdate(double value) const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

  struct Hash {
    std::size_t operator()(const Variable& v) const {
      return reinterpret_cast<size_t>(v._parameter);
    }
  };

  struct Equal {
    constexpr bool operator()(const Variable& lhs, const Variable& rhs) const {
      return lhs._parameter == rhs._parameter;
    }
  };

 private:
  Parameter* _parameter;

  RL_DISALLOW_ASSIGN(Variable);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_VARIABLE_H_
