/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Primitive/ColoredBoxPrimitive.h"
#include "Primitive/ColoredBoxStrokePrimitive.h"
#include "Primitive/ColoredPathPrimitive.h"
#include "Primitive/ColoredPathStrokePrimitive.h"
#include "Primitive/TexturedBoxPrimitive.h"
#include "Primitive/TexturedPathPrimitive.h"
#include "PrimitivesCache.h"

namespace rl {
namespace compositor {

PrimitivesCache::PrimitivesCache() {}

PrimitivesCache::~PrimitivesCache() = default;

std::shared_ptr<Primitive> PrimitivesCache::acquire(
    const PresentationEntity& entity,
    ContentType contentType,
    PrimitiveType primitiveType) {
  if (contentType == ContentType::None ||
      primitiveType == PrimitiveType::None) {
    return nullptr;
  }

  CacheKey key(contentType, primitiveType);

  auto found = _primitivesMap.find(key);

  if (found != _primitivesMap.end()) {
    /*
     *  Found a cached primitive for this key.
     */
    return found->second;
  }

  /*
   *  Nothing in the cache, create a fresh primitive, cache it and return to the
   *  caller.
   */
  auto primitive = createPrimitive(entity, contentType, primitiveType);
  _primitivesMap[key] = primitive;
  return primitive;
}

bool PrimitivesCache::invalidate(ContentType contentType,
                                 PrimitiveType primitiveType) {
  CacheKey key(contentType, primitiveType);

  auto found = _primitivesMap.find(key);

  if (found == _primitivesMap.end()) {
    /*
     *  Nothing to invalidate.
     */
    return false;
  }

  _primitivesMap.erase(found);
  return true;
}

std::shared_ptr<Primitive> PrimitivesCache::createColoredPrimitive(
    const PresentationEntity& entity,
    PrimitiveType type) const {
  switch (type) {
    case PrimitiveType::Box:
      return std::make_shared<ColoredBoxPrimitive>(entity.backgroundColor());
    case PrimitiveType::BoxStroke:
      return std::make_shared<ColoredBoxStrokePrimitive>(entity.strokeColor());
    case PrimitiveType::Path:
      return std::make_shared<ColoredPathPrimitive>(entity.backgroundColor(),
                                                    entity.path());
    case PrimitiveType::PathStroke:
      return std::make_shared<ColoredPathStrokePrimitive>(entity.path(),
                                                          entity.strokeColor());
    case PrimitiveType::None:
      return nullptr;
  }
  return nullptr;
}

std::shared_ptr<Primitive> PrimitivesCache::createTexturedPrimitive(
    const PresentationEntity& entity,
    PrimitiveType type) const {
  switch (type) {
    case PrimitiveType::Box:
      return std::make_shared<TexturedBoxPrimitive>(entity.contents());
    case PrimitiveType::BoxStroke:
      RL_ASSERT("Textured box strokes are not supported");
      return nullptr;
    case PrimitiveType::Path:
      return std::make_shared<TexturedPathPrimitive>(entity.contents(),
                                                     entity.path());
    case PrimitiveType::PathStroke:
      RL_ASSERT("Textured strokes are not supported.");
      return nullptr;
    case PrimitiveType::None:
      return nullptr;
  }

  return nullptr;
}

std::shared_ptr<Primitive> PrimitivesCache::createPrimitive(
    const PresentationEntity& entity,
    ContentType contentType,
    PrimitiveType primitiveType) {
  switch (contentType) {
    case ContentType::None:
      /*
       *  If there is no content to display, there is no sense in looking for
       *  primitives to render.
       */
      return nullptr;
    case ContentType::SolidColor:
      return createColoredPrimitive(entity, primitiveType);
    case ContentType::Image:
      return createTexturedPrimitive(entity, primitiveType);
  }

  return nullptr;
}

}  // namespace compositor
}  // namespace rl
