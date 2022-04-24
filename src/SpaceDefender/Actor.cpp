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
  if (mState == EActive)
  {
    UpdateComponents(deltaTime);
    UpdateActor(deltaTime);
  }
}

void Actor::UpdateComponents(float deltaTime)
{
  for (auto comp : mComponents)
  {
    comp->Update(deltaTime);
  }
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(Component* aComponent)
{
  // Find the insertion point in the sorted vector
  // (i.e. the first element with an order higher than the new Component
  int myOrder = aComponent->GetUpdateOrder();
  auto iter = mComponents.begin();
  for (; iter != mComponents.end(); ++iter)
  {
    if (myOrder < (*iter)->GetUpdateOrder())
      break;
  }

  // Insert element before position of iterator
  mComponents.insert(iter, aComponent);
}

void Actor::RemoveComponent(Component* aComponent)
{
  auto iter = std::find(mComponents.begin(), mComponents.end(), aComponent);
  if (iter != mComponents.end())
    mComponents.erase(iter);
}