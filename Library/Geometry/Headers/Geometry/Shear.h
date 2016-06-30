// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_SHEAR_H_
#define RADAR_GEOMETRY_SHEAR_H_

namespace rl {
namespace geom {

struct Shear {
  double xy;
  double xz;
  double yz;

  Shear() : xy(0.0), xz(0.0), yz(0.0) {}

  Shear(double xy, double xz, double yz) : xy(xy), xz(xz), yz(yz) {}
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_SHEAR_H_
