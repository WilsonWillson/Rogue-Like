#include "Game/Map.hpp"
#include "Game/DialogBoxes.hpp"
#include "Game/Behavior.hpp"
#include "Game/MapGenerator.hpp"
#include "Game/FollowerBehavior.hpp"
#include "Game/LeaderBehavior.hpp"

//-----------------------------Constructor/Deconstructor-----------------------------------------------
Map::Map(std::string MapName, Item* questItem, std::string ExitTileName, int powerLevel, int MaxPowerLevel)
{
	m_currentPathGenerator = new PathGenerator();
	m_currentPath = new Path();
	m_DialogForMap = new DialogBoxes();
	m_MinPowerLevel = powerLevel;
	m_MaxPowerLevel = MaxPowerLevel;
	m_CurrentStoryPositionInMap = new whenToPlay();
	*m_CurrentStoryPositionInMap = START;

	CreateMapFromXML(MapName);
	CreateEntities();

	if (questItem == nullptr)
		AddExitToWalkableSpot(this, ExitTileName);
	else
		AddQuestItemToWalkableSpot(this, questItem);

	AssignTilesTextures();
}

Map::Map()
{

}

Map::~Map()
{

}

//--------------------------------Creation-----------------------------------------------------
void Map::CreateEntities()
{
	Character* newCharacterBuild = new Character(); newCharacterBuild;
	std::map<std::string, Character*>::iterator definition;

	for (int entityStep = 0; entityStep < 4; entityStep++)
	{
		bool notPlayer = false;
		while (!notPlayer)
			{

				//Pick a random Character definition and see if it fits the needed monster type.
				definition = newCharacterBuild->s_characterRegistry.begin();
				std::advance(definition, GetRandomIntInRange(0, newCharacterBuild->s_characterRegistry.size() - 2));

				for (int i = 0; i < m_locationForEnemies.size(); i++)
				{
					if (definition->second->m_Location == m_locationForEnemies[i])
					{

						if (definition->second->m_baseStats.ReturnPowerLevel() > m_MinPowerLevel && definition->second->m_baseStats.ReturnPowerLevel() < m_MaxPowerLevel)
							notPlayer = true;

						if (definition->second->m_name != "Player")
							notPlayer = true;
						break;
					}
				}
			}
		Entity* newCharacter = new Entity();
		newCharacter->m_CharatcerDefintion = definition->second;

		newCharacter->m_currentStats = newCharacter->m_CharatcerDefintion->m_baseStats;
		newCharacter->m_CurrentCords = FindRandomWalkableSpot();
		newCharacter->m_HomePosition = newCharacter->m_CurrentCords;
		newCharacter->m_CurrentMap = this;
		newCharacter->CreateInventory();
		SetTileToHaveEnemyInIt(newCharacter);
		m_entity.push_back(newCharacter);

	}
}

void Map::CreateEntitiy()
{
	Character* newCharacterBuild = new Character(); newCharacterBuild;
	std::map<std::string, Character*>::iterator definition;
	bool notPlayer = false;
	while (!notPlayer)
		{
			definition = newCharacterBuild->s_characterRegistry.begin();
			std::advance(definition, GetRandomIntInRange(0, newCharacterBuild->s_characterRegistry.size() - 2));
			if (definition->second->m_name != "Player")
				notPlayer = true;
		}
	Entity* newCharacter = new Entity();
	newCharacter->m_CharatcerDefintion = definition->second;

	newCharacter->m_currentStats = newCharacter->m_CharatcerDefintion->m_baseStats;
	newCharacter->m_CurrentCords = FindRandomWalkableSpot();
	newCharacter->m_HomePosition = newCharacter->m_CurrentCords;
	newCharacter->m_CurrentMap = this;
	newCharacter->CreateInventory();
	SetTileToHaveEnemyInIt(newCharacter);
	m_entity.push_back(newCharacter);
}

void Map::CreateSpecificEntitiy(std::string entityWantedToCreate)
{
	Character* newCharacterBuild = new Character(); newCharacterBuild;
	std::map<std::string, Character*>::iterator definition;

	definition = newCharacterBuild->s_characterRegistry.find(entityWantedToCreate);

	Entity* newCharacter = new Entity();
	newCharacter->m_CharatcerDefintion = definition->second;

	newCharacter->m_currentStats = newCharacter->m_CharatcerDefintion->m_baseStats;
	newCharacter->m_CurrentCords = FindRandomWalkableSpot();
	newCharacter->m_HomePosition = newCharacter->m_CurrentCords;
	newCharacter->m_CurrentMap = this;
	newCharacter->CreateInventory();
	SetTileToHaveEnemyInIt(newCharacter);
	m_entity.push_back(newCharacter);
}

