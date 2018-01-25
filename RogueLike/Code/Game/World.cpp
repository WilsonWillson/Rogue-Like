#include "Game/World.hpp"
#include "Game/Behavior.hpp"
#include "Game/Magic.hpp"
#include "Game/DialogBoxes.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"
//----------------------------------------Constructors/Deconstructors-------------------------------------
World::World(std::string desieredAdventure)
{
	TileDefinition* newDefinition = new TileDefinition();
	newDefinition->ReadXMLFile();
	delete newDefinition;

	Character* newCharacter = new Character();
	newCharacter->ReadXMLFile();
	delete newCharacter;

	ItemDefinition* newItem = new ItemDefinition();
	newItem->ReadXMLFile();
	delete newItem;

	MapFeature* newFeature = new MapFeature();
	newFeature->ReadXMLFile();
	delete newFeature;


	CreateAdventure(desieredAdventure);
	PlacePlayerInWorld(m_PlayerAdventure.mapOrder[0]->m_MapName);

	m_InstructionSheet = false;
	m_IntroTextBlurbTimer = 0;
	m_CurrentlySelectedEquipment = 999;
	m_DialogBeforeNewLevelPlaying = false;
}

World::World()
{

}

World::~World()
{

}


//---------------------------------Creation---------------------------------------------------
void World::CreateAdventure(std::string desieredAdventure)
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Adventure.xml", "Adventures");
	
	int adventureCount = xMainNode.nChildNode("Adventure");
	for (int adventureStep = 0; adventureStep < adventureCount; adventureStep++)
	{
		std::string adventureName = xMainNode.getChildNode("Adventure", adventureStep).getAttributeValue(0);
		if (adventureName == desieredAdventure)
		{
			m_PlayerAdventure.description = xMainNode.getChildNode("Adventure", adventureStep).getAttributeValue(1);
			XMLNode adventureNode = xMainNode.getChildNode("Adventure", adventureStep);
			CreateAdventureFromXML(adventureNode);
		}
	}
}

void World::CreateAdventureFromXML(XMLNode adventureNode)
{
	int adventureDetailsCount = adventureNode.nChildNode();

	for (int adventureStep = 0; adventureStep < adventureDetailsCount; adventureStep++)
	{
		std::string adventureAttributeName = adventureNode.getChildNode(adventureStep).getName();
		if (adventureAttributeName == "VictoryConditions")
		{
			std::string desieredItem = adventureNode.getChildNode(adventureStep).getAttributeValue();
			ItemDefinition* itemDefine = new ItemDefinition(); itemDefine;
			Item* questItem = new Item();
			questItem->m_definition = itemDefine->m_QuestLootTable[desieredItem];
			questItem->m_currentlyEquipped = false;
			m_PlayerAdventure.victoryItems.push_back(questItem);
		}
		if (adventureAttributeName == "Map")
		{
			std::string exitTileName = "";
			std::string destinationTileName = "";
			Item* questItem = nullptr;
			DialogBoxes* dialogForMap = new DialogBoxes();
			int mapDetails = adventureNode.getChildNode(adventureStep).nChildNode();

			for(int mapDetailSteps = 0; mapDetailSteps < mapDetails; mapDetailSteps++)
			{
				XMLNode mapDetailNode = adventureNode.getChildNode(adventureStep).getChildNode(mapDetailSteps);
				std::string childName = mapDetailNode.getName();
				if (childName == "Item")
				{
					questItem = new Item();
					std::string desieredItem = mapDetailNode.getAttributeValue();
					ItemDefinition* itemDefine = new ItemDefinition(); itemDefine;
					questItem->m_definition = itemDefine->m_QuestLootTable[desieredItem];
				}
				if (childName == "Exit")
				{
					exitTileName = mapDetailNode.getAttributeValue(0);
					destinationTileName = mapDetailNode.getAttributeValue(1);
				}

				if (childName == "Dialogue")
				{
					std::string typeOfDialog = mapDetailNode.getAttributeValue(0);
					std::string speech = mapDetailNode.getAttributeValue(1);
					std::string image = mapDetailNode.getAttributeValue(2);

					dialogForMap->PushBackDialogToScene(typeOfDialog, speech, image);
				}
			}


			std::string desieredMapName = adventureNode.getChildNode(adventureStep).getAttributeValue(0);
			int powerLevel = ParseXmlAttribute(adventureNode, "Map", 0, 1, -1);
			int MaxpowerLevel = ParseXmlAttribute(adventureNode, "Map", 0, 2, -1);
			Map* startingMap = new Map(desieredMapName, questItem, exitTileName, powerLevel, MaxpowerLevel);

			startingMap->m_DialogForMap = dialogForMap;
			m_PlayerAdventure.mapOrder.push_back(startingMap);
			m_maps[startingMap->m_MapName] = startingMap;
		}
	}
}

void World::PlacePlayerInWorld(std::string desieredMapName)
{
	m_currentMap = m_maps[desieredMapName];

	m_Player = new Character();
	Character* newCharacterBuild = new Character(); newCharacterBuild;
	std::map<std::string, Character*>::iterator definition;
	definition = newCharacterBuild->s_characterRegistry.find("Player");
	m_Player->m_CharatcerDefintion = definition->second;
	m_Player->m_faction = "Player";
	m_Player->m_CurrentCords = m_currentMap->FindWalkableSpotFarthestFromExitForStart(m_PlayerAdventure.victoryItems);
	m_Player->m_currentStats = m_Player->m_CharatcerDefintion->m_baseStats;
	m_Player->m_baseStats = m_Player->m_currentStats;
	m_Player->m_inventory = new Inventory();
	m_Player->m_MagicSystem = new Magic();
	m_currentMap->SetVisibilityOfMap(m_Player);

	m_PlayerActionHasHappened = false;
}

