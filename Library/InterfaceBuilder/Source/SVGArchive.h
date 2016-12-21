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
  SVGArchive(const uint8_t* data, size_t size);

  ~SVGArchive() override;

  bool isValid() const override;

  interface::ModelEntity::Ref inflate(interface::Interface& interface,
                                      EntityMap& map) const override;

 private:
  std::unique_ptr<pugi::xml_document> _document;
  std::unordered_map<std::string, pugi::xml_node> _definitions;

  void findDefinitions(const pugi::xml_node& node);

  interface::ModelEntity::Ref visitNodeChildren(const pugi::xml_node& node,
                                                interface::Interface& interface,
                                                EntityMap& map) const;

  interface::ModelEntity::Ref configureNode(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureSVG(interface::ModelEntity::Ref entity,
                                           const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureRect(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureEllipse(
      interface::ModelEntity::Ref entity,
      const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureG(interface::ModelEntity::Ref entity,
                                         const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureCircle(
      interface::ModelEntity::Ref entity,
      const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configurePolygon(
      interface::ModelEntity::Ref entity,
      const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureLine(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureUse(interface::ModelEntity::Ref entity,
                                           const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureText(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configurePath(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureImage(interface::ModelEntity::Ref entity,
                                             const pugi::xml_node& node) const;

  interface::ModelEntity::Ref configureMask(interface::ModelEntity::Ref entity,
                                            const pugi::xml_node& node) const;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGARCHIVE_H_
