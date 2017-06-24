/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

#define RL_CONSOLE_GET_VALUE_ONCE(label, defaultValue)    \
  ({                                                      \
    static ::rl::compositor::console::FirstInFrame once;  \
    static __typeof__(defaultValue) value = defaultValue; \
    if (IsFirstInFrame(once)) {                           \
      ::rl::compositor::console::GetValue(label, &value); \
    }                                                     \
    value;                                                \
  })

#define RL_CONSOLE_DISPLAY_VALUE(label, ...) \
  ::rl::compositor::console::DisplayValue(label, ##__VA_ARGS__);

#define RL_CONSOLE_SECTION(section)            \
  ::rl::compositor::console::AutoEndSection    \
      _rl_console_section_auto_end_##__LINE__; \
  ::rl::compositor::console::BeginSection(section);

namespace rl {
namespace compositor {
namespace console {

struct FirstInFrame {
  int frameCount = -1;
};

void BeginSection(const char* section);

void EndSection();

bool IsFirstInFrame(FirstInFrame& once);

void DisplayValue(const char* format, ...);

void GetValue(const char* label, bool* current);

class AutoEndSection {
 public:
  AutoEndSection() = default;

  ~AutoEndSection() { EndSection(); }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AutoEndSection);
};

}  // namespace console
}  // namespace compositor
}  // namespace rl