void World::GoToNextLevel()
{
	std::string currentMapName = m_currentMap->m_MapName;

	for (unsigned int adventureMapStep = 0; adventureMapStep < m_PlayerAdventure.mapOrder.size(); adventureMapStep++)
	{
		if (currentMapName == m_PlayerAdventure.mapOrder[adventureMapStep]->m_MapName)
		{
			int nextMap = adventureMapStep + 1;
			m_currentMap = m_maps[m_PlayerAdventure.mapOrder[nextMap]->m_MapName];
			m_Player->m_CurrentCords = m_currentMap->FindWalkableSpotFarthestFromExitForStart(m_PlayerAdventure.victoryItems);
			m_currentMap->SetVisibilityOfMap(m_Player);

			TileDefinition* starisUpTile = new TileDefinition(); starisUpTile;
			std::map<std::string, TileDefinition*>::iterator it;
			it = starisUpTile->m_registry.find("Stairs Up");
			Tile* startTile = m_currentMap->GetTileAtTileCoords(m_Player->m_CurrentCords);
			startTile->m_definition = it->second;
			m_DialogBeforeNewLevelPlaying = false;
			return;
		}
	}
}


void World::GoToPreviousLevel()
{
	std::string currentMapName = m_currentMap->m_MapName;

	for (unsigned int adventureMapStep = 0; adventureMapStep < m_PlayerAdventure.mapOrder.size(); adventureMapStep++)
	{
		if (currentMapName == m_PlayerAdventure.mapOrder[adventureMapStep]->m_MapName)
		{
			int nextMap = adventureMapStep - 1;
			m_currentMap = m_maps[m_PlayerAdventure.mapOrder[nextMap]->m_MapName];
			Tile* exitTile = m_currentMap->FindExitTile();
			m_Player->m_CurrentCords = exitTile->m_tileCords;
			m_currentMap->SetVisibilityOfMap(m_Player);
			return;
		}
	}
}

//-------------------------------Update----------------------------------------------------------
void World::Update(std::string gameState, float deltaSeconds)
{
	if (g_inputSystem->WasKeyJustPressed(KEY_F5))
	{
		delete m_currentMap;
		m_maps.clear();
		//CreateMap();
		return;
	}
	
	UpdatePlayer(gameState, deltaSeconds);
	m_currentMap->Update(m_PlayerActionHasHappened, m_Player, deltaSeconds);
	m_PlayerActionHasHappened = false;
	m_IntroTextBlurbTimer += deltaSeconds;

	if(m_DialogBeforeNewLevelPlaying == true && m_currentMap->m_DialogForMap->m_DialogDone == true)
		GoToNextLevel();
}

void World::UpdatePlayer(std::string gameState, float deltaSeconds)
{
	for (unsigned int damageTakenStep = 0; damageTakenStep < m_Player->m_LastTimeSinceDamageTaken.size(); damageTakenStep++)
	{
		m_Player->m_LastTimeSinceDamageTaken[damageTakenStep] += deltaSeconds * 10;

		//m_Player->m_LastTimeSinceDamageTaken[damageTakenStep] = m_Player->m_LastTimeSinceDamageTaken.back();
		//m_Player->m_LastTimeSinceDamageTaken.pop_back();
		//m_Player->m_damageTaken[damageTakenStep] = m_Player->m_damageTaken.back();
		//m_Player->m_damageTaken.pop_back();
	}
	
	if (m_Player->m_currentStats.m_stamina < 1)
		return;
	if (gameState != "")
		return;

	PlayerInput();
}


void World::PlayerInput()
{
	PlayerMovement();
	PlayerAction();
}

