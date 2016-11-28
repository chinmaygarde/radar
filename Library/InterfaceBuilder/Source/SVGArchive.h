// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
#define RADAR_INTERFACEBUILDER_SVGARCHIVE_H_

#include <Core/Macros.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>
#include <pugixml.hpp>
#include <unordered_map>

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
  std::unordered_map<std::string, pugi::xml_node> _definitions;

  void findDefinitions(const pugi::xml_node& node);

  void visitNodeChildren(const pugi::xml_node& node,
                         interface::Interface& interface,
                         interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitNode(const pugi::xml_node& node,
                                        interface::Interface& interface,
                                        interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitRect(const pugi::xml_node& node,
                                        interface::Interface& interface,
                                        interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitEllipse(
      const pugi::xml_node& node,
      interface::Interface& interface,
      interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitG(const pugi::xml_node& node,
                                     interface::Interface& interface,
                                     interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitCircle(const pugi::xml_node& node,
                                          interface::Interface& interface,
                                          interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitPolygon(
      const pugi::xml_node& node,
      interface::Interface& interface,
      interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitLine(const pugi::xml_node& node,
                                        interface::Interface& interface,
                                        interface::ModelEntity& parent) const;

  interface::ModelEntity::Ref visitUse(const pugi::xml_node& node,
                                       interface::Interface& interface,
                                       interface::ModelEntity& parent) const;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
