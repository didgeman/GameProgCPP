// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// define a distinct animation, based on the textures-list
	void DefineAnimation(std::string name, int startIdx, int endIdx, bool isLooping = true);
	void SetCurrentAnim(std::string name);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	struct AnimDef {
		std::string name;
		int startIdx;
		int endIdx;
		bool isLooping;

		AnimDef(const std::string& strName,
						int iStart, int iEnd, bool isLooping = true)
			: name(strName),
				startIdx(iStart),
				endIdx(iEnd),
				isLooping(isLooping) {}

		AnimDef(const std::string& strName = "")
			: name(strName),
				startIdx(0),
				endIdx(0),
				isLooping(true) {}

		bool operator==(const AnimDef& rhs) const {
			return this->name == rhs.name;
		}
	};
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// keeps all defined animations
	std::vector<AnimDef> mAnimations;
	// the active animation
	AnimDef mActiveAnim;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};
