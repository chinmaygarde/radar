// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_RIDL_SCANNER_H_
#define RL_RIDL_SCANNER_H_

#include <Core/Macros.h>
#include "Lexer.h"
#include "RLDecls.h"

namespace rl {

class Scanner {
 public:
  Scanner(const std::string& text);

  ~Scanner();

  bool isReady() const;

  yyscan_t handle();

 private:
  yyscan_t _scanner;
  YY_BUFFER_STATE _buffer;

  bool _isReady;

  RL_DISALLOW_COPY_AND_ASSIGN(Scanner);
};

}  // namespace rl

#endif  // RL_RIDL_SCANNER_H_
