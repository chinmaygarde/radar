// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
#define RADAR_INTERFACEBUILDER_SVGARCHIVE_H_

#include <Core/Macros.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>

namespace rl {
namespace ib {

class SVGArchive : public InterfaceBuilderArchive {
 public:
  SVGArchive(core::FileHandle handle);

  ~SVGArchive() override;

 private:
  core::FileHandle _handle;
  bool _isValid;

  bool isArchiveReadable() const override;

  std::unique_ptr<entity::Entity> onInflate() const override;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
