/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <BodymovinParser/Value.h>
#include <Core/Macros.h>
#include <memory>
#include <vector>

namespace rl {
namespace bodymovin {

class Effect {
 public:
  enum class Type {
    Slider = 0,
    Angle = 1,
    Color = 2,
    Point = 3,
    Checkbox = 4,
    Group = 5,
    NoValue = 6,
    DropDown = 7,
    CustomValue = 9,
    LayerIndex = 10,
    Tint = 20,
    Fill = 21,
    Stroke = 22,
    Tritone = 23,
    ProLevels = 24,
  };

  Effect() = default;

  ~Effect() = default;

  Type type() const { return _type; }

  uint64_t index() const { return _index; }

  const std::string& matchName() const { return _matchName; }

  const std::string& name() const { return _name; };

  const std::unique_ptr<ValueBase>& value() const { return _value; }

  const std::vector<std::unique_ptr<Effect>>& effects() const {
    return _effects;
  }

  void setType(Type type) { _type = type; }

  void setIndex(uint64_t index) { _index = index; }

  void setMatchName(std::string matchName) {
    _matchName = std::move(matchName);
  }

  void setName(std::string name) { _name = std::move(name); }

  void setValue(std::unique_ptr<ValueBase> value) { _value = std::move(value); }

  void setEffects(std::vector<std::unique_ptr<Effect>> effects) {
    _effects = std::move(effects);
  }

 private:
  uint64_t _index = 0;
  std::string _matchName;
  std::string _name;
  Type _type = Type::Slider;
  std::unique_ptr<ValueBase> _value;
  std::vector<std::unique_ptr<Effect>> _effects;

  RL_DISALLOW_COPY_AND_ASSIGN(Effect);
};

}  // namespace bodymovin
}  // namespace rl