void World::PlayerMovement()
{
	if (g_inputSystem->WasKeyJustPressed(KEY_LEFTARROW))
	{
		Tile* tempPosition = m_currentMap->GetTileToTheLeftOfGivenTile(m_Player->m_CurrentCords);
		if (tempPosition->m_definition->m_isSolid == true)
			return;

		if (PlayerAttack(tempPosition))
			return;

		m_currentMap->SetTileToHaveEnemyLeaveIt(m_Player);
		m_Player->m_CurrentCords.x -= 1;
		m_currentMap->SetTileToHaveEnemyInIt(m_Player);
		m_PlayerActionHasHappened = true;
		m_currentMap->SetVisibilityOfMap(m_Player);

		m_Player->currentMaxSprite = 7;
		m_Player->g_explosionAnimations[0]->m_currentSpriteIndex = m_Player->currentMaxSprite - 3;

		return;
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_RIGHTARROW))
	{
		Tile* tempPosition = m_currentMap->GetTileToTheRightOfGivenTile(m_Player->m_CurrentCords);
		if (tempPosition->m_definition->m_isSolid == true)
			return;

		if (PlayerAttack(tempPosition))
			return;

		m_currentMap->SetTileToHaveEnemyLeaveIt(m_Player);
		m_Player->m_CurrentCords.x += 1; 
		m_currentMap->SetTileToHaveEnemyInIt(m_Player);
		m_PlayerActionHasHappened = true;
		m_currentMap->SetVisibilityOfMap(m_Player);

		m_Player->currentMaxSprite = 11;
		m_Player->g_explosionAnimations[0]->m_currentSpriteIndex = m_Player->currentMaxSprite - 3;

		return;
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_UPARROW))
	{
		Tile* tempPosition = m_currentMap->GetTileToTheTopOfGivenTile(m_Player->m_CurrentCords);
		if (tempPosition->m_definition->m_isSolid == true)
			return;

		if (PlayerAttack(tempPosition))
			return;

		m_currentMap->SetTileToHaveEnemyLeaveIt(m_Player);
		m_Player->m_CurrentCords.y += 1; 
		m_currentMap->SetTileToHaveEnemyInIt(m_Player);
		m_PlayerActionHasHappened = true;
		m_currentMap->SetVisibilityOfMap(m_Player);

		m_Player->currentMaxSprite = 15;
		m_Player->g_explosionAnimations[0]->m_currentSpriteIndex = m_Player->currentMaxSprite - 3;

		return;
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_DOWNARROW))
	{
		Tile* tempPosition = m_currentMap->GetTileToTheBottomOfGivenTile(m_Player->m_CurrentCords);
		if (tempPosition->m_definition->m_isSolid == true)
			return;

		if (PlayerAttack(tempPosition))
			return;

		m_currentMap->SetTileToHaveEnemyLeaveIt(m_Player);
		m_Player->m_CurrentCords.y -= 1;
		m_currentMap->SetTileToHaveEnemyInIt(m_Player);
		m_PlayerActionHasHappened = true;
		m_currentMap->SetVisibilityOfMap(m_Player);

		m_Player->currentMaxSprite = 3;
		m_Player->g_explosionAnimations[0]->m_currentSpriteIndex = m_Player->currentMaxSprite - 3;

		return;
	}
}

void World::PlayerAction()
{
	if (g_inputSystem->WasKeyJustPressed(KEY_SPACE))
	{
		Tile* currentTile = m_currentMap->GetTileAtTileCoords(m_Player->m_CurrentCords);

		if (currentTile->m_InventoryInSpace != nullptr)
		{
			for (unsigned int inventoryStep = 0; inventoryStep < currentTile->m_InventoryInSpace->m_Inventory.size(); inventoryStep++)
			{
				if (currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]->m_definition->m_equipSlot == NONE)
				{
					m_Player->m_inventory->m_Inventory.push_back(currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]);
					CheckQuestItemCompletion();
				}
				else
				{
					//For A02 Automatically equip best item for each slot Since no UI has been made yet.  Currently since this is temp code, it just equipds items that are empty slots for player
					if (!m_Player->IsThereACurrentItemEquippedInThisSlot(currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]))
					{
						currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]->m_currentlyEquipped = true;
						m_Player->ChangeStatsWithEquippedItem(currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]);
					}
					m_Player->m_inventory->m_Inventory.push_back(currentTile->m_InventoryInSpace->m_Inventory[inventoryStep]);
				}
			}
			m_PlayerActionHasHappened = true;
			currentTile->m_InventoryInSpace = nullptr;
		}

		if (currentTile->m_definition->m_TileName == "Exit" || currentTile->m_definition->m_TileName == "Teleport")
		{ 
			if (m_currentMap->m_DialogForMap->m_DialogDone != true)
			{
				*m_currentMap->m_CurrentStoryPositionInMap = END;
				m_DialogBeforeNewLevelPlaying = true;
			}
			else
				GoToNextLevel();
		}
		if (currentTile->m_definition->m_TileName == "Stairs Up")
			GoToPreviousLevel();
	}
}

void World::CheckQuestItemCompletion()
{
	m_PlayerAdventure.questDone = true;
	for (unsigned int questCompletionStep = 0; questCompletionStep < m_PlayerAdventure.victoryItems.size(); questCompletionStep++)
	{
		bool foundItem = false;
		for (unsigned int playerInventoryStep = 0; playerInventoryStep < m_Player->m_inventory->m_Inventory.size(); playerInventoryStep++)
		{
			if (m_PlayerAdventure.victoryItems[questCompletionStep]->m_definition == m_Player->m_inventory->m_Inventory[playerInventoryStep]->m_definition)
			{
				foundItem = true;
				*m_currentMap->m_CurrentStoryPositionInMap = QUEST;
				return;
			}

		}

		if (foundItem == false)
			m_PlayerAdventure.questDone = false;
	}
}


bool World::PlayerAttack(Tile* tempPosition)
{
	Entity* m_enemyInSpace = m_currentMap->GetEnemyInGivenTile(tempPosition->m_tileCords);
	int damage = 0;
	if (m_enemyInSpace == nullptr)
		return false;

	if(m_enemyInSpace->m_CharatcerDefintion->m_weakness == m_Player->GetCurrentlyEquippedWeaponType())
		damage -=(int)( 1.5f * m_Player->m_currentStats.m_strength);
	else
		damage -= m_Player->m_currentStats.m_strength;

	float critChance = GetRandomFloatInRange(0, 1);

	if (m_Player->m_currentStats.m_agility * .001 > critChance)
		damage *= 2;

	m_enemyInSpace->m_currentStats.m_stamina += damage;

	m_enemyInSpace->m_damageTaken.push_back(m_Player->m_currentStats.m_strength);
	m_enemyInSpace->m_LastTimeSinceDamageTaken.push_back(0);


	m_PlayerActionHasHappened = true;
	return true;
}


