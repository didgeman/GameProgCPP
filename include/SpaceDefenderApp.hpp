// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
	// manage the Actor-objects
	void AddActor(class Actor* anActor);
	void RemoveActor(class Actor* anActor);
	// ... and special other game objects
	void AddSprite(class SpriteComponent* aSprite);
	void RemoveSprite(class SpriteComponent* aSprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	// flag to show we are in critical status of updating all the actors right now
	bool mUpdatingActors;

	//---  game data structures  ----

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	// all the actors in the game
	std::vector<class Actor*> mActors;
	// and the pending ones
	std::vector<class Actor*> mPendingActors;
	// all the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	// game objects
	class Ship* mShip;		// player's ship
};
