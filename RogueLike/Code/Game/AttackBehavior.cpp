#include "Game/AttackBehavior.hpp"
#include "Game/Entity.hpp"
#include "Game/Map.hpp"

AttackBehavior::AttackBehavior()
{

}

AttackBehavior::~AttackBehavior()
{

}

float AttackBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap)
{
	if (myself->m_target == nullptr)
		return 0.f;
	else
	{
		if (currentMap->GetTileDistanceBetweenTwoTiles(myself->m_CurrentCords, character->m_CurrentCords) <= 1)
			return 2.0f;
		else
			return 0.f;
	}
}

void AttackBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	currentMap;
	if (character->m_CharatcerDefintion->m_faction != myself.m_CharatcerDefintion->m_faction)
		character->m_currentStats.m_stamina -= myself.m_currentStats.m_strength;

	character->m_damageTaken.push_back(myself.m_currentStats.m_strength);
	character->m_LastTimeSinceDamageTaken.push_back(0);
}

void AttackBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);

	g_renderSystem->DrawAABB2(AABB2(myTile->m_renderPosition, 25, 25), Rgba(255, 0, 0, 255));
}