void World::UpdateStatsScreen()
{	
	if (m_CurrentlySelectedEquipment != 999)
	{
		CheckToReplaceItem();
		return;
	}
	if (m_CurrentlySelectedEquipment == 999)
	{
		CheckToPickItem();
		return;
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_BACKSPACE))
		m_CurrentlySelectedEquipment = 999;
}

void World::CheckToReplaceItem()
{
	int selectedInventoryItem = 999;
	if (g_inputSystem->WasKeyJustPressed(KEY_1))
		selectedInventoryItem = 1;
	if (g_inputSystem->WasKeyJustPressed(KEY_2))
		selectedInventoryItem = 2;
	if (g_inputSystem->WasKeyJustPressed(KEY_3))
		selectedInventoryItem = 3;
	if (g_inputSystem->WasKeyJustPressed(KEY_4))
		selectedInventoryItem = 4;
	if (g_inputSystem->WasKeyJustPressed(KEY_5))
		selectedInventoryItem = 5;
	if (g_inputSystem->WasKeyJustPressed(KEY_6))
		selectedInventoryItem = 6;
	if (g_inputSystem->WasKeyJustPressed(KEY_7))
		selectedInventoryItem = 7;
	if (g_inputSystem->WasKeyJustPressed(KEY_8))
		selectedInventoryItem = 8;
	if (g_inputSystem->WasKeyJustPressed(KEY_9))
		selectedInventoryItem = 9;
	if (g_inputSystem->WasKeyJustPressed(KEY_0))
		selectedInventoryItem = 0;

	if (selectedInventoryItem != 999)
	{
		int trackCounter = 0;
		int equippedItem = m_CurrentlySelectedEquipment;
		m_CurrentlySelectedEquipment = 999;
		Item* selectedItem = nullptr;
		for (unsigned int equippedStep = 0; equippedStep < m_Player->m_inventory->m_Inventory.size(); equippedStep++)
		{
			if (m_Player->m_inventory->m_Inventory[equippedStep]->m_currentlyEquipped)
			{ 
				trackCounter++;
				if(trackCounter == equippedItem)
					selectedItem = m_Player->m_inventory->m_Inventory[equippedStep];
			}
		}


		trackCounter = 0;
		for (unsigned int unequippedStep = 0; unequippedStep < m_Player->m_inventory->m_Inventory.size(); unequippedStep++)
		{
			if (!m_Player->m_inventory->m_Inventory[unequippedStep]->m_currentlyEquipped)
			{
				trackCounter++;
				if (trackCounter == selectedInventoryItem)
				{
					m_Player->m_inventory->m_Inventory[unequippedStep]->m_currentlyEquipped = true;
					m_Player->ChangeStatsWithEquippedItem(m_Player->m_inventory->m_Inventory[unequippedStep]);

					m_Player->ChangeStatsByUnequipingItem(selectedItem);
					selectedItem->m_currentlyEquipped = false;
				}
			}
		}
	}
}

void World::CheckToPickItem()
{
	if (g_inputSystem->WasKeyJustPressed(KEY_1))
		m_CurrentlySelectedEquipment = 1;
	if (g_inputSystem->WasKeyJustPressed(KEY_2))
		m_CurrentlySelectedEquipment = 2;
	if (g_inputSystem->WasKeyJustPressed(KEY_3))
		m_CurrentlySelectedEquipment = 3;
	if (g_inputSystem->WasKeyJustPressed(KEY_4))
		m_CurrentlySelectedEquipment = 4;
	if (g_inputSystem->WasKeyJustPressed(KEY_5))
		m_CurrentlySelectedEquipment = 5;
	if (g_inputSystem->WasKeyJustPressed(KEY_6))
		m_CurrentlySelectedEquipment = 6;
	if (g_inputSystem->WasKeyJustPressed(KEY_7))
		m_CurrentlySelectedEquipment = 7;
	if (g_inputSystem->WasKeyJustPressed(KEY_8))
		m_CurrentlySelectedEquipment = 8;
	if (g_inputSystem->WasKeyJustPressed(KEY_9))
		m_CurrentlySelectedEquipment = 9;
	if (g_inputSystem->WasKeyJustPressed(KEY_0))
		m_CurrentlySelectedEquipment = 0;
}


//---------------------------------------Rendering---------------------------------
void World::Render()
{
	RenderCurrentPlayerInfo();
	m_currentMap->Render(m_Player);
	RenderCurrentGameStatusDescriptions();
}

