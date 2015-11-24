// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_PARAMETER_H_
#define RADARLOVE_LAYOUT_PARAMETER_H_

#include <Core/Core.h>
#include <Layout/Variable.h>
#include <Interface/Entity.h>

namespace rl {
namespace layout {

class Parameter {
 public:
  Parameter(double value = 0.0);

  Parameter(const interface::Entity& entity,
            interface::Entity::Property property);

  bool setValue(double value);

  double value() const;

  interface::Entity::Property property() const;

  Variable asVariable() const;

 private:
  interface::Entity::Identifier _identifier;
  interface::Entity::Property _property;
  double _value;

  RL_DISALLOW_COPY_AND_ASSIGN(Parameter);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_PARAMETER_H_
