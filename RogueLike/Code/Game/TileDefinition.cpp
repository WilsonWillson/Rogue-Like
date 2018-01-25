#include "Game/TileDefinition.hpp"
#include "ThirdParty/XMLParserUserTools.hpp"

//----------------------------Constructors/Deconstructors---------------------------------
TileDefinition::TileDefinition()
{
	m_color = new Rgba();
	m_glyphColor = new Rgba();
}

TileDefinition::~TileDefinition()
{
	delete m_color;
	delete m_glyphColor;
}


//----------------------------------Read File-----------------------------------------------
void TileDefinition::ReadXMLFile()
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Tile.xml", "TileDefinition");

	//Get the Number of Tile Nodes
	XMLNode xNode = xMainNode;
	int tileNodeCount = xNode.nChildNode("Tile");


	//Get Tile Definition
	for (int tileStep = 0; tileStep < tileNodeCount; tileStep++)
	{
		TileDefinition* newTileBeingAdded = new TileDefinition();

		std::string name = xMainNode.getChildNode("Tile", tileStep).getAttributeValue(0);
		newTileBeingAdded->m_TileName = name;

		std::string solidCheck = xMainNode.getChildNode("Tile", tileStep).getAttributeValue(1);
		if (solidCheck == "yes" || solidCheck == "Yes" || solidCheck == "Y" || solidCheck == "y")
			newTileBeingAdded->m_isSolid = true;
		else
			newTileBeingAdded->m_isSolid = false;

		std::string opaqueCheck = xMainNode.getChildNode("Tile", tileStep).getAttributeValue(2);
		if (opaqueCheck == "yes" || opaqueCheck == "Yes" || opaqueCheck == "Y" || opaqueCheck == "y")
			newTileBeingAdded->m_isOpaque = true;
		else
			newTileBeingAdded->m_isOpaque = false;
		

		const float gCost = ParseXmlAttribute(xMainNode, "Tile", tileStep, 3, 999.0f);
		newTileBeingAdded->m_gCost = gCost;

		const float biome = ParseXmlAttribute(xMainNode, "Tile", tileStep, 4, 'N');
		newTileBeingAdded->m_biome = biome;

		m_registry[name] = newTileBeingAdded;
	}
}


//-------------------------------Declarations--------------------------------------------
std::map<std::string, TileDefinition*> TileDefinition::m_registry;

