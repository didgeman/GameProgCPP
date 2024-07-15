// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "imgui.h"
#include "SDL_image.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "SpaceDefenderApp.hpp"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "Ship.h"
#include <algorithm>


Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mUpdatingActors(false)
{
	
}

bool Game::Initialize()
{
	//----------- Initialize SDL  ------------------------
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Space Defender in C++ (Chapter 2)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		(SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI)		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// create the Player-Ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-80.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	//-----------  Init IMGUI  -----------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup the Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(mWindow, mRenderer);
	ImGui_ImplSDLRenderer2_Init(mRenderer);
		
	//-----------  init SDL-Image library  ---------
	IMG_Init(IMG_INIT_PNG);

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// process the input for the ship
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// move any pending actors to the actors-List
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor:mActors)
	{
		if (actor->GetState() == Actor::EDead)
			deadActors.emplace_back(actor);
	}

	// Delete dead actors which automatically (how?) removes them also from mActors
	for (auto actor : deadActors)
	{
		delete actor;
	}

}

void Game::GenerateOutput()
{
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	static ImGuiIO &io = ImGui::GetIO();


	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);
	SDL_SetRenderDrawColor(mRenderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	
	// Draw all sprite components
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), mRenderer);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	// unload game data (actors...)
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	// and their textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

	//--------  SDL-Image library cleanup ----------
	IMG_Quit();

	//-------- Cleanup Imgui resources  ------------
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//-------- Cleanup SDL resources  --------------
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* anActor)
{
	// if in updating process add to pending actors
	if (mUpdatingActors) {
		mPendingActors.emplace_back(anActor);
	}
	else {
		mActors.emplace_back(anActor);
	}
}

void Game::RemoveActor(Actor* anActor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), anActor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), anActor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* aSprite)
{
	// find the insertion point in the sorted vector
	// i.e the first element with a higher draw order than me
	int myDrawOrder = aSprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if(myDrawOrder < (*iter)->GetDrawOrder())
			break;
	}

	// inserts element before position of iterator
	mSprites.insert(iter, aSprite);
}

void Game::RemoveSprite(SpriteComponent* aSprite)
{
	// we can't swap because it ruins ordering
	auto iter = std::find(mSprites.begin(), mSprites.end(), aSprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);		// iter should be of type sthg. "std::pair<std::string, SDL_Texture*>*"
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// else load texture from file right here
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s (Err:%s)", fileName.c_str(), SDL_GetError());
			return nullptr;
		}
		// Create Texture from loaded surface data
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s (Err: %s)", fileName.c_str(), SDL_GetError());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
