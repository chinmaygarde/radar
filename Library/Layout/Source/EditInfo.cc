/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Layout/EditInfo.h>

namespace rl {
namespace layout {

EditInfo::EditInfo(const Tag& tag,
                   const Constraint& constraint,
                   double constant)
    : _tag(tag), _constraint(constraint), _constant(constant) {}

const Tag& EditInfo::tag() const {
  return _tag;
}

void EditInfo::setTag(const Tag& tag) {
  _tag = tag;
}

const Constraint& EditInfo::constraint() const {
  return _constraint;
}

double EditInfo::constant() const {
  return _constant;
}

void EditInfo::setConstant(double constant) {
  _constant = constant;
}

}  // namespace layout
}  // namespace rl
