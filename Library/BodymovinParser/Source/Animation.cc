/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BodymovinParser/Animation.h>
#include <rapidjson/document.h>
#include <limits>
#include "JSONHelper.h"

namespace rl {
namespace bodymovin {

static void ReadBasicLayerProperties(
    Layer& layer,
    const rapidjson::GenericValue<rapidjson::UTF8<>>& value) {
  if (!value.IsObject()) {
    return;
  }

  const auto& transform = value.FindMember("ks");
  if (transform != value.MemberEnd() && transform->value.IsObject()) {
    RL_LOG_HERE;
  }

  bool autoOrient = false;
  if (ReadMember(value, "ao", autoOrient)) {
    layer.setShouldAutoOrient(autoOrient);
  }

  bool is3DLayer = false;
  if (ReadMember(value, "ddd", is3DLayer)) {
    layer.setIs3DLayer(is3DLayer);
  }

  int64_t index = 0;
  if (ReadMember(value, "inx", index)) {
    layer.setIndex(index);
  }

  std::string clasz;
  if (ReadMember(value, "cl", clasz)) {
    layer.setClassName(std::move(clasz));
  }

  std::string layerHTMLID;
  if (ReadMember(value, "ln", layerHTMLID)) {
    layer.setLayerHtmlID(std::move(layerHTMLID));
  }

  double inPoint = 0.0;
  if (ReadMember(value, "ip", inPoint)) {
    layer.setInPoint(inPoint);
  }

  double outPoint = 0.0;
  if (ReadMember(value, "op", outPoint)) {
    layer.setOutPoint(outPoint);
  }

  double startTime = 0.0;
  if (ReadMember(value, "st", startTime)) {
    layer.setStartTime(startTime);
  }

  std::string name;
  if (ReadMember(value, "nm", name)) {
    layer.setName(std::move(name));
  }

  // TODO: Read effects "ef"

  double stretch = 0.0;
  if (ReadMember(value, "sr", stretch)) {
    layer.setStretch(stretch);
  }

  int64_t parentIndex = 0;
  if (ReadMember(value, "parent", parentIndex)) {
    layer.setParentIndex(parentIndex);
  }
}

static std::unique_ptr<Layer> ParseLayer(
    const rapidjson::GenericValue<rapidjson::UTF8<>>& value) {
  if (!value.IsObject()) {
    return nullptr;
  }

  int64_t type = std::numeric_limits<int64_t>::max();

  if (!ReadMember(value, "ty", type)) {
    /*
     *  Could not figure out layer type.
     */
    return nullptr;
  }

  std::unique_ptr<Layer> layer;

  switch (type) {
    case 0: {
      auto precomp = std::make_unique<PrecompLayer>();

      std::string referenceID;
      if (ReadMember(value, "refId", referenceID)) {
        precomp->setReferenceID(std::move(referenceID));
      }
      double timeRemapping = 0.0;
      if (ReadMember(value, "tm", timeRemapping)) {
        precomp->setTimeRemapping(timeRemapping);
      }

      layer = std::move(precomp);
    } break;
    case 1: {
      auto solid = std::make_unique<SolidLayer>();

      std::string solidColor;
      if (ReadMember(value, "sc", solidColor)) {
        solid->setSolidColor(std::move(solidColor));
      }

      geom::Size solidSize;
      ReadMember(value, "sw", solidSize.width);
      ReadMember(value, "sh", solidSize.height);
      solid->setSolidSize(solidSize);

      layer = std::move(solid);
    } break;
    case 2: {
      auto image = std::make_unique<ImageLayer>();

      std::string referenceID;
      if (ReadMember(value, "refId", referenceID)) {
        image->setReferenceID(std::move(referenceID));
      }

      layer = std::move(image);
    } break;
    case 3:
      layer = std::make_unique<NullLayer>();
      break;
    case 4: {
      auto shape = std::make_unique<ShapeLayer>();

      // TODO: Read shape items "it"

      layer = std::move(shape);
    } break;
    case 5: {
      auto text = std::make_unique<TextLayer>();

      // TODO: Read text "t"

      layer = std::move(text);
    }

    break;
    default:
      return nullptr;
  }

  if (layer == nullptr) {
    return nullptr;
  }

  ReadBasicLayerProperties(*layer, value);

  return layer;
}

Animation::Animation(const core::FileMapping& mapping) {
  rapidjson::Document document;
  document.Parse(reinterpret_cast<const char*>(mapping.mapping()),
                 mapping.size());
  if (document.HasParseError() || !document.IsObject()) {
    return;
  }

  if (!ReadMember(document, "ip", _inPoint)) {
    return;
  }

  if (!ReadMember(document, "op", _outPoint)) {
    return;
  }

  if (!ReadMember(document, "fr", _frameRate)) {
    return;
  }

  if (!ReadMember(document, "w", _compositionSize.width)) {
    return;
  }

  if (!ReadMember(document, "h", _compositionSize.height)) {
    return;
  }

  if (!ReadMember(document, "v", _bodymovinVersion)) {
    return;
  }

  auto layers = document.FindMember("layers");
  if (layers != document.MemberEnd() && layers->value.IsArray()) {
    auto array = layers->value.GetArray();
    const auto length = array.Size();
    _layers.reserve(length);
    for (size_t i = 0; i < length; i++) {
      if (auto layer = ParseLayer(array[i])) {
        _layers.emplace_back(std::move(layer));
      }
    }
  }

  _valid = true;
}

Animation::~Animation() = default;

bool Animation::isValid() const {
  return _valid;
}

double Animation::inPoint() const {
  return _inPoint;
}

double Animation::outPoint() const {
  return _outPoint;
}

double Animation::frameRate() const {
  return _frameRate;
}

geom::Size Animation::compositionSize() const {
  return _compositionSize;
}

const std::string& Animation::bodymovinVersion() const {
  return _bodymovinVersion;
}

const std::vector<std::unique_ptr<Layer>>& Animation::layers() const {
  return _layers;
}

const std::vector<Asset>& Animation::assets() const {
  return _assets;
}

const std::vector<Character>& Animation::characters() const {
  return _characters;
}

}  // namespace bodymovin
}  // namespace rl
