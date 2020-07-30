#pragma once

#include "SDL.h"

const int thickness = 15;
const int ball_width = 15;
const int ball_height = 15;
const int paddle_height = 45;
const int paddle_width = 15;

struct Vector2 {
  float x;
  float y;
};

class Game {
  public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
    Vector2 mPaddlePos;
    Vector2 mBallPos;
    Vector2 mBallVelocity;
    int mPaddleDir;
    float GetElapsedTime();
  private:
    // game loop helper functions
    void ProcessInput();
    void UpdateGame(float deltaTime);    // apply evolve rules to the world
    void GenerateOutput();

    Uint32 mCurrentTicks;

    SDL_Window* mWindow;   // reference to our main SDL-window
    bool mIsRunning;       // controls lifetime of our App
    SDL_Renderer* mRenderer;   // reference to SDL's render system

};