void Map::CreateMapFromXML(std::string desiredMapName)
{
	if (desiredMapName == "Code")
		MapGeneratorFromFile(desiredMapName, this);
	else
		GenerateRandomMapBasedOnCode(desiredMapName, this);
}


void Map::AssignTilesTextures()
{
	for (unsigned int i = 0; i < m_tiles.size(); i++)
	{
		m_tiles[i].AssignTextureBasedOffOfName();
	}
}


//-----------------------------Update-------------------------------------------------------
void Map::Update(bool playerActionHasHappened, Character* m_Player, float deltaSeconds)
{
	for (size_t entityStep = 0; entityStep < m_entity.size(); entityStep++)
	{
		for (unsigned int damageTakenStep = 0; damageTakenStep < m_entity[entityStep]->m_LastTimeSinceDamageTaken.size(); damageTakenStep++)
		{
			m_entity[entityStep]->m_LastTimeSinceDamageTaken[damageTakenStep] += deltaSeconds * 10;
		}

		m_entity[entityStep]->m_AnimatePosition += deltaSeconds * 10;
		if (m_entity[entityStep]->m_AnimatePosition > 5)
			m_entity[entityStep]->m_AnimatePosition = 0;
		
		if (playerActionHasHappened == false)
			continue;

		m_entity[entityStep]->m_AnimatePosition = 0;


		if (m_entity[entityStep]->m_currentStats.m_stamina < 1)
		{
			KillEntity(entityStep);
			continue;
		}

		SetTileToHaveEnemyLeaveIt(m_entity[entityStep]);

		m_entity[entityStep]->Act(m_Player, m_entity, this);
		if (IsTileSolid(m_entity[entityStep]->m_CurrentCords))
			m_entity[entityStep]->m_CurrentCords = m_entity[entityStep]->m_previousCords;

		SetTileToHaveEnemyInIt(m_entity[entityStep]);
	}

	m_Player->Update(deltaSeconds);


	//DIALOG
	if (m_DialogForMap->m_DialogDone != true && *m_CurrentStoryPositionInMap == m_DialogForMap->GetCurrentPlayForDialog())
		m_DialogEnabled = true;
	else
		m_DialogEnabled = false;

	if (g_inputSystem->WasKeyJustPressed(KEY_ENTER) && m_DialogEnabled == true)
	{
		m_DialogForMap->UpdateDialogBox();
	}
}

void Map::SetVisibilityOfMap(Character* m_Player)
{
	for (unsigned int tileStep = 0; tileStep < m_tiles.size(); tileStep++)
	{
		m_tiles[tileStep].m_Visible = false;
	}

	Tile* currentTile = GetTileAtTileCoords(m_Player->m_CurrentCords);
	currentTile->m_Visible = true;

	//std::vector<Tile*> squareTile = GetTilesInNSquareAroundPassedInCords(3, m_Player->m_CurrentCords);
	std::vector<Tile*> squareTile = RaycastToVisableTiles(m_Player);

	for (unsigned int squareTileStep = 0; squareTileStep < squareTile.size(); squareTileStep++)
	{
		squareTile[squareTileStep]->m_Visible = true;
		squareTile[squareTileStep]->m_WasSeen = true;
		if (squareTile[squareTileStep]->m_CharacterInSpace != nullptr)
		{
			m_Player->m_VisableEntities.push_back(squareTile[squareTileStep]->m_CharacterInSpace);
		}
	}
}

std::vector<Tile*> Map::RaycastToVisableTiles(Character* character)
{
	std::vector<Tile*> visableTile;

	const int NUM_SIDES = character->rayCastAmount;
	const float DEGREES_PER_SIDE = 360.f / (float)NUM_SIDES;

	Tile* currentPlayerTile = GetTileAtTileCoords(character->m_CurrentCords);

	for (int amountOfRaycasts = 0; amountOfRaycasts < character->rayCastAmount; amountOfRaycasts++)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)amountOfRaycasts;
		Vector2 visionDistance;
		visionDistance.x = currentPlayerTile->m_renderPosition.x + character->distance * CosDegrees(angleDegrees);
		visionDistance.y = currentPlayerTile->m_renderPosition.y + character->distance * SinDegrees(angleDegrees);

		LineOfSightAndMarkVision(visableTile, currentPlayerTile->m_renderPosition, visionDistance);
	}

	return visableTile;
}

