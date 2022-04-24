#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* anOwner, int drawOrder)
    :SpriteComponent(anOwner, drawOrder)
    ,mScrollSpeed(0.0f)
{
	
}

void BGSpriteComponent::Update(float deltaTime)
{
  // give the base class a chance for doing its stuff
  SpriteComponent::Update(deltaTime);
  for (auto& bg : mBGTextures)
  {
    // Update the x offset
    bg.mOffset.x += mScrollSpeed * deltaTime;
    if (bg.mOffset.x < -mScreenSize.x)
    {
      bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
    }
  }
}

void BGSpriteComponent::Draw(SDL_Renderer* aRenderer)
{
	// Draw each background texture
	for (auto& bg : mBGTextures)
	{
		SDL_Rect r;
		// Assume screen size dimensions
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);

		// Draw this background
		SDL_RenderCopy(aRenderer,
			bg.mTexture,
			nullptr,
			&r
		);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		// Each texture is screen width in offset
		temp.mOffset.x = count * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		++count;
	}
}
