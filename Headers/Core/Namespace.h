// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/Serializable.h>

#include <atomic>
#include <mutex>
#include <unordered_map>

namespace rl {
namespace core {

class Namespace;

class Name : public Serializable {
 public:
  using Handle = uint32_t;

  Name(Namespace* ns);

  Name(Handle handle, Namespace* ns);

  ~Name();

  Handle handle() const;

  Namespace* ns() const;

  size_t hash() const;

  bool serialize(Message& message) const override;

  bool deserialize(Message& message, Namespace* ns) override;

  constexpr bool operator==(const Name& other) const {
    return _handle == other._handle;
  }

  constexpr bool operator!=(const Name& other) const {
    return _handle != other._handle;
  }

  constexpr bool operator>(const Name& other) const {
    return _handle > other._handle;
  }

  constexpr bool operator<(const Name& other) const {
    return _handle < other._handle;
  }

  struct Hash {
    size_t operator()(const Name& name) const {
      return std::hash<Handle>()(name._handle);
    }
  };

  struct Equal {
    bool operator()(const Name& lhs, const Name& rhs) const {
      return lhs._handle == rhs._handle;
    }
  };

 private:
  Handle _handle;
  Namespace* _ns;
};

static const Name::Handle DeadHandle = 0;

class Namespace {
 public:
  Namespace();

  Name::Handle createHandle(Name::Handle counterpart = DeadHandle);

  Name create(Name::Handle counterpart = DeadHandle);

  void destroy(Name::Handle name);

  size_t mappedNamesCount() const;

 private:
  using NameMap = std::unordered_map<Name::Handle, Name::Handle>;

  std::atomic<Name::Handle> _last;
  mutable std::mutex _lock;

  NameMap _localToCounterpartMap;
  NameMap _counterpartToLocalMap;

  RL_DISALLOW_COPY_AND_ASSIGN(Namespace);
};

extern const Name DeadName;

}  // namespace core
}  // namespace rl
