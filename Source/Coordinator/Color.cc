// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Color.h>
#include <algorithm>
#include <cmath>

namespace rl {
namespace coordinator {

ColorHSB ColorHSB::FromRGB(Color rgb) {
  double R = rgb.red;
  double G = rgb.green;
  double B = rgb.blue;

  double v = 0.0;
  double x = 0.0;
  double f = 0.0;

  int64_t i = 0;

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

Color ColorHSB::ToRGBA() const {
  double h = hue * 6.0;
  double s = saturation;
  double v = brightness;

  double m = 0.0;
  double n = 0.0;
  double f = 0.0;

  int64_t i = 0;

  if (h == 0)
    h = 0.01;

  if (h == 0.0)
    return Color(v, v, v, alpha);

  i = static_cast<int64_t>(floor(h));

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

static inline double ColorClamp(double value, double min, double max) {
  return std::max(min, std::min(value, max));
}

Color Color::operator+(const Color& o) const {
  /*
   *  FIXME: We need natural color mixing. This following implementation is
   *  complete BS. But I just need to get past this templating stuff quickly. My
   *  bad.
   *
   *  Similar to how we do interpolation on colors (by converting to HSB and
   *  interpolating HSB components for a more "natural" interpolation), I
   *  suspect, a conversion to Lab color space for mixing is in order. Till
   *  then, this will have to do :/
   */
  return Color(ColorClamp(red + o.red, 0.0, 1.0),
               ColorClamp(green + o.green, 0.0, 1.0),
               ColorClamp(blue + o.blue, 0.0, 1.0),
               ColorClamp(alpha + o.alpha, 0.0, 1.0));
}

}  // namespace coordinator
}  // namespace rl
