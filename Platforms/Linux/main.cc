// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>
#include <Core/Core.h>
#include <SDL.h>

#include <Core/Core.h>
#include <Shell/Shell.h>
#include <Compositor/RenderSurface.h>
#include "Sample.h"

namespace rl {
class RenderSurfaceLinux : public RenderSurface {
 public:
  explicit RenderSurfaceLinux(SDL_Renderer* renderer)
      : RenderSurface(), _renderer(renderer) {
    RL_ASSERT(renderer);
  }

  bool makeCurrent() { return true; }

  bool present() {
    SDL_RenderPresent(_renderer);
    return true;
  }

 private:
  SDL_Renderer* _renderer;
  DISALLOW_COPY_AND_ASSIGN(RenderSurfaceLinux);
};
}

static SDL_Renderer* SetupSDL(void) {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_RendererInfo rendererInfo = {0};
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);
  SDL_GetRendererInfo(renderer, &rendererInfo);

  RL_ASSERT((rendererInfo.flags & SDL_RENDERER_ACCELERATED) &&
            (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) &&
            "Must be able to fetch an accelerated render target");

  return renderer;
}

static void SetupEventLoop(SDL_Renderer* renderer) {
  auto renderSurface = std::make_shared<rl::RenderSurfaceLinux>(renderer);
  auto application = std::make_shared<sample::SampleApplication>();
  rl::Shell shell(renderSurface, application);
  renderSurface->surfaceWasCreated();

  bool keepRunning = true;
  while (keepRunning) {
    SDL_Event event;
    if (SDL_WaitEvent(&event) == 1) {
      /*
       *  Window Event
       */
      if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
          keepRunning = false;
          continue;
        }
      }

      /*
       *  Quit Event
       */
      if (event.type == SDL_QUIT) {
        keepRunning = false;
        continue;
      }
    }
  }
}

int main(int argc, const char* argv[]) {
  /*
   *  Initialize SDL
   */
  auto result = SDL_Init(SDL_INIT_VIDEO);

  if (result != 0) {
    RL_LOG("Could not initialize SDL: %s", SDL_GetError());
    RL_ASSERT(false);
  }

  /*
   *  Setup the renderer
   */
  auto renderer = SetupSDL();

  /*
   *  Start the event loop with the renderer
   */
  SetupEventLoop(renderer);

  /*
   *  Destroy the renderer
   */
  SDL_DestroyRenderer(renderer);

  /*
   *  Teardown SDL
   */
  SDL_Quit();
  return EXIT_SUCCESS;
}
