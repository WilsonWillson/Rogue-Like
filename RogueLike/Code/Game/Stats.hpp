#pragma once
#include <string>


#define MAX_STATS = 4;

class Stats
{

public:
	Stats();
	~Stats();

	void DeterminePowerLevel();

	int ReturnPowerLevel();
public:
	std::string m_preferedStat;
	std::string m_secondPreferedStat;
	int m_strength;
	int m_intellegence;
	int m_agility;
	int m_stamina;
	int	m_PowerLevel;
};
