// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_URL_H_
#define RADAR_CORE_URL_H_

#include <Core/Macros.h>

namespace rl {
namespace core {

class URI {
 public:
  URI(const std::string& string);

  ~URI();

  bool isValid() const;

  std::string filesystemRepresentation() const;

  std::string toString() const;

 private:
  void* _state;
  bool _valid;

  std::string unixFilesystemRepresentation(const std::string& uriString) const;
  std::string windowsFilesystemRepresentation(
      const std::string& uriString) const;

  RL_DISALLOW_COPY_AND_ASSIGN(URI);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_URL_H_
