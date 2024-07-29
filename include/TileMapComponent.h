#pragma once

#include "SpriteComponent.h"

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* theOwner, int drawOrder = 100);
	~TileMapComponent();

private:

};

