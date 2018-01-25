#include "Game/TeleportBehavior.hpp"
#include "Game/Map.hpp"

TeleportBehavior::TeleportBehavior()
{
	hasTeleportedYet = false;
}

TeleportBehavior::~TeleportBehavior()
{

}

float TeleportBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap)
{
	currentMap; character;
	if (hasTeleportedYet)
		return 0.0f;

	float healthPercent = (float)myself->m_currentStats.m_stamina / (float)myself->m_CharatcerDefintion->m_baseStats.m_stamina;
	if (healthPercent < .50f)
		return 2.5f;

	return 0.0f;
}

void TeleportBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	character; currentMap;
	myself.m_CurrentCords = currentMap->FindRandomWalkableSpot();
	hasTeleportedYet = true;
}

void TeleportBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	currentMap; myself;
}
