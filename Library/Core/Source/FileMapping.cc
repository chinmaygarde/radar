// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/FileMapping.h>

#include <sys/mman.h>

namespace rl {
namespace core {

FileMapping::FileMapping() : _mapping(nullptr), _size(0) {}

FileMapping::FileMapping(const FileHandle& handle, size_t size)
    : _mapping(nullptr), _size(0) {
  if (size == 0) {
    return;
  }

  auto mapping =
      ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, handle.handle(), 0);

  if (mapping == MAP_FAILED) {
    return;
  }

  _mapping = reinterpret_cast<uint8_t*>(mapping);
  _size = size;
}

FileMapping::FileMapping(FileMapping&& other)
    : _mapping(other._mapping), _size(other._size) {
  other._mapping = nullptr;
  other._size = 0;
}

uint8_t* FileMapping::mapping() const {
  return _mapping;
}

size_t FileMapping::size() const {
  return _size;
}

FileMapping::~FileMapping() {
  if (_mapping != nullptr) {
    RL_CHECK(::munmap(_mapping, _size));
  }

  _mapping = nullptr;
  _size = 0;
}

}  // namespace core
}  // namespace rl
