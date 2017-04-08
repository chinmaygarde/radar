/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/WorkQueue.h>
#include <Image/Image.h>
#include "TextureCache.h"

namespace rl {
namespace compositor {

class TextureTransaction {
 public:
  TextureTransaction();

  ~TextureTransaction();

  RL_WARN_UNUSED_RESULT
  std::shared_ptr<Texture> registerTexture(std::shared_ptr<Texture> texture);

  bool commit(core::WorkQueue* workqueue);

 private:
  TextureCache _textureCache;
  std::vector<std::shared_ptr<Texture>> _compressedTextures;
  std::vector<std::shared_ptr<Texture>> _uncompressedTextures;

  RL_DISALLOW_COPY_AND_ASSIGN(TextureTransaction);
};

}  // namespace compositor
}  // namespace rl