void World::RenderCurrentPlayerInfo()
{
	g_renderSystem->DrawAABB2(AABB2(Vector2(1500, 0), Vector2(1600, 900)), Rgba(200, 170, 130, 255));


  Vector2 toolTipLeftCorner;
  Vector2 previousEndLocation;
  Vector2 mouseCords = g_inputSystem->m_currentMouseCords;
  mouseCords.y = RangeMapFloat(mouseCords.y, 0, 900, 900, 0);
  Tile* tile = m_currentMap->getLocalTileAtGivenScreenCoords(mouseCords);


  if (tile != nullptr)
  {
	  // g_renderSystem->DrawText2D(AABB2(Vector2(1510, 870), Vector2(1590, 870)), tile->m_definition->m_TileName, Rgba(0, 0, 0, 255), g_textSprites);
	  g_renderSystem->DrawCenteredText2D(Vector2(1550, 870), tile->m_definition->m_TileName, 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);

	  if (tile->m_CharacterInSpace != nullptr)
	  {
		  g_renderSystem->DrawCenteredText2D(Vector2(1550, 750), "Enemy:", 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);
		  g_renderSystem->DrawCenteredText2D(Vector2(1550, 700), tile->m_CharacterInSpace->m_CharatcerDefintion->m_name, 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);
		  std::string monsterHealth = std::to_string(tile->m_CharacterInSpace->m_currentStats.m_stamina) + " / " + std::to_string(tile->m_CharacterInSpace->m_CharatcerDefintion->m_baseStats.m_stamina);
		  g_renderSystem->DrawCenteredText2D(Vector2(1550, 650), monsterHealth, 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);
	  }

	  if (tile->m_InventoryInSpace != nullptr)
	  {
		  g_renderSystem->DrawCenteredText2D(Vector2(1550, 550), "Items:", 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);

		  float currentYPosition = 500;
		  for (unsigned int inventoryStep = 0; inventoryStep < tile->m_InventoryInSpace->m_Inventory.size(); inventoryStep++)
		  {
			  g_renderSystem->DrawCenteredText2D(Vector2(1550, currentYPosition), tile->m_InventoryInSpace->m_Inventory[inventoryStep]->m_definition->m_name, 50.0f, 85.0f, Rgba(0, 0, 0, 255), g_textSprites);
			  currentYPosition = currentYPosition - 50;
		  }
	  }
  }
}


void World::RenderCurrentGameStatusDescriptions()
{
	if (m_Player->m_currentStats.m_stamina <= 0)
	{
		g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 200));
		g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 450.0f), "You Are Dead", 100.0f, 600.0f, Rgba(255, 0, 0, 255), g_textSprites);
	}

	if (m_PlayerAdventure.questDone == true && m_currentMap->m_DialogEnabled == false)
	{
		g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 200));
		g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 450.0f), "You Have Finished your Quest! Press Esc to start a new one", 100.0f, 1000.0f, Rgba(255, 255, 255, 255), g_textSprites);
	}

	if (m_IntroTextBlurbTimer < 3)
	{
		g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 125));
		g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 450.0f), m_PlayerAdventure.description, 100.0f, 600.0f, Rgba(255, 255, 255, 255), g_textSprites);
	}
}

void World::RenderStatsScreen()
{
	g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 175));
	g_renderSystem->DrawCenteredText2D(Vector2(300.0f, 750.0f), "Stat Screen", 100.0f, 600.0f, Rgba(255, 255, 255, 255), g_textSprites);

	RenderPlayerStats();

	RenderInventory();
}


void World::RenderPlayerStats()
{
	std::stringstream ss;
	g_renderSystem->DrawCenteredText2D(Vector2(100.0f, 675.0f), "Stats", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);

	ss << m_Player->m_currentStats.m_stamina << "/" << m_Player->m_baseStats.m_stamina;
	g_renderSystem->DrawCenteredText2D(Vector2(125.0f, 645.0f), "Stamina:" + ss.str(), 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);

	ss.str("");
	ss << m_Player->m_currentStats.m_strength << "/" << m_Player->m_baseStats.m_strength;
	g_renderSystem->DrawCenteredText2D(Vector2(125.0f, 615.0f), "Strength:" + ss.str(), 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);

	ss.str("");
	ss << m_Player->m_currentStats.m_intellegence << "/" << m_Player->m_baseStats.m_intellegence;
	g_renderSystem->DrawCenteredText2D(Vector2(125.0f, 585.0f), "Intelligence:" + ss.str(), 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);

	ss.str("");
	ss << m_Player->m_currentStats.m_agility << "/" << m_Player->m_baseStats.m_agility;
	g_renderSystem->DrawCenteredText2D(Vector2(125.0f, 555.0f), "Agility:" + ss.str(), 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);
}


void World::RenderInventory()
{
	//g_renderSystem->DrawCenteredText2D(Vector2(400.0f, 675.0f), "Inventory:", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawCenteredText2D(Vector2(900.0f, 645.0f), "Click on two items to switch them in your inventory", 30.0f, 1150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	int   currentInventoryPosition = 1;
	Rgba unselectedColor = Rgba(255, 255, 255, 255);
	Rgba selectedColor = Rgba(0, 255, 0, 255);
	Vector2 mouseCords = g_inputSystem->m_currentMouseCords;
	mouseCords.y = RangeMapFloat(mouseCords.y, 0, 900, 900, 0);
	g_renderSystem->DrawCenteredText2D(Vector2(400.0f, 675.0f), std::to_string(mouseCords.y), 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);

	DrawCurrentEquipment();
	DrawInventory();
}

