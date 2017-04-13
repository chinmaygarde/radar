/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Platform.h>

#if RL_OS_MAC
#include <OpenGL/gl.h>
#elif RL_OS_LINUX || RL_OS_NACL || RL_OS_BSD
#include <GLES2/gl2.h>
#else
#error Unsupported Platform
#endif
