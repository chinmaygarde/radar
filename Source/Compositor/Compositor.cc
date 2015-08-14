// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Compositor.h>
#include <Compositor/Frame.h>
#include <Compositor/Primitive.h>

#include <cassert>

namespace rl {

Compositor::Compositor(std::shared_ptr<RenderSurface> surface)
    : _surface(surface),
      _looper(nullptr),
      _lock(),
      _surfaceSize(SizeZero),
      _programCatalog(nullptr),
      _interfaceLease(nullptr) {
  assert(_surface != nullptr && "A surface must be provided to the compositor");
  _surface->setObserver(this);
}

Compositor::~Compositor() {
  _surface->setObserver(nullptr);
}

void Compositor::run(Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_looper != nullptr) {
    ready();
    return;
  }

  _looper = Looper::Current();
  setupChannels();
  _looper->loop(ready);
}

void Compositor::shutdown(Latch& shutdownLatch) {
  if (_looper == nullptr) {
    shutdownLatch.countDown();
    return;
  }

  _looper->dispatchAsync([&] {
    teardownChannels();
    stopComposition();
    _looper->terminate();
    shutdownLatch.countDown();
  });
}

bool Compositor::isRunning() const {
  return _looper != nullptr;
}

void Compositor::surfaceWasCreated() {
  _looper->dispatchAsync([&] { startComposition(); });
}

void Compositor::surfaceSizeUpdated(const Size& size) {
  _looper->dispatchAsync([&, size] { commitCompositionSizeUpdate(size); });
}

void Compositor::surfaceWasDestroyed() {
  _looper->dispatchAsync([&] { stopComposition(); });
}

void Compositor::startComposition() {
  drawSingleFrame();
}

void Compositor::stopComposition() {
}

void Compositor::commitCompositionSizeUpdate(const Size& size) {
  if (size == _surfaceSize) {
    return;
  }

  // Commit size update
  _surfaceSize = size;
}

/**
 *  Initializes and returns the compositor program catalog. Must be accessed
 *  on the compositor thread
 *
 *  @return the program catalog
 */
std::shared_ptr<ProgramCatalog> Compositor::accessCatalog() {
  if (_programCatalog != nullptr) {
    return _programCatalog;
  }

  _programCatalog = std::make_shared<ProgramCatalog>();

  // Setup catalog

  return _programCatalog;
}

void Compositor::drawSingleFrame() {
  ScopedRenderSurfaceAccess access(*_surface);
  ScopedFrame frame(_surfaceSize, accessCatalog());

  if (!frame.isReady()) {
    return;
  }

  /*
   *  Update the read head
   */
  auto readArena = _interfaceLease->accessReadArena(false);
  for (size_t i = 0, size = readArena.encodedEntities(); i < size; i++) {
    auto& entity = readArena[i];
    Primitive p;
    p.setContentColor(entity.backgroundColor());
    p.setModelMatrixAndSize(entity.modelMatrix(), entity.bounds().size);
    p.render(frame);
  }
}

void Compositor::setupChannels() {
  manageInterfaceUpdates(true);
}

void Compositor::teardownChannels() {
  manageInterfaceUpdates(false);
}

InterfaceLease& Compositor::acquireLease(size_t count) {
  if (_interfaceLease != nullptr) {
    return *_interfaceLease;
  }

  _interfaceLease = rl::make_unique<InterfaceLease>(count);
  manageInterfaceUpdates(true);

  assert(_interfaceLease != nullptr);
  return *_interfaceLease;
}

void Compositor::manageInterfaceUpdates(bool schedule) {
  if (_looper == nullptr || _interfaceLease == nullptr) {
    return;
  }

  if (schedule) {
    auto source = _interfaceLease->writeNotificationSource();
    source->setWakeFunction(std::bind(&Compositor::onInterfaceDidUpdate, this));
    _looper->addSource(source);
  } else {
    _looper->removeSource(_interfaceLease->writeNotificationSource());
  }
}

void Compositor::onInterfaceDidUpdate() {
  if (_interfaceLease == nullptr) {
    return;
  }
  _interfaceLease->accessReadArena(true);
  drawSingleFrame();
}

}  // namespace rl
