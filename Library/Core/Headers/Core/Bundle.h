// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_BUNDLE_H_
#define RADAR_CORE_BUNDLE_H_

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Core/File.h>

namespace rl {
namespace core {

class Bundle {
 public:
  Bundle();

  File fileForURI(const URI& url) const;

  virtual ~Bundle();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Bundle);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_BUNDLE_H_
