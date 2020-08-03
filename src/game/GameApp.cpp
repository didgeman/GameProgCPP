#include <cmath>
#include "GameApp.hpp"

Game::Game() {
  mIsRunning = true;
  mWindow = nullptr;
  mBallPos.x = 512.0f;
  mBallPos.y = 384.0f;
  mBallVelocity = {-200.0f, 235.0f};
  // Player 1 paddle
  mP1_PaddlePos.x = 10.0f;
  mP1_PaddlePos.y = 384.0f;
  mP1_PaddleDir = 0;
  // Player 2 paddle
  mP2_PaddlePos.x = 1010.0f;
  mP2_PaddlePos.y = 384.0f;
  mP2_PaddleDir = 0;

  mCurrentTicks = SDL_GetTicks();
}

/// <summary>
/// calculates elapsed time since last call of this function
/// </summary>
/// <returns>time in seconds</returns>
float Game::GetElapsedTime() {
  Uint32 currTime = SDL_GetTicks();
  float delta = (currTime - mCurrentTicks) / 1000.0f;
  mCurrentTicks = currTime;   // store current time for next watchstop action in future
  return delta;
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
  
  // the paddle's
  SDL_Rect paddle {
    static_cast<int>(mP1_PaddlePos.x - (paddle_width/2)),
    static_cast<int>(mP1_PaddlePos.y - (paddle_height/2)),
    paddle_width,
    paddle_height
  };
  SDL_RenderFillRect(mRenderer, &paddle);

  paddle.x = static_cast<int>(mP2_PaddlePos.x - (paddle_width / 2));
  paddle.y = static_cast<int>(mP2_PaddlePos.y - (paddle_width / 2));
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
  // check for paddle player1 movement
  mP1_PaddleDir = 0;
  if(keyb_state[SDL_SCANCODE_W])
    mP1_PaddleDir = -1;
  if (keyb_state[SDL_SCANCODE_S])
    mP1_PaddleDir = 1;
  // check for paddle player2 movement
  mP2_PaddleDir = 0;
  if (keyb_state[SDL_SCANCODE_I])
    mP2_PaddleDir = -1;
  if (keyb_state[SDL_SCANCODE_K])
    mP2_PaddleDir = 1;

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

void Game::UpdateGame(float deltaTime) {
  // update the Player1 (left side) paddle position
  if (mP1_PaddleDir != 0) {
    mP1_PaddlePos.y += mP1_PaddleDir * 300.0f * deltaTime;
    if(mP1_PaddlePos.y < (0 + thickness + paddle_height/2.0f))
      mP1_PaddlePos.y = thickness + (paddle_height / 2.0f);
    else if(mP1_PaddlePos.y > (768 - thickness - (paddle_height / 2.0f)))
      mP1_PaddlePos.y = 768 - thickness - (paddle_height / 2.0f);
  }

  // update the Player2 (right side) paddle position
  if (mP2_PaddleDir != 0) {
    mP2_PaddlePos.y += mP2_PaddleDir * 300.0f * deltaTime;
    if (mP2_PaddlePos.y < (0 + thickness + paddle_height / 2.0f))
      mP2_PaddlePos.y = thickness + (paddle_height / 2.0f);
    else if (mP2_PaddlePos.y > (768 - thickness - (paddle_height / 2.0f)))
      mP2_PaddlePos.y = 768 - thickness - (paddle_height / 2.0f);
  }
    
  // update the ball
  mBallPos.x += mBallVelocity.x * deltaTime;
  mBallPos.y += mBallVelocity.y * deltaTime;
  // check for collisions with the walls
  if((mBallPos.y - ball_height/2.0f <= 0 + thickness) &&
      mBallVelocity.y < 0.0f)    // with top wall only when moving upwards
    mBallVelocity.y = -1 * mBallVelocity.y;
  if((mBallPos.y + ball_height / 2.0f >= 768 - thickness) &&
      mBallVelocity.y > 0.0f)    // with bottom wall when moving downwards
    mBallVelocity.y = -1 * mBallVelocity.y;
  /*
  if(mBallPos.x >= (1024 - ball_width/2.0f))
    mBallVelocity.x = -1 * mBallVelocity.x;
  */

  // collision with the player1 paddle?
  if(
     (std::abs(mP1_PaddlePos.y - mBallPos.y) < (paddle_height/2.0f + ball_height/2.0f)) &&
     (mBallPos.x <= mP1_PaddlePos.x + thickness/2.0f) &&
     (mBallVelocity.x < 0)
    )
    mBallVelocity.x = -1 * mBallVelocity.x;
  // collision with the player2 paddle?
  if (
      (std::abs(mP2_PaddlePos.y - mBallPos.y) < (paddle_height / 2.0f + ball_height / 2.0f)) &&
      (mBallPos.x >= mP2_PaddlePos.x - thickness / 2.0f) &&
      (mBallVelocity.x > 0)
     )
    mBallVelocity.x = -1 * mBallVelocity.x;

  // is ball beyond paddle?
  if (mBallPos.x < 0 || mBallPos.x > 1024) {
    mBallPos = { 512.0f, 384.0f };
    mBallVelocity = { -200.0f, 235.0f };
  }
    
}

void Game::RunLoop() {
  float deltaTime;
  int waitTime;

  while (mIsRunning) {
    deltaTime = GetElapsedTime();
    // ensure frame rate cap by waiting
    waitTime = 16 - int(deltaTime * 1000.0f + 0.5f);
    if (waitTime > 0) {
      SDL_Delay(std::abs(waitTime));
      deltaTime = deltaTime + waitTime;
    }
    ProcessInput();
    UpdateGame(deltaTime);
    GenerateOutput();
  }
}

void Game::Shutdown() {
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}
