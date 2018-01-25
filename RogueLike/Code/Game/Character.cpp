#include "Game/Character.hpp"
#include "Game/Behavior.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteAnimation.hpp"
#include "Game/Magic.hpp"
#include "Game/Game.hpp"
//-------------------------Constructor/Deconstructor----------------------------------------
Character::Character()
{
	m_inventory = nullptr;
	Texture* texture = g_renderSystem->CreateOrGetTexture("Data/Images/prince.png");
	m_CharacterTexture = new SpriteSheet(texture, 4, 4);
	g_explosionAnimations.push_back(new SpriteAnimation(*m_CharacterTexture, 3, SPRITE_ANIM_MODE_LOOPING, 0, 15, Vector2(0,0)));
	g_explosionAnimations[g_explosionAnimations.size() - 1]->Start();

	Magic* m_MagicSystem = new Magic();

}

Character::~Character()
{

}

//---------------------------Read File--------------------------------------------------------
void Character::ReadXMLFile()
{
	XMLNode xMainNode = XMLNode::openFileHelper("Data/XML/Actor.xml", "Actor");
	//XMLNode xBehaviorNode = xMainNode.getChildNode("Character", tileStep);

	int AmountOfCharacterDefinitions = xMainNode.nChildNode("Character");

	//Get Tile Definition
	for (int tileStep = 0; tileStep < AmountOfCharacterDefinitions; tileStep++)
	{
		Character* newCharacterDefinition = new Character();

		int amountOfAttributesOnDefinition = xMainNode.getChildNode("Character", tileStep).nAttribute();

		for (int attributeStep = 0; attributeStep < amountOfAttributesOnDefinition; attributeStep++)
		{
			std::string attributeName = xMainNode.getChildNode("Character", tileStep).getAttributeName(attributeStep);

			if ("Name" == attributeName)
				newCharacterDefinition->m_name = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "invalidName");

			if ("LootTable" == attributeName)
				newCharacterDefinition->m_lootTable = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "InvalidLootTable");

			if ("Location" == attributeName)
				newCharacterDefinition->m_Location = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "Everywhere");

			if ("Faction" == attributeName)
				newCharacterDefinition->m_faction = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "InvalidFaction");

			//Stats
			if ("Strength" == attributeName)
				newCharacterDefinition->m_baseStats.m_strength = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, -1);

			if ("Agility" == attributeName)
				newCharacterDefinition->m_baseStats.m_agility = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, -1);

			if ("Intelligence" == attributeName)
				newCharacterDefinition->m_baseStats.m_intellegence = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, -1);

			if ("Stamina" == attributeName)
				newCharacterDefinition->m_baseStats.m_stamina = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, -1);

			if ("PreferedStat" == attributeName)
				newCharacterDefinition->m_baseStats.m_preferedStat = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "nothing");

			if ("SecondPreferedStat" == attributeName)
				newCharacterDefinition->m_baseStats.m_secondPreferedStat = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "nothing");

			if ("Weakness" == attributeName)
				newCharacterDefinition->m_weakness = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "nothing");

			if ("Image" == attributeName)
			{
 				std::string textureName = ParseXmlAttribute(xMainNode, "Character", tileStep, attributeStep, "nothing");
				newCharacterDefinition->m_Texture = g_renderSystem->CreateOrGetTexture(textureName);
			}
		}

		CreateBehaviorsFromXML(tileStep, newCharacterDefinition, xMainNode);

		newCharacterDefinition->m_baseStats.DeterminePowerLevel();
		s_characterRegistry[newCharacterDefinition->m_name] = newCharacterDefinition;
	}
}

void Character::CreateBehaviorsFromXML(int tileStep, Character*  newCharacterDefinition, XMLNode xMainNode)
{
	XMLNode xNode = xMainNode;

	int amountOfBehaviors = xNode.getChildNode("Character", tileStep).getChildNode("Behaviors").nChildNode();

	XMLNode xNode2 = xNode.getChildNode("Character", tileStep).getChildNode("Behaviors");
	for (int attributeStep = 0; attributeStep < amountOfBehaviors; attributeStep++)
		{
			Behavior* testBehavior = Behavior::CreateBehavior(xNode2, attributeStep, newCharacterDefinition->m_name);
			newCharacterDefinition->m_behaviors.push_back(testBehavior);
		}
}

//-----------------------------------------Render--------------------------------------------------------------------------
void Character::RenderPlayer(float baseLocationX, float baseLocationY)
{

	AABB2 bulletTexCoords = g_explosionAnimations[0]->GetCurrentTexCoords();
	Texture* texture = g_explosionAnimations[0]->GetTexture();


	g_renderSystem->DrawTexturedAABB2(AABB2(baseLocationX, baseLocationY, baseLocationX + 50, baseLocationY + 35), *texture, 0, bulletTexCoords, Rgba (255, 255, 255, 255));
}

//-----------------------------------Player Specific Functions-------------------------------------------------------------

std::string Character::GetCurrentlyEquippedWeaponType()
{
	for (unsigned int inventoryStep = 0; inventoryStep < m_inventory->m_Inventory.size(); inventoryStep++)
	{
		if (m_inventory->m_Inventory[inventoryStep]->m_definition->m_equipSlot == WEAPONS && m_inventory->m_Inventory[inventoryStep]->m_currentlyEquipped)
			return m_inventory->m_Inventory[inventoryStep]->m_definition->m_type;
	}
	return "";
}



void Character::Update(float deltaSeconds)
{
	if (g_explosionAnimations[0] != nullptr)
		g_explosionAnimations[0]->Update(deltaSeconds);

	if (currentMaxSprite == 15 && g_explosionAnimations[0]->m_currentSpriteIndex < 12)
		g_explosionAnimations[0]->m_currentSpriteIndex = 12;

	if (g_explosionAnimations[0]->m_currentSpriteIndex > currentMaxSprite)
	{
		g_explosionAnimations[0]->m_currentSpriteIndex = currentMaxSprite - 3;
	}

	if (g_inputSystem->WasKeyJustPressed(KEY_Q) && g_inputSystem->WasKeyJustPressed(KEY_PAGEUP))
		m_MagicSystem->ChangeActiveSpell(true);
	if (g_inputSystem->WasKeyJustPressed(KEY_E) && g_inputSystem->WasKeyJustPressed(KEY_PAGEDOWN))
		m_MagicSystem->ChangeActiveSpell(false);
	if (g_inputSystem->WasKeyJustPressed(KEY_M))
		m_MagicSystem->CastSpell(this);
}

std::map<std::string, Character*> Character::s_characterRegistry;

