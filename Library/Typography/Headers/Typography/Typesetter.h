/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <Typography/AttributedString.h>
#include <Typography/FontLibrary.h>
#include <Typography/ShapedTextRun.h>
#include <Typography/TextRun.h>
#include <Typography/TypeFrame.h>
#include <vector>

namespace rl {
namespace type {

class Typesetter {
 public:
  Typesetter(AttributedString string);

  ~Typesetter();

  bool isValid() const;

  std::vector<ShapedTextRun> createShapedRuns(const FontLibrary& library) const;

  const TextRuns& runs() const;

 private:
  AttributedString _string;
  TextRuns _runs;
  bool _valid = false;

  RL_DISALLOW_COPY_AND_ASSIGN(Typesetter);
};

}  // namespace type
}  // namespace rl
