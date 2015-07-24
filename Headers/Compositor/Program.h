// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_PROGRAM__
#define __RADARLOVE_COMPOSITOR_PROGRAM__

#include "Core/Base.h"

#include <memory>
#include <vector>
#include <string>

namespace rl {
class Program {
 public:
  Program(std::vector<std::string> attributes,
          std::string vertexShader,
          std::string fragmentShader);
  virtual ~Program();

  void linkIfNecessary();
  bool use();

  unsigned int indexForAttribute(const std::string& attribute);
  unsigned int indexForUniform(const std::string& uniform);

 private:
  std::vector<std::string> _knownAttributes;
  std::string _vertexShader;
  std::string _fragmentShader;

  bool _linkingComplete;
  unsigned int _program;

  unsigned int registerNamedAttribute();

  DISALLOW_COPY_AND_ASSIGN(Program);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_PROGRAM__) */
