#include "Game/Tile.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
//-------------------------------------Constructor/Deconstructor-------------------------------------------
Tile::Tile()

{
	m_definition = nullptr;
	m_CharacterInSpace = nullptr;
	m_MapFeatureInSpace = nullptr;
	m_InventoryInSpace = nullptr;
	m_pathID = 0;

	m_InventoryInSpace = new Inventory();
}

Tile::~Tile()
{

}

void Tile::AssignTextureBasedOffOfName()
{
	if (m_definition->m_TileName == "Grass")
	{
		PickRandomGrassTexture();
		return;
	}

	if (m_definition->m_TileName == "Bog")
	{
		PickRandomBogTexture();
		return;
	}

	if (m_definition->m_TileName == "Stone")
	{
		PickRandomStoneTexture();
		return;
	}
	if (m_definition->m_TileName == "CrystalFloor")
	{
		PickRandomCrystalFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "DarkRedFloor")
	{
		PickRandomDarkRedFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "VineFloor")
	{
		PickRandomVineFloorTexture();
		return;
	}

	if (m_definition->m_TileName == "Dirt")
	{
		PickRandomDirtTexture();
		return;
	}
	if (m_definition->m_TileName == "GrayDirt")
	{
		PickRandomGrayDirtTexture();
		return;
	}
	if (m_definition->m_TileName == "GrassDirt")
	{
		PickRandomGrassDirtTexture();
		return;
	}
	if (m_definition->m_TileName == "DarkGrassDirt")
	{
		PickRandomDarkGrassDirtTexture();
		return;
	}
	if (m_definition->m_TileName == "Sand")
	{
		PickRandomSandTexture();
		return;
	}
	if (m_definition->m_TileName == "Ice")
	{
		PickRandomIceTexture();
		return;
	}
	if (m_definition->m_TileName == "Lava")
	{
		PickRandomLavaTexture();
		return;
	}

	if (m_definition->m_TileName == "MarbleFloor")
	{
		PickRandomMarbleFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "Mesh")
	{
		PickRandomMeshTexture();
		return;
	}
	if (m_definition->m_TileName == "PebbleBrown")
	{
		PickRandomPebbleBrownTexture();
		return;
	}
	if (m_definition->m_TileName == "CobbleFloor")
	{
		PickRandomCobbleFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "RedFloor")
	{
		PickRandomRedFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "SandstoneFloor")
	{
		PickRandomSandStoneFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "TombFloor")
	{
		PickRandomTombFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "VolcanicFloor")
	{
		PickRandomVolcanicFloorTexture();
		return;
	}
	if (m_definition->m_TileName == "Swamp")
	{
		PickRandomSwampTexture();
		return;
	}
	if (m_definition->m_TileName == "ShallowWater")
	{
		PickRandomShallowWaterTexture();
		return;
	}
	if (m_definition->m_TileName == "DeepWater")
	{
		PickRandomDeepWaterTexture();
		return;
	}
	if (m_definition->m_TileName == "DeepestWater")
	{
		PickRandomDeepestWaterTexture();
		return;
	}
	if (m_definition->m_TileName == "Markings")
	{
		PickRandomMarkingsTexture();
		return;
	}

	if (m_definition->m_TileName == "LavaRock")
	{
		PickRandomLavaRockTexture();
		return;
	}
	if (m_definition->m_TileName == "FleshWall")
	{
		PickRandomFleshWallTexture();
		return;
	}
	if (m_definition->m_TileName == "YellowRockWall")
	{
		PickRandomYellowRockWallTexture();
		return;
	}
	if (m_definition->m_TileName == "VineWall")
	{
		PickRandomVineWallTexture();
		return;
	}
	if (m_definition->m_TileName == "BoneWall")
	{
		PickRandomBoneWallTexture();
		return;
	}
	if (m_definition->m_TileName == "Tree")
	{
		PickRandomTreeTexture();
		return;
	}
	if (m_definition->m_TileName == "YellowTombWall")
	{
		PickRandomYellowTombWallTexture();
		return;
	}
	if (m_definition->m_TileName == "StoneGrayWall")
	{
		PickRandomStoneGrayWallTexture();
		return;
	}
	if (m_definition->m_TileName == "StoneDarkWall")
	{
		PickRandomStoneDarkWallTexture();
		return;
	}
	if (m_definition->m_TileName == "StoneBrownWall")
	{
		PickRandomStoneBrownWallTexture();
		return;
	}
	if (m_definition->m_TileName == "SlimeWall")
	{
		PickRandomSlimeWallTexture();
		return;
	}
	if (m_definition->m_TileName == "SandStoneWall")
	{
		PickRandomStandStoneWallTexture();
		return;
	}
	if (m_definition->m_TileName == "MarbleWall")
	{
		PickRandomMarbleWallTexture();
		return;
	}
	if (m_definition->m_TileName == "CrystalWall")
	{
		PickRandomCrystalWallTexture();
		return;
	}
	if (m_definition->m_TileName == "BrickBrownWall")
	{
		PickRandomBrickBrownWallTexture();
		return;
	}
	if (m_definition->m_TileName == "BrickVineWall")
	{
		PickRandomBrickVineWallTexture();
		return;
	}
	if (m_definition->m_TileName == "BrickDarkWall")
	{
		PickRandomBrickDarkWallTexture();
		return;
	}
	if (m_definition->m_TileName == "BrickGrayWall")
	{
		PickRandomBrickGrayWallTexture();
		return;
	}
	if (m_definition->m_TileName == "Exit")
	{
		PickRandomExitTexture();
		return;
	}
	if (m_definition->m_TileName == "Stairs Up")
	{
		PickRandomStairsTexture();
		return;
	}
}


