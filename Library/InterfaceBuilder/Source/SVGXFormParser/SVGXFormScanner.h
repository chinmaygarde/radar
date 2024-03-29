/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_SVGXFORMPARSER_SVGXFORMSCANNER_H_
#define RL_SVGXFORMPARSER_SVGXFORMSCANNER_H_

#include <Core/Macros.h>
#include "RLDecls.h"
#include "SVGXFormLexer.h"

namespace rl {

class SVGXFormScanner {
 public:
  SVGXFormScanner(const std::string& text);

  ~SVGXFormScanner();

  bool isReady() const;

  yyscan_t handle();

 private:
  yyscan_t _scanner;
  YY_BUFFER_STATE _buffer;
  bool _isReady;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGXFormScanner);
};

}  // namespace rl

#endif  // RL_SVGXFORMPARSER_SVGXFORMSCANNER_H_
