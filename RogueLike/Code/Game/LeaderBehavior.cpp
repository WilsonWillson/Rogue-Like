#include "Game/LeaderBehavior.hpp"
#include "Game/FollowerBehavior.hpp"
#include "Game/PursueBehavior.hpp"
#include "Game/Entity.hpp"
#include "Game/Map.hpp"

LeaderBehavior::LeaderBehavior()
{
	m_hasFollower = true;
}

LeaderBehavior::~LeaderBehavior()
{

}

float LeaderBehavior::CalcUtility(Entity* character, Entity* myself, Map* currentMap)
{
	character;
	m_hasFollower = CheckForAnyFollowerActorsWithMyFaction(myself, currentMap);

	if (m_hasFollower == false)
		return 0.0f;
	if (m_hasFollower == true)
		return 1.3f;

	return 0.0f;
}

void LeaderBehavior::Act(Entity& myself, Entity* character, Map* currentMap)
{
	//Determine If I Have Target
	DoIHaveTarget(myself,character, currentMap);

	if (myself.m_target != nullptr)
		HuntDownPlayer(myself, character, currentMap);
	else
		WanderAround(myself, currentMap);
}

void LeaderBehavior::DebugRender(Entity& myself, Map* currentMap) const
{
	if (myself.m_currentPath->m_path.size() == 0)
		return;

	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* myTargetTile = currentMap->GetTileAtTileCoords(myself.m_currentPath->m_path[0]);

	g_renderSystem->DrawLine2D(myTile->m_renderPosition, myTargetTile->m_renderPosition, Rgba(0, 255, 0, 255), Rgba(255, 0, 0, 255));
}

//----------------------------------------AI Actions--------------------------------------------------------------
void LeaderBehavior::HuntDownPlayer(Entity& myself, Entity* character, Map* currentMap)
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

void LeaderBehavior::WanderAround(Entity& myself, Map* currentMap)
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
		IntVector2 newGoal = currentMap->FindRandomWalkableSpot();
		myself.m_currentPath = currentMap->GenerateWalkablePath(&myself.m_CurrentCords, &newGoal, &myself);
		IntVector2 destinationCoords = myself.m_currentPath->m_path.back();

		Tile* destinationTile = currentMap->GetTileAtTileCoords(destinationCoords);
		if (destinationTile->m_CharacterInSpace != nullptr)
			return;

		myself.m_currentPath->m_path.pop_back();
		myself.m_CurrentCords = destinationCoords;
	}
}

//----------------------------------------Helper Functions------------------------------------------------
bool LeaderBehavior::CheckForAnyFollowerActorsWithMyFaction(Entity* myself, Map* currentMap)
{
	for (unsigned int entityStep = 0; entityStep < currentMap->m_entity.size(); entityStep++)
	{

		for (unsigned int behaviorStep = 0; behaviorStep < currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
		{
			if (typeid(*currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_behaviors[behaviorStep]) == typeid(FollowerBehavior))
			{
				if (currentMap->m_entity[entityStep]->m_CharatcerDefintion->m_faction == myself->m_CharatcerDefintion->m_faction)
				{
					//Check within Range now
					int distanceBetween = currentMap->GetDistanceBetweenTwoEntities(myself, currentMap->m_entity[entityStep]);
					if (distanceBetween <= m_inspireRange)
						return true;
				}
			}
		}
	}
	return false;
}

void LeaderBehavior::DoIHaveTarget(Entity& myself, Entity* character, Map* currentMap)
{
	PursueBehavior* pursueBehavior;
	
	Tile* myTile = currentMap->GetTileAtTileCoords(myself.m_CurrentCords);
	Tile* playerTile = currentMap->GetTileAtTileCoords(character->m_CurrentCords);
	if (currentMap->HasLineOfSight(myTile->m_renderPosition, playerTile->m_renderPosition))
	{
		//Check if they are in our Pursue Ranger
		for (unsigned int behaviorStep = 0; behaviorStep < myself.m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
		{

			if (typeid(*myself.m_CharatcerDefintion->m_behaviors[behaviorStep]) == typeid(PursueBehavior))
			{
				pursueBehavior = (PursueBehavior*)myself.m_CharatcerDefintion->m_behaviors[behaviorStep];
				if (currentMap->GetTileDistanceBetweenTwoTiles(myself.m_HomePosition, playerTile->m_tileCords) > pursueBehavior->m_pursueRange)
					return;
				else
				{
					myself.m_target = character;
					myself.m_VisableEntities.push_back(character);
					return;
				}

			}
		}	
	}
}
