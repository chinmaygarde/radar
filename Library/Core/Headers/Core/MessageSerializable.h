/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Utilities.h>

namespace rl {
namespace core {

class Message;
class Namespace;

class MessageSerializable {
 public:
  virtual bool serialize(Message& message) const = 0;

  virtual bool deserialize(Message& message, Namespace* ns) = 0;

  using EncodedSize = size_t;
};

}  // namespace core
}  // namespace rl
