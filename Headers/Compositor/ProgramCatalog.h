// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PROGRAMCATALOG__
#define __RADARLOVE_COMPOSITOR_PROGRAMCATALOG__

#include "Compositor/Program.h"

#include <map>
#include <memory>

namespace rl {
class ProgramCatalog {
 public:
  enum ProgramType {
    ProgramTypeNone = 0,
    ProgramTypeBasicPrimitve,
    ProgramTypeTexturedPrimitive,
  };

  ProgramCatalog();

  /**
   *  Must be called before catalog items are about to be used
   */
  void startUsing();

  /**
   *  Switches the current program to the one specified. If the same program is
   *  specified consecutively, this operation is a no-op.
   *
   *  @param type the new program type
   */
  bool useProgramType(ProgramType type);

  /**
   *  Must be called after items in the catalog have been used
   */
  void stopUsing();

 private:
  bool _prepared;
  ProgramType _currentProgramType;
  std::map<ProgramType, std::unique_ptr<Program>> _catalog;

  void prepareIfNecessary();

  DISALLOW_COPY_AND_ASSIGN(ProgramCatalog);
};

class BasicPrimitiveProgram : public Program {
 public:
  BasicPrimitiveProgram();

 private:
  DISALLOW_COPY_AND_ASSIGN(BasicPrimitiveProgram);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PROGRAMCATALOG__) */
