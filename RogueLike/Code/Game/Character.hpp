#pragma once
#include "Game/Entity.hpp"
#include "Game/Inventory.hpp"
#include "Game/Stats.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include <string>
#include <vector>
class Tile;


class Character : public Entity
{

public:
	Character();
	~Character();

	static void ReadXMLFile();

	static void CreateBehaviorsFromXML(int tileStep, Character* newCharacterDefinition, XMLNode xMainNode);
	//static Character* BuildNewCharacter(std::string characterTypeName);


	void RenderPlayer(float baseLocationX, float baseLocationY);

	std::string GetCurrentlyEquippedWeaponType();

	void Update(float deltaSeconds);
public:
	static std::map<std::string, Character*> s_characterRegistry;
	std::string m_name;
	std::string m_lootTable;
	std::string m_faction;
	std::string m_Location;
	Stats		m_baseStats; 
	std::string m_weakness;
	std::vector<Behavior*> m_behaviors;
	Texture* m_Texture;
	SpriteSheet* m_CharacterTexture;
	std::vector< SpriteAnimation* > g_explosionAnimations;

	int rayCastAmount = 80;
	int distance = 250;

	int currentMaxSprite = 0;
};