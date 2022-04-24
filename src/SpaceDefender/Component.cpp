#include "Component.h"
#include "Actor.h"

Component::Component(Actor* theOwner, int updateOrder)
    :mOwner(theOwner)
    ,mUpdateOrder(updateOrder)
{
  // add to actor's list of components
  mOwner->AddComponent(this);
}

Component::~Component()
{
  mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{

}
