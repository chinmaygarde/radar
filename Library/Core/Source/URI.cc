// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/URI.h>
#include <Core/Allocation.h>
#include <uriparser/Uri.h>
#include <string>

#define _uri (reinterpret_cast<UriUriA*>(_state))

namespace rl {
namespace core {

URI::URI() : _state(nullptr), _valid(false) {}

URI::URI(void* state) : _state(state), _valid(state != nullptr) {}

URI::URI(std::string source)
    : _state(calloc(1, sizeof(UriUriA))), _valid(false) {
  UriParserStateA parser;

  parser.uri = _uri;

  auto stringBacking = std::make_shared<std::string>(std::move(source));
  _valid = uriParseUriA(&parser, stringBacking->data()) == URI_SUCCESS;

  if (_valid) {
    _sources.insert(std::move(stringBacking));
  }
}

URI::URI(URI&& other)
    : _sources(std::move(other._sources)),
      _state(other._state),
      _valid(other._valid) {
  other._state = nullptr;
  other._valid = false;
}

URI::~URI() {
  if (_state != nullptr) {
    uriFreeUriMembersA(_uri);
    free(_state);
  }
}

bool URI::isValid() const {
  return _valid;
}

std::string URI::toString() const {
  if (!_valid) {
    return "";
  }

  int maxChars = 0;

  if (uriToStringCharsRequiredA(_uri, &maxChars) != 0) {
    return "";
  }

  if (maxChars == 0) {
    return "";
  }

  /*
   *  For the terminator.
   */
  maxChars++;

  Allocation allocation;

  if (!allocation.resize(maxChars)) {
    return "";
  }

  int written = 0;

  if (uriToStringA(reinterpret_cast<char*>(allocation.data()), _uri, maxChars,
                   &written) != 0) {
    return "";
  }

  return reinterpret_cast<char*>(allocation.data());
}

std::string URI::filesystemRepresentation() const {
  if (!_valid) {
    return "";
  }

  auto path = toString();

#ifdef RL_OS_WINDOWS
  return windowsFilesystemRepresentation(path);
#else
  return unixFilesystemRepresentation(path);
#endif
}

URI URI::append(const URI& component) const {
  if (!_valid || !component._valid) {
    return {};
  }

  UriUriA* absoluteDest =
      reinterpret_cast<UriUriA*>(calloc(1, sizeof(UriUriA)));
  UriUriA* relativeSource = reinterpret_cast<UriUriA*>(component._state);
  UriUriA* absoluteBase = reinterpret_cast<UriUriA*>(_state);

  if (uriAddBaseUriExA(absoluteDest, relativeSource, absoluteBase,
                       URI_RESOLVE_IDENTICAL_SCHEME_COMPAT) != 0) {
    uriFreeUriMembersA(absoluteDest);
    free(absoluteDest);
    return {};
  }

  auto resultant = URI{absoluteDest};

  for (auto source : _sources) {
    resultant._sources.insert(source);
  }

  for (auto source : component._sources) {
    resultant._sources.insert(source);
  }

  return resultant;
}

bool URI::normalize() {
  if (!_valid) {
    return false;
  }

  return uriNormalizeSyntaxA(_uri) == 0;
}

std::string URI::unixFilesystemRepresentation(const std::string& path) const {
  /*
   *  From Documentation:
   *  The destination buffer must be large enough to hold
   *  7 + 3 * len(filename) + 1 characters in case
   *  of an absolute filename or 3 * len(filename) + 1 in case
   *  of a relative filename.
   */
  const int destinationBufferSize = _uri->absolutePath == URI_TRUE
                                        ? (7 + (3 * path.size()) + 1)
                                        : ((3 * path.size()) + 1);

  Allocation allocation;

  if (!allocation.resize(destinationBufferSize + 1)) {
    return "";
  }

  if (uriUriStringToUnixFilenameA(
          path.c_str(), reinterpret_cast<char*>(allocation.data())) != 0) {
    return "";
  }

  return reinterpret_cast<char*>(allocation.data());
}

std::string URI::windowsFilesystemRepresentation(
    const std::string& path) const {
  /*
   *  The destination buffer must be large enough to hold 8 + 3 * len(filename)
   *  + 1 characters in case of an absolute filename or 3 * len(filename) + 1 in
   *  case of a relative filename.
   */
  const int destinationBufferSize = _uri->absolutePath == URI_TRUE
                                        ? (8 + (3 * path.size()) + 1)
                                        : ((3 * path.size()) + 1);

  Allocation allocation;

  if (!allocation.resize(destinationBufferSize + 1)) {
    return "";
  }

  if (uriUriStringToWindowsFilenameA(
          path.c_str(), reinterpret_cast<char*>(allocation.data())) != 0) {
    return "";
  }

  return reinterpret_cast<char*>(allocation.data());
}

}  // namespace core
}  // namespace rl
