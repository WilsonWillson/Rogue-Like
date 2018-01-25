#pragma once
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"

class Entity;

class FollowerBehavior : public Behavior
{
public:
	FollowerBehavior();
	~FollowerBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override;
	virtual void Act(Entity& myself, Entity* character, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new FollowerBehavior(*this); }

	void FollowTheLeader(Entity& myself, Map* currentMap);
	void HuntDownPlayer(Entity& myself, Entity* character, Map* currentMap);
	void RunAwayFromPlayer(Entity& myself, Entity* character, Map* currentMap);
	bool CheckForAnyLeaderActorsWithMyFaction(Entity* myself, Map* currentMap);

public:
	int m_devotion;
	bool m_hasLeader;
	bool m_LeaderIsDead;
	bool m_isDevotedAfterDeath;
	Entity* m_myLeader;

};