/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <hb-ft.h>
#include <memory>
#include <unordered_map>
#include "FTFace.h"
#include "HBFont.h"

namespace rl {
namespace type {

class FTLibrary {
 public:
  using Handle = FT_Library;

  FTLibrary();

  ~FTLibrary();

  bool registerFont(const core::URI& uri);

  HBFont fontForTypeface(const std::string& postscriptName);

 private:
  struct MutexFacePair {
    std::mutex mutex;
    FTFace face;

    MutexFacePair(FTFace aFace);
  };

  using FaceRegistry =
      std::unordered_map<std::string, std::unique_ptr<MutexFacePair>>;

  Handle _library;
  FaceRegistry _registry;

  RL_DISALLOW_COPY_AND_ASSIGN(FTLibrary);
};

}  // namespace type
}  // namespace rl
