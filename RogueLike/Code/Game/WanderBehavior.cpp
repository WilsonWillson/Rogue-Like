#pragma once
#include "Game/WanderBehavior.hpp"
#include "Game/PursueBehavior.hpp"
#include "Game/Map.hpp"
#include "Game/Tile.hpp"
//-------------------------Constructor/Deconstructor----------------------------
WanderBehavior::WanderBehavior()
{

}

WanderBehavior::~WanderBehavior()
{

}


//-------------------------Utility-----------------------------------------------
float WanderBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap) 
{
	IntVector2 ultamiteGoal;
	PursueBehavior* pursueBehavior;

	//Do We see a target
	Tile* myTile = currentMap->GetTileAtTileCoords(myself->m_CurrentCords);
	Tile* playerTile = currentMap->GetTileAtTileCoords(character->m_CurrentCords);
	if (currentMap->HasLineOfSight(myTile->m_renderPosition, playerTile->m_renderPosition))
	{
		myself->m_target = character;
		ultamiteGoal = myself->m_target->m_CurrentCords;
		myself->m_VisableEntities.push_back(character);
	}

	//Check if they are in our Pursue Ranger
	for (unsigned int behaviorStep = 0; behaviorStep < myself->m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
	{
		if (typeid(*myself->m_CharatcerDefintion->m_behaviors[behaviorStep]) == typeid(PursueBehavior))
		{
			pursueBehavior = (PursueBehavior*)myself->m_CharatcerDefintion->m_behaviors[behaviorStep];
			
			if (currentMap->GetTileDistanceBetweenTwoTiles(myself->m_HomePosition, ultamiteGoal) > pursueBehavior->m_pursueRange)
				return 1.0f;
		}
	}

	if (myself->m_target == nullptr)
		return 1.0f;
	if (myself->m_target != nullptr)
		return 0.0f;
	
	return 0.0f;
}

//-----------------------------Update---------------------------------------
void WanderBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	if (myself.m_currentPath->m_path.size() != 0)
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
		bool legalSpot = false;
		IntVector2 newGoal;
		while (legalSpot == false)
		{
			newGoal = currentMap->FindRandomWalkableSpot();
			if (currentMap->GetTileDistanceBetweenTwoTiles(myself.m_HomePosition, newGoal) <= m_wanderRange)
				legalSpot = true;
		}
		myself.m_currentPath = currentMap->GenerateWalkablePath(&myself.m_CurrentCords, &newGoal, &myself);

		if (myself.m_currentPath->m_path.size() == 0)
			return;
		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile == nullptr || destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
	character;
}

//---------------------------------Render--------------------------------------
void WanderBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	if (myself.m_currentPath->m_path.size() == 0)
		return;

	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* myTargetTile = currentMap->GetTileAtTileCoords(myself.m_currentPath->m_path[0]);

	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myTargetTile->m_renderPosition, Rgba(255,255, 255, 255), Rgba(255, 255, 255, 255));
}
