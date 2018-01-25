#include "Game/SpawnerBehavior.hpp"
#include "Game/TeleportBehavior.hpp"
#include "Game/Character.hpp"
#include "Game/Tile.hpp"
#include "Game/Map.hpp"


SpawnerBehavior::SpawnerBehavior()
{
	m_whatToSpawn = nullptr;
}

SpawnerBehavior::~SpawnerBehavior()
{

}

float SpawnerBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap)
{
	character; currentMap;
	if (myself->m_previousBehavior == nullptr)
		return 0.0f;

	if (typeid(*myself->m_previousBehavior) == typeid(TeleportBehavior))
		return 1.8f;

	return 0.0f;
}

void SpawnerBehavior::Act(Entity& myself, Entity* charcter, Map* currentMap)
{
	charcter;
	for (int amountOfEntitesToCreate = 0; amountOfEntitesToCreate < m_amountToSpawn; amountOfEntitesToCreate++)
	{
		currentMap->CreateSpecificEntitiy(m_creatureToSpawn);
		Entity* justCreatedEntity = currentMap->m_entity.back();

		justCreatedEntity->m_CurrentCords = MakeMonsterSpawnNextToMe(currentMap, myself.m_CurrentCords);

		if (justCreatedEntity->m_CurrentCords == IntVector2(-1, -1))
			currentMap->m_entity.pop_back();
	}
}

void SpawnerBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	currentMap; myself;
}


//---------------------------------------------Helper Functions---------------------------------------------------
IntVector2 SpawnerBehavior::MakeMonsterSpawnNextToMe(Map* currentMaps, IntVector2 currentCord)
{
	Tile* neighborTile = currentMaps->GetTileToTheTopOfGivenTile(currentCord);
	if (neighborTile->m_CharacterInSpace == nullptr && neighborTile->m_definition->m_isSolid != true)
		return neighborTile->m_tileCords;

	neighborTile = currentMaps->GetTileToTheBottomOfGivenTile(currentCord);
	if (neighborTile->m_CharacterInSpace == nullptr && neighborTile->m_definition->m_isSolid != true)
		return neighborTile->m_tileCords;

	neighborTile = currentMaps->GetTileToTheLeftOfGivenTile(currentCord);
	if (neighborTile->m_CharacterInSpace == nullptr && neighborTile->m_definition->m_isSolid != true)
		return neighborTile->m_tileCords;

	neighborTile = currentMaps->GetTileToTheRightOfGivenTile(currentCord);
	if (neighborTile->m_CharacterInSpace == nullptr && neighborTile->m_definition->m_isSolid != true)
		return neighborTile->m_tileCords;

	return IntVector2(-1, -1);
}

