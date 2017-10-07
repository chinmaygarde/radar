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

static std::unique_ptr<Shape> ParseShape(
    const rapidjson::GenericValue<rapidjson::UTF8<>>& json) {
  if (!json.IsObject()) {
    return nullptr;
  }

  std::string shapeTypeString;
  if (!ReadMember(json, "ty", shapeTypeString)) {
    return nullptr;
  }

  std::unique_ptr<Shape> shape;

  if (shapeTypeString == "el") {
    auto actual = std::make_unique<EllipseShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "fl") {
    auto actual = std::make_unique<FillShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "gf") {
    auto actual = std::make_unique<GradientFillShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "gr") {
    auto actual = std::make_unique<GroupShape>();

    /*
     *  Properties Count.
     */
    uint64_t propertiesCount = 0;
    if (ReadMember(json, "np", propertiesCount)) {
      actual->setPropertiesCount(propertiesCount);
    }

    /*
     *  Recursively parse shape item members.
     */
    auto shapeItemsMember = json.FindMember("it");
    if (shapeItemsMember != json.MemberEnd()) {
      auto shapeItems = shapeItemsMember->value.GetArray();
      auto shapeItemsCount = shapeItems.Size();
      std::vector<std::unique_ptr<Shape>> shapes;
      shapes.reserve(shapeItemsCount);
      for (size_t i = 0; i < shapeItemsCount; i++) {
        if (auto shape = ParseShape(shapeItems[i])) {
          shapes.emplace_back(std::move(shape));
        }
      }
      actual->setShapeItems(std::move(shapes));
    }

    shape = std::move(actual);
  } else if (shapeTypeString == "gs") {
    auto actual = std::make_unique<GradientStrokeShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "tm") {
    auto actual = std::make_unique<MergeShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "rc") {
    auto actual = std::make_unique<RectShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "rd") {
    auto actual = std::make_unique<RoundShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "sh") {
    auto actual = std::make_unique<VertexShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "sr") {
    auto actual = std::make_unique<StarShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "st") {
    auto actual = std::make_unique<StrokeShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "tm") {
    auto actual = std::make_unique<TrimShape>();
    // TODO
    shape = std::move(actual);
  } else if (shapeTypeString == "tr") {
    auto actual = std::make_unique<TransformShape>();
    // TODO
    shape = std::move(actual);
  } else {
    RL_LOG("Warning: Unknown shape type '%s'.", shapeTypeString.c_str());
  }

  if (shape == nullptr) {
    return nullptr;
  }

  /*
   *  Read common shape properties.
   */
  std::string matchName;
  if (ReadMember(json, "mn", matchName)) {
    shape->setMatchName(std::move(matchName));
  }

  std::string name;
  if (ReadMember(json, "nm", name)) {
    shape->setName(std::move(name));
  }

  double direction = 0.0;
  if (ReadMember(json, "d", direction)) {
    shape->setDirection(direction);
  }

  return shape;
}

static std::unique_ptr<ValueBase> ParseValue(
    const rapidjson::GenericValue<rapidjson::UTF8<>>& json) {
  if (!json.IsObject()) {
    return nullptr;
  }

  auto valueMember = json.FindMember("k");
  if (valueMember == json.MemberEnd()) {
    return nullptr;
  }

  std::unique_ptr<ValueBase> value;

  if (valueMember->value.IsNumber()) {
    /*
     *  Single Value.
     */
    auto singleValue = std::make_unique<Value>();

    double number = 0.0;
    if (ReadMember(json, "k", number)) {
      singleValue->setValue(number);
    }

    value = std::move(singleValue);
  } else if (valueMember->value.IsArray()) {
    /*
     *  Keyframed value.
     */
    auto keyframedValue = std::make_unique<KeyframedValue>();
    auto array = valueMember->value.GetArray();

    for (size_t i = 0, length = array.Size(); i < length; i++) {
      switch (i) {
        case 0:
          if (array[i].IsNumber()) {
            keyframedValue->setKeyframeEnd(array[i].GetDouble());
          }
          break;
        case 1:
          if (array[i].IsNumber()) {
            keyframedValue->setKeyframeStart(array[i].GetDouble());
          }
          break;
        case 2:
          if (array[i].IsNumber()) {
            keyframedValue->setStartTime(array[i].GetDouble());
          }
          break;
        case 3:
          // TODO: Read bezier curve in value.
          break;
        case 4:
          // TODO: Read bezier curve out value.
          break;
        default:
          break;
      }
    }

    value = std::move(keyframedValue);
  } else {
    return nullptr;
  }

  if (!value) {
    return nullptr;
  }

  std::string expression;
  if (ReadMember(json, "x", expression)) {
    value->setExpression(std::move(expression));
  }

  uint64_t propertyIndex = 0;
  if (ReadMember(json, "ix", propertyIndex)) {
    value->setIndex(propertyIndex);
  }

  bool isAnimated = false;
  if (ReadMember(json, "a", isAnimated)) {
    value->setIsAnimated(isAnimated);
  }

  return value;
}

static std::unique_ptr<ValueBase> FindValueNamed(
    const char* name,
    const rapidjson::GenericValue<rapidjson::UTF8<>>& value) {
  auto member = value.FindMember(name);

  if (member == value.MemberEnd() || !member->value.IsObject()) {
    return nullptr;
  }

  return ParseValue(member->value);
}

static std::unique_ptr<Transform> ParseTransform(
    const rapidjson::GenericValue<rapidjson::UTF8<>>& value) {
  if (!value.IsObject()) {
    return nullptr;
  }

  auto transform = std::make_unique<Transform>();

  if (auto anchor = FindValueNamed("a", value)) {
    transform->setAnchorPoint(std::move(anchor));
  }

  if (auto position = FindValueNamed("p", value)) {
    transform->setPosition(std::move(position));
  }

  if (auto scale = FindValueNamed("s", value)) {
    transform->setScale(std::move(scale));
  }

  if (auto rotation = FindValueNamed("r", value)) {
    transform->setRotation(std::move(rotation));
  }

  if (auto opacity = FindValueNamed("o", value)) {
    transform->setOpacity(std::move(opacity));
  }

  if (auto xPosition = FindValueNamed("px", value)) {
    transform->setPositionX(std::move(xPosition));
  }

  if (auto yPosition = FindValueNamed("py", value)) {
    transform->setPositionX(std::move(yPosition));
  }

  return transform;
}

static void ReadBasicLayerProperties(
    Layer& layer,
    const rapidjson::GenericValue<rapidjson::UTF8<>>& value) {
  if (!value.IsObject()) {
    return;
  }

  auto transformNode = value.FindMember("ks");
  if (transformNode != value.MemberEnd()) {
    if (auto transform = ParseTransform(transformNode->value)) {
      layer.setTransform(std::move(transform));
    }
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

      auto shapeItemsMember = value.FindMember("shapes");
      if (shapeItemsMember != value.MemberEnd() &&
          shapeItemsMember->value.IsArray()) {
        auto shapeItems = shapeItemsMember->value.GetArray();
        std::vector<std::unique_ptr<Shape>> shapes;
        auto shapeCount = shapeItems.Size();
        shapes.reserve(shapeCount);
        for (size_t i = 0; i < shapeCount; i++) {
          if (auto shapeItem = ParseShape(shapeItems[i])) {
            shapes.emplace_back(std::move(shapeItem));
          }
        }
        shape->setShapeItems(std::move(shapes));
      }

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

  // TODO: Read assets.

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
