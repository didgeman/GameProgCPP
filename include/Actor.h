#pragma once
#include "MadMath.h"
#include <SpaceDefenderApp.hpp>

/// represents a game object which has interactive, dynamic capabilities.
class Actor {
 public :
  enum State  /* for tracking various states of an an actor */
  {
    EActive,
    EPause,
    EDead
  };
  // ctor/dtor
  Actor(Game* aGame);
  virtual ~Actor();

  /// Update function called from Game (not overridable)
  /// <param name="deltaTime">passed time since starting point in the past</param>
  void Update(float deltaTime);
  // Updates all components attached to the Actor (not overridable)
  void UpdateComponents(float deltaTime);
  // Any actor-specific update code (overridable)
  virtual void UpdateActor(float deltaTime);
  
  class Game* GetGame() { return mGame; }
  
  // Add/Remove components
  void AddComponent(class Component* aComponent);
  void RemoveComponent(class Component* aComponent);
  // Getters/Setters
  void SetState(State newState) { mState = newState; }
  State GetState() const { return mState; }
  float GetScale() const { return mScale; }
  void SetScale(float scale) { mScale = scale; }
  float GetRotation() const { return mRotation; }
  void SetRotation(float rotation) { mRotation = rotation; }

  void SetPosition(const Vector2 newPos) { mPosition = newPos; }
  const Vector2& GetPosition() const { return mPosition; }

private:
  // Actor's state 
  State mState;
  // Transform
  Vector2 mPosition;  // Center position of actor
  float mScale;       // Uniform scale of actor (1.0f for 100%)
  float mRotation;    // ? degree? radians?
  // Components held by this actor
  std::vector<class Component*> mComponents;
  class Game* mGame;
};
