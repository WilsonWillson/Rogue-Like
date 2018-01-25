#pragma once
#include "Game/Map.hpp"
#include "Game/Character.hpp"
#include <map>
#include <vector>
#include <sstream>


struct Adventure
{
	std::vector<Map*> mapOrder;
	std::vector<Item*> victoryItems;
	bool				questDone;
	std::string	        description;
};



class Character;

class World
{

public:
	World();
	World(std::string desieredAdventure);
	~World();

	void Update(std::string gameState, float deltaSeconds);
	void UpdatePlayer(std::string gameState, float deltaSeconds);

	void UpdateStatsScreen();
	void CheckToReplaceItem();

	void CheckQuestItemCompletion();

	void Render();
	void RenderStatsScreen();
	void RenderControlScheme(std::string gameState);

	void UpdateDebugPathing();
	void RenderBehaviorDebugging();
	void RenderPathDebugging();
	void RenderCurrentPlayerInfo();
	void RenderRayCast();

	void RenderTooltip();
	AABB2 GetToolTipBounds(Tile* tile);

	void GoToNextLevel();

private:
	void PlacePlayerInWorld(std::string desieredMapName);
	void CreateAdventure(std::string desieredAdventure);
	void CreateAdventureFromXML(XMLNode adventureNode);

	bool PlayerAttack(Tile* tempPosition);
	void PlayerInput();
	void PlayerMovement();
	void PlayerAction();

	void RenderPlayerStats();
	void RenderCurrentGameStatusDescriptions();
	void RenderInventory();
	void GoToPreviousLevel();
	void CheckToPickItem();
	void WriteItemStatsOnStatsScreen(ItemDefinition* item);
	void DrawInventory();
	void DrawCurrentEquipment();
public:
	Adventure					m_PlayerAdventure;
	std::map< std::string, Map* > m_maps;
	Map*						  m_currentMap;
	Character*					  m_Player;
	bool						  m_PlayerActionHasHappened;
	bool						  m_DoneDebugging;
	bool						  m_InstructionSheet;
	bool						  m_DialogBeforeNewLevelPlaying;
	float						  m_IntroTextBlurbTimer;
	int							  m_CurrentlySelectedEquipment;
};