void World::WriteItemStatsOnStatsScreen(ItemDefinition* item)
{
	g_renderSystem->DrawText2D(Vector2(600.0f, 540), item->m_name, 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);
	std::string itemType = item->ConvertSlotEnumValuetoString(item);
	g_renderSystem->DrawText2D(Vector2(600.0f, 510), itemType, 30.0f, 100.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(600.0f, 480), "Stamina: ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(750.0f, 480), std::to_string(item->m_statModifiers.m_stamina), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(600.0f, 450), "Strength: ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(750.0f, 450), std::to_string(item->m_statModifiers.m_strength), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(600.0f, 420), "Agilty: ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(750.0f, 420), std::to_string(item->m_statModifiers.m_agility), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(600.0f, 390), "Intellect: ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(750.0f, 390), std::to_string(item->m_statModifiers.m_intellegence), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
	

}


void World::DrawInventory()
{
	int   currentInventoryPosition = 1;
	Vector2 mouseCords = g_inputSystem->m_currentMouseCords;
	mouseCords.y = RangeMapFloat(mouseCords.y, 0, 900, 900, 0);
	float currentYPosition = 350.0f;
	g_renderSystem->DrawText2D(Vector2(325.0f, 380), "Current Inventory:", 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);
	for (unsigned int equipStep = 0; equipStep < m_Player->m_inventory->m_Inventory.size(); equipStep++)
	{
		if (!m_Player->m_inventory->m_Inventory[equipStep]->m_currentlyEquipped)
		{
			currentYPosition -= 30;

			if(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
				g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, currentYPosition), Vector2(350.f, currentYPosition + 25)), Rgba(0,150,0,250));

			g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(325.0f, currentYPosition), Vector2(350.f, currentYPosition + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

			if (mouseCords.x > 325 && mouseCords.x < 350 && mouseCords.y > currentYPosition && mouseCords.y < currentYPosition + 25)
			{
				WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
				g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
				g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
				if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
				{
					m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
				}
			}

			currentInventoryPosition++;
		}
	}
}

void World::DrawCurrentEquipment()
{
	Vector2 mouseCords = g_inputSystem->m_currentMouseCords;
	mouseCords.y = RangeMapFloat(mouseCords.y, 0, 900, 900, 0);
	g_renderSystem->DrawText2D(Vector2(325.0f, 550.0f), "Currently Equipment:", 30.0f, 200.0f, Rgba(255, 255, 255, 255), g_textSprites);


	Rgba EmptySlotColor = Rgba(255, 255, 255, 255);
	//Head
	g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 505), Vector2(350.f, 505 + 25)), EmptySlotColor);
	//Chest
	g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 475), Vector2(350.f, 475 + 25)), EmptySlotColor);
	//Gloves
	g_renderSystem->DrawAABB2(AABB2(Vector2(295.0f, 475), Vector2(320.f, 475 + 25)), EmptySlotColor);
	//Feet
	g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 445), Vector2(350.f, 445 + 25)), EmptySlotColor);
	//Weapons
	g_renderSystem->DrawAABB2(AABB2(Vector2(355.0f, 475), Vector2(380.f, 475 + 25)), EmptySlotColor);
	//Shield
	g_renderSystem->DrawAABB2(AABB2(Vector2(385.0f, 475), Vector2(410.f, 475 + 25)), EmptySlotColor);
	//Rings
	g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 415), Vector2(350.f, 415 + 25)), EmptySlotColor);
	//Neck
	g_renderSystem->DrawAABB2(AABB2(Vector2(355.0f, 415), Vector2(380.f, 415 + 25)), EmptySlotColor);

	for (unsigned int equipStep = 0; equipStep < m_Player->m_inventory->m_Inventory.size(); equipStep++)
	{
		if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlyEquipped)
		{

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == HEAD)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(325.0f, 505), Vector2(350.f, 505 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);
				
				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 505), Vector2(350.f, 505 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 325 && mouseCords.x < 350 && mouseCords.y > 505 && mouseCords.y < 505 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}

			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == CHEST)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(325.0f, 475), Vector2(350.f, 475 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 475), Vector2(350.f, 475 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 325 && mouseCords.x < 350 && mouseCords.y > 475 && mouseCords.y < 475 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == GLOVES)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(295.0f, 475), Vector2(320.f, 475 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(295.0f, 475), Vector2(320.f, 475 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 295 && mouseCords.x < 320 && mouseCords.y > 475 && mouseCords.y < 475 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == FEET)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(325.0f, 445), Vector2(350.f, 445 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 445), Vector2(350.f, 445 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 325 && mouseCords.x < 350 && mouseCords.y > 445 && mouseCords.y < 445 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == WEAPONS)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(355.0f, 475), Vector2(380.f, 475 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(355.0f, 475), Vector2(380.f, 475 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 355 && mouseCords.x < 380 && mouseCords.y > 475 && mouseCords.y < 475 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == SHIELD) 
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(385.0f, 475), Vector2(410.f, 475 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(385.0f, 475), Vector2(410.f, 475 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 385 && mouseCords.x < 410 && mouseCords.y > 475 && mouseCords.y < 475 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition); 
					g_renderSystem->DrawText2D(Vector2(600.0f, 360), "Selected? ", 30.0f, 150.0f, Rgba(255, 255, 255, 255), g_textSprites);
					g_renderSystem->DrawText2D(Vector2(750.0f, 360), std::to_string(m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected), 30.0f, 20.0f, Rgba(255, 255, 255, 255), g_textSprites);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == RINGS) 
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(325.0f, 415), Vector2(350.f, 415 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 415), Vector2(350.f, 415 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 325 && mouseCords.x < 350 && mouseCords.y > 415 && mouseCords.y < 415 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition);
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}

			if (m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_equipSlot == NECK)
			{
				g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(355.0f, 415), Vector2(380.f, 415 + 25)), *m_Player->m_inventory->m_Inventory[equipStep]->m_definition->m_Texture, 0);

				if (m_Player->m_inventory->m_Inventory[equipStep]->m_currentlySelected == true)
					g_renderSystem->DrawAABB2(AABB2(Vector2(325.0f, 415), Vector2(350.f, 415 + 25)), Rgba(0, 150, 0, 250));

				if (mouseCords.x > 355 && mouseCords.x < 380 && mouseCords.y > 415 && mouseCords.y < 415 + 25)
				{
					WriteItemStatsOnStatsScreen(m_Player->m_inventory->m_Inventory[equipStep]->m_definition); 
					if (g_inputSystem->IsKeyDown(MOUSE_LBUTTON))
					{
						m_Player->m_inventory->SelectItem(m_Player->m_inventory->m_Inventory[equipStep], m_Player);
					}
				}
			}
		}
	}
}

