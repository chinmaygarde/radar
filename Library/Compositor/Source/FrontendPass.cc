/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/FrontendPass.h>
#include "Primitive/Primitive.h"

namespace rl {
namespace compositor {

FrontEndPass::FrontEndPass() = default;

FrontEndPass::~FrontEndPass() = default;

FrontEndPass::FrontEndPass(FrontEndPass&&) = default;

bool FrontEndPass::hasRenderables() const {
  return _primitives.size() > 0;
}

size_t FrontEndPass::primitivesCount() const {
  return _primitives.size();
}

void FrontEndPass::addPrimitive(std::shared_ptr<Primitive> primitive) {
  if (primitive != nullptr) {
    _primitives.emplace_back(std::move(primitive));
  }
}

bool FrontEndPass::prepareInBackendPass(BackEndPass& pass) {
  for (const auto& primitive : _primitives) {
    primitive->prepareToRender(pass);
  }

  return true;
}

bool FrontEndPass::renderInBackEndPass(const BackEndPass& pass,
                                       Frame& frame) const {
  for (const auto& primitive : _primitives) {
    RL_RETURN_IF_FALSE(primitive->render(frame));
  }

  return true;
}

}  // namespace compositor
}  // namespace rl
