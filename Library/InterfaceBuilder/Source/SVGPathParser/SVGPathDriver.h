/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_SVGPATHPARSER_SVGPATHDRIVER_H_
#define RL_SVGPATHPARSER_SVGPATHDRIVER_H_

#include <Core/Macros.h>
#include <Geometry/PathBuilder.h>
#include <Geometry/Point.h>
#include <string>
#include "SVGElement.h"
#include "location.hh"

namespace rl {

class SVGPathDriver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  SVGPathDriver();

  ~SVGPathDriver();

  ParserResult parse(const std::string& text);

  const geom::PathBuilder& pathBuilder() const;

  rl::location location;

 private:
  friend class SVGPathParser;

  geom::PathBuilder _pathBuilder;

  void error(rl::location loc, const std::string& message);

  template <class T>
  void visitElement(const T& element) {
    if (element.isValid()) {
      processElement(element);
    }
  }

  template <class T>
  void processElement(const T& element);

  RL_DISALLOW_COPY_AND_ASSIGN(SVGPathDriver);
};

}  // namespace rl

#endif  // RL_SVGPATHPARSER_SVGPATHDRIVER_H_
