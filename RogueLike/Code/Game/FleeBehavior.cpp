#include "Game/FleeBehavior.hpp"
#include "Game/Map.hpp"
FleeBehavior::FleeBehavior()
{

}

FleeBehavior::~FleeBehavior()
{

}

//----------------------Utility------------------------------------------------
float FleeBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap)
{
	currentMap;
	float fleeUtility =0.0f; 
	
	if (character->m_target == nullptr)
		fleeUtility = 0.0f;
	else
	{
		float healthPercent = (float)(myself->m_currentStats.m_stamina / (float)myself->m_CharatcerDefintion->m_baseStats.m_stamina);
		if (healthPercent < .25)
			fleeUtility = 1.0f + healthPercent;
	}
	return fleeUtility;
}

//----------------------------Update----------------------------------------------------
void FleeBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	if (myself.m_currentPath != nullptr || myself.m_currentPath->m_path.size() != 0)
	{
		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
	else
	{
		IntVector2 newGoal;
		int distance = -999999;
		for (int newPositionStep = 0; newPositionStep < 10; newPositionStep++)
		{
			IntVector2 tempPosition = currentMap->FindRandomWalkableSpot();
			int tempDistance = currentMap->GetTileDistanceBetweenTwoTiles(character->m_CurrentCords, tempPosition);
			if (tempDistance > distance)
			{
				newGoal = tempPosition;
				tempDistance = distance;
			}
		}
		myself.m_currentPath = currentMap->GenerateWalkablePath(&myself.m_CurrentCords, &newGoal, &myself);
		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
}

//------------------------------------Render---------------------------------------------------
void FleeBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	if (myself.m_currentPath->m_path.size() == 0)
		return;
	
	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* myTargetTile = currentMap->GetTileAtTileCoords(myself.m_currentPath->m_path[0]);

	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myTargetTile->m_renderPosition, Rgba(255, 105, 180, 255), Rgba(255, 105, 180, 255));
}

