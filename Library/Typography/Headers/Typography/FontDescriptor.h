/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>

namespace rl {
namespace type {

class FontDescriptor {
 public:
  FontDescriptor();

  FontDescriptor(std::string postscriptName, double pointSize);

  ~FontDescriptor();

  const std::string& postscriptName() const;

  double pointSize() const;

  bool operator==(const FontDescriptor& other) const;

 private:
  std::string _postscriptName;
  double _pointSize = 14.0;
};

}  // namespace type
}  // namespace rl
