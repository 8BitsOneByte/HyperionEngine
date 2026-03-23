//
// Created by 13191 on 2026/2/1.
//

#ifndef HYPERIONENGINE_MOUSEBUTTONCODES_H
#define HYPERIONENGINE_MOUSEBUTTONCODES_H

#include <cstdint>

namespace Hyperion
{
    // Platform-agnostic mouse button code alias; maps to backend mouse button codes (e.g., GLFW).
    using MouseCode = std::uint8_t;
}

/* Mouse button codes from glfw3.h */
#define HYPERION_MOUSE_BUTTON_1         0
#define HYPERION_MOUSE_BUTTON_2         1
#define HYPERION_MOUSE_BUTTON_3         2
#define HYPERION_MOUSE_BUTTON_4         3
#define HYPERION_MOUSE_BUTTON_5         4
#define HYPERION_MOUSE_BUTTON_6         5
#define HYPERION_MOUSE_BUTTON_7         6
#define HYPERION_MOUSE_BUTTON_8         7
#define HYPERION_MOUSE_BUTTON_LAST      HYPERION_MOUSE_BUTTON_8
#define HYPERION_MOUSE_BUTTON_LEFT      HYPERION_MOUSE_BUTTON_1
#define HYPERION_MOUSE_BUTTON_RIGHT     HYPERION_MOUSE_BUTTON_2
#define HYPERION_MOUSE_BUTTON_MIDDLE    HYPERION_MOUSE_BUTTON_3

#endif //HYPERIONENGINE_MOUSEBUTTONCODES_H