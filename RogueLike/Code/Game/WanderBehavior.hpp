#pragma once
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"
class WanderBehavior : public Behavior
{
public:
	WanderBehavior();
	~WanderBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override; //{ return 1.0f; }
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new WanderBehavior(*this); }

public:
	int m_wanderRange;
};