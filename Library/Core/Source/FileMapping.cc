/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/File.h>
#include <Core/FileMapping.h>
#include <sys/mman.h>

namespace rl {
namespace core {

FileMapping::FileMapping() : _mapping(nullptr), _size(0) {}

FileMapping::FileMapping(const FileHandle& handle)
    : _mapping(nullptr), _size(0) {
  File file(handle);

  if (file.isValid()) {
    setupMapping(handle, file.size());
  }
}

FileMapping::FileMapping(const FileHandle& handle, size_t size)
    : _mapping(nullptr), _size(0) {
  setupMapping(handle, size);
}

FileMapping::FileMapping(FileMapping&& other)
    : _mapping(other._mapping), _size(other._size) {
  other._mapping = nullptr;
  other._size = 0;
}

void FileMapping::setupMapping(const FileHandle& handle, size_t size) {
  if (!handle.isValid() || size == 0) {
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
