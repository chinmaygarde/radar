// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Bundle.h>
#include <Core/Utilities.h>

#include <string>
#include <sstream>

namespace rl {
namespace core {

Bundle::Bundle() {
  setupWithBaseURI(GetResourcesPath());
}

Bundle::Bundle(URI baseURI) {
  setupWithBaseURI(std::move(baseURI));
}

void Bundle::setupWithBaseURI(URI baseURI) {
  if (!baseURI.isValid()) {
    return;
  }

  _baseURI = std::move(baseURI);
}

URI Bundle::uriForResource(URI uri) const {
  if (!_baseURI.isValid()) {
    return URI{};
  }

  return _baseURI.append(uri);
}

}  // namespace core
}  // namespace rl
