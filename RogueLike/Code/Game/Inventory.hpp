#pragma once
#include "Game/Item.hpp"
#include <vector>

class Character;

class Inventory
{

public:
	Inventory();
	~Inventory();

	float CalcWeight();

	void SelectItem(Item* m_selectedItem, Character* player);
	
private:
void UpdateStatsBasedOnItemSwap(Character* player, Item* m_Inventory, Item* m_selectedItem);


public:
	std::vector<Item*> m_Inventory;
	float m_weight;

};