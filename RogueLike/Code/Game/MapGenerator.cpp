#include "Game/MapGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"

//--------------------------Generate Map From File--------------------------------------
void MapGeneratorFromFile(std::string desiredMapName, Map* currentMap)
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Map.xml", "Maps");
	std::vector<std::string> mapInStringForm;
	XMLNode xNode = xMainNode;
	int amountOfMaps = xNode.nChildNode("Map");

	for (int findNeededMap = 0; findNeededMap < amountOfMaps; findNeededMap++)
	{
		std::string mapName = xMainNode.getChildNode("Map", findNeededMap).getAttributeValue(0);
		currentMap->m_MinPowerLevel = ParseXmlAttribute(xMainNode, "Item", findNeededMap, 1, -1);
		currentMap->m_MaxPowerLevel = ParseXmlAttribute(xMainNode, "Item", findNeededMap, 2, -1);
		currentMap->m_dimensions = ParseXmlAttribute(xMainNode, "Map", findNeededMap, 1, IntVector2(-1, -1));
		if (mapName == desiredMapName)
		{
			int linesInMap = xNode.getChildNode("Map", findNeededMap).nChildNode("Row");
			for (int mapAttributes = 0; mapAttributes < linesInMap; mapAttributes++)
			{
				std::string attributeName = xMainNode.getChildNode("Map", findNeededMap).getChildNode("Row", mapAttributes).getAttributeValue(0);
				mapInStringForm.push_back(attributeName);
			}

			ConvertStringMapToTiles(mapInStringForm, currentMap);
			//AddExitToWalkableSpot(currentMap);
			return;
		}
	}
}

void ConvertStringMapToTiles(std::vector<std::string> mapInStringForm, Map* currentMap)
{
	IntVector2 currentDimensions = IntVector2(0, 0);
	for (int mapStep = 0; mapStep <  currentMap->m_dimensions.y; mapStep++)
	{
		Tile newTileForMap;
		std::map<std::string, TileDefinition*>::iterator tileDefinition;
		for (char& c : mapInStringForm[mapStep])
		{
			TileDefinition* tile = new TileDefinition(); tile;

			if (c == 'X')
				tileDefinition = tile->m_registry.find("Stone");

			if (c == '.')
				tileDefinition = tile->m_registry.find("Grass");

			newTileForMap.m_definition = tileDefinition->second;
			newTileForMap.m_tileCords = currentDimensions;
			currentDimensions.x++;
			currentMap->m_tiles.push_back(newTileForMap);
		}
		currentDimensions.x = 0;
		currentDimensions.y++;
	}
}


//-------------------------------------Generate Random Map From Code---------------------------------------------------------------------
void GenerateRandomMapBasedOnCode(std::string desiredMapName, Map* currentMap)
{

	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Map.xml", "Maps");
	int amountOfMaps = xMainNode.nChildNode("Map");

	for (int findNeededMap = 0; findNeededMap < amountOfMaps; findNeededMap++)
	{
		std::string mapName = xMainNode.getChildNode("Map", findNeededMap).getAttributeValue(0);
		currentMap->m_dimensions = ParseXmlAttribute(xMainNode, "Map", findNeededMap, 1, IntVector2(-1,-1));
		std::string locations = ParseXmlAttribute(xMainNode, "Map", findNeededMap, 2, "none");

		currentMap->m_locationForEnemies = ParseStringIntoPiecesByDelimiter(locations, ' ');
		if (mapName == desiredMapName)
		{
			currentMap->m_MapName = mapName;
			XMLNode mapNode = xMainNode.getChildNode("Map", findNeededMap).getChildNode("Generators");
			ParseDataFromDesiredMap(mapNode, desiredMapName, currentMap);
			return;
		}
	}
}

