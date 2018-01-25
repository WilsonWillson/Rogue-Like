#pragma once
#include "Engine/Math/IntVector2.hpp"
#include "Game/TileDefinition.hpp"
#include "Game/Tile.hpp"
#include "Game/Map.hpp"
#include "Engine/Core/Noise.hpp"

#include <string>

//--------------------------Generate Map From File--------------------------------------
void MapGeneratorFromFile(std::string desiredMapName, Map* currentMap);
void ConvertStringMapToTiles(std::vector<std::string> mapInStringForm, Map* currentMap);

//-------------------------------------Generate Random Map From Code---------------------------------------------
void GenerateRandomMapBasedOnCode(std::string desiredMapName, Map* currentMap);
void MakeWholeMapOneTileType(Map* currentMap, std::string tileName);

//---------------------------------------Parsing Data-----------------------------------------------------------------
void ParseDataFromDesiredMap(XMLNode xMainNode, std::string desiredMapName, Map* currentMap);

//----------------------------File Features-----------------------------------------------------------------------------
void GenerateAnyFileFeatures(XMLNode mapNode, Map* currentMap);
void GenerateFileFeatures(Map* currentMap, const char* xmlFeatureFile, IntVector2 offset, int rotation, IntVector2 startPosition, bool isMirrored);
void ChangeTilesBasedOnFileFeature(XMLNode xMainNode, Map* currentMap, Tile* currentTile, Tile* TopLeftCornerTile, int linesInMap, int rotation, bool isMirrored);
Tile* DetermineNextTileForFromFile(Map* currentMap, IntVector2 m_tileCords, int rotation, bool isMirrored);

//------------------------------Map Types-----------------------------------------------------------------------------------------------
void ParseBasicMapRequirments(XMLNode mapNode, Map* currentMap, int* numberOfRooms, IntVector2* roomLength, std::string* defaultFillTile, std::string* defaultRoomFloor, std::string* defaultHallwayFloor, bool* overlap, float* straightness, std::string desriedMapName);
void GenerateMapBasedOffRoomsAndCorridors(Map* currentMap, int numberOfRooms, IntVector2 roomLength, std::string defaultFillTile, std::string  defaultHallwayFloor, std::string defaultRoomFloor, bool overlap);

//-------------------------------------------------------Room Generator Code------------------------------------------------------
std::vector<IntVector2> CarveOutRoomsSaveCenterLocationsOfRooms(Map* currentMap, int numberOfRooms, IntVector2 roomLength, std::string  defaultRoomFloor, bool overlap);
void ConnectRoomsWithHallways(std::vector<IntVector2> middleOfStartingRoomLocations, Map* currentMap, std::string  defaultHallwayFloor);

//--------------------------------------Perlin Noise---------------------------------------------------------------------------------
void PerlinNoiseGenerator(XMLNode mapNode, Map* currentMap);
void CalculatePerlinRules(Tile* tile, std::map<std::string, float> perlinRules, float perlinNoise);

//------------------------- CellularAutomata --------------------------------------------------------------------------------------
void CellularAutomata(XMLNode mapNode, Map* currentMap);
void CreateCARules(XMLNode CARuleNode, Map* currentMap, int passses);
void WhichCARuleToUse(Map* currentMap, std::map<std::string, TileDefinition*>::iterator ifTile, std::map<std::string, TileDefinition*>::iterator replaceTile, std::map<std::string, TileDefinition*>::iterator effectTileIterator, 
						int surround, int percentToRun, int passes);
void ReplaceTileBasedOnSurroundings(Map* currentMap, std::map<std::string, TileDefinition*>::iterator ifTile, std::map<std::string, TileDefinition*>::iterator replaceTile,
									std::map<std::string, TileDefinition*>::iterator effectTileIterator, int surround, int percentToRun);

//------------------------------------------Helper Functions--------------------------------------------------------------------------
bool DoRoomsOverlap(Map* currentMap, Tile* randomBottomLeftCornerTile, std::string defaultRoomFloor, IntVector2 roomLength);
void MakeSureMapEdgesAreWalls(Map* currentMap, std::string defaultFillTile);
void AddExitToWalkableSpot(Map* currentMap, std::string ExitTileType);
void AddQuestItemToWalkableSpot(Map* currentMap, Item* desieredItem);