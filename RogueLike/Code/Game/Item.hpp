#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Game/ItemDefinition.hpp"
#include "Game/Stats.hpp"
#include <string>

class ItemDefinition;

class Item
{

public:
	Item();
	~Item();

public:
	ItemDefinition* m_definition;
	bool			m_currentlyEquipped;
	bool			m_currentlySelected;
};