// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_TEXTURE_H_
#define RADAR_COMPOSITOR_TEXTURE_H_

#include <Core/Macros.h>

#include <Image/Image.h>

#include "OpenGL.h"

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

  void dispose();

 private:
  image::Image _image;
  image::ImageResult _imageResult;
  State _state;
  GLuint _textureHandle;
  bool _disposed;

  RL_DISALLOW_COPY_AND_ASSIGN(Texture);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_TEXTURE_H_
