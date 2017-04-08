/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_RIDL_DRIVER_H_
#define RL_RIDL_DRIVER_H_

#include <Core/Macros.h>
#include <string>
#include "location.hh"

namespace rl {

class Driver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  Driver();

  ~Driver();

  ParserResult parse(const std::string& text);

  rl::location location;

 private:
  friend class Parser;

  void error(rl::location loc, const std::string& message);

  RL_DISALLOW_COPY_AND_ASSIGN(Driver);
};

}  // namespace rl

#endif  // RL_RIDL_DRIVER_H_
