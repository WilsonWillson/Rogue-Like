#pragma once
#include "ThirdParty/XMLParser/XMLParser.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Game/Stats.hpp"
#include <string>
#include <vector>

class Magic;
class Map;
class Inventory;
class Character;
class Behavior;
class Item;
class Behavior;
class Path;


class Entity
{

public:
	Entity();
	~Entity();

	void CreateInventory();
	void CreateBehaviors();

	void Act(Character* m_Player, std::vector<Entity*> m_entity, Map* currentMap);
	void CalculateVisionVector(std::vector<Entity *> m_entity, Map* currentMap);

	void Attack(Character* m_Player);
	void Attack(Entity* m_Player);

	bool IsThereACurrentItemEquippedInThisSlotAndIsItBetterThanPassedInItem(Item* newItem);
	bool IsThereACurrentItemEquippedInThisSlot(Item* newItem);
	float CalulateItemUtility(Item* item);
	void ChangeStatsWithEquippedItem(Item* newItem);
	void ChangeStatsByUnequipingItem(Item* newItem);

public:
	Map*		m_CurrentMap;
	IntVector2	m_HomePosition;
	IntVector2	m_CurrentCords;
	IntVector2	m_previousCords;
	Inventory*	m_inventory;
	Character*	m_CharatcerDefintion;
	Stats		m_currentStats;
	Behavior* m_currentBehavior = nullptr;
	Behavior* m_previousBehavior = nullptr;
	//std::vector<Behavior*> m_behaviors;
	std::string	m_glyph;
	Rgba*		m_glyphColor;
	Rgba*		m_fillColor;
	Path*		m_currentPath;
	Entity*		m_target;
	Magic*		m_MagicSystem;
	float		m_AnimatePosition;
	
	std::vector<Entity*> m_VisableEntities;
	std::vector<int> m_damageTaken;
	std::vector<float> m_LastTimeSinceDamageTaken;
};