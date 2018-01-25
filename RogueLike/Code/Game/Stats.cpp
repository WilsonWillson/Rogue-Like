#include "Game/Stats.hpp"

//----------------------------------Constructor/Deconstructor----------------------------------------
Stats::Stats()
{

}

Stats::~Stats()
{

}

void Stats::DeterminePowerLevel()
{
	m_PowerLevel = m_agility + m_intellegence + m_stamina + m_strength;
}

int Stats::ReturnPowerLevel()
{
	return m_agility + m_intellegence + m_stamina + m_strength;
}
