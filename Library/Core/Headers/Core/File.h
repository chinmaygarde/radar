/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileHandle.h>
#include <Core/FileMapping.h>
#include <Core/Macros.h>
#include <Core/URI.h>
#include <sys/stat.h>

namespace rl {
namespace core {

class File {
 public:
  static bool SetAsWorkingDirectory(const URI& uri);

  enum class Type {
    Unknown,
    File,
    Directory,
    CharacterDevice,
    BlockDevice,
    NamedPipe,
    SymbolicLink,
    Socket,
  };

  File(const FileHandle& handle);

  File(const URI& uri);

  File(File&& file);

  ~File();

  Type type() const;

  size_t size() const;

  bool isValid() const;

 private:
  struct stat _stat;
  bool _valid;

  RL_DISALLOW_COPY_AND_ASSIGN(File);
};

}  // namespace core
}  // namespace rl
