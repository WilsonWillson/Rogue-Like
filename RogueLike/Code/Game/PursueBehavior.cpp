#pragma once
#include "Game/PursueBehavior.hpp"
#include "Game/Entity.hpp"
#include "Game/Map.hpp"
PursueBehavior::PursueBehavior()
{

}

PursueBehavior::~PursueBehavior()
{

}

//-----------------Utility--------------------------------------------------
float PursueBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap) 
{
	character;

	if (myself->m_target == nullptr)
		return 0.0f;
	else
	{
		IntVector2 ultamiteGoal = myself->m_target->m_CurrentCords;
		if (currentMap->GetTileDistanceBetweenTwoTiles(myself->m_HomePosition, ultamiteGoal) > m_pursueRange)
			return 0.0f;
		else
			return 1.0f;
	}
}

//----------------------------------Update------------------------------------
void PursueBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	if (myself.m_currentPath->m_path.size() != 0 && character->m_CurrentCords == myself.m_currentPath->m_path[0])
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
		IntVector2 newGoal = character->m_CurrentCords;
		myself.m_currentPath = currentMap->GenerateWalkablePath(&myself.m_CurrentCords, &newGoal, &myself);
		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
}

//------------------------------Render------------------------------------
void PursueBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	if (myself.m_currentPath->m_path.size() == 0)
		return;
	
	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* myTargetTile = currentMap->GetTileAtTileCoords(myself.m_target->m_CurrentCords);

	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myTargetTile->m_renderPosition, Rgba(255,0,0,255), Rgba(255,0,0,255));
}