void Map::LineOfSightAndMarkVision(std::vector<Tile *>& visableTiles, Vector2 startPos, Vector2 endPos)
{
	float space_Width = ((float)m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)m_dimensions.y / 900) * 900;

	startPos = Vector2(startPos.x + space_Width / 2, startPos.y + space_Height / 2);
	endPos = Vector2(endPos.x + space_Width / 2, endPos.y + space_Height / 2);
	Vector2 currentPosition = startPos;

	float linedisplacementAmount = .90f;
	Vector2 lineDisplacement = endPos - startPos;

	while (abs(lineDisplacement.x) > space_Width)
	{
		lineDisplacement *= linedisplacementAmount;
		linedisplacementAmount *= linedisplacementAmount;
	}
	while (abs(lineDisplacement.y) > space_Height)
	{
		lineDisplacement *= linedisplacementAmount;
		linedisplacementAmount *= linedisplacementAmount;
	}


	int MaxSteps = (int)ceil(1 / linedisplacementAmount);
	for (int lineIndex = 0; lineIndex < MaxSteps; lineIndex++)
	{
		Tile* currentTile = GetTileAtWorldPos(currentPosition);

		if (currentTile == nullptr)
			return;

		visableTiles.push_back(currentTile);
		if (currentTile->m_definition->m_isOpaque == false)
			return;
		currentPosition += lineDisplacement;
	}

}

