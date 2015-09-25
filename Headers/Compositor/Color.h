// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_COLOR__
#define __RADARLOVE_GEOMETRY_COLOR__

#include <cmath>

namespace rl {
struct Color {
  double red;
  double green;
  double blue;
  double alpha;

  Color(double r, double g, double b, double a)
      : red(r), green(g), blue(b), alpha(a) {}

  bool operator==(const Color& c) const {
    return red == c.red && green == c.green && blue == c.blue &&
           alpha == c.alpha;
  }
};

struct ColorHSB {
  double hue;
  double saturation;
  double brightness;
  double alpha;

  ColorHSB(double h, double s, double b, double a)
      : hue(h), saturation(s), brightness(b){};

  static ColorHSB FromRGB(Color rgb) {
    double R = rgb.red;
    double G = rgb.green;
    double B = rgb.blue;

    double v = 0.0;
    double x = 0.0;
    double f = 0.0;

    long i = 0;

    x = fmin(R, G);
    x = fmin(x, B);

    v = fmax(R, G);
    v = fmax(v, B);

    if (v == x)
      return ColorHSB(0.0, 0.0, v, rgb.alpha);

    f = (R == x) ? G - B : ((G == x) ? B - R : R - G);
    i = (R == x) ? 3 : ((G == x) ? 5 : 1);

    return ColorHSB(((i - f / (v - x)) / 6.0), (v - x) / v, v, rgb.alpha);
  }

  Color ToRGBA() const {
    double h = hue * 6.0;
    double s = saturation;
    double v = brightness;

    double m = 0.0;
    double n = 0.0;
    double f = 0.0;

    long i = 0;

    if (h == 0)
      h = 0.01;

    if (h == 0.0)
      return Color(v, v, v, alpha);

    i = floor(h);

    f = h - i;

    if (!(i & 1))
      f = 1 - f;

    m = v * (1 - s);
    n = v * (1 - s * f);

    switch (i) {
      case 6:
      case 0:
        return Color(v, n, m, alpha);
      case 1:
        return Color(n, v, m, alpha);
      case 2:
        return Color(m, v, n, alpha);
      case 3:
        return Color(m, n, v, alpha);
      case 4:
        return Color(n, m, v, alpha);
      case 5:
        return Color(v, m, n, alpha);
    }
    return Color(0, 0, 0, alpha);
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

#endif  // __RADARLOVE_GEOMETRY_COLOR__