//---------------------------------------Parsing Data-----------------------------------------------------------------
void ParseDataFromDesiredMap(XMLNode mapNode, std::string desiredMapName, Map* currentMap)
{
	int numberOfRooms = 0;
	IntVector2 roomLength = IntVector2(-1,-1);
	std::string defaultFillTile = "";
	std::string defaultRoomFloor = "";
	std::string defaultHallwayFloor = "";
	bool overlap = false;
	float straightness = 1.0f;



	int mapPasses = mapNode.nChildNode();
	for (int passOverSteps = 0; passOverSteps < mapPasses; passOverSteps++)
	{
		XMLNode childNode = mapNode.getChildNode(passOverSteps);
		std::string childName = childNode.getName();
		if (childName == "Requirments")
			ParseBasicMapRequirments(mapNode, currentMap, &numberOfRooms, &roomLength, &defaultFillTile, &defaultRoomFloor, &defaultHallwayFloor, &overlap, &straightness, desiredMapName);
		if(childName == "PerlinNode")
			PerlinNoiseGenerator(mapNode, currentMap);
		if(childName == "FromFile")
			GenerateAnyFileFeatures(mapNode, currentMap);
		if(childName == "CARules")
			CellularAutomata(mapNode, currentMap);
	}

	MakeSureMapEdgesAreWalls(currentMap, defaultFillTile);
}

void ParseBasicMapRequirments(XMLNode mapNode, Map* currentMap, int* numberOfRooms, IntVector2* roomLength, std::string* defaultFillTile, std::string* defaultRoomFloor, std::string* defaultHallwayFloor, bool* overlap, float* straightness, std::string desieredMapName)
{
	std::string requirmentsName = "";
	int amountOfRequirmentsForMap = mapNode.getChildNode("Requirments").nAttribute();
	for (int attributeStep = 0; attributeStep < amountOfRequirmentsForMap; attributeStep++)
	{
		std::string attributeName = mapNode.getChildNode("Requirments").getAttributeName(attributeStep);

		if (attributeName == "Name")
			requirmentsName= ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, "None");
		if (attributeName == "NumberOfRooms")
			*numberOfRooms = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, -1);
		if (attributeName == "RoomLengthX")
			roomLength->x = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, -1);
		if (attributeName == "RoomLengthY")
			roomLength->y = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, -1);
		if (attributeName == "DefaultFillTile")
			*defaultFillTile = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, "Bog");
		if (attributeName == "DefaultRoomFloor")
			*defaultRoomFloor = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, "Grass");
		if (attributeName == "DefaultHallwayFloor")
			*defaultHallwayFloor = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, "Grass");
		if (attributeName == "Overlap")
			*overlap = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep);
		if (attributeName == "Straightness")
			*straightness = ParseXmlAttribute(mapNode, "Requirments", 0, attributeStep, 1.0f);
	}

	//How we start the Map. A basic Building Block of positions, render position and filling it with our default fill
	MakeWholeMapOneTileType(currentMap, *defaultFillTile);

	if (requirmentsName == "MapGeneratorRoomsAndCorridors")
		GenerateMapBasedOffRoomsAndCorridors(currentMap, *numberOfRooms, *roomLength, *defaultFillTile, *defaultHallwayFloor, *defaultRoomFloor, *overlap);

}

//----------------------------File Features-----------------------------------------------------------------------------
void GenerateAnyFileFeatures(XMLNode mapNode, Map* currentMap)
{
	int amountOfFromFileFeaturesToAdd = mapNode.nChildNode();
	const char* xmlFeatureFile = nullptr;
	IntVector2 offset = IntVector2(0, 0);
	int rotation = 0;
	IntVector2 startPosition = IntVector2(0, 0);
	std::string tempWord;
	bool isMirrored = false;
	for (int fileFeatureStep = 0; fileFeatureStep < amountOfFromFileFeaturesToAdd; fileFeatureStep++)
	{
		int amountOfFileRequirments = mapNode.getChildNode("FromFile", fileFeatureStep).nAttribute();
		for (int attributeStep = 0; attributeStep < amountOfFileRequirments; attributeStep++)
		{
			std::string attributeName = mapNode.getChildNode("FromFile", fileFeatureStep).getAttributeName(attributeStep);

			if (attributeName == "file")
			{
				tempWord = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep, "none");
				xmlFeatureFile = tempWord.c_str();
			}
			if (attributeName == "offsetX")
				offset.x = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep, -1);
			if (attributeName == "offsetY")
				offset.y = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep, -1);
			if (attributeName == "Rotations")
				rotation = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep, -1);
			if (attributeName == "Position")
				startPosition = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep, IntVector2(-1, -1));
			if (attributeName == "isMirrored")
				isMirrored = ParseXmlAttribute(mapNode, "FromFile", fileFeatureStep, attributeStep);
		}
		if (xmlFeatureFile != nullptr)
			GenerateFileFeatures(currentMap, xmlFeatureFile, offset, rotation, startPosition, isMirrored);
		xmlFeatureFile = nullptr;
	}
}