void Map::KillEntity(size_t entityStep)
{
	for (unsigned int behaviorStep = 0; behaviorStep < m_entity[entityStep]->m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
	{
		if (typeid(*m_entity[entityStep]->m_CharatcerDefintion->m_behaviors[behaviorStep]) == typeid(LeaderBehavior))
		{
			for (size_t followersStep = 0; followersStep < m_entity.size(); followersStep++)
			{
				if (typeid(*m_entity[followersStep]->m_currentBehavior) == typeid(FollowerBehavior))
				{
					FollowerBehavior* m_tempFollower = (FollowerBehavior*)m_entity[followersStep]->m_currentBehavior;
					if (m_tempFollower->m_myLeader == m_entity[entityStep])
					{
						m_tempFollower->m_LeaderIsDead = true;

						int fearIndex = rand() % 100 + 1;
						if (m_tempFollower->m_devotion > fearIndex)
							m_tempFollower->m_isDevotedAfterDeath = true;
						else
							m_tempFollower->m_isDevotedAfterDeath = false;
					}
				}
			}
		}
	}




	SetTileToHaveEnemyLeaveIt(m_entity[entityStep]);

	Tile* currentEntityLocation = GetTileAtTileCoords(m_entity[entityStep]->m_CurrentCords);
	CreateItemAtThatSpot(m_entity[entityStep]->m_CharatcerDefintion->m_lootTable, currentEntityLocation->m_InventoryInSpace);
	/*
	for (unsigned int inventoryStep = 0; inventoryStep < m_entity[entityStep]->m_inventory->m_Inventory.size(); inventoryStep++)
	{
		m_entity[entityStep]->m_inventory->m_Inventory[inventoryStep]->m_currentlyEquipped = false;
	}

	Tile* currentEntityLocation = GetTileAtTileCoords(m_entity[entityStep]->m_CurrentCords);
	currentEntityLocation->m_InventoryInSpace = m_entity[entityStep]->m_inventory;*/
	m_entity.erase(m_entity.begin() + entityStep);
}

Item* Map::CreateItemAtThatSpot(std::string lootTable, Inventory* inventory)
{
	ItemDefinition* newItemDefinition = new ItemDefinition(lootTable);
	Item* newItem = new Item();
	newItem->m_definition = newItemDefinition;

	inventory->m_Inventory.push_back(newItem);

	return newItem;
}

//---------------------------------A* Path Generation----------------------------------------------------

Path* Map::GenerateWalkablePath(IntVector2* start, IntVector2* end, Entity* entity)
{
	StartStepPeth(start, end, entity);
	bool isComplete = false;

	while (!isComplete)
	{
		isComplete = ContinueStepPath();
	}

	return m_currentPath;
}

//Can call the following two method seperately to display the path on its own.
void Map::StartStepPeth(IntVector2* start, IntVector2* end, Entity* entity)
{
	if (m_currentPathGenerator)
		delete m_currentPathGenerator;

	m_currentPathGenerator = new PathGenerator(this, start, end, entity);
	m_currentPath = new Path();
}

bool Map::ContinueStepPath()
{
	bool isComplete = m_currentPathGenerator->Step(m_currentPath);
	if (isComplete)
		return true;
	return false;
}


//------------------------Rendering-------------------------------------------
void Map::Render(Character* m_Player)
{
	for (unsigned int tileStep = 0; tileStep < m_tiles.size(); tileStep++)
	{
		RenderTiles(tileStep, m_tiles[tileStep].m_renderPosition.x, m_tiles[tileStep].m_renderPosition.y);

		//RenderEntitiesAndItems(m_Player, tileStep, m_tiles[tileStep].m_renderPosition.x, m_tiles[tileStep].m_renderPosition.y, m_tiles[tileStep].m_tileCords);
	}

	for (unsigned int tileStep = 0; tileStep < m_tiles.size(); tileStep++)
	{
		//RenderTiles(tileStep, m_tiles[tileStep].m_renderPosition.x, m_tiles[tileStep].m_renderPosition.y);

		RenderEntitiesAndItems(m_Player, tileStep, m_tiles[tileStep].m_renderPosition.x, m_tiles[tileStep].m_renderPosition.y, m_tiles[tileStep].m_tileCords);
	}	
	
	
	if(m_DialogEnabled == true)
		m_DialogForMap->RenderDialogBox();
}

void Map::RenderTiles(int tileStep, float baseLocationX, float baseLocationY)
{
	if (m_tiles[tileStep].m_Visible == false && m_tiles[tileStep].m_WasSeen == false)
		return;


	if(m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree1_lightred.png" || 
		m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree2_lightred.png" ||
		m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree1_red.png" ||
		m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree2_red.png" ||
		m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree1_yellow.png" ||
		m_tiles[tileStep].m_Texture->m_imageFilePath == "Data/Images/floor/tree2_yellow.png")
		g_renderSystem->DrawTexturedAABB2(AABB2(baseLocationX, baseLocationY, baseLocationX + 50.0f, baseLocationY + 50.0f), *g_renderSystem->CreateOrGetTexture("Data/Images/floor/ice0.png"), 0);


	g_renderSystem->DrawTexturedAABB2(AABB2(baseLocationX, baseLocationY, baseLocationX + 50.0f, baseLocationY + 50.0f), *m_tiles[tileStep].m_Texture, 0);
	if (m_tiles[tileStep].m_WasSeen == true && m_tiles[tileStep].m_Visible == false)
	{
		g_renderSystem->DrawAABB2(AABB2(baseLocationX, baseLocationY, baseLocationX + 50.0f, baseLocationY + 50.0f), Rgba(0, 0, 0, 155));
	}
}

void Map::RenderEntitiesAndItems(Character* m_Player,int tileStep, float baseLocationX, float baseLocationY, IntVector2 currentCoords)
{
	float space_Width = ((float)m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)m_dimensions.y / 900) * 900;

	if (m_tiles[tileStep].m_InventoryInSpace != nullptr && m_tiles[tileStep].m_InventoryInSpace->m_Inventory.size() != 0 && m_tiles[tileStep].m_Visible == true )
	{
		std::string glyphRepresentingAllInventory = m_tiles[tileStep].m_InventoryInSpace->m_Inventory[0]->m_definition->m_glyph;
		Rgba* colorRepresentingAllInventory = m_tiles[tileStep].m_InventoryInSpace->m_Inventory[0]->m_definition->m_glyphColor;
		g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(baseLocationX, baseLocationY), Vector2(baseLocationX + 50, baseLocationY + 50)), *m_tiles[tileStep].m_InventoryInSpace->m_Inventory[0]->m_definition->m_Texture, 0);
		//g_renderSystem->DrawText2D(Vector2(baseLocationX, baseLocationY), glyphRepresentingAllInventory, space_Height, space_Width, *colorRepresentingAllInventory, g_textSprites);
	}


	if (currentCoords == m_Player->m_CurrentCords) 
	{
		if (currentCoords == m_Player->m_CurrentCords && m_Player->m_currentStats.m_stamina > 0)
			m_Player->RenderPlayer(baseLocationX, baseLocationY);

		for (unsigned int damageStep = 0; damageStep < m_Player->m_damageTaken.size(); damageStep++)
		{
			if(m_Player->m_LastTimeSinceDamageTaken[damageStep] < 25)
				g_renderSystem->DrawText2D(Vector2(baseLocationX, baseLocationY + m_Player->m_LastTimeSinceDamageTaken[damageStep]), std::to_string(m_Player->m_damageTaken[damageStep]), 20, 20, Rgba(255, 0, 0), g_textSprites);
		}
	}



	for (unsigned int entityStep = 0; entityStep < m_entity.size(); entityStep++)
	{
		if (currentCoords == m_entity[entityStep]->m_CurrentCords && m_tiles[tileStep].m_Visible == true)
		{
			//g_renderSystem->DrawText2D(Vector2(baseLocationX, baseLocationY), m_entity[entityStep]->m_CharatcerDefintion->m_glyph, space_Height, space_Width, *m_entity[entityStep]->m_CharatcerDefintion->m_glyphColor, g_textSprites);
			g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(baseLocationX, baseLocationY + m_entity[entityStep]->m_AnimatePosition), Vector2(baseLocationX + 50, baseLocationY + 35 + m_entity[entityStep]->m_AnimatePosition)), *m_entity[entityStep]->m_CharatcerDefintion->m_Texture, 0);
			for (unsigned int damageStep = 0; damageStep < m_entity[entityStep]->m_damageTaken.size(); damageStep++)
			{
				if  (m_entity[entityStep]->m_LastTimeSinceDamageTaken[damageStep] < 25)
					g_renderSystem->DrawText2D(Vector2(baseLocationX, baseLocationY + m_entity[entityStep]->m_LastTimeSinceDamageTaken[damageStep]), std::to_string(m_entity[entityStep]->m_damageTaken[damageStep]), 20, 20, Rgba(255, 0, 0), g_textSprites);
			}
		}
	}
}


