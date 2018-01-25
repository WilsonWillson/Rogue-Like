#include "Game/Inventory.hpp"
#include "Game/Character.hpp"
//--------------------------Constructor/Deconstructor-------------------------
Inventory::Inventory()
{

}

Inventory::~Inventory()
{

}

//------------------------Helper Functions------------------------------------
float Inventory::CalcWeight()
{
	m_weight = 0;
	for (unsigned int invetoryStep = 0; invetoryStep < m_Inventory.size(); invetoryStep++)
	{
		m_weight += m_Inventory[invetoryStep]->m_definition->m_weight;
	}
	return m_weight;
}

void Inventory::SelectItem(Item* m_selectedItem, Character* player)
{
	m_selectedItem->m_currentlySelected = true;

	for (int i = 0; i < m_Inventory.size(); i++)
	{
		//We don't care about the same item
		if (m_Inventory[i] == m_selectedItem)
			continue;

		//Check if it's the same type of item if it save it off, if not its now not selected.
		if (m_Inventory[i]->m_currentlySelected == true)
		{
			if (m_Inventory[i]->m_definition->m_equipSlot == m_selectedItem->m_definition->m_equipSlot)
			{
				m_selectedItem->m_currentlyEquipped = !m_selectedItem->m_currentlyEquipped;
				m_selectedItem->m_currentlySelected = false;

				m_Inventory[i]->m_currentlyEquipped = !m_Inventory[i]->m_currentlyEquipped;
				m_Inventory[i]->m_currentlySelected = false;

				UpdateStatsBasedOnItemSwap(player, m_Inventory[i], m_selectedItem);

				//Inventory[i]->m_currentlyEquipped = !m_Inventory[i]->m_currentlyEquipped;
			//_Inventory[i]->m_currentlySelected = false;
			}
			else if (m_Inventory[i]->m_definition->m_equipSlot != m_selectedItem->m_definition->m_equipSlot)
				m_Inventory[i]->m_currentlySelected = false;
		}
	}
}

void Inventory::UpdateStatsBasedOnItemSwap(Character* player, Item* UnequippedItem, Item* EquippedItem)
{
	player->m_currentStats.m_strength -= UnequippedItem->m_definition->m_statModifiers.m_strength;
	player->m_currentStats.m_agility -= UnequippedItem->m_definition->m_statModifiers.m_agility;
	player->m_currentStats.m_intellegence -= UnequippedItem->m_definition->m_statModifiers.m_intellegence;
	player->m_currentStats.m_stamina -= UnequippedItem->m_definition->m_statModifiers.m_stamina;

	player->m_currentStats.m_strength += EquippedItem->m_definition->m_statModifiers.m_strength;
	player->m_currentStats.m_agility += EquippedItem->m_definition->m_statModifiers.m_agility;
	player->m_currentStats.m_intellegence += EquippedItem->m_definition->m_statModifiers.m_intellegence;
	player->m_currentStats.m_stamina += EquippedItem->m_definition->m_statModifiers.m_stamina;
}

