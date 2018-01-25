#pragma once
#include "Engine/Math/IntVector2.hpp"
#include "Game/TileDefinition.hpp"
#include "Game/Character.hpp"
#include "Game/MapFeature.hpp"
#include "Game/Inventory.hpp"


/*
Render Order
•	Actor, if present; if not:
•	Feature, if present; if not:
•	Item on ground (in Tile’s inventory), if present; if not:
•	The Tile’s own glyph (if any)
*/



class Tile
{

public:
	Tile();
	~Tile();

	
	void ChangeType(TileDefinition* tileDef) { m_definition = tileDef; }
	void ChangeType(std::string tileDefName){ m_definition = TileDefinition::GetTileDefinition(tileDefName);}

	void AssignTextureBasedOffOfName();

private:
	void PickRandomGrassTexture();
	void PickRandomBogTexture();
	void PickRandomStoneTexture();
	void PickRandomCrystalFloorTexture();
	void PickRandomDarkRedFloorTexture();
	void PickRandomSandTexture();
	void PickRandomVineFloorTexture();
	void PickRandomDirtTexture();
	void PickRandomGrassDirtTexture();
	void PickRandomGrayDirtTexture();
	void PickRandomLavaTexture();
	void PickRandomIceTexture();
	void PickRandomMarbleFloorTexture();
	void PickRandomMeshTexture();
	void PickRandomPebbleBrownTexture();
	void PickRandomCobbleFloorTexture();
	void PickRandomRedFloorTexture();
	void PickRandomSandStoneFloorTexture();
	void PickRandomTombFloorTexture();
	void PickRandomVolcanicFloorTexture();
	void PickRandomSwampTexture();
	void PickRandomShallowWaterTexture();
	void PickRandomDeepWaterTexture();
	void PickRandomMarkingsTexture();
	void PickRandomLavaRockTexture();
	void PickRandomFleshWallTexture();
	void PickRandomYellowRockWallTexture();
	void PickRandomVineWallTexture();
	void PickRandomBoneWallTexture();
	void PickRandomTreeTexture();
	void PickRandomYellowTombWallTexture();
	void PickRandomStoneGrayWallTexture();
	void PickRandomStoneDarkWallTexture();
	void PickRandomStoneBrownWallTexture();
	void PickRandomSlimeWallTexture();
	void PickRandomStandStoneWallTexture();
	void PickRandomMarbleWallTexture();
	void PickRandomCrystalWallTexture();
	void PickRandomBrickBrownWallTexture();
	void PickRandomBrickVineWallTexture();
	void PickRandomBrickDarkWallTexture();
	void PickRandomBrickGrayWallTexture();


	void PickRandomDarkGrassDirtTexture();
	void PickRandomDeepestWaterTexture();
	void PickRandomExitTexture();
	void PickRandomStairsTexture();
public:
	TileDefinition* m_definition;
	IntVector2 m_tileCords;
	Vector2 m_renderPosition;
	Entity* m_CharacterInSpace;
	MapFeature* m_MapFeatureInSpace;
	Inventory* m_InventoryInSpace;
	Texture*			m_Texture;
	//std::vector<std::string> m_tags;
	int m_pathID;
	float m_permenance = 0.0f;
	bool m_Visible = false;
	bool m_WasSeen = false;

};