//------------------------Debugging------------------------------------------------
void Map::RenderPathDebugging()
{
	float space_Width = ((float)m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)m_dimensions.y / 900) * 900;

	for (unsigned int openNodeStep = 0; openNodeStep < m_currentPathGenerator->m_OpenNodes.size(); openNodeStep++)
	{
		AABB2 basicSquare(m_currentPathGenerator->m_OpenNodes[openNodeStep]->m_tile->m_renderPosition,
			Vector2(m_currentPathGenerator->m_OpenNodes[openNodeStep]->m_tile->m_renderPosition.x + space_Width, 
					m_currentPathGenerator->m_OpenNodes[openNodeStep]->m_tile->m_renderPosition.y + space_Height));

		g_renderSystem->DrawAABB2(basicSquare, Rgba(255, 0, 0, 255));

	}

	for (unsigned int closedNodeStep = 0; closedNodeStep < m_currentPathGenerator->m_ClosedNodes.size(); closedNodeStep++)
	{
		AABB2 basicSquare(m_currentPathGenerator->m_ClosedNodes[closedNodeStep]->m_tile->m_renderPosition,
			Vector2(m_currentPathGenerator->m_ClosedNodes[closedNodeStep]->m_tile->m_renderPosition.x + space_Width, 
					m_currentPathGenerator->m_ClosedNodes[closedNodeStep]->m_tile->m_renderPosition.y + space_Height));

		g_renderSystem->DrawAABB2(basicSquare, Rgba(255, 255, 255, 255));
	}
}


//-------------------------Helper Functions------------------------------------------

IntVector2 Map::FindWalkableSpotFarthestFromExitForStart(std::vector<Item*> questItem)
{
	IntVector2 startingLocation = IntVector2(0, 0);
	int distanceAwayFromPlayer = -1;
	//IntVector2 startingLocation = IntVector2(0, 0);
	Tile* exitTile = FindExitTile();
	if (exitTile == nullptr)
		exitTile = FindQuestItem(questItem);

	if (exitTile == nullptr)
		exitTile =GetTileAtTileCoords(FindRandomWalkableSpot());

	for (unsigned int tileIndex = 0; tileIndex < m_tiles.size(); tileIndex++)
	{
		if (!IsTileSolid(&m_tiles[tileIndex]))
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[tileIndex].m_tileCords, exitTile->m_tileCords);

			if (tempDistance > distanceAwayFromPlayer)
			{
				startingLocation = m_tiles[tileIndex].m_tileCords;
				distanceAwayFromPlayer = tempDistance;
			}
		}
	}
	return startingLocation;
}