void GenerateFileFeatures(Map* currentMap, const char* xmlFeatureFile, IntVector2 offset, int rotation, IntVector2 startPosition, bool isMirrored)
{
	XMLNode xMainNode = XMLNode::openFileHelper(xmlFeatureFile, "Maps");
	int linesInMap = xMainNode.getChildNode("Map").nChildNode("Row");
	IntVector2 dimentions = ParseXmlAttribute(xMainNode, "Map", 0, 0, IntVector2(-1, -1));

	IntVector2 TopLeftCorner;
	if (startPosition.x != -1)
	{
		TopLeftCorner.x = startPosition.x + offset.x;
		TopLeftCorner.y = startPosition.y + offset.y;
	}
	else
		TopLeftCorner = IntVector2(GetRandomIntInRange(0, currentMap->m_dimensions.x - dimentions.x - offset.x - 1), GetRandomIntInRange(0, currentMap->m_dimensions.y - dimentions.y - offset.y - 1));

	Tile* TopLeftCornerTile = currentMap->GetTileAtTileCoords(TopLeftCorner);
	Tile* currentTile = TopLeftCornerTile;

	ChangeTilesBasedOnFileFeature(xMainNode, currentMap, currentTile, TopLeftCornerTile, linesInMap, rotation, isMirrored);
}

void ChangeTilesBasedOnFileFeature(XMLNode xMainNode, Map* currentMap, Tile* currentTile, Tile* TopLeftCornerTile, int linesInMap, int rotation, bool isMirrored)
{
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator grass;
	grass = tile->m_registry.find("Grass");
	std::map<std::string, TileDefinition*>::iterator stone;
	stone = tile->m_registry.find("Stone");


	for (int lineStep = 0; lineStep < linesInMap; lineStep++)
	{
		std::string attributeName = xMainNode.getChildNode("Map").getChildNode("Row", lineStep).getAttributeValue(0);

		for (char& c : attributeName)
		{
			if (c == 'X')
				currentTile->m_definition = stone->second;

			if (c == '.')
				currentTile->m_definition = grass->second;

			currentTile->m_permenance = 1;
			currentTile = DetermineNextTileForFromFile(currentMap, currentTile->m_tileCords, rotation, isMirrored);

			if (currentTile == nullptr)
				break;
		}
		TopLeftCornerTile = DetermineNextTileForFromFile(currentMap, TopLeftCornerTile->m_tileCords, rotation - 1, isMirrored);
		currentTile = TopLeftCornerTile;

		if (currentTile == nullptr)
			break;
	}
}

Tile* DetermineNextTileForFromFile(Map* currentMap, IntVector2 m_tileCords, int rotation, bool isMirrored)
{
	int whatDirectionDoWeNeedForNextTile = 0;

	whatDirectionDoWeNeedForNextTile += rotation;

	if (isMirrored == true)
		whatDirectionDoWeNeedForNextTile += 2;

	if (whatDirectionDoWeNeedForNextTile == -1)
		whatDirectionDoWeNeedForNextTile = 3;
	if (whatDirectionDoWeNeedForNextTile == 4)
		whatDirectionDoWeNeedForNextTile = 0;
	if (whatDirectionDoWeNeedForNextTile >= 5)
		whatDirectionDoWeNeedForNextTile = 1;

	if (whatDirectionDoWeNeedForNextTile == 0)
		return currentMap->GetTileToTheRightOfGivenTile(m_tileCords);
	if (whatDirectionDoWeNeedForNextTile == 1)
		return currentMap->GetTileToTheTopOfGivenTile(m_tileCords);
	if (whatDirectionDoWeNeedForNextTile == 2)
		return currentMap->GetTileToTheLeftOfGivenTile(m_tileCords);
	if (whatDirectionDoWeNeedForNextTile == 3)
		return currentMap->GetTileToTheBottomOfGivenTile(m_tileCords);

	return currentMap->GetTileToTheRightOfGivenTile(m_tileCords);
}



