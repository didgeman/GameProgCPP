#include "SpriteComponent.h"
#include "Actor.h"
#include "SpaceDefenderApp.hpp"

SpriteComponent::SpriteComponent(Actor* theOwner, int drawOrder)
      :Component(theOwner)
      ,mTexture(nullptr)
      ,mDrawOrder(drawOrder)
      ,mTexWidth(0)
      ,mTexHeight(0)
{
  mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
  if (mTexture)
  {
    SDL_Rect r;
    // Scale the width/height by owner's scale
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    // Center the rectangle around the position of the owner
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

    // Draw (have to convert angle from radians to degrees, and clockwise to counter)
    SDL_RenderCopyEx(renderer,
      mTexture,
      nullptr,
      &r,
      -MadMath::ToDegrees(mOwner->GetRotation()),
      nullptr,
      SDL_FLIP_NONE);
  }
}

void SpriteComponent::SetTexture(SDL_Texture* aTex)
{
  mTexture = aTex;
  // Set width/height
  SDL_QueryTexture(aTex, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