IntVector2 Map::FindRandomWalkableSpot()
{
	bool WalkableSpotFound = false;
	int randIndex = 0;
	unsigned int attemptCounter = 0;

	while (!WalkableSpotFound)
	{
		randIndex = (rand() % m_tiles.size());
		if (!IsTileSolid(&m_tiles[randIndex]))
			WalkableSpotFound = true;

		attemptCounter++;
		if (attemptCounter > m_tiles.size() * 2)
		{
			WalkableSpotFound = true;
			randIndex = 0;
		}
	}
	return m_tiles[randIndex].m_tileCords;
}

IntVector2 Map::GetTileCordsForWorldPos(Vector2 currentPosition)
{
	IntVector2 tileCords(-1,-1);
	for (unsigned int tileIndex = 0; tileIndex < m_tiles.size(); tileIndex++)
	{

		Tile* topNeighbor = GetTileToTheTopOfGivenTile(m_tiles[tileIndex].m_tileCords);

		if (topNeighbor == nullptr)
			return tileCords;

		if (m_tiles[tileIndex].m_renderPosition.x <= currentPosition.x 
			&& m_tiles[tileIndex + 1].m_renderPosition.x > currentPosition.x 
			&&m_tiles[tileIndex].m_renderPosition.y <= currentPosition.y 
			&& topNeighbor->m_renderPosition.y > currentPosition.y)
				return m_tiles[tileIndex].m_tileCords;
	}
	return tileCords;
}

int Map::GetDistanceBetweenTwoEntities(Entity* firstPerson, Entity* secondPerson)
{
	return GetTileDistanceBetweenTwoTiles(firstPerson->m_CurrentCords, secondPerson->m_CurrentCords);
}
int Map::GetTileDistanceBetweenTwoTiles(IntVector2 firstTile, IntVector2 secondTile)
{
	IntVector2 tileDistance;
	tileDistance = firstTile - secondTile;
	return abs(tileDistance.x) + abs(tileDistance.y);
}

int Map::GetTileIndexForTile(Tile& tile)
{
	return tile.m_tileCords.x + (tile.m_tileCords.y * m_dimensions.y);
}

std::vector<Tile *> Map::GetTilesInNSquareAroundPassedInCords(int squareLength, IntVector2 startCoords)
{
	std::vector<Tile *> tiles;
	IntVector2 currentRightCoords = startCoords;
	IntVector2 currentLeftCoords = startCoords;

	Tile* aboveTile = GetTileAtTileCoords(startCoords);
	Tile* bottomTile = GetTileAtTileCoords(startCoords);

	//Around me
	for (int sqaureVerticalStep = 0; sqaureVerticalStep < squareLength; sqaureVerticalStep++)
	{
		if (aboveTile != nullptr)
		{
			aboveTile = GetTileToTheTopOfGivenTile(aboveTile->m_tileCords);
			if (aboveTile != nullptr)
				tiles.push_back(aboveTile);
		}
		if (bottomTile != nullptr)
		{
			bottomTile = GetTileToTheBottomOfGivenTile(bottomTile->m_tileCords);
			if (bottomTile != nullptr)
				tiles.push_back(bottomTile);
		}
	}

	for (int sqaureHorizontalStep = 0; sqaureHorizontalStep < squareLength; sqaureHorizontalStep++)
	{
		Tile* rightTile = GetTileToTheRightOfGivenTile(currentRightCoords);
		if (rightTile != nullptr)
			tiles.push_back(rightTile);
		currentRightCoords.x += 1;

		aboveTile = rightTile;
		bottomTile = rightTile;
		for (int sqaureVerticalStep = 0; sqaureVerticalStep < squareLength; sqaureVerticalStep++)
		{
			if (aboveTile != nullptr)
			{
				aboveTile = GetTileToTheTopOfGivenTile(aboveTile->m_tileCords);
				if (aboveTile != nullptr)
					tiles.push_back(aboveTile);
			}

			if (bottomTile != nullptr)
			{
				bottomTile = GetTileToTheBottomOfGivenTile(bottomTile->m_tileCords);
				if (bottomTile != nullptr)
					tiles.push_back(bottomTile);
			}
		}



		Tile* leftTile = GetTileToTheLeftOfGivenTile(currentLeftCoords);
		if (leftTile != nullptr)
			tiles.push_back(leftTile);
		currentLeftCoords.x -= 1;

		aboveTile = leftTile;
		bottomTile = leftTile;
		for (int sqaureVerticalStep = 0; sqaureVerticalStep < squareLength; sqaureVerticalStep++)
		{
			if (aboveTile != nullptr)
			{
				aboveTile = GetTileToTheTopOfGivenTile(aboveTile->m_tileCords);
				if (aboveTile != nullptr)
					tiles.push_back(aboveTile);
			}
			if (bottomTile != nullptr)
			{
				bottomTile = GetTileToTheBottomOfGivenTile(bottomTile->m_tileCords);
				if (bottomTile != nullptr)
					tiles.push_back(bottomTile);
			}
		}

	}
	return tiles;
}

