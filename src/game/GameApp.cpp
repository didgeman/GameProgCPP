#include "GameApp.hpp"

Game::Game() {
  mIsRunning = true;
  mWindow = nullptr;
}

void Game::ProcessInput() {
  SDL_Event event;
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
  if (mWindow == nullptr) {
    SDL_Log("Unable to create main window! Errorcode:%s", SDL_GetError());
    return false;
  }
  
  // if we reach here, everything seems fine
  return true;
}

void Game::RunLoop() {
  while (mIsRunning) {
    ProcessInput();
    // UpdateGame();
    // GenerateOutput();
  }
}

void Game::Shutdown() {
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}
