#pragma once

#include "SDL.h"
#include <vector>

const int thickness = 15;
const int ball_width = 15;
const int ball_height = 15;
const int paddle_height = 45;
const int paddle_width = 15;

struct Vector2 {
  float x;
  float y;
};

struct Ball {
  Vector2 position;
  Vector2 velocity;
  SDL_Color color;
};

class Game {
  public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
    Vector2 mP1_PaddlePos;
    int mP1_PaddleDir;
    Vector2 mP2_PaddlePos;
    int mP2_PaddleDir;

    std::vector<Ball> balls;    /* should we specify the expected size = number of balls? (e.g. max. 3) */
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