#pragma once
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"

class FleeBehavior : public Behavior
{
public:
	FleeBehavior();
	~FleeBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override;
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new FleeBehavior(*this); }

public:
	int m_safeDistance;
};