Tile* Map::GetTileAtTileCoords(const IntVector2& tileCoords)
{
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_tileCords == tileCoords)
			return &m_tiles[mapStep];
	}
	return nullptr;
}

Tile* Map::GetTileAtWorldPos(Vector2 tileWorldCords)
{
	IntVector2 tileCords = GetTileCordsForWorldPos(tileWorldCords);
	return GetTileAtTileCoords(tileCords);
}

Tile* Map::GetTileToTheLeftOfGivenTile(const IntVector2& tileCoords)
{
	IntVector2 tileCordsWest = tileCoords + IntVector2(-1, 0);
	return GetTileAtTileCoords(tileCordsWest);
}

Tile* Map::GetTileToTheRightOfGivenTile(const IntVector2& tileCoords)
{
	IntVector2 tileCordsEast = tileCoords + IntVector2(1, 0);
	return GetTileAtTileCoords(tileCordsEast);
}

Tile* Map::GetTileToTheTopOfGivenTile(const IntVector2& tileCoords)
{

	IntVector2 tileCordsNorth = tileCoords + IntVector2(0, 1);
	return GetTileAtTileCoords(tileCordsNorth);
}

Tile* Map::GetTileToTheBottomOfGivenTile(const IntVector2& tileCoords)
{
IntVector2 tileCordsSouth = tileCoords + IntVector2(0, -1);
return GetTileAtTileCoords(tileCordsSouth);
}

Tile* Map::getLocalTileAtGivenScreenCoords(Vector2 mouseCords)
{
	float space_Width = ((float)m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)m_dimensions.y / 900) * 900;
	IntVector2 localTileCoords = IntVector2((int)mouseCords.x / (int)space_Width, (int)mouseCords.y / (int)space_Height);
	return GetTileAtTileCoords(localTileCoords);
}

Tile* Map::FindNearestTileOfType(Tile* myself)
{
	int distance = 9999999;
	Tile* nearestTile = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_definition == myself->m_definition)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_tileCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestTile = &m_tiles[mapStep];
			}
		}
	}
	return nearestTile;
}

Tile* Map::FindNearestTileNotOfType(Tile* myself)
{
	int distance = 9999999;
	Tile* nearestTile = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_definition != myself->m_definition)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_tileCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestTile = &m_tiles[mapStep];
			}
		}
	}
  return nearestTile;
}

Tile* Map::FindExitTile()
{
	for (unsigned int mapStep = 0; mapStep < m_tiles.size(); mapStep++)
	{
		if (m_tiles[mapStep].m_definition->m_TileName == "Exit" || m_tiles[mapStep].m_definition->m_TileName == "Teleport")
			return &m_tiles[mapStep];
	}
	return nullptr;
}

Tile* Map::FindQuestItem(std::vector<Item*> questItem)
{
	for (unsigned int mapStep = 0; mapStep < m_tiles.size(); mapStep++)
	{
		for(unsigned int questItemStep = 0; questItemStep < questItem.size(); questItemStep++)
			if(m_tiles[mapStep].m_InventoryInSpace != nullptr && m_tiles[mapStep].m_InventoryInSpace->m_Inventory.size() != 0)
				if (m_tiles[mapStep].m_InventoryInSpace->m_Inventory[0]->m_definition == questItem[questItemStep]->m_definition)
					return &m_tiles[mapStep];
	}
	return nullptr;
}

Entity* Map::GetEnemyInGivenTile(const IntVector2& tileCoords)
{
	for (unsigned int entityStep = 0; entityStep < m_entity.size(); entityStep++)
	{
		if (tileCoords == m_entity[entityStep]->m_CurrentCords)
			return m_entity[entityStep];
	}

	return nullptr;
}

