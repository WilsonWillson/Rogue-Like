#pragma once

#include "Game/Entity.hpp"
#include "ThirdParty/XMLParserUserTools.hpp"
#include <map>
class Tile; 

class MapFeature : public Entity
{

public:
	MapFeature();
	~MapFeature();

	static void ReadXMLFile();

public:
	static std::map<std::string, MapFeature*> m_mapFeatureRegistry;
	Tile*	m_target; //What is this feature going to affect
	std::string m_name;
	bool m_isSolid;
	bool m_isOpaque;
};