#pragma once

#include "SDL.h"

class Game {
  public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
  private:
    // game loop helper functions
    void ProcessInput();
    void UpdateGame();    // apply evolve rules to the world
    void GenerateOutput();

    SDL_Window* mWindow;   // reference to our main SDL-window
    bool mIsRunning;       // controls lifetime of our App
};