/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "LayoutUtilities.h"

namespace rl {
namespace layout {

bool NearZero(double value) {
  static const double epsilon = 1e-6;
  return value < 0.0 ? -value < epsilon : value < epsilon;
}

}  // namespace layout
}  // namespace rl
