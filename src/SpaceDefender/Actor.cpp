#include "Actor.h"
#include "Component.h"


Actor::Actor(Game* aGame)
    :mState(EActive)
    ,mPosition(Vector2::Zero)
    ,mScale(1.0f)
    ,mRotation(0.0f)
    ,mGame(aGame)
{
  mGame->AddActor(this);
}

Actor::~Actor()
{
  mGame->RemoveActor(this);
  // we need to delete all our attached components
  // but beware: a deleted component removes itselft from our vector 
  //  are just iterating over!  so we use a suitable loop style:
  while (!mComponents.empty())
  {
    delete mComponents.back();
  }
}

void Actor::Update(float deltaTime)
{
  
}

void Actor::UpdateComponents(float deltaTime)
{
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(Component* aComponent)
{
}

void Actor::RemoveComponent(Component* aComponent)
{
}