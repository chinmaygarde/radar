/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/RawAttachment.h>
#include <Core/URI.h>

namespace rl {
namespace core {

class FileHandle final : public Attachment {
 public:
  enum class AccessType {
    Read,
    Write,
    ReadWrite,
  };

  FileHandle();

  FileHandle(Handle handle);

  FileHandle(RawAttachment attachment);

  FileHandle(const URI& uri);

  FileHandle(const URI& uri, AccessType type, bool createIfNecessary);

  FileHandle(FileHandle&& file);

  ~FileHandle();

  bool isValid() const;

  Handle handle() const override;

  MessageHandle messageHandle() const override;

 private:
  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(FileHandle);
};

}  // namespace core
}  // namespace rl
