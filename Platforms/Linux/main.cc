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

using SDLWindowAndRenderer = std::pair<SDL_Window*, SDL_Renderer*>;

class RenderSurfaceLinux : public rl::RenderSurface {
 public:
  explicit RenderSurfaceLinux(SDLWindowAndRenderer& windowAndRenderer)
      : RenderSurface(), _windowAndRenderer(windowAndRenderer) {
    RL_ASSERT(_windowAndRenderer.first);
    RL_ASSERT(_windowAndRenderer.second);
    _context = SDL_GL_GetCurrentContext();
  }

  bool makeCurrent() {
    auto result = SDL_GL_MakeCurrent(_windowAndRenderer.first, _context);
    RL_ASSERT(result == 0 && "Must be able to make the context current");
    return result == 0;
  }

  bool present() {
    SDL_RenderPresent(_windowAndRenderer.second);
    return true;
  }

 private:
  SDLWindowAndRenderer& _windowAndRenderer;
  SDL_GLContext _context;
  DISALLOW_COPY_AND_ASSIGN(RenderSurfaceLinux);
};

static const int kInitialWindowWidth = 800;
static const int kInitialWindowHeight = 600;

static SDLWindowAndRenderer SetupSDL(void) {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_RendererInfo rendererInfo = {0};
  SDL_CreateWindowAndRenderer(kInitialWindowWidth, kInitialWindowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, &window,
                              &renderer);
  SDL_GetRendererInfo(renderer, &rendererInfo);

  RL_ASSERT((rendererInfo.flags & SDL_RENDERER_ACCELERATED) &&
            (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) &&
            "Must be able to fetch an accelerated render target");

  return std::make_pair(window, renderer);
}

static void ResizeInterface(rl::Shell& shell,
                            RenderSurfaceLinux& surface,
                            int width,
                            int height) {
  rl::Size size(width, height);
  surface.surfaceSizeUpdated(size);
  shell.interface().setSize(size);
}

static void SetupEventLoop(SDLWindowAndRenderer& windowAndRenderer) {
  auto renderSurface = std::make_shared<RenderSurfaceLinux>(windowAndRenderer);
  auto application = std::make_shared<sample::SampleApplication>();
  rl::Shell shell(renderSurface, application);

  renderSurface->surfaceWasCreated();

  ResizeInterface(shell, *renderSurface, kInitialWindowWidth,
                  kInitialWindowHeight);

  bool keepRunning = true;
  while (keepRunning) {
    SDL_Event event;
    if (SDL_WaitEvent(&event) == 1) {
      /*
       *  Window Event
       */
      if (event.type == SDL_WINDOWEVENT) {
        switch (event.window.event) {
          case SDL_WINDOWEVENT_RESIZED:
            ResizeInterface(shell, *renderSurface, event.window.data1,
                            event.window.data2);
            break;
          case SDL_WINDOWEVENT_CLOSE:
            keepRunning = false;
            break;
        }
        continue;
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

  renderSurface->surfaceWasDestroyed();
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
   *  Setup the window and renderer
   */
  auto windowAndRenderer = SetupSDL();

  /*
   *  Start the event loop with the renderer
   */
  SetupEventLoop(windowAndRenderer);

  /*
   *  Destroy the window and renderer
   */
  SDL_DestroyRenderer(windowAndRenderer.second);
  SDL_DestroyWindow(windowAndRenderer.first);

  /*
   *  Teardown SDL
   */
  SDL_Quit();
  return EXIT_SUCCESS;
}
