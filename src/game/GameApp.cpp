#include "GameApp.hpp"

Game::Game() {
  mIsRunning = true;
  mWindow = nullptr;
  mBallPos.x = 512.0f;
  mBallPos.y = 384.0f;
  mPaddlePos.x = 10.0f;
  mPaddlePos.y = 384.0f;
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

  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
  SDL_Rect wall{
    0,    // top left x
    0,    // top left y
    1024, // width
    thickness  // height
  };
  // draw the top wall
  SDL_RenderFillRect(mRenderer, &wall);

  // define bottom wall
  wall.x = 0; wall.y = 768 - thickness; wall.w = 1024; wall.h = thickness;
  // draw the bottom wall
  SDL_RenderFillRect(mRenderer, &wall);

  // the ball
  SDL_Rect ball {
    static_cast<int>(mBallPos.x - (ball_width/2)),
    static_cast<int>(mBallPos.y - (ball_height/2)),
    ball_width,
    ball_height
  };
  SDL_RenderFillRect(mRenderer, &ball);
  
  // the player paddle
  SDL_Rect paddle{
    static_cast<int>(mPaddlePos.x - (paddle_width/2)),
    static_cast<int>(mPaddlePos.y - (paddle_height/2)),
    paddle_width,
    paddle_height
  };
  SDL_RenderFillRect(mRenderer, &paddle);

  // time to show the resulting image
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