//------------------------------Map Types-----------------------------------------------------------------------------------------------
void GenerateMapBasedOffRoomsAndCorridors(Map* currentMap, int numberOfRooms, IntVector2 roomLength, std::string defaultFillTile, std::string  defaultHallwayFloor, std::string defaultRoomFloor, bool overlap)
{
	std::vector<IntVector2> middleOfStartingRoomLocations = CarveOutRoomsSaveCenterLocationsOfRooms(currentMap, numberOfRooms, roomLength, defaultRoomFloor, overlap);
	ConnectRoomsWithHallways(middleOfStartingRoomLocations, currentMap, defaultHallwayFloor);
}

void MakeWholeMapOneTileType(Map* currentMap, std::string tileName)
{
	IntVector2 currentDimensions = IntVector2(0, 0);

	float baseLocationX = 0.0f;
	float baseLocationY = 0.0f;
	float space_Width = ((float)currentMap->m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)currentMap->m_dimensions.y / 900) * 900;

	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator it;
	it = tile->m_registry.find(tileName);

	if (it == tile->m_registry.end())
		it = tile->m_registry.find("Stone");

	for (int stoneStep = 0; stoneStep < currentMap->m_dimensions.x * currentMap->m_dimensions.y; stoneStep++)
	{
		Tile newTile;

		newTile.m_definition = it->second;

		newTile.m_renderPosition = Vector2(baseLocationX, baseLocationY);
		newTile.m_tileCords = currentDimensions;

		currentDimensions.x++;
		if (currentDimensions.x >= currentMap->m_dimensions.x)
		{
			currentDimensions.x = 0;
			currentDimensions.y++;
			baseLocationX = 0;
			baseLocationY += space_Height;
		}
		else
		{
			baseLocationX += space_Width;
		}

		newTile.m_permenance = 0;
		currentMap->m_tiles.push_back(newTile);
	}
}

//-------------------------------------------------------Room Generator Code----------------------------------------------------------------

std::vector<IntVector2> CarveOutRoomsSaveCenterLocationsOfRooms(Map* currentMap, int numberOfRooms, IntVector2 roomLength, std::string  defaultRoomFloor, bool overlap)
{
	std::vector<IntVector2> middleOfStartingRoomLocations;
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator it;
	it = tile->m_registry.find(defaultRoomFloor);

	//Second Pass Make Rooms
	for (int roomCreationStep = 0; roomCreationStep < numberOfRooms; roomCreationStep++)
	{
		Tile* randomBottomLeftCornerTile = nullptr;
		IntVector2 randomBottomLeftCorner (0, 0);
		bool illegalRoom = true;
		int attempts = 0;
		do {
			if (attempts > 500)
				break;

			randomBottomLeftCorner = IntVector2(GetRandomIntInRange(0, currentMap->m_dimensions.x - roomLength.x), GetRandomIntInRange(0, currentMap->m_dimensions.y - roomLength.y));
			randomBottomLeftCornerTile = currentMap->GetTileAtTileCoords(randomBottomLeftCorner);

			illegalRoom = DoRoomsOverlap(currentMap, randomBottomLeftCornerTile, defaultRoomFloor, roomLength);

			if (overlap == true)
				illegalRoom = false;
			attempts++;
		} while (illegalRoom);
		if (attempts > 500)
			continue;

		//randomBottomLeftCornerTile->m_definition = it->second;

		IntVector2 currentPositionInWorld = randomBottomLeftCorner;
		for (int roomLengthY = 0; roomLengthY < roomLength.y; roomLengthY++)
		{

			if (currentPositionInWorld.y > currentMap->m_dimensions.y - 1)
				break;

			for (int roomLengthX = 1; roomLengthX < roomLength.x; roomLengthX++)
			{
				currentPositionInWorld.x += roomLengthX;
				if (currentPositionInWorld.x > currentMap->m_dimensions.x - 1)
					break;
				randomBottomLeftCornerTile = currentMap->GetTileAtTileCoords(currentPositionInWorld);
				randomBottomLeftCornerTile->m_definition = it->second;

				if ((currentPositionInWorld.x - randomBottomLeftCorner.x) == (int)(roomLength.x / 2) &&
					(currentPositionInWorld.y - randomBottomLeftCorner.y) == (int)(roomLength.y / 2))
					middleOfStartingRoomLocations.push_back(currentPositionInWorld);

				currentPositionInWorld.x = randomBottomLeftCorner.x;
			}
			currentPositionInWorld.y++;
		}
	}

	return middleOfStartingRoomLocations;
}

