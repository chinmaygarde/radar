// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>
#include <Core/Core.h>
#include <SDL.h>

static void SetupSDL(void) {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_RendererInfo rendererInfo = { 0 };
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);
  SDL_GetRendererInfo(renderer, &rendererInfo);

  RL_ASSERT((rendererInfo.flags & SDL_RENDERER_ACCELERATED) &&
            (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) &&
            "Must be able to fetch an accelerated render target");
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
   *  Setup SDL surface
   */
  SetupSDL();

  /*
   *  Start the event loop
   */
  rl::EventLoop::Current()->loop();

  /*
   *  Teardown SDL
   */
  SDL_Quit();
  return EXIT_SUCCESS;
}
