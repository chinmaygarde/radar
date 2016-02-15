// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_NAMESPACE_H_
#define RADARLOVE_CORE_NAMESPACE_H_

#include <Core/Macros.h>
#include <Core/MessageSerializable.h>

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

namespace rl {
namespace core {

class Namespace;

class Name : public MessageSerializable {
 public:
  using Handle = uint32_t;
  using HandleRef = std::shared_ptr<Handle>;

  Name();

  Name(Namespace* ns);

  Name(Namespace& ns);

  Name(Handle counterpart, Namespace* ns);

  Name(Handle counterpart, Namespace& ns);

  HandleRef handle() const;

  Namespace* ns() const;

  bool isDead() const;

  size_t hash() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message, Namespace* ns) override;

  std::string toString() const;

  void fromString(const std::string& str, Namespace* ns);

  bool operator==(const Name& other) const;

  bool operator!=(const Name& other) const;

  bool operator>(const Name& other) const { return _handle > other._handle; }

  bool operator<(const Name& other) const { return _handle < other._handle; }

  struct Hash {
    size_t operator()(const Name& name) const {
      return std::hash<HandleRef>()(name._handle);
    }
  };

  struct Equal {
    bool operator()(const Name& lhs, const Name& rhs) const {
      return lhs._handle == rhs._handle;
    }
  };

 private:
  HandleRef _handle;
  Namespace* _ns;

  void decodeRaw(Handle handle, Namespace* ns);
};

static const Name::Handle DeadHandle = 0;

class HandleCollector {
 public:
  HandleCollector(Namespace& ns);

  void operator()(Name::Handle* handle) const;

 private:
  Namespace* _ns;
};

class Namespace {
 public:
  Namespace();

  ~Namespace();

  size_t mappedNamesCount() const;

 private:
  using HandleMap = std::unordered_map<Name::Handle, Name::Handle>;
  using HandleRefMap =
      std::unordered_map<Name::Handle, std::weak_ptr<Name::Handle>>;

  friend class Name;
  friend class HandleCollector;

  std::atomic<Name::Handle> _last;

  mutable std::mutex _mapsMutex;
  HandleRefMap _counterpartToLocalMap;
  HandleMap _localToCounterpartMap;

  Name::HandleRef createHandle(Name::Handle counterpart);
  void destroy(Name::Handle name);

  RL_DISALLOW_COPY_AND_ASSIGN(Namespace);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_NAMESPACE_H_
