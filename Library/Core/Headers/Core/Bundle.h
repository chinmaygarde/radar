/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>

namespace rl {
namespace core {

class Bundle {
 public:
  Bundle();

  Bundle(URI baseURI);

  URI uriForResource(URI uri) const;

 private:
  URI _baseURI;

  void setupWithBaseURI(URI baseURI);

  RL_DISALLOW_COPY_AND_ASSIGN(Bundle);
};

}  // namespace core
}  // namespace rl
