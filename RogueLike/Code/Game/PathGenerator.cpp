#include "Game/PathGenerator.hpp"
#include "Game/Path.hpp"
#include "Game/Map.hpp"

PathGenerator::PathGenerator()
{
}

PathGenerator::PathGenerator(Map* map, IntVector2* start, IntVector2* end, Entity* entity)
{
	static int nextPathID = 0;
	nextPathID++;
	m_pathID = nextPathID;
	m_start = start;
	m_end = end;
	m_gCostReferenceActor = entity;
	m_currentMap = map;
	OpenPathNode(start, nullptr);
}

PathGenerator::~PathGenerator()
{

}


bool PathGenerator::Step(Path* generatedPath)
{
	if (m_OpenNodes.empty())
		return true;

	OpenNode* selectedNode = GetAndCloseBestNode();

	if (selectedNode->m_tile->m_tileCords == *m_end)
	{
		BuildFinalPath(*selectedNode, generatedPath);
		return true;
	}

	Tile* leftNeighbor = m_currentMap->GetTileToTheLeftOfGivenTile(selectedNode->m_tile->m_tileCords);
	OpenNeighborIfValid(selectedNode, leftNeighbor);

	Tile* rightNeighbor = m_currentMap->GetTileToTheRightOfGivenTile(selectedNode->m_tile->m_tileCords);
	OpenNeighborIfValid(selectedNode, rightNeighbor);

	Tile* topNeighbor = m_currentMap->GetTileToTheTopOfGivenTile(selectedNode->m_tile->m_tileCords);
	OpenNeighborIfValid(selectedNode, topNeighbor);

	Tile* bottomNeighbor = m_currentMap->GetTileToTheBottomOfGivenTile(selectedNode->m_tile->m_tileCords);
	OpenNeighborIfValid(selectedNode, bottomNeighbor);

	return false;
}

OpenNode* PathGenerator::GetAndCloseBestNode()
{
	OpenNode* desieredNode = new OpenNode();
	float fScore = 9999999;
	unsigned int desieredIndex = 0;
	for (unsigned int nodeIndex = 0; nodeIndex < m_OpenNodes.size(); nodeIndex++)
	{
		if (fScore > m_OpenNodes[nodeIndex]->m_fscore)
		{
			fScore = m_OpenNodes[nodeIndex]->m_fscore;
			desieredIndex = nodeIndex;
			desieredNode = m_OpenNodes[nodeIndex];
		}

	}
	m_ClosedNodes.push_back(m_OpenNodes[desieredIndex]);
	m_OpenNodes[desieredIndex] = m_OpenNodes.back();
	m_OpenNodes.pop_back();

	return desieredNode;
}

void PathGenerator::BuildFinalPath(const OpenNode& goalNode, Path* path)
{
	const OpenNode* currentNode = &goalNode;
	while (currentNode)
	{
		path->m_path.push_back(currentNode->m_tile->m_tileCords);
		currentNode = currentNode->m_parent;
	}
	path->m_path.pop_back();
}


void PathGenerator::OpenNeighborIfValid(OpenNode* selectedNode, Tile* neighbor)
{
	if (neighbor == nullptr)
		return;
	if (neighbor->m_pathID == m_pathID)
		return;
	if (neighbor->m_definition->m_isSolid == true)
		return;

	OpenPathNode(&neighbor->m_tileCords, selectedNode);
}

void PathGenerator::OpenPathNode(const IntVector2* start, OpenNode* parent)
{
	float hImporatanceMultiplier = 1.01f;
	OpenNode* openNode = new OpenNode();
	openNode->m_tile = m_currentMap->GetTileAtTileCoords(*start);
	openNode->m_parent = parent;
	openNode->m_estimateDistanceToGoal = CalculateManhattenDistance(*start, *m_end);
	openNode->m_localGCost = openNode->m_tile->m_definition->m_gCost;

	//If Enemy in Tile localGCost goes up like its a wall
	if (openNode->m_tile->m_CharacterInSpace != nullptr)
	{
		openNode->m_localGCost += 999;
	}

	if (parent != nullptr)
		openNode->m_totalGCost = openNode->m_localGCost + openNode->m_parent->m_totalGCost;
	else
		openNode->m_totalGCost = openNode->m_localGCost;

	openNode->m_fscore = (hImporatanceMultiplier * openNode->m_estimateDistanceToGoal) + openNode->m_totalGCost;
	openNode->m_tile->m_pathID = m_pathID;

	m_OpenNodes.push_back(openNode);
}