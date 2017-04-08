/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Bundle.h>
#include <Core/Utilities.h>
#include <sstream>
#include <string>

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