Entity* Map::FindNearestEntityOfType(Entity* myself)
{
	int distance = 9999999;
	Entity* nearestEntity = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_CharacterInSpace->m_CharatcerDefintion == myself->m_CharatcerDefintion)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_CurrentCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestEntity = m_tiles[mapStep].m_CharacterInSpace;
			}
		}
	}
	return nearestEntity;
}

Entity* Map::FindNearestEntityNotOfType(Entity* myself)
{
	int distance = 9999999;
	Entity* nearestEntity = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_CharacterInSpace->m_CharatcerDefintion != myself->m_CharatcerDefintion)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_CurrentCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestEntity = m_tiles[mapStep].m_CharacterInSpace;
			}
		}
	}
	return nearestEntity;
}

Entity* Map::FindNearestEntityOfFaction(Entity* myself)
{
	int distance = 9999999;
	Entity* nearestEntity = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_CharacterInSpace->m_CharatcerDefintion->m_faction == myself->m_CharatcerDefintion->m_faction)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_CurrentCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestEntity = m_tiles[mapStep].m_CharacterInSpace;
			}
		}
	}
	return nearestEntity;
}

Entity* Map::FindNearestEntityNotOfFaction(Entity* myself)
{
	int distance = 9999999;
	Entity* nearestEntity = nullptr;
	for (int mapStep = 0; mapStep < m_dimensions.x * m_dimensions.y; mapStep++)
	{
		if (m_tiles[mapStep].m_CharacterInSpace->m_CharatcerDefintion->m_faction != myself->m_CharatcerDefintion->m_faction)
		{
			int tempDistance = GetTileDistanceBetweenTwoTiles(m_tiles[mapStep].m_tileCords, myself->m_CurrentCords);
			if (distance > tempDistance)
			{
				distance = tempDistance;
				nearestEntity = m_tiles[mapStep].m_CharacterInSpace;
			}
		}
	}
	return nearestEntity;
}

bool Map::IsTileSolid(const IntVector2& tileCoords)
{
	Tile* tile = GetTileAtTileCoords(tileCoords);
	if (tile == nullptr)
		return true;
	return tile->m_definition->m_isSolid;
}

bool Map::IsTileSolid(const Tile* tile)
{
	if (tile == nullptr)
		return true;
	return tile->m_definition->m_isSolid;
}

bool Map::HasLineOfSight(Vector2 startPos, Vector2 endPos)
{
	
	//Determine if it's too far away to begin with
	//if (CalcDistance(startPos, endPos) > 750.f)
		//return false;
	


	float space_Width = ((float)m_dimensions.x / 1600) * 1600 + 20;
	float space_Height = ((float)m_dimensions.y / 900) * 900;
	
	startPos = Vector2(startPos.x + space_Width / 2, startPos.y + space_Height / 2);
	endPos = Vector2(endPos.x + space_Width / 2, endPos.y + space_Height / 2);
	Vector2 currentPosition = startPos;
	
	float linedisplacementAmount = .90f;
	Vector2 lineDisplacement = endPos - startPos;
	
	while (abs(lineDisplacement.x) > space_Width)
	{
		lineDisplacement *= linedisplacementAmount;
		linedisplacementAmount *= linedisplacementAmount;
	}
	while (abs(lineDisplacement.y) > space_Height)
	{
		lineDisplacement *= linedisplacementAmount;
		linedisplacementAmount *= linedisplacementAmount;
	}


	int MaxSteps = (int)ceil(1/linedisplacementAmount);
	for (int lineIndex = 0; lineIndex < MaxSteps; lineIndex++)
	{
		IntVector2 tileCords = GetTileCordsForWorldPos(currentPosition);
		float distance = CalcDistance(startPos, currentPosition);
		if (IsTileSolid(tileCords) == true || distance > 750.f)
			return false;
		currentPosition += lineDisplacement;
	}
	return true;
}

void Map::SetTileToHaveEnemyInIt(Entity* characterInTile)
{
	Tile* tileWeNeedToSet = GetTileAtTileCoords(characterInTile->m_CurrentCords);
	tileWeNeedToSet->m_CharacterInSpace = characterInTile;
}

void Map::SetTileToHaveEnemyLeaveIt(Entity* characterInTile)
{
	Tile* tileWeNeedToSet = GetTileAtTileCoords(characterInTile->m_CurrentCords);
	tileWeNeedToSet->m_CharacterInSpace = nullptr;
}