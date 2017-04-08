/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */
//
// (c) Dean McNamee <dean@gmail.com>, 2012.
// (c) C++ port by Konstantin Käfer <mail@kkaefer.com>, 2014.
// (c) Adapted for Radar by Chinmay Garde <chinmaygarde@gmail.com>, 2016.
//
// https://github.com/deanm/css-color-parser-js
// https://github.com/kkaefer/css-color-parser-cpp
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "SVGColor.h"
#include <cmath>
#include <map>
#include <sstream>
#include <vector>

namespace rl {
namespace ib {

static const std::map<std::string, entity::Color> kNamedSVGColors = {
    {"none", {0.0, 0.0, 0.0, 0.0}},
    {"aliceblue", {240.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"antiquewhite", {250.0 / 255.0, 235.0 / 255.0, 215.0 / 255.0, 1.0}},
    {"aqua", {0.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"aquamarine", {127.0 / 255.0, 255.0 / 255.0, 212.0 / 255.0, 1.0}},
    {"azure", {240.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"beige", {245.0 / 255.0, 245.0 / 255.0, 220.0 / 255.0, 1.0}},
    {"bisque", {255.0 / 255.0, 228.0 / 255.0, 196.0 / 255.0, 1.0}},
    {"black", {0.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"blanchedalmond", {255.0 / 255.0, 235.0 / 255.0, 205.0 / 255.0, 1.0}},
    {"blue", {0.0 / 255.0, 0.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"blueviolet", {138.0 / 255.0, 43.0 / 255.0, 226.0 / 255.0, 1.0}},
    {"brown", {165.0 / 255.0, 42.0 / 255.0, 42.0 / 255.0, 1.0}},
    {"burlywood", {222.0 / 255.0, 184.0 / 255.0, 135.0 / 255.0, 1.0}},
    {"cadetblue", {95.0 / 255.0, 158.0 / 255.0, 160.0 / 255.0, 1.0}},
    {"chartreuse", {127.0 / 255.0, 255.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"chocolate", {210.0 / 255.0, 105.0 / 255.0, 30.0 / 255.0, 1.0}},
    {"coral", {255.0 / 255.0, 127.0 / 255.0, 80.0 / 255.0, 1.0}},
    {"cornflowerblue", {100.0 / 255.0, 149.0 / 255.0, 237.0 / 255.0, 1.0}},
    {"cornsilk", {255.0 / 255.0, 248.0 / 255.0, 220.0 / 255.0, 1.0}},
    {"crimson", {220.0 / 255.0, 20.0 / 255.0, 60.0 / 255.0, 1.0}},
    {"cyan", {0.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"darkblue", {0.0 / 255.0, 0.0 / 255.0, 139.0 / 255.0, 1.0}},
    {"darkcyan", {0.0 / 255.0, 139.0 / 255.0, 139.0 / 255.0, 1.0}},
    {"darkgoldenrod", {184.0 / 255.0, 134.0 / 255.0, 11.0 / 255.0, 1.0}},
    {"darkgray", {169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0, 1.0}},
    {"darkgreen", {0.0 / 255.0, 100.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"darkgrey", {169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0, 1.0}},
    {"darkkhaki", {189.0 / 255.0, 183.0 / 255.0, 107.0 / 255.0, 1.0}},
    {"darkmagenta", {139.0 / 255.0, 0.0 / 255.0, 139.0 / 255.0, 1.0}},
    {"darkolivegreen", {85.0 / 255.0, 107.0 / 255.0, 47.0 / 255.0, 1.0}},
    {"darkorange", {255.0 / 255.0, 140.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"darkorchid", {153.0 / 255.0, 50.0 / 255.0, 204.0 / 255.0, 1.0}},
    {"darkred", {139.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"darksalmon", {233.0 / 255.0, 150.0 / 255.0, 122.0 / 255.0, 1.0}},
    {"darkseagreen", {143.0 / 255.0, 188.0 / 255.0, 143.0 / 255.0, 1.0}},
    {"darkslateblue", {72.0 / 255.0, 61.0 / 255.0, 139.0 / 255.0, 1.0}},
    {"darkslategray", {47.0 / 255.0, 79.0 / 255.0, 79.0 / 255.0, 1.0}},
    {"darkslategrey", {47.0 / 255.0, 79.0 / 255.0, 79.0 / 255.0, 1.0}},
    {"darkturquoise", {0.0 / 255.0, 206.0 / 255.0, 209.0 / 255.0, 1.0}},
    {"darkviolet", {148.0 / 255.0, 0.0 / 255.0, 211.0 / 255.0, 1.0}},
    {"deeppink", {255.0 / 255.0, 20.0 / 255.0, 147.0 / 255.0, 1.0}},
    {"deepskyblue", {0.0 / 255.0, 191.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"dimgray", {105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0, 1.0}},
    {"dimgrey", {105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0, 1.0}},
    {"dodgerblue", {30.0 / 255.0, 144.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"firebrick", {178.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0, 1.0}},
    {"floralwhite", {255.0 / 255.0, 250.0 / 255.0, 240.0 / 255.0, 1.0}},
    {"forestgreen", {34.0 / 255.0, 139.0 / 255.0, 34.0 / 255.0, 1.0}},
    {"fuchsia", {255.0 / 255.0, 0.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"gainsboro", {220.0 / 255.0, 220.0 / 255.0, 220.0 / 255.0, 1.0}},
    {"ghostwhite", {248.0 / 255.0, 248.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"gold", {255.0 / 255.0, 215.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"goldenrod", {218.0 / 255.0, 165.0 / 255.0, 32.0 / 255.0, 1.0}},
    {"gray", {128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"green", {0.0 / 255.0, 128.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"greenyellow", {173.0 / 255.0, 255.0 / 255.0, 47.0 / 255.0, 1.0}},
    {"grey", {128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"honeydew", {240.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0, 1.0}},
    {"hotpink", {255.0 / 255.0, 105.0 / 255.0, 180.0 / 255.0, 1.0}},
    {"indianred", {205.0 / 255.0, 92.0 / 255.0, 92.0 / 255.0, 1.0}},
    {"indigo", {75.0 / 255.0, 0.0 / 255.0, 130.0 / 255.0, 1.0}},
    {"ivory", {255.0 / 255.0, 255.0 / 255.0, 240.0 / 255.0, 1.0}},
    {"khaki", {240.0 / 255.0, 230.0 / 255.0, 140.0 / 255.0, 1.0}},
    {"lavender", {230.0 / 255.0, 230.0 / 255.0, 250.0 / 255.0, 1.0}},
    {"lavenderblush", {255.0 / 255.0, 240.0 / 255.0, 245.0 / 255.0, 1.0}},
    {"lawngreen", {124.0 / 255.0, 252.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"lemonchiffon", {255.0 / 255.0, 250.0 / 255.0, 205.0 / 255.0, 1.0}},
    {"lightblue", {173.0 / 255.0, 216.0 / 255.0, 230.0 / 255.0, 1.0}},
    {"lightcoral", {240.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"lightcyan", {224.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"lightgoldenrodyellow",
     {250.0 / 255.0, 250.0 / 255.0, 210.0 / 255.0, 1.0}},
    {"lightgray", {211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0, 1.0}},
    {"lightgreen", {144.0 / 255.0, 238.0 / 255.0, 144.0 / 255.0, 1.0}},
    {"lightgrey", {211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0, 1.0}},
    {"lightpink", {255.0 / 255.0, 182.0 / 255.0, 193.0 / 255.0, 1.0}},
    {"lightsalmon", {255.0 / 255.0, 160.0 / 255.0, 122.0 / 255.0, 1.0}},
    {"lightseagreen", {32.0 / 255.0, 178.0 / 255.0, 170.0 / 255.0, 1.0}},
    {"lightskyblue", {135.0 / 255.0, 206.0 / 255.0, 250.0 / 255.0, 1.0}},
    {"lightslategray", {119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0, 1.0}},
    {"lightslategrey", {119.0 / 255.0, 136.0 / 255.0, 153.0 / 255.0, 1.0}},
    {"lightsteelblue", {176.0 / 255.0, 196.0 / 255.0, 222.0 / 255.0, 1.0}},
    {"lightyellow", {255.0 / 255.0, 255.0 / 255.0, 224.0 / 255.0, 1.0}},
    {"lime", {0.0 / 255.0, 255.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"limegreen", {50.0 / 255.0, 205.0 / 255.0, 50.0 / 255.0, 1.0}},
    {"linen", {250.0 / 255.0, 240.0 / 255.0, 230.0 / 255.0, 1.0}},
    {"magenta", {255.0 / 255.0, 0.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"maroon", {128.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"mediumaquamarine", {102.0 / 255.0, 205.0 / 255.0, 170.0 / 255.0, 1.0}},
    {"mediumblue", {0.0 / 255.0, 0.0 / 255.0, 205.0 / 255.0, 1.0}},
    {"mediumorchid", {186.0 / 255.0, 85.0 / 255.0, 211.0 / 255.0, 1.0}},
    {"mediumpurple", {147.0 / 255.0, 112.0 / 255.0, 219.0 / 255.0, 1.0}},
    {"mediumseagreen", {60.0 / 255.0, 179.0 / 255.0, 113.0 / 255.0, 1.0}},
    {"mediumslateblue", {123.0 / 255.0, 104.0 / 255.0, 238.0 / 255.0, 1.0}},
    {"mediumspringgreen", {0.0 / 255.0, 250.0 / 255.0, 154.0 / 255.0, 1.0}},
    {"mediumturquoise", {72.0 / 255.0, 209.0 / 255.0, 204.0 / 255.0, 1.0}},
    {"mediumvioletred", {199.0 / 255.0, 21.0 / 255.0, 133.0 / 255.0, 1.0}},
    {"midnightblue", {25.0 / 255.0, 25.0 / 255.0, 112.0 / 255.0, 1.0}},
    {"mintcream", {245.0 / 255.0, 255.0 / 255.0, 250.0 / 255.0, 1.0}},
    {"mistyrose", {255.0 / 255.0, 228.0 / 255.0, 225.0 / 255.0, 1.0}},
    {"moccasin", {255.0 / 255.0, 228.0 / 255.0, 181.0 / 255.0, 1.0}},
    {"navajowhite", {255.0 / 255.0, 222.0 / 255.0, 173.0 / 255.0, 1.0}},
    {"navy", {0.0 / 255.0, 0.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"oldlace", {253.0 / 255.0, 245.0 / 255.0, 230.0 / 255.0, 1.0}},
    {"olive", {128.0 / 255.0, 128.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"olivedrab", {107.0 / 255.0, 142.0 / 255.0, 35.0 / 255.0, 1.0}},
    {"orange", {255.0 / 255.0, 165.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"orangered", {255.0 / 255.0, 69.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"orchid", {218.0 / 255.0, 112.0 / 255.0, 214.0 / 255.0, 1.0}},
    {"palegoldenrod", {238.0 / 255.0, 232.0 / 255.0, 170.0 / 255.0, 1.0}},
    {"palegreen", {152.0 / 255.0, 251.0 / 255.0, 152.0 / 255.0, 1.0}},
    {"paleturquoise", {175.0 / 255.0, 238.0 / 255.0, 238.0 / 255.0, 1.0}},
    {"palevioletred", {219.0 / 255.0, 112.0 / 255.0, 147.0 / 255.0, 1.0}},
    {"papayawhip", {255.0 / 255.0, 239.0 / 255.0, 213.0 / 255.0, 1.0}},
    {"peachpuff", {255.0 / 255.0, 218.0 / 255.0, 185.0 / 255.0, 1.0}},
    {"peru", {205.0 / 255.0, 133.0 / 255.0, 63.0 / 255.0, 1.0}},
    {"pink", {255.0 / 255.0, 192.0 / 255.0, 203.0 / 255.0, 1.0}},
    {"plum", {221.0 / 255.0, 160.0 / 255.0, 221.0 / 255.0, 1.0}},
    {"powderblue", {176.0 / 255.0, 224.0 / 255.0, 230.0 / 255.0, 1.0}},
    {"purple", {128.0 / 255.0, 0.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"red", {255.0 / 255.0, 0.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"rosybrown", {188.0 / 255.0, 143.0 / 255.0, 143.0 / 255.0, 1.0}},
    {"royalblue", {65.0 / 255.0, 105.0 / 255.0, 225.0 / 255.0, 1.0}},
    {"saddlebrown", {139.0 / 255.0, 69.0 / 255.0, 19.0 / 255.0, 1.0}},
    {"salmon", {250.0 / 255.0, 128.0 / 255.0, 114.0 / 255.0, 1.0}},
    {"sandybrown", {244.0 / 255.0, 164.0 / 255.0, 96.0 / 255.0, 1.0}},
    {"seagreen", {46.0 / 255.0, 139.0 / 255.0, 87.0 / 255.0, 1.0}},
    {"seashell", {255.0 / 255.0, 245.0 / 255.0, 238.0 / 255.0, 1.0}},
    {"sienna", {160.0 / 255.0, 82.0 / 255.0, 45.0 / 255.0, 1.0}},
    {"silver", {192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0, 1.0}},
    {"skyblue", {135.0 / 255.0, 206.0 / 255.0, 235.0 / 255.0, 1.0}},
    {"slateblue", {106.0 / 255.0, 90.0 / 255.0, 205.0 / 255.0, 1.0}},
    {"slategray", {112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0, 1.0}},
    {"slategrey", {112.0 / 255.0, 128.0 / 255.0, 144.0 / 255.0, 1.0}},
    {"snow", {255.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0, 1.0}},
    {"springgreen", {0.0 / 255.0, 255.0 / 255.0, 127.0 / 255.0, 1.0}},
    {"steelblue", {70.0 / 255.0, 130.0 / 255.0, 180.0 / 255.0, 1.0}},
    {"tan", {210.0 / 255.0, 180.0 / 255.0, 140.0 / 255.0, 1.0}},
    {"teal", {0.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0, 1.0}},
    {"thistle", {216.0 / 255.0, 191.0 / 255.0, 216.0 / 255.0, 1.0}},
    {"tomato", {255.0 / 255.0, 99.0 / 255.0, 71.0 / 255.0, 1.0}},
    {"turquoise", {64.0 / 255.0, 224.0 / 255.0, 208.0 / 255.0, 1.0}},
    {"violet", {238.0 / 255.0, 130.0 / 255.0, 238.0 / 255.0, 1.0}},
    {"wheat", {245.0 / 255.0, 222.0 / 255.0, 179.0 / 255.0, 1.0}},
    {"white", {255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0}},
    {"whitesmoke", {245.0 / 255.0, 245.0 / 255.0, 245.0 / 255.0, 1.0}},
    {"yellow", {255.0 / 255.0, 255.0 / 255.0, 0.0 / 255.0, 1.0}},
    {"yellowgreen", {154.0 / 255.0, 205.0 / 255.0, 50.0 / 255.0, 1.0}},
};

template <typename T>
uint8_t ClampSVGByte(T i) {
  /*
   *  Clamp to integer 0 .. 255.
   */
  i = std::round(i);
  return i < 0 ? 0 : i > 255 ? 255 : i;
}

template <typename T>
double ClampSVGDouble(T f) {
  /*
   *  Clamp to double 0.0 .. 1.0.
   */
  return f < 0 ? 0 : f > 1 ? 1 : f;
}

double ParseDouble(const std::string& str) {
  return strtod(str.c_str(), nullptr);
}

int64_t ParseInteger(const std::string& str, uint8_t base = 10) {
  return strtoll(str.c_str(), nullptr, base);
}

uint8_t ParseSVGInteger(const std::string& str) {
  /*
   *  integer or percentage.
   */
  if (str.length() && str.back() == '%') {
    return ClampSVGByte(ParseDouble(str) / 100.0 * 255.0);
  } else {
    return ClampSVGByte(ParseInteger(str));
  }
}

double ParseSVGDouble(const std::string& str) {
  /*
   *  Double or percentage.
   */
  if (str.length() && str.back() == '%') {
    return ClampSVGDouble(ParseDouble(str) / 100.0);
  } else {
    return ClampSVGDouble(ParseDouble(str));
  }
}

double SVGHueToRGB(double m1, double m2, double h) {
  if (h < 0.0) {
    h += 1.0;
  } else if (h > 1.0) {
    h -= 1.0;
  }

  if (h * 6.0 < 1.0) {
    return m1 + (m2 - m1) * h * 6.0;
  }
  if (h * 2.0 < 1.0) {
    return m2;
  }
  if (h * 3.0 < 2.0) {
    return m1 + (m2 - m1) * (2.0 / 3.0 - h) * 6.0;
  }
  return m1;
}

std::vector<std::string> SplitString(const std::string& s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

entity::Color SVGColorMake(uint8_t red,
                           uint8_t green,
                           uint8_t blue,
                           double alpha) {
  return {
      static_cast<double>(red) / 255.0,    //
      static_cast<double>(green) / 255.0,  //
      static_cast<double>(blue) / 255.0,   //
      static_cast<double>(alpha),          //
  };
}

entity::Color ColorFromString(const char* svg_str) {
  std::string str = svg_str;

  /*
   *  Remove all whitespace, not compliant, but should just be more accepting.
   */
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

  /*
   *  Convert to lowercase.
   */
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);

  auto found = kNamedSVGColors.find(str);
  if (found != kNamedSVGColors.end()) {
    return found->second;
  }

  /*
   *  #abc and #abc123 syntax.
   */
  if (str.length() && str.front() == '#') {
    if (str.length() == 4) {
      int64_t iv = ParseInteger(str.substr(1), 16);  // TODO: Stricter parsing.
      if (!(iv >= 0 && iv <= 0xfff)) {
        return {};
      } else {
        return SVGColorMake(
            static_cast<uint8_t>(((iv & 0xf00) >> 4) | ((iv & 0xf00) >> 8)),
            static_cast<uint8_t>((iv & 0xf0) | ((iv & 0xf0) >> 4)),
            static_cast<uint8_t>((iv & 0xf) | ((iv & 0xf) << 4)), 1.0);
      }
    } else if (str.length() == 7) {
      int64_t iv = ParseInteger(str.substr(1), 16);  // TODO: Stricter parsing.
      if (!(iv >= 0 && iv <= 0xffffff)) {
        /*
         *  Covers NaN.
         */
        return {};
      } else {
        return SVGColorMake(static_cast<uint8_t>((iv & 0xff0000) >> 16),
                            static_cast<uint8_t>((iv & 0xff00) >> 8),
                            static_cast<uint8_t>(iv & 0xff), 1.0);
      }
    }

    return {};
  }

  size_t op = str.find_first_of('('), ep = str.find_first_of(')');
  if (op != std::string::npos && ep + 1 == str.length()) {
    const std::string fname = str.substr(0, op);
    const std::vector<std::string> params =
        SplitString(str.substr(op + 1, ep - (op + 1)), ',');

    double alpha = 1.0;

    if (fname == "rgba" || fname == "rgb") {
      if (fname == "rgba") {
        if (params.size() != 4) {
          return {};
        }
        alpha = ParseSVGDouble(params.back());
      } else {
        if (params.size() != 3) {
          return {};
        }
      }

      return SVGColorMake(ParseSVGInteger(params[0]),  //
                          ParseSVGInteger(params[1]),  //
                          ParseSVGInteger(params[2]),  //
                          alpha);

    } else if (fname == "hsla" || fname == "hsl") {
      if (fname == "hsla") {
        if (params.size() != 4) {
          return {};
        }
        alpha = ParseSVGDouble(params.back());
      } else {
        if (params.size() != 3) {
          return {};
        }
      }

      double h = ParseDouble(params[0]) / 360.0;

      while (h < 0.0) {
        h++;
      }

      while (h > 1.0) {
        h--;
      }

      /*
       *  Note: According to the CSS spec s/l should only be percentages, but we
       *  don't bother and let double or percentage.
       */
      double s = ParseSVGDouble(params[1]);
      double l = ParseSVGDouble(params[2]);

      double m2 = l <= 0.5 ? l * (s + 1.0) : l + s - l * s;
      double m1 = l * 2.0 - m2;

      return SVGColorMake(
          ClampSVGByte(SVGHueToRGB(m1, m2, h + 1.0 / 3.0) * 255.0),  //
          ClampSVGByte(SVGHueToRGB(m1, m2, h) * 255.0),              //
          ClampSVGByte(SVGHueToRGB(m1, m2, h - 1.0 / 3.0) * 255.0),  //
          alpha);
    }
  }

  return {};
}

}  // namespace ib
}  // namespace rl
