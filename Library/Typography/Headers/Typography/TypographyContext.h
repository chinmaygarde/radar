/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileMapping.h>
#include <Core/Macros.h>
#include <Typography/Types.h>
#include <atomic>

namespace rl {
namespace type {

class TypographyContext {
 public:
  static TypographyContext& SharedContext();

  bool isValid() const;

  icu::BreakIterator* breakIteratorForThread();

 private:
  core::FileMapping _icuDataMapping;
  std::atomic_bool _valid;

  TypographyContext();

  ~TypographyContext() = delete;

  RL_DISALLOW_COPY_AND_ASSIGN(TypographyContext);
};

}  // namespace type
}  // namespace rl
