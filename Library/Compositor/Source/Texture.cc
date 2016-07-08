// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Texture.h"

namespace rl {
namespace compositor {

Texture::Texture(image::Image image)
    : _image(image),
      _state(State::Compressed),
      _textureHandle(GL_NONE),
      _disposed(false) {}

Texture::~Texture() {
  RL_ASSERT_MSG(_disposed && _textureHandle == GL_NONE,
                "The texture must be disposed on the correct thread before it "
                "can be collected.");
}

Texture::Texture(const Texture& other) : Texture(other._image) {}

Texture::Texture(Texture&& other)
    : _image(std::move(other._image)),
      _imageResult(std::move(other._imageResult)),
      _state(other._state),
      _textureHandle(other._textureHandle),
      _disposed(other._disposed) {
  other._state = State::Error;
  other._textureHandle = GL_NONE;
  other._disposed = true;
}

void Texture::dispose() {
  if (_disposed) {
    return;
  }

  _disposed = true;

  if (_textureHandle != GL_NONE) {
    glDeleteTextures(1, &_textureHandle);
    RL_GLAssert("There must be no errors post texture disposal");
    _textureHandle = GL_NONE;
  }
}

Texture::State Texture::state() const {
  return _state;
}

bool Texture::uncompress() {
  if (_state == State::Uncompressed) {
    return true;
  }

  if (_state != State::Compressed) {
    return false;
  }

  auto result = _image.decode();

  if (result.wasSuccessful()) {
    _imageResult = std::move(result);
    _state = State::Uncompressed;
    return true;
  } else {
    _state = State::Error;
  }

  return false;
}

bool Texture::uploadToVRAM() {
  if (_state == State::UploadedToVRAM) {
    RL_ASSERT(_textureHandle != GL_NONE);
    return true;
  }

  if (_state != State::Uncompressed) {
    /*
     *  We need the image to be uncompressed before it can be uploaded to VRAM.
     */
    return false;
  }

  if (!_imageResult.wasSuccessful()) {
    /*
     *  The decode job itself failed.
     */

    _state = State::Error;
    return false;
  }

  const auto size = _imageResult.size();

  if (size.width <= 0.0 || size.height <= 0.0) {
    /*
     *  Invalid size.
     */

    _state = State::Error;
    return false;
  }

  /*
   *  Select the texture format.
   */

  GLint format = GL_NONE;

  switch (_imageResult.components()) {
    case image::Components::Grey:
      format = GL_LUMINANCE;
      break;
    case image::Components::GreyAlpha:
      format = GL_LUMINANCE_ALPHA;
      break;
    case image::Components::RGB:
      format = GL_RGB;
      break;
    case image::Components::RGBA:
      format = GL_RGB;
      break;
    default:
      _state = State::Error;
      return false;
  }

  /*
   *  Generate texture handles.
   */

  glGenTextures(1, &_textureHandle);
  glBindTexture(GL_TEXTURE_2D, _textureHandle);

  /*
   *  Upload texture data to VRAM.
   */
  glTexImage2D(GL_TEXTURE_2D,                      // target
               0,                                  // level
               format,                             // internalformat
               static_cast<GLsizei>(size.width),   // width
               static_cast<GLsizei>(size.height),  // height
               0,                                  // border
               format,                             // format
               GL_UNSIGNED_BYTE,                   // type
               _imageResult.allocation().data()    // data
               );

  RL_GLAssert("There must be no errors post texture upload.");

  _state = State::UploadedToVRAM;
  return true;
}

std::size_t Texture::Hash::operator()(const Texture& key) const {
  return image::Image::Hash()(key._image);
}

bool Texture::Equal::operator()(const Texture& lhs, const Texture& rhs) const {
  return image::Image::Equal()(lhs._image, rhs._image);
}

}  // namespace compositor
}  // namespace rl
