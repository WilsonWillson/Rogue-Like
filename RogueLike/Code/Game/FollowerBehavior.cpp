#pragma once
#include "Game/FollowerBehavior.hpp"
#include "Game/LeaderBehavior.hpp"
#include "Game/Entity.hpp"
#include "Game/Map.hpp"

FollowerBehavior::FollowerBehavior()
{
	m_hasLeader = false;
	m_LeaderIsDead = false;
	m_isDevotedAfterDeath = false;
}

FollowerBehavior::~FollowerBehavior()
{

}

float FollowerBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap) 
{
	m_hasLeader = CheckForAnyLeaderActorsWithMyFaction(myself, currentMap);

	if (m_hasLeader == false && m_LeaderIsDead == false)
		return 0.0f;
	else
	{

		Tile* myTile = currentMap->GetTileAtTileCoords(myself->m_CurrentCords);
		Tile* playerTile = currentMap->GetTileAtTileCoords(character->m_CurrentCords);
		if (currentMap->HasLineOfSight(myTile->m_renderPosition, playerTile->m_renderPosition))
		{
			myself->m_target = character;
			myself->m_VisableEntities.push_back(character);
		}
	}
	return 1.3f;
}

void FollowerBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{	
	//Check if Target is Alive
	if (m_LeaderIsDead == false)
	{
		FollowTheLeader(myself, currentMap);
	}
	else
	{
		if (m_isDevotedAfterDeath == true)
			HuntDownPlayer(myself, character, currentMap);
		else
			RunAwayFromPlayer( myself, character, currentMap);
	}
}

void FollowerBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	//Draw Line to Leader
	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* myLeaderTile = currentMap->GetTileAtTileCoords(m_myLeader->m_CurrentCords);

	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myLeaderTile->m_renderPosition, Rgba(0, 0, 255, 255), Rgba(0, 0, 255, 255));


	//Draw Line to Target
	if (myself.m_currentPath->m_path.size() == 0)
		return;

	Tile* myTargetTile = currentMap->GetTileAtTileCoords(myself.m_currentPath->m_path[0]);
	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myTargetTile->m_renderPosition, Rgba(0, 255, 0, 255), Rgba(255, 0, 0, 255));
}

//----------------------------------------AI ACTIONS-------------------------------------------------------------------------------------------
void FollowerBehavior::FollowTheLeader(Entity& myself, Map* currentMap)
{
	myself.m_target = m_myLeader->m_target;

	if (m_myLeader->m_currentPath->m_path.size() == 0)
		return;
	else
	{
		IntVector2 newGoal = m_myLeader->m_currentPath->m_path[0];
		myself.m_currentPath = currentMap->GenerateWalkablePath(&myself.m_CurrentCords, &newGoal, &myself);

		if (myself.m_currentPath->m_path.size() == 0)
			return;

		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
}

void FollowerBehavior::HuntDownPlayer(Entity& myself, Entity* character, Map* currentMap)
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

void FollowerBehavior::RunAwayFromPlayer(Entity& myself, Entity* character, Map* currentMap)
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

//----------------------------------------Helper Functions--------------------------------------------------------------------------------------------

bool FollowerBehavior::CheckForAnyLeaderActorsWithMyFaction(Entity* myself, Map* currentMap)
{
	LeaderBehavior* m_tempLeader;
	for (unsigned int entityStep = 0; entityStep < currentMap->m_entity.size(); entityStep++)
	{	
		for (unsigned int behaviorStep = 0; behaviorStep < currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
		{
			if (typeid(*currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_behaviors[behaviorStep]) == typeid(LeaderBehavior))
			{
				if (currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_faction == myself->m_CharatcerDefintion->m_faction)
				{
					m_tempLeader = (LeaderBehavior*)currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_behaviors[behaviorStep];

					//Check within Range now
					int distanceBetween = currentMap->GetDistanceBetweenTwoEntities(myself, currentMap->m_entity[entityStep]);
					if (distanceBetween <= m_tempLeader->m_inspireRange)
					{
						m_myLeader = currentMap->m_entity[entityStep];
						return true;
					}
				}
			}
		}
	}
	return false;
}
