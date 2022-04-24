#pragma once

class Component
{
public:
  // ctor
  Component(class Actor* theOwner, int updateOrder = 100);
  // dtor
  virtual ~Component();
  // Update this component by deltaTime
  virtual void Update(float deltaTime);

  int GetUpdateOrder() const { return mUpdateOrder; }

protected:
  // Owning actor
  class Actor* mOwner;
  // Update order of this component
  int mUpdateOrder;   // the lower, the earlier the component updates
};
