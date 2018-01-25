#pragma once

#include "Game/DialogBoxes.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/Tile.hpp"
#include "Game/Entity.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Path.hpp"
#include "Game/PathGenerator.hpp"
#include <vector>


class DialogBoxes;


class Map
{

public:
	Map();
	Map(std::string MapName, Item* questItem, std::string ExitType, int powerLevel, int MaxPowerLevel);
	~Map();
	void CreateEntitiy();
	void CreateSpecificEntitiy(std::string entityWantedToCreate);


	void Update(bool playerActionHasHappened, Character* m_Player, float deltaSeconds);
	void SetVisibilityOfMap(Character* m_Player);
	void KillEntity(size_t entityStep);

	void Render(Character* m_Player);
	void RenderTiles(int tileStep, float baseLocationX, float baseLocationY);
	void RenderEntitiesAndItems(Character* m_Player, int tileStep, float baseLocationX, float baseLocationY, IntVector2 counter);
	
	Path* GenerateWalkablePath(IntVector2* start, IntVector2* end, Entity* entity);
	void StartStepPeth(IntVector2* start, IntVector2* end, Entity* entity);
	bool ContinueStepPath();

	void RenderPathDebugging();


	IntVector2 FindWalkableSpotFarthestFromExitForStart(std::vector<Item*> questItem);
	IntVector2 FindRandomWalkableSpot();
	IntVector2 GetTileCordsForWorldPos(Vector2 currentPosition);
	int GetDistanceBetweenTwoEntities(Entity* firstPerson, Entity* secondPerson);
	int GetTileDistanceBetweenTwoTiles(IntVector2 firstTile, IntVector2 secondTile);
	int GetTileIndexForTile(Tile& tile);
	Tile* GetTileAtTileCoords(const IntVector2& tileCoords);
	Tile* GetTileAtWorldPos(Vector2 tileWorldCords);
	Tile* GetTileToTheLeftOfGivenTile(const IntVector2& tileCoords);
	Tile* GetTileToTheRightOfGivenTile(const IntVector2& tileCoords);
	Tile* GetTileToTheTopOfGivenTile(const IntVector2& tileCoords);
	Tile* GetTileToTheBottomOfGivenTile(const IntVector2& tileCoords);
	Tile* getLocalTileAtGivenScreenCoords(Vector2 mouseCords);
	Tile* FindNearestTileOfType(Tile* myself);
	Tile* FindNearestTileNotOfType(Tile* myself);
	Tile* FindExitTile();
	Tile* FindQuestItem(std::vector<Item*> questItem);
	std::vector<Tile *> GetTilesInNSquareAroundPassedInCords(int squareLength, IntVector2 startCoords);
	std::vector<Tile *> RaycastToVisableTiles(Character* currentCords);
	Entity* GetEnemyInGivenTile(const IntVector2& tileCoords);
	Entity* FindNearestEntityOfType(Entity* myself);
	Entity* FindNearestEntityNotOfType(Entity* myself);
	Entity* FindNearestEntityOfFaction(Entity* myself);
	Entity* FindNearestEntityNotOfFaction(Entity* myself);
	bool  IsTileSolid(const IntVector2& tileCoords);
	bool IsTileSolid(const Tile* tile);
	bool HasLineOfSight(Vector2 startPos, Vector2 endPos);
	void LineOfSightAndMarkVision(std::vector<Tile *>& visableTiles, Vector2 startPost, Vector2 endPos);
	void SetTileToHaveEnemyInIt(Entity* characterInTile);
	void SetTileToHaveEnemyLeaveIt(Entity* characterInTile);

private:
	void CreateEntities();
	void CreateMapFromXML(std::string MapName);


	void AssignTilesTextures();
public:
	std::string m_MapName;
	IntVector2 m_dimensions;
	std::vector<Tile> m_tiles;
	std::vector<Entity*> m_entity;
	std::vector<MapFeature*> m_features;
	PathGenerator* m_currentPathGenerator;
	Path*		m_currentPath;
	int			m_MinPowerLevel;
	int			m_MaxPowerLevel;
	bool		m_DialogEnabled;
	whenToPlay*  m_CurrentStoryPositionInMap;
	std::string m_Location;
	std::vector<std::string> m_locationForEnemies;
	DialogBoxes*	m_DialogForMap;

private:
	Item* CreateItemAtThatSpot(std::string  lootTable, Inventory* inventory);
};