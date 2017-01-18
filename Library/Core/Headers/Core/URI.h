// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_URL_H_
#define RADAR_CORE_URL_H_

#include <Core/Macros.h>
#include <memory>
#include <string>
#include <unordered_set>

namespace rl {
namespace core {

class URI {
 public:
  URI();

  URI(std::string source);

  URI(URI&& uri);

  URI& operator=(URI&& uri);

  ~URI();

  bool isValid() const;

  std::string fragment() const;

  std::string filesystemRepresentation() const;

  std::string toString() const;

  URI append(const URI& component) const;

  bool normalize();

 private:
  using StringComponents = std::unordered_set<std::shared_ptr<std::string>>;

  StringComponents _sources;
  void* _state;
  bool _valid;

  URI(void* state);

  std::string unixFilesystemRepresentation(const std::string& uriString) const;
  std::string windowsFilesystemRepresentation(
      const std::string& uriString) const;

  void collectState();

  RL_DISALLOW_COPY_AND_ASSIGN(URI);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_URL_H_
