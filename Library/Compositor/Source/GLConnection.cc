/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "GLConnection.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

namespace rl {
namespace compositor {

static std::string GLGetString(GLenum name) {
  auto string = reinterpret_cast<const char*>(glGetString(name));
  return string != nullptr ? string : "";
}

static GLConnection::Version GLGetVersion(GLenum name) {
  GLConnection::Version version;

  auto versionString = GLGetString(name);

  if (versionString.length() == 0) {
    return version;
  }

  {
    // Check for the GLSL ES prefix.
    const std::string glslesPrefix("OpenGL ES GLSL ES ");
    if (versionString.compare(0, glslesPrefix.length(), glslesPrefix) == 0) {
      version.isES = true;
      versionString = versionString.substr(glslesPrefix.length());
    }
  }

  {
    // Check for the GL ES prefix.
    const std::string glesPrefix("OpenGL ES ");
    if (versionString.compare(0, glesPrefix.length(), glesPrefix) == 0) {
      version.isES = true;
      versionString = versionString.substr(glesPrefix.length());
    }
  }

  std::istringstream stream(versionString);

  for (size_t i = 0; i < 3; i++) {
    size_t item = 0;
    if (stream >> item) {
      version.items[i] = item;

      if (stream.peek() == ' ') {
        stream.ignore(1);  // space
        stream >> version.vendorString;
        break;
      } else {
        stream.ignore(1);  // dot
      }
    } else {
      break;
    }
  }

  return version;
}

static std::string VersionToString(GLConnection::Version version) {
  if (version.major == 0 && version.minor == 0 && version.release == 0) {
    return "Unknown";
  }

  std::stringstream stream;

  stream << version.major << "." << version.minor;

  if (version.release != 0) {
    stream << "." << version.release;
  }

  if (version.vendorString.size() != 0) {
    stream << " " << version.vendorString;
  }

  if (version.isES) {
    stream << " ES";
  }

  return stream.str();
}

GLConnection::GLConnection()
    : _vendor(GLGetString(GL_VENDOR)),
      _renderer(GLGetString(GL_RENDERER)),
      _version(GLGetVersion(GL_VERSION)),
      _shadingLanguageVersion(GLGetVersion(GL_SHADING_LANGUAGE_VERSION)) {
  std::istringstream extensionsStream(GLGetString(GL_EXTENSIONS));
  extensionsStream >> std::skipws;
  std::string extension;
  while (extensionsStream >> extension) {
    _extensions.emplace(std::move(extension));
  }
}

GLConnection::~GLConnection() = default;

const std::string& GLConnection::vendor() const {
  return _vendor;
}

const std::string& GLConnection::renderer() const {
  return _renderer;
}

const GLConnection::Version& GLConnection::version() const {
  return _version;
}

std::string GLConnection::versionString() const {
  return VersionToString(_version);
}

const GLConnection::Version& GLConnection::shadingLanguageVersion() const {
  return _shadingLanguageVersion;
}

std::string GLConnection::shadingLanguageVersionString() const {
  return VersionToString(_shadingLanguageVersion);
}

std::string GLConnection::platform() const {
  std::stringstream stream;
  stream << vendor() << ": " << renderer();
  return stream.str();
}

const std::set<std::string>& GLConnection::extensions() const {
  return _extensions;
}

std::string GLConnection::description() const {
  std::vector<std::pair<std::string, std::string>> items;

  items.emplace_back("Vendor", vendor());
  items.emplace_back("Renderer", renderer());
  items.emplace_back("Version", versionString());
  items.emplace_back("Shader Version", shadingLanguageVersionString());

  std::string extensionsLabel("Extensions");

  size_t padding = extensionsLabel.size();

  for (const auto& item : items) {
    padding = std::max(padding, item.first.size());
  }

  padding += 1;

  std::stringstream stream;

  stream << std::endl;

  for (const auto& item : items) {
    stream << std::setw(padding) << item.first << std::setw(0) << ": "
           << item.second << std::endl;
  }

  if (_extensions.size() != 0) {
    std::string paddingString;
    paddingString.resize(padding + 2, ' ');

    stream << std::setw(padding) << extensionsLabel << std::setw(0) << ": "
           << _extensions.size() << " Available" << std::endl;

    for (const auto& extension : _extensions) {
      stream << paddingString << extension << std::endl;
    }
  }

  return stream.str();
}

}  // namespace compositor
}  // namespace rl
