#pragma once
#include "Game/Tile.hpp"
#include "Engine/Math/MathUtils.hpp"

class OpenNode
{
public:
	Tile* m_tile;
	Tile* m_origin;
	OpenNode* m_parent;
	float m_localGCost;
	float m_totalGCost;
	float m_estimateDistanceToGoal;
	float m_fscore;

public:
	OpenNode() {};
	~OpenNode() {};
};

class Path;

class PathGenerator
{
public:
	PathGenerator();
	PathGenerator(Map* map, IntVector2* start, IntVector2* end, Entity* entity);
	~PathGenerator();

	bool Step(Path* generatedPath);

private:
	void OpenNeighborIfValid(OpenNode* selectedNode, Tile* leftNeighbor);
	OpenNode* GetAndCloseBestNode();
	void BuildFinalPath(const OpenNode& goalNode, Path* path);
	void OpenPathNode(const IntVector2* start, OpenNode* parent);

public:
	IntVector2* m_start;
	IntVector2* m_end;
	Map*		m_currentMap;
	Entity*		m_gCostReferenceActor;
	std::vector<OpenNode*>	m_OpenNodes;
	std::vector<OpenNode*>	m_ClosedNodes;
	int			m_pathID;
	int			m_GCost;
};