// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
#define RADAR_INTERFACEBUILDER_SVGARCHIVE_H_

#include <Core/Macros.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include <pugixml.hpp>

namespace rl {
namespace ib {

class SVGArchive : public InterfaceBuilderArchive {
 public:
  SVGArchive(const core::FileHandle& handle);

  ~SVGArchive() override;

  bool isValid() const override;

  bool inflate(interface::Interface& interface,
               interface::ModelEntity& container) const override;

 private:
  std::unique_ptr<pugi::xml_document> _document;

  void visitRect(const pugi::xml_node& node,
                 interface::Interface& interface,
                 interface::ModelEntity& parent) const;

  void visitEllipse(const pugi::xml_node& node,
                    interface::Interface& interface,
                    interface::ModelEntity& parent) const;

  void visitG(const pugi::xml_node& node,
              interface::Interface& interface,
              interface::ModelEntity& parent) const;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
