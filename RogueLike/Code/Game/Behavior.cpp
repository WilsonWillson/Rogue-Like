#include "Game/Behavior.hpp"
#include "Game/WanderBehavior.hpp"
#include "Game/FleeBehavior.hpp"
#include "Game/PursueBehavior.hpp"
#include "Game/AttackBehavior.hpp"
#include "Game/FollowerBehavior.hpp"
#include "Game/LeaderBehavior.hpp"
#include "Game/TeleportBehavior.hpp"
#include "Game/SpawnerBehavior.hpp"

//-----------------------------------Constructor/Deconstructor--------------------------
Behavior::Behavior()
{

}

Behavior::~Behavior()
{

}

//-------------------------------CreateBehavior--------------------------------------------------
Behavior* Behavior::CreateBehavior(const XMLNode behaviorXMLElement, int attributeStep, std::string name)
{
	std::string attributeName = behaviorXMLElement.getChildNode(attributeStep).getName();

	if (attributeName == "Wander")
	{
		WanderBehavior* wander = new WanderBehavior();
		wander->m_wanderRange = ParseXmlAttribute(behaviorXMLElement, "Wander", 0, 0, -1);
		return wander;
	}
	if (attributeName == "Pursue")
	{
		PursueBehavior* pursue = new PursueBehavior();
		pursue->m_pursueRange = ParseXmlAttribute(behaviorXMLElement, "Pursue", 0, 0, -1);
		return pursue;
	}
	if (attributeName == "Flee")
	{
		FleeBehavior* flee = new FleeBehavior();
		flee->m_safeDistance = ParseXmlAttribute(behaviorXMLElement, "Flee", 0, 0, -1);
		return flee;
	}
	if (attributeName == "Attack")
	{
		AttackBehavior* attack = new AttackBehavior();
		return attack;
	}

	if (attributeName == "Follower")
	{
		FollowerBehavior* follow = new FollowerBehavior();
		follow->m_devotion = ParseXmlAttribute(behaviorXMLElement, "Follower", 0, 0, -1);
		return follow;
	}

	if (attributeName == "Leader")
	{
		LeaderBehavior* leader = new LeaderBehavior();
		leader->m_inspireRange = ParseXmlAttribute(behaviorXMLElement, "Leader", 0, 0, -1);
		return leader;
	}

	if (attributeName == "Teleport")
	{
		TeleportBehavior* teleport = new TeleportBehavior();
		return teleport;
	}

	if (attributeName == "Spawner")
	{
		SpawnerBehavior* spawn = new SpawnerBehavior();
		spawn->m_creatureToSpawn = ParseXmlAttribute(behaviorXMLElement, "Spawner", 0, 0, "Nothing");
		spawn->m_amountToSpawn = ParseXmlAttribute(behaviorXMLElement, "Spawner", 0, 1, -1);
		return spawn;
	}
	return nullptr;
}

