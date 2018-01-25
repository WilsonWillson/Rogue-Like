#include "Game/MapFeature.hpp"


//-------------------------------------Constructors/Deconstructors-------------------------------
MapFeature::MapFeature()
{

}

MapFeature::~MapFeature()
{

}

//---------------------------------------File Read---------------------------------------------------
void MapFeature::ReadXMLFile()
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/MapFeature.xml", "MapFeature");

	//Get the Number of Tile Nodes
	XMLNode xNode = xMainNode;
	int FeatureNodeCount = xNode.nChildNode("Feature");

	//Get Tile Definition
	for (int tileStep = 0; tileStep < FeatureNodeCount; tileStep++)
	{
		MapFeature* newMapFeature = new MapFeature();
		std::string key;

		int featureAttributeCount = xMainNode.getChildNode("Feature", tileStep).nAttribute();
		for (int attributeStep = 0; attributeStep < featureAttributeCount; attributeStep++)
		{
			std::string attributeName = xMainNode.getChildNode("Feature", tileStep).getAttributeName(attributeStep);


			if ("Name" == attributeName)
			{
				newMapFeature->m_name = ParseXmlAttribute(xMainNode, "Feature", tileStep, attributeStep, "invalidName");
				key = newMapFeature->m_name;
			}
			if ("GLYPH" == attributeName)
			{
				newMapFeature->m_glyph = ParseXmlAttribute(xMainNode, "Feature", tileStep, attributeStep, "invalidGlyph");
			}
			if ("GLYPHRGBA" == attributeName)
			{
				*newMapFeature->m_glyphColor = ParseXmlAttribute(xMainNode, "Feature", tileStep, attributeStep, Rgba(255, 255, 255, 255));
			}
			if ("Solid" == attributeName)
			{
				newMapFeature->m_isSolid = ParseXmlAttribute(xMainNode, "Feature", tileStep, attributeStep);
			}
			if ("Opaque" == attributeName)
			{
				newMapFeature->m_isSolid = ParseXmlAttribute(xMainNode, "Feature", tileStep, attributeStep);
			}
		}

		m_mapFeatureRegistry[newMapFeature->m_name] = newMapFeature;
	}

}

//---------------------------------Declarations------------------------------------------------
std::map<std::string, MapFeature*> MapFeature::m_mapFeatureRegistry;