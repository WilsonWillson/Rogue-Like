#pragma once
#include <vector>

enum SPELL
{
	HEALS,
	NUM_OF_SPELLS
};

class Character;

class Magic
{

public:
	Magic();
	~Magic();

	void Initalize();

	void CastSpell(Character* charcter);
	void ChangeActiveSpell(bool GoUp);

private:
	void CastHealSelf(Character* charcter);


public:
	std::vector<SPELL> known_spells;
	int    m_ActiveSpell;
	
};
