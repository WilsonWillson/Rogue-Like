#include "Game/Magic.hpp"
#include "Game/Character.hpp"


Magic::Magic()
{
	known_spells.push_back(HEALS);
	m_ActiveSpell = 0;
}

Magic::~Magic()
{

}

void Magic::Initalize()
{
	known_spells.push_back(HEALS);
	m_ActiveSpell = 0;
}


void Magic::ChangeActiveSpell(bool GoUp)
{
	if(GoUp == true)
		++m_ActiveSpell;
	if (GoUp == false)
		--m_ActiveSpell;

	if (known_spells.size() - 1> m_ActiveSpell)
		m_ActiveSpell = 0;
	if (0 < m_ActiveSpell)
		m_ActiveSpell = known_spells.size() - 1;
}


void Magic::CastSpell(Character* charcter)
{
	if (m_ActiveSpell == HEALS)
		CastHealSelf(charcter);
}

void Magic::CastHealSelf(Character* charcter)
{
	charcter->m_currentStats.m_stamina += charcter->m_currentStats.m_intellegence;
	charcter->m_currentStats.m_intellegence -= charcter->m_currentStats.m_intellegence / 3;
}

