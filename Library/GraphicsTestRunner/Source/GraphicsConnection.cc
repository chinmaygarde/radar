/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Platform.h>
#include "GraphicsConnection.h"

#if RL_OS_MAC

#include "GraphicsConnectionMac.h"
using PlatformGraphicsConnection = rl::testrunner::GraphicsConnectionMac;

#else

#error Unsupported Platform.

#endif

namespace rl {
namespace testrunner {

std::unique_ptr<GraphicsConnection> GraphicsConnection::Create() {
  return std::make_unique<PlatformGraphicsConnection>();
}

GraphicsConnection::GraphicsConnection() = default;

GraphicsConnection::~GraphicsConnection() = default;

bool GraphicsConnection::isValid() const {
  return _impl != nullptr && _impl->isValid();
}

bool GraphicsConnection::activate() {
  return _impl != nullptr && _impl->activate();
}

bool GraphicsConnection::deactivate() {
  return _impl != nullptr && _impl->deactivate();
}

}  // namespace testrunner
}  // namespace rl