void World::RenderControlScheme(std::string gameState)
{
	if(gameState != "Instructions")
		g_renderSystem->DrawCenteredText2D(Vector2(800, 875), "Press F1 for Controls", 25, 400, Rgba(255, 255, 255, 255), g_textSprites);
	else
	{
		g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 175));
		g_renderSystem->DrawCenteredText2D(Vector2(800, 700), "CONTROLS", 100, 300, Rgba(255, 255, 255, 255), g_textSprites);

		g_renderSystem->DrawText2D(Vector2(500, 550), "Arrow Keys/WASD: Movement", 50, 300, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 500), "Space Bar: Pick Up Weapon", 50, 350, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 450), "I: Inventory Screen", 50, 200, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 400), "ESC: Quit To Main Menu", 50, 300, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 350), "Tilde: Open Console Command", 50, 365, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 300), "F1: Toggle Instruction Sheet", 50, 375, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 250), "F2: A* Path Debug View ", 50, 350, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 200), "F3: Behavior Debug View", 50, 350, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 150), "F4: Block Info View", 50, 250, Rgba(255, 255, 255, 255), g_textSprites);
		g_renderSystem->DrawText2D(Vector2(500, 100), "F5: Create New Map at Random", 50, 385, Rgba(255, 255, 255, 255), g_textSprites);
	}
}


//--------------------------------Debugging----------------------------------------
void World::RenderTooltip()
{
	Vector2 toolTipLeftCorner;
	Vector2 previousEndLocation;
	Vector2 mouseCords = g_inputSystem->m_currentMouseCords;
	mouseCords.y = RangeMapFloat(mouseCords.y, 0, 900, 900, 0);
	Tile* tile = m_currentMap->getLocalTileAtGivenScreenCoords(mouseCords);

	if (mouseCords.x > 800)
		toolTipLeftCorner.x = mouseCords.x - 200;
	else
		toolTipLeftCorner.x = mouseCords.x + 200;

	if (mouseCords.y > 450)
		toolTipLeftCorner.y = mouseCords.y - 350;
	else
		toolTipLeftCorner.y = mouseCords.y + 100;


	AABB2 boxBounds = GetToolTipBounds(tile);
	int howManySteps = (int)boxBounds.m_maxs.y / 50;
	int currentTextStep = 1;

	if (tile != nullptr)
	{
		AABB2 locationInTheWorldBoxBounds = AABB2(Vector2(boxBounds.m_mins.x + toolTipLeftCorner.x, boxBounds.m_mins.y + toolTipLeftCorner.y), Vector2(boxBounds.m_maxs.x + toolTipLeftCorner.x, boxBounds.m_maxs.y + toolTipLeftCorner.y));
		g_renderSystem->DrawDebugAABB2(locationInTheWorldBoxBounds, 1, Rgba(255, 255, 255, 255), Rgba(0, 0, 0, 255));
		locationInTheWorldBoxBounds.m_mins.x += 10;


		AABB2 textLocation = AABB2(locationInTheWorldBoxBounds.m_mins, Vector2(locationInTheWorldBoxBounds.m_maxs.x - 5, currentTextStep * (boxBounds.m_maxs.y / howManySteps) + toolTipLeftCorner.y));
		previousEndLocation = Vector2(locationInTheWorldBoxBounds.m_mins.x, textLocation.m_maxs.y);
		g_renderSystem->DrawText2D(textLocation, tile->m_definition->m_TileName, Rgba(0, 0, 0, 255), g_textSprites);
		currentTextStep++;

		if (tile->m_CharacterInSpace != nullptr)
		{
			textLocation = AABB2(previousEndLocation, Vector2(locationInTheWorldBoxBounds.m_maxs.x - 5, currentTextStep * (boxBounds.m_maxs.y / howManySteps) + toolTipLeftCorner.y));
			g_renderSystem->DrawText2D(textLocation, tile->m_CharacterInSpace->m_CharatcerDefintion->m_name, Rgba(0, 0, 0, 255), g_textSprites);
			previousEndLocation = Vector2(locationInTheWorldBoxBounds.m_mins.x, textLocation.m_maxs.y);
			currentTextStep++;

			textLocation = AABB2(previousEndLocation, Vector2(locationInTheWorldBoxBounds.m_maxs.x - 40, currentTextStep * (boxBounds.m_maxs.y / howManySteps) + toolTipLeftCorner.y));
			g_renderSystem->DrawText2D(textLocation, std::to_string(tile->m_CharacterInSpace->m_currentStats.m_stamina), Rgba(0, 0, 0, 255), g_textSprites);
			previousEndLocation = Vector2(locationInTheWorldBoxBounds.m_mins.x, textLocation.m_maxs.y);
			currentTextStep++;
		}

		if (tile->m_InventoryInSpace != nullptr)
		{
			for (unsigned int inventoryStep = 0; inventoryStep < tile->m_InventoryInSpace->m_Inventory.size(); inventoryStep++)
			{
				std::string ItemName = tile->m_InventoryInSpace->m_Inventory[inventoryStep]->m_definition->m_name;
				textLocation = AABB2(previousEndLocation, Vector2(locationInTheWorldBoxBounds.m_maxs.x - 5, currentTextStep * (boxBounds.m_maxs.y / howManySteps) + toolTipLeftCorner.y));
				g_renderSystem->DrawText2D(textLocation, ItemName, Rgba(0, 0, 0, 255), g_textSprites);
				previousEndLocation = Vector2(locationInTheWorldBoxBounds.m_mins.x, textLocation.m_maxs.y);
				currentTextStep++;
			}
		}

		if (tile->m_MapFeatureInSpace != nullptr)
		{
			textLocation = textLocation = AABB2(locationInTheWorldBoxBounds.m_mins, Vector2(locationInTheWorldBoxBounds.m_maxs.x - 5, boxBounds.m_maxs.y / howManySteps + toolTipLeftCorner.y));
			g_renderSystem->DrawText2D(textLocation, tile->m_MapFeatureInSpace->m_name, Rgba(0, 0, 0, 255), g_textSprites);
			previousEndLocation = Vector2(locationInTheWorldBoxBounds.m_mins.x, textLocation.m_maxs.y);
			currentTextStep++;
		}
	}
}