void ConnectRoomsWithHallways(std::vector<IntVector2> middleOfStartingRoomLocations, Map* currentMap, std::string  defaultHallwayFloor)
{
	std::vector<IntVector2> roomsThatArentConnectedStill = middleOfStartingRoomLocations;
	int roomWeAreUsing = 0;
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator it;
	it = tile->m_registry.find(defaultHallwayFloor);

	if (middleOfStartingRoomLocations.size() == 0)
		return;

	for (unsigned int roomStep = 0; roomStep < middleOfStartingRoomLocations.size() - 1; roomStep++)
	{
		//find room with smallest x that is still not connected and pop it off the stack
		IntVector2 smallestXRoom = IntVector2(100000, 100000);
		for (unsigned int connectionStep = 0; connectionStep < roomsThatArentConnectedStill.size(); connectionStep++)
		{
			if (smallestXRoom.x > roomsThatArentConnectedStill[connectionStep].x)
			{
				smallestXRoom = roomsThatArentConnectedStill[connectionStep];
				roomWeAreUsing = connectionStep;
			}
		}
		roomsThatArentConnectedStill.erase(roomsThatArentConnectedStill.begin() + roomWeAreUsing);

		//Find the closest X out of ALL rooms in the game
		int currentClosestDistance = 100000;
		IntVector2 connectionRoom;
		for (unsigned int connectionStep = 0; connectionStep < roomsThatArentConnectedStill.size(); connectionStep++)
		{
			if (abs(smallestXRoom.x - roomsThatArentConnectedStill[connectionStep].x) < currentClosestDistance)
			{
				connectionRoom = roomsThatArentConnectedStill[connectionStep];
				currentClosestDistance = abs(smallestXRoom.x - roomsThatArentConnectedStill[connectionStep].x);
			}
		}

		//Connect Those Rooms-Currently Straight Lines
		while (connectionRoom.x != smallestXRoom.x)
		{
			Tile* randomBottomLeftCornerTile = currentMap->GetTileAtTileCoords(smallestXRoom);
			randomBottomLeftCornerTile->m_definition = it->second;
			randomBottomLeftCornerTile->m_permenance = .75f;
			smallestXRoom.x++;
		}
		while (connectionRoom.y != smallestXRoom.y)
		{
			Tile* randomBottomLeftCornerTile = currentMap->GetTileAtTileCoords(smallestXRoom);
			randomBottomLeftCornerTile->m_definition = it->second;
			randomBottomLeftCornerTile->m_permenance = .75f;
			if (smallestXRoom.y > connectionRoom.y)
				smallestXRoom.y--;
			else
				smallestXRoom.y++;
		}
	}

}


//--------------------------------------Perlin Noise---------------------------------------------------------------------------------

