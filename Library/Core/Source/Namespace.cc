// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Namespace.h>

#include <Core/Message.h>

#include <sstream>

namespace rl {
namespace core {

Name::Name() : _handle(nullptr), _ns(nullptr) {}

Name::Name(Namespace& ns) : Name(DeadHandle, &ns) {}

Name::Name(Namespace* ns) : Name(DeadHandle, ns) {}

Name::Name(Handle counterpart, Namespace& ns) : Name(counterpart, &ns) {}

Name::Name(Handle counterpart, Namespace* ns)
    : _handle(ns == nullptr ? std::make_shared<Handle>(counterpart)
                            : ns->createHandle(counterpart)),
      _ns(ns) {}

bool Name::operator==(const Name& other) const {
  return _handle == other._handle;
}

bool Name::operator!=(const Name& other) const {
  return _handle != other._handle;
}

bool Name::serialize(Message& message) const {
  auto handle = _handle ? *_handle : DeadHandle;
  return message.encode(handle);
}

bool Name::deserialize(Message& message, Namespace* ns) {
  auto decodedHandle = DeadHandle;

  if (message.decode(decodedHandle, ns)) {
    decodeRaw(decodedHandle, ns);
    return true;
  }

  return false;
}

std::string Name::toString() const {
  return std::to_string(_handle ? *_handle : DeadHandle);
}

void Name::fromString(const std::string& str, Namespace* ns) {
  std::stringstream stream(str);
  auto newHandle = DeadHandle;
  if (stream >> newHandle) {
    decodeRaw(newHandle, ns);
  }
}

void Name::decodeRaw(Handle handle, Namespace* ns) {
  if (handle == DeadHandle) {
    return;
  }

  _handle = ns == nullptr ? nullptr : ns->createHandle(handle);
  _ns = ns;
}

Namespace* Name::ns() const {
  return _ns;
}

Name::HandleRef Name::handle() const {
  return _handle;
}

bool Name::isDead() const {
  return _handle == nullptr;
}

size_t Name::hash() const {
  return std::hash<HandleRef>()(_handle);
}

HandleCollector::HandleCollector(Namespace& ns) : _ns(&ns) {}

void HandleCollector::operator()(Name::Handle* handle) const {
  _ns->destroy(*handle);
  delete handle;
}

Namespace::Namespace() : _last(DeadHandle) {}

Namespace::~Namespace() {
  std::lock_guard<std::mutex> lock(_mapsMutex);
  RL_ASSERT_MSG(_counterpartToLocalMap.size() == 0,
                "The namespace is being collected but some members still have "
                "pending references. Names from this namespace are being "
                "leaked or have name lifecycles longer that their namespace");
}

static inline Name::HandleRef HandleRefMake(Name::Handle handle,
                                            Namespace& ns) {
  RL_ASSERT(handle != DeadHandle);
  return std::shared_ptr<Name::Handle>(new Name::Handle(handle),
                                       HandleCollector(ns));
}

Name::HandleRef Namespace::createHandle(Name::Handle counterpart) {
  if (counterpart == DeadHandle) {
    /*
     *  The vast majority of handles will be for locally used names, in such
     *  cases, there is no need to supply a custom deleter (with the associated
     *  overhead, such as it is)
     */
    return std::make_shared<Name::Handle>(++_last);
  }

  std::lock_guard<std::mutex> lock(_mapsMutex);

  /*
   *  Create or return a new shared reference to a local handle that is mapped
   *  to this remote counterpart
   */

  auto localFound = _counterpartToLocalMap.find(counterpart);

  if (localFound != _counterpartToLocalMap.end()) {
    auto locked = localFound->second.lock();
    RL_ASSERT_MSG(locked,
                  "Must be able to lock a new referece to the weak pointer "
                  "maintained in the table");
    return locked;
  }

  auto local = ++_last;
  auto localRef = HandleRefMake(local, *this);

  /*
   *  Setup a reverse map between the local handle and its counterpart for
   *  easy deletion.
   */

  _counterpartToLocalMap[counterpart] = localRef;
  auto result = _localToCounterpartMap.emplace(local, counterpart);
  RL_ASSERT(result.second);

  return localRef;
}

void Namespace::destroy(Name::Handle local) {
  RL_ASSERT(local != DeadHandle);

  std::lock_guard<std::mutex> lock(_mapsMutex);

  auto mappingFound = _localToCounterpartMap.find(local);

  /*
   *  Since we don't supply a custom deleter for local names, we should never
   *  get into a situation where a mapped named dies and we don't find it in
   *  the table.
   */
  RL_ASSERT(mappingFound != _localToCounterpartMap.end());

  auto erased = _counterpartToLocalMap.erase(mappingFound->second);
  RL_ASSERT(erased == 1);

  _localToCounterpartMap.erase(mappingFound);
}

size_t Namespace::mappedNamesCount() const {
  std::lock_guard<std::mutex> lock(_mapsMutex);
  return _localToCounterpartMap.size();
}

}  // namespace core
}  // namespace rl