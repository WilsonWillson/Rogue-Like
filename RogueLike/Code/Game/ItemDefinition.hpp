#pragma once
#include "ThirdParty/XMLParserUserTools.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/Stats.hpp"
#include "Game/Item.hpp"
#include "Engine/Renderer/Texture.hpp"
#include <string>
#include <map>
#include <iterator>

enum EquipmentSlot
{
	NONE,
	HEAD,
	CHEST,
	GLOVES,
	FEET,
	RINGS,
	NECK,
	WEAPONS,
	SHIELD,
	NUM_EQUIP_SLOTS
};

class Item;

class ItemDefinition
{

public:
	ItemDefinition();
	ItemDefinition(std::string desiredLootTable);
	~ItemDefinition();

	static void ReadXMLFile();

	void GetNewLoot(std::map<std::string, ItemDefinition*> lootTable);

public:
	static void ConvertSlotStringNametoSlotEnumValue(std::string slotName, ItemDefinition* slotItem);
	static std::string ConvertSlotEnumValuetoString(ItemDefinition* slotItem);
public:
	static std::map<std::string, ItemDefinition*> m_StartingArmorLootTable;
	static std::map<std::string, ItemDefinition*> m_startingWeaponLootTable;
	static std::map<std::string, ItemDefinition*> m_QuestLootTable;
	EquipmentSlot	m_equipSlot;
	Texture*		m_Texture;
	std::string		m_name;
	std::string		m_glyph;
	std::string		m_type;
	Rgba*			m_glyphColor;
	Stats			m_statModifiers;
	int				m_weight;
};