void Tile::PickRandomGrassTexture()
{
	int random = GetRandomIntInRange(0, 14);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_blue1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_blue2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_blue3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_red1.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_red2.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_red3.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_yellow1.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_yellow2.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_flowers_yellow3.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass0.png");
		break;
	case 10:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass1.png");
		break;
	case 11:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass2.png");
		break;
	case 12:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass0-dirt-mix1.png");
		break;
	case 13:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass0-dirt-mix2.png");
		break;
	case 14:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass0-dirt-mix3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBogTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floorbog_green0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floorbog_green1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floorbog_green2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floorbog_green3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStoneTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rect_gray0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rect_gray1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rect_gray2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rect_gray3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomCrystalFloorTexture()
{
	int random = GetRandomIntInRange(0, 5);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_floor5.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomDarkRedFloorTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_nerves6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomSandTexture()
{
	int random = GetRandomIntInRange(0, 9);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall8.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_wall9.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomVineFloorTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/floor_vines6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomDirtTexture()
{
	int random = GetRandomIntInRange(0, 2);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt2.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomGrassDirtTexture()
{
	int random = GetRandomIntInRange(0, 7);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_e.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_full.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_n.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_nw.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_s.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_se.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_sw.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grass_w.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomDarkGrassDirtTexture()
{
	int random = GetRandomIntInRange(0, 8);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_full.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_e.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_n.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_ne.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_nw.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_s.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_se.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_sw.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/dirt_w.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomGrayDirtTexture()
{
	int random = GetRandomIntInRange(0, 7);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/grey_dirt7.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomLavaTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/lava0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/lava1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/lava2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/lava3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomIceTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/ice0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/ice1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/ice2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/ice3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomMarbleFloorTexture()
{
	int random = GetRandomIntInRange(0, 5);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor4.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor5.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_floor6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomMeshTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/mesh0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/mesh1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/mesh2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/mesh3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomPebbleBrownTexture()
{
	int random = GetRandomIntInRange(0, 8);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/pebble_brown8.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomCobbleFloorTexture()
{
	int random = GetRandomIntInRange(0, 11);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood4.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood5.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood6.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood7.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood8.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood9.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood10.png");
		break;
	case 10:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood11.png");
		break;
	case 11:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/cobble_blood12.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomRedFloorTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rough_red0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rough_red1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rough_red2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/rough_red3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomSandStoneFloorTexture()
{
	int random = GetRandomIntInRange(0, 9);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor8.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor9.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomTombFloorTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomVolcanicFloorTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_floor6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomSwampTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/swamp0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/swamp1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/swamp2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/swamp3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomShallowWaterTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Shallow_Water6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomDeepWaterTexture()
{
	int random = GetRandomIntInRange(0, 8);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/Deep_Water8.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomDeepestWaterTexture()
{
	int random = GetRandomIntInRange(0, 1);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/DeepestWater0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/DeepestWater1.png");
		break;
	default:
		break;
	}
}


void Tile::PickRandomMarkingsTexture()
{
	int random = GetRandomIntInRange(0, 8);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_black_marked8.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomLavaRockTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/volcanic_wall6.png");
		break;
	default:
		break;
	}
}


//-------------------------------wALLS-----------------------------------------------------------------
void Tile::PickRandomFleshWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_flesh6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomYellowRockWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_yellow_rock0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_yellow_rock1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_yellow_rock2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_yellow_rock3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomVineWallTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/wall_vines6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBoneWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/undead0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/undead1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/undead2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/undead3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomTreeTexture()
{
	int random = GetRandomIntInRange(0, 5);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree1_lightred.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree2_lightred.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree1_red.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree2_red.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree1_yellow.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tree2_yellow.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomYellowTombWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/tomb3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStoneGrayWallTexture()
{
	int random = GetRandomIntInRange(0, 7);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_gray0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_gray1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_gray2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_gray3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_gray0.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_gray1.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_gray2.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_gray3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStoneDarkWallTexture()
{
	int random = GetRandomIntInRange(0, 7);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_dark0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_dark1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_dark2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone_dark3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_dark0.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_dark1.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_dark2.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_dark3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStoneBrownWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_brown0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_brown1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_brown2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/stone2_brown3.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomSlimeWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/slime1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/slime2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/slime3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/slime0.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStandStoneWallTexture()
{
	int random = GetRandomIntInRange(0, 9);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor7.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor8.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/sandstone_floor9.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomMarbleWallTexture()
{
	int random = GetRandomIntInRange(0, 11);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall4.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall5.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall6.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall7.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall8.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall9.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall10.png");
		break;
	case 10:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall11.png");
		break;
	case 11:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/marble_wall12.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomCrystalWallTexture()
{
	int random = GetRandomIntInRange(0, 13);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall00.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall01.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall02.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall03.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall04.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall05.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall06.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall07.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall08.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall09.png");
		break;
	case 10:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall10.png");
		break;
	case 11:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall11.png");
		break;
	case 12:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall12.png");
		break;
	case 13:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/crystal_wall13.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBrickBrownWallTexture()
{
	int random = GetRandomIntInRange(0, 7);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown6.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown7.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBrickVineWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown-vines1.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown-vines2.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown-vines3.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_brown-vines4.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBrickDarkWallTexture()
{
	int random = GetRandomIntInRange(0, 6);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark3.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark4.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark5.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_dark6.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomBrickGrayWallTexture()
{
	int random = GetRandomIntInRange(0, 3);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_gray0.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_gray1.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_gray2.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/floor/brick_gray3.png");
		break;
	default:
		break;
	}
}


//---------------------------------------SPECIAL TILES----------------------------------------------------------------
void Tile::PickRandomExitTexture()
{
	int random = GetRandomIntInRange(0, 17);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_closed_door.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_abyss.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_cocytus.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_dis.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_gehenna.png");
		break;
	case 5:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_hell.png");
		break;
	case 6:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_labyrinth.png");
		break;
	case 7:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_pandemonium.png");
		break;
	case 8:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_tartarus.png");
		break;
	case 9:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_zot_closed.png");
		break;
	case 10:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter_zot_open.png");
		break;
	case 11:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_entrance.png");
		break;
	case 12:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_exit_abyss.png");
		break;
	case 13:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_exit_abyss_flickering.png");
		break;
	case 14:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_return_hell.png");
		break;
	case 15:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_return_zot.png");
		break;
	case 16:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_stone_arch.png");
		break;
	case 17:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/sewer_portal_rusted.png");
		break;
	default:
		break;
	}
}

void Tile::PickRandomStairsTexture()
{
	int random = GetRandomIntInRange(0, 4);

	switch (random)
	{
	case 0:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_enter.png");
		break;
	case 1:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/dngn_return.png");
		break;
	case 2:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/escape_hatch_down.png");
		break;
	case 3:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/stone_stairs_down.png");
		break;
	case 4:
		m_Texture = g_renderSystem->CreateOrGetTexture("Data/Images/gateways/stone_stairs_up.png");
		break;
	default:
		break;
	}
}