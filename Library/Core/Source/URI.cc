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

URI::URI(const std::string& string)
    : _state(calloc(1, sizeof(UriUriA))), _valid(false) {
  UriParserStateA parser;
  parser.uri = _uri;
  _valid = uriParseUriA(&parser, string.data()) == URI_SUCCESS;
}

URI::~URI() {
  uriFreeUriMembersA(_uri);
  free(_state);
}

bool URI::isValid() const {
  return _valid;
}

std::string URI::toString() const {
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
  auto path = toString();

#ifdef RL_OS_WINDOWS
  return windowsFilesystemRepresentation(path);
#else
  return unixFilesystemRepresentation(path);
#endif
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

  if (!allocation.resize(destinationBufferSize)) {
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

  if (!allocation.resize(destinationBufferSize)) {
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