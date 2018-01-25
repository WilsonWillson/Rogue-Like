#include "Game/ItemDefinition.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Game/Game.hpp"

//-----------------------------------------Constructors/Deconsturctors-------------------------------
ItemDefinition::ItemDefinition()
{

}

ItemDefinition::ItemDefinition(std::string desiredLootTable)
{
	if (desiredLootTable == "StartingArmor")
		GetNewLoot(m_StartingArmorLootTable);
	if (desiredLootTable == "StarterWeapon")
		GetNewLoot(m_startingWeaponLootTable);
}

ItemDefinition::~ItemDefinition()
{

}

//------------------------------------------File Reader-----------------------------------------------
void ItemDefinition::ReadXMLFile()
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Item.xml", "Items");
	XMLNode xNode = xMainNode;
	int LootTableCount = xNode.nChildNode("LootCategory");


	for (int lootTableStep = 0; lootTableStep < LootTableCount; lootTableStep++)
	{
		int amountOfItemsInLootTable = xNode.getChildNode("LootCategory", lootTableStep).nChildNode("Item");

		for (int itemStep = 0; itemStep < amountOfItemsInLootTable; itemStep++)
		{
			std::string lootTableName = xMainNode.getChildNode("LootCategory", lootTableStep).getAttributeValue();
			int lootAttributeCount = xMainNode.getChildNode("LootCategory", lootTableStep).getChildNode("Item", itemStep).nAttribute();

			ItemDefinition* newItem = new ItemDefinition();
			newItem->m_glyphColor = new Rgba();
			for (int attributeStep = 0; attributeStep < lootAttributeCount; attributeStep++)
			{
				std::string attributeName = xMainNode.getChildNode("LootCategory", lootTableStep).getChildNode("Item", itemStep).getAttributeName(attributeStep);
				XMLNode x2Node = xMainNode.getChildNode("LootCategory", lootTableStep);

				if ("Name" == attributeName)
					newItem->m_name = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, "invalidName");

				if ("GLYPH" == attributeName)
					newItem->m_glyph = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, "invalidGlyph");

				if ("GLYPHRGBA" == attributeName)
					*newItem->m_glyphColor = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, Rgba(255, 255, 255, 255));

				if ("Slot" == attributeName)
				{
					std::string slotName = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, "InvalidLootTable");
					ConvertSlotStringNametoSlotEnumValue(slotName, newItem);
				}


				//Stats
				if ("Strength" == attributeName)
					newItem->m_statModifiers.m_strength = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, -1);

				if ("Agility" == attributeName)
					newItem->m_statModifiers.m_agility = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, -1);

				if ("Intellegence" == attributeName)
					newItem->m_statModifiers.m_intellegence = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, -1);

				if ("Stamina" == attributeName)
					newItem->m_statModifiers.m_stamina = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, -1);

				if ("Weight" == attributeName)
					newItem->m_weight = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, 1000);

				if("Type" == attributeName)
					newItem->m_type = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, "Normal");

				if ("Image" == attributeName)
				{
					std::string textureName = ParseXmlAttribute(x2Node, "Item", itemStep, attributeStep, "Data/Images/weapon/dagger.png");
					newItem->m_Texture = g_renderSystem->CreateOrGetTexture(textureName);
				}
			}


			if ("StartingArmor" == lootTableName)
				m_StartingArmorLootTable[newItem->m_name] = newItem;
			if ("StarterWeapon" == lootTableName)
				m_startingWeaponLootTable[newItem->m_name] = newItem;
			if ("Quest" == lootTableName)
				m_QuestLootTable[newItem->m_name] = newItem;
		}
	}
}


//------------------------------Loot Generation-----------------------------------------------------
void ItemDefinition::GetNewLoot(std::map<std::string, ItemDefinition*> lootTable)
{
	std::map<std::string, ItemDefinition*>::iterator item = lootTable.begin();
	int randomItemNumber = GetRandomIntInRange(0, (int)lootTable.size()-1);
	std::advance(item, randomItemNumber);

	m_equipSlot = item->second->m_equipSlot;
	m_glyph = item->second->m_glyph;
	m_glyphColor = item->second->m_glyphColor;
	m_name = item->second->m_name;
	m_statModifiers = item->second->m_statModifiers;
	m_weight = item->second->m_weight;
	m_Texture = item->second->m_Texture;
}

void ItemDefinition::ConvertSlotStringNametoSlotEnumValue(std::string slotName, ItemDefinition* slotItem)
{
	if (slotName == "None")
		slotItem->m_equipSlot = NONE;
	if (slotName == "Head")
		slotItem->m_equipSlot = HEAD;
	if (slotName == "Chest")
		slotItem->m_equipSlot = CHEST;
	if (slotName == "Gloves")
		slotItem->m_equipSlot = GLOVES;
	if (slotName == "Feet")
		slotItem->m_equipSlot = FEET;
	if (slotName == "Rings")
		slotItem->m_equipSlot = RINGS;
	if (slotName == "Neck")
		slotItem->m_equipSlot = NECK;
	if (slotName == "Weapon")
		slotItem->m_equipSlot = WEAPONS;
	if (slotName == "Shield")
		slotItem->m_equipSlot = SHIELD;
}

std::string ItemDefinition::ConvertSlotEnumValuetoString(ItemDefinition* slotItem)
{
	if (slotItem->m_equipSlot == NONE)
		return "Quest";
	if (slotItem->m_equipSlot == HEAD)
		return "Head";
	if (slotItem->m_equipSlot == CHEST)
		return "Chest";
	if (slotItem->m_equipSlot == GLOVES)
		return "Gloves";
	if (slotItem->m_equipSlot == FEET)
		return "Feet";
	if (slotItem->m_equipSlot == RINGS)
		return "Ring";
	if (slotItem->m_equipSlot == NECK)
		return "Neck";
	if (slotItem->m_equipSlot == WEAPONS)
		return "Weapon";
	if (slotItem->m_equipSlot == SHIELD)
		return "Shield";
}

//---------------------------------Declarations------------------------------------------------------
std::map<std::string, ItemDefinition*> ItemDefinition::m_StartingArmorLootTable;
std::map<std::string, ItemDefinition*> ItemDefinition::m_startingWeaponLootTable;
std::map<std::string, ItemDefinition*> ItemDefinition::m_QuestLootTable;