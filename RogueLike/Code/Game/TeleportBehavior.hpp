#pragma once
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"

class TeleportBehavior : public Behavior
{
public:
	TeleportBehavior();
	~TeleportBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override;
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new TeleportBehavior(*this); }

public:
	bool hasTeleportedYet;
};