void PerlinNoiseGenerator(XMLNode mapNode, Map* currentMap)
{
	float scale = 0.0f; int numOctaves = 0; float octavePersistence = 0.0f; float octaveScale = 0.0f; bool renormalize = false; int seed = 0;
	XMLNode PerlinNode = mapNode.getChildNode("PerlinNode");
	int perlinNodeAttributes = PerlinNode.nAttribute();
	for (int perlinStep = 0; perlinStep < perlinNodeAttributes; perlinStep++)
	{
		std::string ruleNode = PerlinNode.getAttributeName(perlinStep);

		if (ruleNode == "scale")
			scale = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep, 0.0f);
		if (ruleNode == "numOctaves")
			numOctaves = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep, 0);
		if (ruleNode == "octavePersistence")
			octavePersistence = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep, 0.0f);
		if (ruleNode == "octaveScale")
			octaveScale = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep, 0.0f);
		if (ruleNode == "renormalize")
			renormalize = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep);
		if (ruleNode == "seed")
			seed = ParseXmlAttribute(mapNode, "PerlinNode", 0, perlinStep, 0);
	}

	float tickCount = (float)GetTickCount();
	float rando = GetRandomFloatInRange(0, tickCount);
	seed =(int)( seed + (float)rando);
	//seed += 200;

	int perlinNodeChildrenAmount = PerlinNode.nChildNode();
	std::map<std::string, float> perlinRules;
	for (int perlinChildStep = 0; perlinChildStep < perlinNodeChildrenAmount; perlinChildStep++)
	{
		std::string perlinTileType;
		float rule =0.0f;

		int attributeCount = PerlinNode.getChildNode("Rule", perlinChildStep).nAttribute();
		for (int attributeStep = 0; attributeStep < attributeCount; attributeStep++)
		{
			std::string ruleNode = PerlinNode.getChildNode("Rule", perlinChildStep).getAttributeName(attributeStep);

			if (ruleNode == "tile")
				perlinTileType = ParseXmlAttribute(PerlinNode, "Rule", perlinChildStep, attributeStep, "None");
			if (ruleNode == "chance")
				rule = ParseXmlAttribute(PerlinNode, "Rule", perlinChildStep, attributeStep, 0.f);
		}

		perlinRules[perlinTileType] = rule;
	}
	
	for (unsigned int tileStep = 0; tileStep < currentMap->m_tiles.size(); tileStep++)
	{
		float perlinNoise = Compute2dPerlinNoise((float)currentMap->m_tiles[tileStep].m_tileCords.x, (float)currentMap->m_tiles[tileStep].m_tileCords.y, scale,numOctaves, octavePersistence, octaveScale, renormalize, seed);
		CalculatePerlinRules(&currentMap->m_tiles[tileStep], perlinRules, perlinNoise);
	}
}


void CalculatePerlinRules(Tile* tile, std::map<std::string, float> perlinRules, float perlinNoise)
{
	TileDefinition* tileDef = new TileDefinition(); tileDef;
	std::map<std::string, TileDefinition*>::iterator tileIterator;
	//tileIterator = tileDef->m_registry.find("Grass");

	for (std::map<std::string, float>::iterator ruleIterator = perlinRules.begin(); ruleIterator != perlinRules.end(); ++ruleIterator)
	{
		tileIterator = tileDef->m_registry.find(ruleIterator->first);

		if (ruleIterator->second > 0 && tile->m_permenance < .5)
		{
			if (perlinNoise > ruleIterator->second)
				tile->m_definition = tileIterator->second;
		}
		if (ruleIterator->second < 0)
		{
			if (perlinNoise < ruleIterator->second && tile->m_permenance < .5)
				tile->m_definition = tileIterator->second;
		}
	}
}


//------------------------- CellularAutomata --------------------------------------------------
void CellularAutomata(XMLNode mapNode, Map* currentMap)
{
	int amountCARulesToAdd = mapNode.nChildNode();
	int passes = ParseXmlAttribute(mapNode, "CARules", 0, 0, 0);
	for (int fileFeatureStep = 0; fileFeatureStep < amountCARulesToAdd; fileFeatureStep++)
	{
		XMLNode CARuleNode = mapNode.getChildNode("CARules", fileFeatureStep);
		CreateCARules(CARuleNode, currentMap, passes);
	}
}

