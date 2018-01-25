#include "Game/Entity.hpp"
#include "Game/Map.hpp"
#include "Game/Character.hpp"
#include "Game/Behavior.hpp"
#include "Game/Path.hpp"
#include <stdlib.h> 

//-----------------------------Constructor/Deconstructor--------------------------------------------
Entity::Entity()
{
	m_glyph = 'e';
	m_glyphColor = new Rgba();
	m_glyphColor->r = 255;
	m_glyphColor->g = 255;
	m_glyphColor->b = 255;
	m_glyphColor->a = 255;
	m_CurrentMap = nullptr;
	m_CurrentCords = IntVector2(0,0);
	m_inventory = new Inventory();
	m_currentPath = new Path();
	m_target = nullptr;
}

Entity::~Entity()
{

}

void Entity::CreateInventory()
{
	ItemDefinition* newItemDefinition = new ItemDefinition(m_CharatcerDefintion->m_lootTable);
	Item* newItem = new Item();
	newItem->m_definition = newItemDefinition;
	if (!IsThereACurrentItemEquippedInThisSlotAndIsItBetterThanPassedInItem(newItem))
	{
		newItem->m_currentlyEquipped = true;
		ChangeStatsWithEquippedItem(newItem);
	}
	m_inventory->m_Inventory.push_back(newItem);
}

void Entity::CreateBehaviors()
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Actor.xml", "Actor");
	XMLNode xNode = xMainNode;

	int amountOfEnemies = xNode.nChildNode();
	for (int enemyStep = 0; enemyStep < amountOfEnemies; enemyStep++)
	{
		int amountOfBehaviors = xNode.getChildNode("Character", enemyStep).getChildNode("Behaviors").nChildNode();

		XMLNode xNode2 = xNode.getChildNode("Character", 0).getChildNode("Behaviors");
		for (int attributeStep = 0; attributeStep < amountOfBehaviors; attributeStep++)
		{
			Behavior* testBehavior = Behavior::CreateBehavior(xNode2, attributeStep, m_CharatcerDefintion->m_name); testBehavior;
		}
	}
}

//-----------------------------------------Act/Update-----------------------------
void Entity::Act(Character* m_Player, std::vector<Entity*> m_entity, Map* currentMap)
{
	//What Behavior to do
	float highestUtility = 0.f;
	Behavior* highestUtilityBehavior = nullptr;
	m_previousBehavior = m_currentBehavior;

	for (unsigned int behaviorStep = 0; behaviorStep  < m_CharatcerDefintion->m_behaviors.size(); behaviorStep++)
	{
		float currentUtility;
		currentUtility = m_CharatcerDefintion->m_behaviors[behaviorStep]->CalcUtility(m_Player, this, currentMap);

		if (currentUtility > highestUtility)
		{
			highestUtilityBehavior = m_CharatcerDefintion->m_behaviors[behaviorStep];
			highestUtility = currentUtility;
		}
	}
	m_currentBehavior = highestUtilityBehavior;


	if (m_previousBehavior != m_currentBehavior)
		m_currentPath->m_path.clear();

	//This Does the action depending on the behavior
	if (m_currentBehavior)
		m_currentBehavior->Act(*this, m_Player, currentMap);

	CalculateVisionVector(m_entity, currentMap);
}

void Entity::CalculateVisionVector(std::vector<Entity *> m_entity, Map* currentMap)
{	
	for (unsigned int entityStep = 0; entityStep < m_entity.size(); entityStep++)
	{
		//First Check if the actor already saw us and added us (How nice of him)
		bool alreadyAdded = false;
		for (unsigned int checkStep = 0; checkStep < m_VisableEntities.size(); checkStep++)
		{
			if (m_VisableEntities[checkStep] == m_entity[entityStep])
				alreadyAdded = true;
		}
		if(alreadyAdded)
			continue;
		
		Tile* myTile = currentMap->GetTileAtTileCoords(m_CurrentCords);
		Tile* otherActorsTile = currentMap->GetTileAtTileCoords(m_entity[entityStep]->m_CurrentCords);

		if (currentMap->HasLineOfSight(myTile->m_renderPosition, otherActorsTile->m_renderPosition))
		{
			m_VisableEntities.push_back(m_entity[entityStep]);
			m_entity[entityStep]->m_VisableEntities.push_back(this);
		}
	}
}




