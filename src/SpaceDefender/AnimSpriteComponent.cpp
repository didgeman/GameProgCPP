// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(12.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;
		
		// Wrap current frame if needed
		while (mCurrFrame >= mActiveAnim.endIdx+1)
		{
			mCurrFrame = mActiveAnim.startIdx;
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);  // TODO: check if this is compatible with the multiple-animations feature.
	}
}

void AnimSpriteComponent::DefineAnimation(std::string name, int startIdx, int endIdx, bool isLooping)
{
	AnimDef tmpData {
		name,
		startIdx,
		endIdx,
		isLooping
	};
	mAnimations.push_back(tmpData);
}

void AnimSpriteComponent::SetCurrentAnim(std::string anim_name)
{
	auto iter = std::find(mAnimations.begin(), mAnimations.end(), anim_name);
	if (iter != mAnimations.end())
		mActiveAnim = *iter;
	mCurrFrame = (*iter).startIdx;
}
