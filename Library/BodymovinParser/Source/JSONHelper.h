/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <rapidjson/document.h>

namespace rl {
namespace bodymovin {

template <class ValueType>
bool __IsCorrectType(const rapidjson::Value& member);

template <class ValueType>
void __ReadValue(const rapidjson::Value& value, ValueType& dest);

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

template <class ValueType>
bool ReadMember(const rapidjson::Value& object,
                const char* name,
                ValueType& value) {
  auto member = object.FindMember(name);
  if (member == object.MemberEnd()) {
    return false;
  }
  if (!__IsCorrectType<ValueType>(member->value)) {
    return false;
  }
  __ReadValue(member->value, value);
  return true;
}

}  // namespace bodymovin
}  // namespace rl
