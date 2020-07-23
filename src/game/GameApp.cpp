#include "GameApp.hpp"

Game::Game() {
  mIsRunning = true;
  mWindow = nullptr;
}

void Game::GenerateOutput() {
  // first set bckground color
  SDL_SetRenderDrawColor(mRenderer,
    0,  /* red */
    0,  /* green */
    255, /* blue */
    255 /* alpha */
  );
  SDL_RenderClear(mRenderer);
  SDL_RenderPresent(mRenderer);
}


void Game::ProcessInput() {
  SDL_Event event;

  const Uint8* keyb_state = SDL_GetKeyboardState(NULL);
  // if ESC-key is pressed, then signal game loop to stop running
  if(keyb_state[SDL_SCANCODE_ESCAPE])
    mIsRunning = false;

  // while there are still events in the queue
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        mIsRunning = false;
        break;
    }
  }
}

bool Game::Initialize() {
  
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0) {
    SDL_Log("Unable to init SDL! Errorcode:%s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow(
    "Game Programming in C++ (Ch1)",    // title
    100,                                // top left x-coord of window
    100,                                // top left y-coord of window
    1024,                               // width of window
    768,                                // height of window
    0                                   // flags set
  );
  if (!mWindow) {
    SDL_Log("Unable to create main window! Errorcode:%s", SDL_GetError());
    return false;
  }
  
  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!mRenderer) {
    SDL_Log("Unable to create Renderer! Errorcode:%s", SDL_GetError());
    return false;
  }

  // if we reach here, everything seems fine
  return true;
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    // UpdateGame();
    GenerateOutput();
  }
}

void Game::Shutdown() {
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}
