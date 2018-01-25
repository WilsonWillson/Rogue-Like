#pragma once
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"

class LeaderBehavior : public Behavior
{
public:
	LeaderBehavior();
	~LeaderBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override;
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new LeaderBehavior(*this); }

	bool CheckForAnyFollowerActorsWithMyFaction(Entity* myself, Map* currentMap);
	void HuntDownPlayer(Entity& myself, Entity* character, Map* currentMap);
	void WanderAround(Entity& myself, Map* currentMap);
	void DoIHaveTarget(Entity& myself, Entity* character, Map* currentMap);

public:
	int m_inspireRange;
	bool m_hasFollower;

};