void Entity::Attack(Character* m_Player)
{
	if(m_Player->m_faction != m_CharatcerDefintion->m_faction)
		m_Player->m_currentStats.m_stamina -= m_currentStats.m_strength;
}

void Entity::Attack(Entity* m_Player)
{
	if (m_Player->m_CharatcerDefintion->m_faction != m_CharatcerDefintion->m_faction)
		m_Player->m_currentStats.m_stamina -= m_currentStats.m_strength;
}

//-------------------------------------------Helper Functions----------------------------------------------------
bool Entity::IsThereACurrentItemEquippedInThisSlotAndIsItBetterThanPassedInItem(Item* newItem)
{
	for (unsigned int inventoryStep = 0; inventoryStep < m_inventory->m_Inventory.size(); inventoryStep++)
	{
		if (newItem->m_definition->m_equipSlot == m_inventory->m_Inventory[inventoryStep]->m_definition->m_equipSlot)
		{
			float equippedItemUtility;
			float newItemUtility;
			equippedItemUtility = CalulateItemUtility(m_inventory->m_Inventory[inventoryStep]);
			newItemUtility = CalulateItemUtility(newItem);

			if (newItemUtility > equippedItemUtility)
				return false;
			if (equippedItemUtility >= newItemUtility)
				return true;
		}
	}
	return false;
}

bool Entity::IsThereACurrentItemEquippedInThisSlot(Item* newItem)
{
	for (unsigned int inventoryStep = 0; inventoryStep < m_inventory->m_Inventory.size(); inventoryStep++)
	{
		if (newItem->m_definition->m_equipSlot == m_inventory->m_Inventory[inventoryStep]->m_definition->m_equipSlot)
		{
			return true;
		}
	}
	return false;
}

float Entity::CalulateItemUtility(Item* item)
{
	float staminaUtility = (float)item->m_definition->m_statModifiers.m_stamina;
	float strengthUtility = (float)item->m_definition->m_statModifiers.m_strength;
	float agilityUtility = (float)item->m_definition->m_statModifiers.m_agility;
	float intellegenceUtility = (float)item->m_definition->m_statModifiers.m_intellegence;
	
	if (m_currentStats.m_preferedStat == "Stamina")
		staminaUtility *= 2;
	if (m_currentStats.m_preferedStat == "Intelligence")
		intellegenceUtility *= 2;
	if (m_currentStats.m_preferedStat == "Agility")
		agilityUtility *= 2;
	if (m_currentStats.m_preferedStat == "Strength")
		strengthUtility *= 2;

	if (m_currentStats.m_secondPreferedStat == "Stamina")
		staminaUtility *= 1.5;
	if (m_currentStats.m_secondPreferedStat == "Intelligence")
		intellegenceUtility *= 1.5;
	if (m_currentStats.m_secondPreferedStat == "Agility")
		agilityUtility *= 1.5;
	if (m_currentStats.m_secondPreferedStat == "Strength")
		strengthUtility *= 1.5;

	return agilityUtility + strengthUtility + strengthUtility + staminaUtility;
}

void Entity::ChangeStatsWithEquippedItem(Item* newItem)
{
	m_currentStats.m_strength += newItem->m_definition->m_statModifiers.m_strength;
	m_currentStats.m_agility += newItem->m_definition->m_statModifiers.m_agility;
	m_currentStats.m_intellegence += newItem->m_definition->m_statModifiers.m_intellegence;
	m_currentStats.m_stamina += newItem->m_definition->m_statModifiers.m_stamina;
}

void Entity::ChangeStatsByUnequipingItem(Item* newItem)
{
	m_currentStats.m_strength -= newItem->m_definition->m_statModifiers.m_strength;
	m_currentStats.m_agility -= newItem->m_definition->m_statModifiers.m_agility;
	m_currentStats.m_intellegence -= newItem->m_definition->m_statModifiers.m_intellegence;
	m_currentStats.m_stamina -= newItem->m_definition->m_statModifiers.m_stamina;
}
