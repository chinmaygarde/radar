/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "TextureTransaction.h"

namespace rl {
namespace compositor {

TextureTransaction::TextureTransaction() {}

TextureTransaction::~TextureTransaction() = default;

std::shared_ptr<Texture> TextureTransaction::registerTexture(
    std::shared_ptr<Texture> texture) {
  auto cachedTexture = _textureCache.registerTexture(texture);

  RL_ASSERT(cachedTexture != nullptr);

  switch (cachedTexture->state()) {
    case Texture::State::Compressed:
      _compressedTextures.emplace_back(texture);
      break;
    case Texture::State::Uncompressed:
      _uncompressedTextures.emplace_back(texture);
      break;
    default:
      break;
  }

  return cachedTexture;
}

bool TextureTransaction::commit(core::WorkQueue* workqueue) {
  if (_compressedTextures.size() == 0 && _uncompressedTextures.size() == 0) {
    return true;
  }

  bool result = true;

  core::Latch latch(_compressedTextures.size());

  /*
   *  Enqueue all the compressed textures onto the workqueue.
   */

  for (auto& texture : _compressedTextures) {
    core::WorkQueue::WorkItem workItem = [texture, &latch]() {
      texture->uncompress();
      latch.countDown();
    };

    if (workqueue != nullptr) {
      RL_UNUSED(workqueue->dispatch(workItem));
    } else {
      workItem();
    }
  }

  /*
   *  While the textures are being decompressed on the workqueue, upload the
   *  uncompressed textures.
   */
  for (auto& texture : _uncompressedTextures) {
    result &= texture->uploadToVRAM();
  }

  /*
   *  This thread has nothing more to do that to wait for the textures to be
   *  decompressed. Wait for that and upload the when ready.
   */
  latch.wait();

  for (auto& texture : _compressedTextures /* that are now uncompressed */) {
    result &= texture->uploadToVRAM();
  }

  return result;
}

}  // namespace compositor
}  // namespace rl
