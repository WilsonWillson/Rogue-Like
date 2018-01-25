#pragma once

#include "ThirdParty/XMLParser/XMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Renderer/Texture.hpp"
#include <string>
#include <map>



class TileDefinition
{

public:
	TileDefinition();
	~TileDefinition();

	static void ReadXMLFile();
	
	static TileDefinition* GetTileDefinition(std::string tileDefName);
	
	static void AssignTextureBasedOffOfName();

public:
	std::string			m_TileName;
	bool				m_isSolid;
	bool				m_isOpaque;
	Rgba*				m_color;
	Rgba*				m_glyphColor;
	std::string			m_glyph;
	std::string			m_biome;
	float m_gCost;
	static std::map<std::string, TileDefinition*> m_registry;
};