void CreateCARules(XMLNode CARuleNode, Map* currentMap, int passes)
{
	TileDefinition* tile = new TileDefinition(); tile;
	std::string ifTile = "None";
	std::string effectTile = "None";
	std::string replaceTile = "None";
	int surround = 999;
	int percentToRun = 0;

	int amountOfCARules = CARuleNode.nChildNode();
	for (int caRulesStep = 0; caRulesStep <  amountOfCARules; caRulesStep++)
	{
		int amountOfAttributes = CARuleNode.getChildNode("Rule", caRulesStep).nAttribute();
		for (int caRuleAttributes = 0; caRuleAttributes < amountOfAttributes; caRuleAttributes++)
		{
			std::string ruleNode = CARuleNode.getChildNode("Rule", caRulesStep).getAttributeName(caRuleAttributes);

			if (ruleNode == "ifTile")
				ifTile = ParseXmlAttribute(CARuleNode, "Rule", caRulesStep, caRuleAttributes, "None");
			if (ruleNode == "effectTile")
				effectTile = ParseXmlAttribute(CARuleNode, "Rule", caRulesStep, caRuleAttributes, "None");
			if (ruleNode == "replaceTile")
				replaceTile = ParseXmlAttribute(CARuleNode, "Rule", caRulesStep, caRuleAttributes, "None");
			if (ruleNode == "surround")
				surround = ParseXmlAttribute(CARuleNode, "Rule", caRulesStep, caRuleAttributes, 999);
			if (ruleNode == "percentToRun")
				percentToRun = ParseXmlAttribute(CARuleNode, "Rule", caRulesStep, caRuleAttributes, 100);
		}
		if (ifTile == "None" && replaceTile == "None")
				break;
		std::map<std::string, TileDefinition*>::iterator ifTileIterator;
		ifTileIterator = tile->m_registry.find(ifTile);

		std::map<std::string, TileDefinition*>::iterator replaceTileIterator;
		replaceTileIterator = tile->m_registry.find(replaceTile);

		std::map<std::string, TileDefinition*>::iterator effectTileIterator;
		effectTileIterator = tile->m_registry.find(effectTile);


		WhichCARuleToUse(currentMap, ifTileIterator, replaceTileIterator, effectTileIterator, surround, percentToRun,passes);
	}
}

void WhichCARuleToUse(Map* currentMap, std::map<std::string, TileDefinition*>::iterator ifTile, std::map<std::string, TileDefinition*>::iterator replaceTile, 
	std::map<std::string, TileDefinition*>::iterator effectTile, int surround, int percentToRun, int passes)
{
	for (int passAttempts = 0; passAttempts < passes; passAttempts++)
	{
		if (surround != 999)
			ReplaceTileBasedOnSurroundings(currentMap, ifTile, replaceTile, effectTile, surround, percentToRun);
	}
}

void ReplaceTileBasedOnSurroundings(Map* currentMap, std::map<std::string, TileDefinition*>::iterator ifTile, std::map<std::string, TileDefinition*>::iterator replaceTile,
	std::map<std::string, TileDefinition*>::iterator effectTile, int surround, int percentToRun)
{
	for (unsigned int tileStep = 0; tileStep < currentMap->m_tiles.size(); tileStep++)
	{
		int checkSurroundCounter = 0;

		if(currentMap->m_tiles[tileStep].m_definition != ifTile->second)
			continue;

		Tile* neighbor = currentMap->GetTileToTheTopOfGivenTile(currentMap->m_tiles[tileStep].m_tileCords);
		if (neighbor != nullptr && neighbor->m_definition == effectTile->second)
			checkSurroundCounter++;

		neighbor = currentMap->GetTileToTheBottomOfGivenTile(currentMap->m_tiles[tileStep].m_tileCords);
		if (neighbor != nullptr && neighbor->m_definition == effectTile->second)
			checkSurroundCounter++;

		neighbor = currentMap->GetTileToTheLeftOfGivenTile(currentMap->m_tiles[tileStep].m_tileCords);
		if (neighbor != nullptr && neighbor->m_definition == effectTile->second)
			checkSurroundCounter++;

		neighbor = currentMap->GetTileToTheRightOfGivenTile(currentMap->m_tiles[tileStep].m_tileCords);
		if (neighbor != nullptr && neighbor->m_definition == effectTile->second)
			checkSurroundCounter++;

		int chanceToHappen = rand() % 100;

		if(surround > 0)
			if (neighbor != nullptr && checkSurroundCounter >= abs(surround) && chanceToHappen < percentToRun /*&& currentMap->m_tiles[tileStep].m_permenance <= .75*/)
				currentMap->m_tiles[tileStep].m_definition = replaceTile->second;

		if (surround < 0)
			if (neighbor != nullptr && checkSurroundCounter <= abs(surround) && chanceToHappen < percentToRun /*&& currentMap->m_tiles[tileStep].m_permenance <= .75*/)
				currentMap->m_tiles[tileStep].m_definition = replaceTile->second;
	}
}


