/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Platform.h>

#if RL_OS_MAC

namespace rl {
namespace core {

void ObjCRetain(void*);

void ObjCRelease(void*);

template <class T>
class ScopedNSObject {
 public:
  using ObjCClass = T;

  ScopedNSObject() : _object(nullptr) {}

  ScopedNSObject(ObjCClass* object) : _object(object) {}

  ~ScopedNSObject() { ObjCRelease(_object); }

  operator bool() const { return _object != nullptr; }

  operator ObjCClass*() const { return _object; }

  ScopedNSObject& operator=(const ScopedNSObject<ObjCClass>& that) {
    ObjCRelease(_object);
    ObjCRetain(that._object);
    _object = that._object;
    return *this;
  }

  ScopedNSObject& operator=(ScopedNSObject<ObjCClass>&& that) {
    ObjCRelease(_object);
    _object = that._object;
    that._object = nullptr;
    return *this;
  }

 private:
  ObjCClass* _object;

  RL_DISALLOW_COPY(ScopedNSObject);
};

}  // namespace core
}  // namespace rl

#endif  // RL_OS_MAC
