/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <stdint.h>
#include <string>

namespace rl {
namespace entity {

struct ColorHSB;

/**
 *  Represents a RGBA color
 */
struct Color {
  /**
   *  The red color component (0 to 1)
   */
  double red;

  /**
   *  The green color component (0 to 1)
   */
  double green;

  /**
   *  The blue color component (0 to 1)
   */
  double blue;

  /**
   *  The alpha component of the color (0 to 1)
   */
  double alpha;

  Color() : red(0.0), green(0.0), blue(0.0), alpha(0.0) {}

  Color(const ColorHSB& hsbColor);

  Color(double r, double g, double b, double a)
      : red(r), green(g), blue(b), alpha(a) {}

  bool operator==(const Color& c) const {
    return red == c.red && green == c.green && blue == c.blue &&
           alpha == c.alpha;
  }

  Color operator+(const Color& other) const;

  std::string toString() const;

  void fromString(const std::string& str);

  static Color White() { return {1.0, 1.0, 1.0, 1.0}; }
  static Color Black() { return {0.0, 0.0, 0.0, 1.0}; }
  static Color WhiteTransparent() { return {1.0, 1.0, 1.0, 0.0}; }
  static Color BlackTransparent() { return {0.0, 0.0, 0.0, 0.0}; }
  static Color Red() { return {1.0, 0.0, 0.0, 1.0}; }
  static Color Green() { return {0.0, 1.0, 0.0, 1.0}; }
  static Color Blue() { return {0.0, 0.0, 1.0, 1.0}; }
};

/**
 *  Represents a color by its constituent hue, saturation, brightness and alpha
 */
struct ColorHSB {
  /**
   *  The hue of the color (0 to 1)
   */
  double hue;

  /**
   *  The saturation of the color (0 to 1)
   */
  double saturation;

  /**
   *  The brightness of the color (0 to 1)
   */
  double brightness;

  /**
   *  The alpha of the color (0 to 1)
   */
  double alpha;

  ColorHSB(double h, double s, double b, double a)
      : hue(h), saturation(s), brightness(b), alpha(a) {}

  static ColorHSB FromRGB(Color rgb);

  Color ToRGBA() const;
};

}  // namespace entity
}  // namespace rl
