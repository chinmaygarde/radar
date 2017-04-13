/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <GLFoundation/GLFoundation.h>
#include <set>
#include <string>

namespace rl {
namespace compositor {

class GLConnection {
 public:
  struct Version {
    union {
      struct {
        size_t major;
        size_t minor;
        size_t release;
      };
      size_t items[3];
    };

    std::string vendorString;

    bool isES;

    Version() : major(0), minor(0), release(0), isES(false) {}

    Version(size_t theMajor, size_t theMinor, size_t theRelease)
        : major(theMajor), minor(theMinor), release(theRelease), isES(false) {}
  };

  GLConnection();

  ~GLConnection();

  const std::string& vendor() const;

  const std::string& renderer() const;

  std::string platform() const;

  const Version& version() const;

  std::string versionString() const;

  const Version& shadingLanguageVersion() const;

  std::string shadingLanguageVersionString() const;

  const std::set<std::string>& extensions() const;

  std::string description() const;

 private:
  std::string _vendor;
  std::string _renderer;
  Version _version;
  Version _shadingLanguageVersion;
  std::set<std::string> _extensions;

  RL_DISALLOW_COPY_AND_ASSIGN(GLConnection);
};

}  // namespace compositor
}  // namespace rl
