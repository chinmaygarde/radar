/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Platform.h>

#if RL_OS_MAC

#if __OBJC__

#define RL_OBJC_CLASS(clasz) @class clasz;

#else  // __OBJC__

#define RL_OBJC_CLASS(clasz) class clasz;

#endif  // __OBJC__

#endif  // RL_OS_MAC
