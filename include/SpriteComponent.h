#pragma once

#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component
{
public:
  SpriteComponent(class Actor* theOwner, int drawOrder = 100);
  ~SpriteComponent();

  virtual void Draw(SDL_Renderer* renderer);
  virtual void SetTexture(SDL_Texture* aTex);

  int GetDrawOrder() { return mDrawOrder; }

private:
  SDL_Texture* mTexture;
  int mDrawOrder;         // used to control the painter alogrithm
  // dimensions of the held texture
  int mTexWidth;
  int mTexHeight;
};
