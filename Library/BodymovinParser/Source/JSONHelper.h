/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <rapidjson/document.h>
#include <vector>

namespace rl {
namespace bodymovin {

template <class ValueType>
bool __IsCorrectType(const rapidjson::Value& member);

template <class ValueType>
void __ReadValue(const rapidjson::Value& value, ValueType& dest);

template <class ValueType>
bool ReadMember(const rapidjson::Value& object, ValueType& value) {
  if (!__IsCorrectType<ValueType>(object)) {
    return false;
  }
  __ReadValue(object, value);
  return true;
}

template <class ValueType>
bool ReadMember(const rapidjson::Value& object,
                const char* name,
                ValueType& value) {
  auto member = object.FindMember(name);
  if (member == object.MemberEnd()) {
    return false;
  }
  return ReadMember(member->value, value);
}

template <class ValueType>
bool ReadMembers(const rapidjson::Value& array,
                 std::vector<std::unique_ptr<ValueType>>& collection) {
  if (!array.IsArray()) {
    return false;
  }
  collection.reserve(collection.size() + array.Size());
  for (size_t i = 0; i < array.Size(); i++) {
    std::unique_ptr<ValueType> value;
    if (ReadMember(array[i], value) && value) {
      collection.emplace_back(std::move(value));
    }
  }
  return true;
}

template <class ValueType>
bool ReadMembers(const rapidjson::Value& object,
                 const char* name,
                 std::vector<std::unique_ptr<ValueType>>& collection) {
  auto member = object.FindMember(name);
  if (member == object.MemberEnd()) {
    return false;
  }
  return ReadMembers(member->value, collection);
}

/*
 *  bool
 */

template <>
bool __IsCorrectType<bool>(const rapidjson::Value& value) {
  return value.IsBool();
}

template <>
void __ReadValue<bool>(const rapidjson::Value& value, bool& dest) {
  dest = value.GetBool();
}

/*
 *  double
 */

template <>
bool __IsCorrectType<double>(const rapidjson::Value& value) {
  return value.IsNumber();
}

template <>
void __ReadValue<double>(const rapidjson::Value& value, double& dest) {
  dest = value.GetDouble();
}

/*
 *  int64_t
 */

template <>
bool __IsCorrectType<int64_t>(const rapidjson::Value& value) {
  return value.IsNumber();
}

template <>
void __ReadValue<int64_t>(const rapidjson::Value& value, int64_t& dest) {
  dest = value.GetInt64();
}

/*
 *  uint64_t
 */

template <>
bool __IsCorrectType<uint64_t>(const rapidjson::Value& value) {
  return value.IsNumber();
}

template <>
void __ReadValue<uint64_t>(const rapidjson::Value& value, uint64_t& dest) {
  dest = value.GetUint64();
}

/*
 *  std::string
 */

template <>
bool __IsCorrectType<std::string>(const rapidjson::Value& value) {
  return value.IsString();
}

template <>
void __ReadValue<std::string>(const rapidjson::Value& value,
                              std::string& dest) {
  dest = {value.GetString(), value.GetStringLength()};
}

/*
 *  Animation
 */

template <>
bool __IsCorrectType<Animation>(const rapidjson::Value& value) {
  return value.IsObject();
}

template <>
void __ReadValue<Animation>(const rapidjson::Value& json,
                            Animation& animation) {
  double inPoint = 0.0;
  if (ReadMember(json, "ip", inPoint)) {
    animation.setInPoint(inPoint);
  }

  double outPoint = 0.0;
  if (ReadMember(json, "op", outPoint)) {
    animation.setOutPoint(outPoint);
  }

  double frameRate = 0.0;
  if (ReadMember(json, "fr", frameRate)) {
    animation.setFrameRate(frameRate);
  }

  geom::Size compositionSize;
  ReadMember(json, "w", compositionSize.width);
  ReadMember(json, "h", compositionSize.height);
  animation.setCompositionSize(compositionSize);

  std::string bodymovinVersion;
  if (ReadMember(json, "v", bodymovinVersion)) {
    animation.setBodymovinVersion(std::move(bodymovinVersion));
  }

  std::vector<std::unique_ptr<Layer>> layers;
  if (ReadMembers(json, "layers", layers)) {
    animation.setLayers(std::move(layers));
  }

  // TODO: Read assets.
  RL_WIP;
}

/*
 *  std::unique_ptr<Layer>
 */

template <>
bool __IsCorrectType<std::unique_ptr<Layer>>(const rapidjson::Value& value) {
  return value.IsObject();
}

template <>
void __ReadValue<std::unique_ptr<Layer>>(const rapidjson::Value& json,
                                         std::unique_ptr<Layer>& layer) {
  int64_t type = std::numeric_limits<int64_t>::max();
  if (!ReadMember(json, "ty", type)) {
    /*
     *  Could not figure out layer type.
     */
    return;
  }

  switch (type) {
    case 0: {
      auto precomp = std::make_unique<PrecompLayer>();

      std::string referenceID;
      if (ReadMember(json, "refId", referenceID)) {
        precomp->setReferenceID(std::move(referenceID));
      }
      double timeRemapping = 0.0;
      if (ReadMember(json, "tm", timeRemapping)) {
        precomp->setTimeRemapping(timeRemapping);
      }

      layer = std::move(precomp);
    } break;
    case 1: {
      auto solid = std::make_unique<SolidLayer>();

      std::string solidColor;
      if (ReadMember(json, "sc", solidColor)) {
        solid->setSolidColor(std::move(solidColor));
      }

      geom::Size solidSize;
      ReadMember(json, "sw", solidSize.width);
      ReadMember(json, "sh", solidSize.height);
      solid->setSolidSize(solidSize);

      layer = std::move(solid);
    } break;
    case 2: {
      auto image = std::make_unique<ImageLayer>();

      std::string referenceID;
      if (ReadMember(json, "refId", referenceID)) {
        image->setReferenceID(std::move(referenceID));
      }

      layer = std::move(image);
    } break;
    case 3:
      layer = std::make_unique<NullLayer>();
      break;
    case 4: {
      auto shape = std::make_unique<ShapeLayer>();

      std::vector<std::unique_ptr<Shape>> shapes;
      if (ReadMembers(json, "shapes", shapes)) {
        shape->setShapeItems(std::move(shapes));
      }

      layer = std::move(shape);
    } break;
    case 5: {
      auto text = std::make_unique<TextLayer>();

      // TODO: Read text "t"
      RL_WIP;

      layer = std::move(text);
    }

    break;
    default:
      return;
  }

  if (!layer) {
    return;
  }

  auto transform = std::make_unique<Transform>();
  if (ReadMember(json, "ks", *transform)) {
    layer->setTransform(std::move(transform));
  }

  bool autoOrient = false;
  if (ReadMember(json, "ao", autoOrient)) {
    layer->setShouldAutoOrient(autoOrient);
  }

  bool is3DLayer = false;
  if (ReadMember(json, "ddd", is3DLayer)) {
    layer->setIs3DLayer(is3DLayer);
  }

  int64_t index = 0;
  if (ReadMember(json, "inx", index)) {
    layer->setIndex(index);
  }

  std::string clasz;
  if (ReadMember(json, "cl", clasz)) {
    layer->setClassName(std::move(clasz));
  }

  std::string layerHTMLID;
  if (ReadMember(json, "ln", layerHTMLID)) {
    layer->setLayerHtmlID(std::move(layerHTMLID));
  }

  double inPoint = 0.0;
  if (ReadMember(json, "ip", inPoint)) {
    layer->setInPoint(inPoint);
  }

  double outPoint = 0.0;
  if (ReadMember(json, "op", outPoint)) {
    layer->setOutPoint(outPoint);
  }

  double startTime = 0.0;
  if (ReadMember(json, "st", startTime)) {
    layer->setStartTime(startTime);
  }

  std::string name;
  if (ReadMember(json, "nm", name)) {
    layer->setName(std::move(name));
  }

  std::vector<std::unique_ptr<Effect>> effects;
  if (ReadMembers(json, "ef", effects)) {
    layer->setEffects(std::move(effects));
  }

  double stretch = 0.0;
  if (ReadMember(json, "sr", stretch)) {
    layer->setStretch(stretch);
  }

  int64_t parentIndex = 0;
  if (ReadMember(json, "parent", parentIndex)) {
    layer->setParentIndex(parentIndex);
  }
}

/*
 *  Transform
 */

template <>
bool __IsCorrectType<Transform>(const rapidjson::Value& value) {
  return value.IsObject();
}

template <>
void __ReadValue<Transform>(const rapidjson::Value& json,
                            Transform& transform) {
  std::unique_ptr<ValueBase> value;
  if (ReadMember(json, "a", value)) {
    transform.setAnchorPoint(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "p", value)) {
    transform.setPosition(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "s", value)) {
    transform.setScale(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "r", value)) {
    transform.setRotation(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "o", value)) {
    transform.setOpacity(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "px", value)) {
    transform.setPositionX(std::move(value));
    value.reset();
  }

  if (ReadMember(json, "py", value)) {
    transform.setPositionX(std::move(value));
    value.reset();
  }
}

/*
 *  std::unique_ptr<Shape>
 */

template <>
bool __IsCorrectType<std::unique_ptr<Shape>>(const rapidjson::Value& value) {
  return value.IsObject();
}

template <>
void __ReadValue<std::unique_ptr<Shape>>(const rapidjson::Value& json,
                                         std::unique_ptr<Shape>& shape) {
  std::string shapeTypeString;
  if (!ReadMember(json, "ty", shapeTypeString)) {
    return;
  }

  if (shapeTypeString == "el") {
    auto actual = std::make_unique<EllipseShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "fl") {
    auto actual = std::make_unique<FillShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "gf") {
    auto actual = std::make_unique<GradientFillShape>();
    RL_WIP;
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
    std::vector<std::unique_ptr<Shape>> shapes;
    if (ReadMembers(json, "it", shapes)) {
      actual->setShapeItems(std::move(shapes));
    }

    shape = std::move(actual);
  } else if (shapeTypeString == "gs") {
    auto actual = std::make_unique<GradientStrokeShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "tm") {
    auto actual = std::make_unique<MergeShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "rc") {
    auto actual = std::make_unique<RectShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "rd") {
    auto actual = std::make_unique<RoundShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "sh") {
    auto actual = std::make_unique<VertexShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "sr") {
    auto actual = std::make_unique<StarShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "st") {
    auto actual = std::make_unique<StrokeShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "tm") {
    auto actual = std::make_unique<TrimShape>();
    RL_WIP;
    shape = std::move(actual);
  } else if (shapeTypeString == "tr") {
    auto actual = std::make_unique<TransformShape>();
    RL_WIP;
    shape = std::move(actual);
  } else {
    RL_LOG("Warning: Unknown shape type '%s'.", shapeTypeString.c_str());
  }

  if (shape == nullptr) {
    return;
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
}

/*
 *  std::unique_ptr<ValueBase>
 */

template <>
bool __IsCorrectType<std::unique_ptr<ValueBase>>(
    const rapidjson::Value& value) {
  return value.IsObject();
}

template <>
void __ReadValue<std::unique_ptr<ValueBase>>(
    const rapidjson::Value& json,
    std::unique_ptr<ValueBase>& value) {
  auto valueMember = json.FindMember("k");
  if (valueMember == json.MemberEnd()) {
    return;
  }

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
          RL_WIP;
          break;
      }
    }

    value = std::move(keyframedValue);
  } else {
    return;
  }

  if (!value) {
    return;
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
}

/*
 *  std::unique_ptr<Effect>
 */

template <>
bool __IsCorrectType<std::unique_ptr<Effect>>(const rapidjson::Value& value) {
  RL_WIP;
  return value.IsObject();
}

template <>
void __ReadValue<std::unique_ptr<Effect>>(const rapidjson::Value& json,
                                          std::unique_ptr<Effect>& effect) {
  RL_WIP;
}

}  // namespace bodymovin
}  // namespace rl
