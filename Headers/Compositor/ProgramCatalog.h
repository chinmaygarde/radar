// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PROGRAMCATALOG__
#define __RADARLOVE_COMPOSITOR_PROGRAMCATALOG__

#include "Compositor/Program.h"

#include <algorithm>
#include <map>
#include <memory>

namespace rl {
class ProgramCatalog {
 public:
  enum Type {
    None = 0,
    BasicPrimitve,
    TexturedPrimitive,
  };

  ProgramCatalog();

  /**
   *  Must be called before catalog items are about to be used
   */
  void startUsing();

  typedef Program const* ProgramRef;
  /**
   *  Switches the current program to the one specified. If the same program is
   *  specified consecutively, this operation is a no-op.
   *
   *  @param type the new program type
   *  @return the selected program
   */
  ProgramRef useProgramType(Type type);

  /**
   *  Must be called after items in the catalog have been used
   */
  void stopUsing();

 private:
  bool _prepared;
  std::pair<Type, ProgramRef> _current;
  std::map<Type, std::unique_ptr<Program>> _catalog;

  void prepareIfNecessary();

  DISALLOW_COPY_AND_ASSIGN(ProgramCatalog);
};

/*
 *  The program to be used for drawing basic colored primitives
 */
class BasicPrimitiveProgram : public Program {
 public:
  BasicPrimitiveProgram();

  unsigned int modelViewProjectionUniform() const {
    return _modelViewProjectionUniform;
  }

  unsigned int contentColorUniform() const { return _contentColorUniform; };

  unsigned int sizeUniform() const { return _sizeUniform; };

 protected:
  virtual void onLinkSuccess() override;

 private:
  unsigned int _modelViewProjectionUniform;
  unsigned int _contentColorUniform;
  unsigned int _sizeUniform;

  DISALLOW_COPY_AND_ASSIGN(BasicPrimitiveProgram);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PROGRAMCATALOG__) */
