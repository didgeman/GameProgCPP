#pragma once

#include <vector>
#include "SpriteComponent.h"
#include "MadMath.h"

class BGSpriteComponent : public SpriteComponent
{
public:
  // ctor with default to a very low draw order, so it's in the background
  BGSpriteComponent(class Actor* anOwner, int drawOrder = 10);
  // Update and Draw-methods are overridden from parent
  void Update(float deltaTime) override;
  void Draw(SDL_Renderer* aRenderer) override;
  // set the textures used for the background
  void SetBGTextures(const std::vector<SDL_Texture*>& textures);

  // Getters&Setters for ScrollSpeed and ScreenSize
  void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
  float GetScrollSpeed() { return mScrollSpeed; }
  void SetScreenSize(const Vector2& aSize) { mScreenSize = aSize; }

private:
  // structure to encapsulate each bg image and its offset
  struct BGTexture
  {
    SDL_Texture* mTexture;
    Vector2 mOffset;
  };
  std::vector<BGTexture> mBGTextures;
  Vector2 mScreenSize;
  float mScrollSpeed;
};