//---------------------Helper Functions---------------------------------------------------------
bool DoRoomsOverlap(Map* currentMap, Tile* randomBottomLeftCornerTile, std::string defaultRoomFloor, IntVector2 roomLength)
{
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator it;
	it = tile->m_registry.find(defaultRoomFloor);


	if (randomBottomLeftCornerTile->m_definition == it->second)
		return true;
	
	Tile* rightTopWall = currentMap->GetTileAtTileCoords(IntVector2(randomBottomLeftCornerTile->m_tileCords.x + roomLength.x + 1, randomBottomLeftCornerTile->m_tileCords.y + 1));
	if (rightTopWall == nullptr || rightTopWall->m_definition == it->second)
		return true;

	Tile* leftBottomWall = currentMap->GetTileAtTileCoords(IntVector2(randomBottomLeftCornerTile->m_tileCords.x +1, randomBottomLeftCornerTile->m_tileCords.y + roomLength.y+1));
	if (leftBottomWall == nullptr || leftBottomWall->m_definition == it->second)
		return true;

	Tile* rightBottomWall = currentMap->GetTileAtTileCoords(IntVector2(randomBottomLeftCornerTile->m_tileCords.x + roomLength.x+1, randomBottomLeftCornerTile->m_tileCords.y + roomLength.y+1));
	if (rightBottomWall == nullptr || rightBottomWall->m_definition == it->second)
		return true;

	return false;
}

void MakeSureMapEdgesAreWalls(Map* currentMap, std::string defaultFillTile)
{
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator it;
	it = tile->m_registry.find(defaultFillTile);

	if (it == tile->m_registry.end())
		it = tile->m_registry.find("Stone");


	for (unsigned int currentMapStep = 0; currentMapStep < currentMap->m_tiles.size(); currentMapStep++)
	{
		if (currentMap->m_tiles[currentMapStep].m_tileCords.y == 0 && !currentMap->m_tiles[currentMapStep].m_definition->m_isSolid)
			currentMap->m_tiles[currentMapStep].m_definition = it->second;
		if (currentMap->m_tiles[currentMapStep].m_tileCords.y == currentMap->m_dimensions.y-1 && !currentMap->m_tiles[currentMapStep].m_definition->m_isSolid)
			currentMap->m_tiles[currentMapStep].m_definition = it->second;

		if (currentMap->m_tiles[currentMapStep].m_tileCords.x == 0 && !currentMap->m_tiles[currentMapStep].m_definition->m_isSolid)
			currentMap->m_tiles[currentMapStep].m_definition = it->second;
		if (currentMap->m_tiles[currentMapStep].m_tileCords.x == currentMap->m_dimensions.x-1 && !currentMap->m_tiles[currentMapStep].m_definition->m_isSolid)
			currentMap->m_tiles[currentMapStep].m_definition = it->second;

	}
}


void AddExitToWalkableSpot(Map* currentMap, std::string ExitTileType)
{
	TileDefinition* tile = new TileDefinition(); tile;
	std::map<std::string, TileDefinition*>::iterator ifTileIterator;
	ifTileIterator = tile->m_registry.find(ExitTileType);
	
	Tile* randomWalkableSpot = currentMap->GetTileAtTileCoords(currentMap->FindRandomWalkableSpot());
	randomWalkableSpot->m_definition = ifTileIterator->second;
}

void AddQuestItemToWalkableSpot(Map* currentMap, Item* desieredItem)
{
	Tile* randomWalkableSpot = currentMap->GetTileAtTileCoords(currentMap->FindRandomWalkableSpot());
	Inventory* newInventory = new Inventory();
	newInventory->m_Inventory.push_back(desieredItem);
		randomWalkableSpot->m_InventoryInSpace = newInventory;
}

