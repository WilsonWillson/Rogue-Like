#pragma once
#include "Engine/Math/IntVector2.hpp"
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"
#include <string>
class SpawnerBehavior : public Behavior
{
public:
	SpawnerBehavior();
	~SpawnerBehavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) override;
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) override;
	virtual void DebugRender(Entity& myself, Map* currentMap) const override;
	virtual Behavior* Clone() const override { return new SpawnerBehavior(*this); }

private:
	IntVector2 MakeMonsterSpawnNextToMe(Map* currentMap, IntVector2 currentCords);


public:
	Entity* m_whatToSpawn;
	std::string m_creatureToSpawn;
	int m_amountToSpawn;

};