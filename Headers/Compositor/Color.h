// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_COLOR__
#define __RADARLOVE_GEOMETRY_COLOR__

namespace rl {
struct Color {
  double r;
  double g;
  double b;
  double a;

  Color(double red, double green, double blue, double alpha)
      : r(red), g(green), b(blue), a(alpha) {}

  bool operator==(const Color& c) const {
    return r == c.r && g == c.g && b == c.b && a == c.a;
  }
};

static const Color ColorWhite(1.0, 1.0, 1.0, 1.0);
static const Color ColorBlack(0.0, 0.0, 0.0, 1.0);

static const Color ColorWhiteTransparent(1.0, 1.0, 1.0, 0.0);
static const Color ColorBlackTransparent(0.0, 0.0, 0.0, 0.0);

static const Color ColorRed(1.0, 0.0, 0.0, 1.0);
static const Color ColorGreen(0.0, 1.0, 0.0, 1.0);
static const Color ColorBlue(0.0, 0.0, 1.0, 1.0);

/**
 *  The alpha at which we decide the layer or primitive if transparent for
 *  rendering purposes.
 */
static const double TransparencyAlphaThreshold = 0.01;
}

#endif /* defined(__RADARLOVE_GEOMETRY_COLOR__) */
