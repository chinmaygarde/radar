/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileMapping.h>
#include <Core/Macros.h>
#include <Typography/Types.h>

namespace rl {
namespace type {

class TypographyContext {
 public:
  static TypographyContext& SharedContext();

  bool isValid() const;

  icu::BreakIterator* GetBreakIteratorForThread();

 private:
  core::FileMapping _icuDataMapping;
  bool _valid = false;

  TypographyContext();

  ~TypographyContext() = delete;

  RL_DISALLOW_COPY_AND_ASSIGN(TypographyContext);
};

}  // namespace type
}  // namespace rl
