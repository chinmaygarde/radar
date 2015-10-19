// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_GEOMETRY_COLOR_
#define RADARLOVE_GEOMETRY_COLOR_

#include <stdint.h>

#include <cmath>

namespace rl {
struct Color {
  double red;
  double green;
  double blue;
  double alpha;

  Color() : red(0.0), green(0.0), blue(0.0), alpha(0.0) {}

  Color(double r, double g, double b, double a)
      : red(r), green(g), blue(b), alpha(a) {}

  bool operator==(const Color& c) const {
    return red == c.red && green == c.green && blue == c.blue &&
           alpha == c.alpha;
  }

  Color operator+(const Color& other) const;
};

struct ColorHSB {
  double hue;
  double saturation;
  double brightness;
  double alpha;

  ColorHSB(double h, double s, double b, double a)
      : hue(h), saturation(s), brightness(b) {}

  static ColorHSB FromRGB(Color rgb);
  Color ToRGBA() const;
};

static const Color ColorWhite(1.0, 1.0, 1.0, 1.0);
static const Color ColorBlack(0.0, 0.0, 0.0, 1.0);

static const Color ColorWhiteTransparent(1.0, 1.0, 1.0, 0.0);
static const Color ColorBlackTransparent(0.0, 0.0, 0.0, 0.0);

static const Color ColorRed(1.0, 0.0, 0.0, 1.0);
static const Color ColorGreen(0.0, 1.0, 0.0, 1.0);
static const Color ColorBlue(0.0, 0.0, 1.0, 1.0);

/**
 *  The alpha at which we decide the layer or primitive is transparent for
 *  rendering purposes.
 */
static const double TransparencyAlphaThreshold = 0.01;
}  // namespace rl

#endif  // RADARLOVE_GEOMETRY_COLOR_