AABB2 World::GetToolTipBounds(Tile* tile)
{
	AABB2 tooltipBounds;
	tooltipBounds.m_mins = Vector2(0, 0);
	tooltipBounds.m_maxs = Vector2(175, 0);
	if (tile != nullptr)
	{
		tooltipBounds.m_maxs.y = 55;

		if (tile->m_CharacterInSpace != nullptr)
		{
			tooltipBounds.m_maxs.y = 155;
		}

		if (tile->m_InventoryInSpace != nullptr)
		{
			for (unsigned int inventoryStep = 0; inventoryStep < tile->m_InventoryInSpace->m_Inventory.size(); inventoryStep++)
			{
				tooltipBounds.m_maxs.y += 50;
			}
		}

		if (tile->m_MapFeatureInSpace != nullptr)
			tooltipBounds.m_maxs.y += 50;
	}
	return tooltipBounds;
}



void World::RenderBehaviorDebugging()
{
	for (size_t entityStep = 0; entityStep < m_currentMap->m_entity.size(); entityStep++)
	{
		if(m_currentMap->m_entity[entityStep]->m_currentBehavior != nullptr)
			m_currentMap->m_entity[entityStep]->m_currentBehavior->DebugRender(*m_currentMap->m_entity[entityStep], m_currentMap);
	}
}


void World::UpdateDebugPathing()
{
	if (g_inputSystem->WasKeyJustPressed(KEY_SPACE))
	{
		if (m_DoneDebugging == true)
			{
				m_DoneDebugging = false;
				for (unsigned int entityStep = 0; entityStep < m_currentMap->m_entity.size(); entityStep++)
				{
					if (m_currentMap->m_entity[entityStep]->m_currentPath == nullptr)
					{
						m_currentMap->StartStepPeth(&m_currentMap->m_entity[entityStep]->m_CurrentCords, &m_Player->m_CurrentCords, m_currentMap->m_entity[entityStep]);
						return;
					}
				}
			}
		else
			{
				m_DoneDebugging = m_currentMap->ContinueStepPath();
			}
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_ENTER))
	{
		if (m_DoneDebugging == true)
		{
			m_DoneDebugging = false;
			for (unsigned int entityStep = 0; entityStep < m_currentMap->m_entity.size(); entityStep++)
			{
				if (m_currentMap->m_entity[entityStep]->m_currentPath == nullptr)
				{
					m_currentMap->GenerateWalkablePath(&m_currentMap->m_entity[entityStep]->m_CurrentCords, &m_Player->m_CurrentCords, m_currentMap->m_entity[entityStep]);
					return;
				}
			}
		}
		else
		{
			while (!m_DoneDebugging) 
			{
				m_DoneDebugging = m_currentMap->ContinueStepPath();
			}
		}
	}
}

void World::RenderPathDebugging()
{
	g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(0, 0, 0, 125));
	m_currentMap->RenderPathDebugging();
}

void World::RenderRayCast()
{
	const int NUM_SIDES = m_Player->rayCastAmount;
	const float DEGREES_PER_SIDE = 360.f / (float)NUM_SIDES;

	Tile* currentPlayerTile = m_currentMap->GetTileAtTileCoords(m_Player->m_CurrentCords);

	for (int amountOfRaycasts = 0; amountOfRaycasts < m_Player->rayCastAmount; amountOfRaycasts++)
	{
		float angleDegrees = DEGREES_PER_SIDE * (float)amountOfRaycasts;
		Vector2 visionDistance;
		visionDistance.x = currentPlayerTile->m_renderPosition.x + m_Player->distance * CosDegrees(angleDegrees);
		visionDistance.y = currentPlayerTile->m_renderPosition.y + m_Player->distance * SinDegrees(angleDegrees);

		g_renderSystem->DrawLine2D(currentPlayerTile->m_renderPosition, visionDistance, Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
	}
}
