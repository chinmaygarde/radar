/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_
#define RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_

#include <Core/Macros.h>
#include <string>
#include "SVGXFormElement.h"
#include "location.hh"

namespace rl {

class SVGXFormDriver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  SVGXFormDriver();

  ~SVGXFormDriver();

  ParserResult parse(const std::string& text);

  rl::location location;

  const geom::Matrix& transformation() const;

 private:
  friend class SVGXFormParser;

  geom::Matrix _transformation;

  void error(rl::location loc, const std::string& message);

  template <class T>
  void visitElement(const T& element) {
    _transformation = _transformation * element.effectiveTransformation();
  }

  RL_DISALLOW_COPY_AND_ASSIGN(SVGXFormDriver);
};

}  // namespace rl

#endif  // RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_
