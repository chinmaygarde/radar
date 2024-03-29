/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include <GLFoundation/GLFoundation.h>
#include <Image/Image.h>

namespace rl {
namespace compositor {

class Texture {
 public:
  enum class State {
    Compressed,      //
    Uncompressed,    //
    UploadedToVRAM,  //
    Error,           //
  };

  Texture(image::Image image);

  ~Texture();

  Texture(Texture&&);

  bool uncompress();

  bool uploadToVRAM();

  State state() const;

  bool bind(GLint samplerUniform, size_t activeIndex = 0) const;

  struct Hash {
    std::size_t operator()(const Texture& key) const;
  };

  struct Equal {
    bool operator()(const Texture& lhs, const Texture& rhs) const;
  };

 private:
  RL_DEBUG_THREAD_GUARD(_guard);
  image::Image _image;
  image::ImageResult _imageResult;
  State _state;
  GLuint _textureHandle;

  RL_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace compositor
}  // namespace rl
