#pragma once
#include "Game/Entity.hpp"
#include "ThirdParty/XMLParser/XMLParser.hpp"

class Behavior
{
public:
	Behavior();
	~Behavior();

	virtual float CalcUtility(Entity* character, Entity* myself, Map* currentMap) { character; myself; currentMap; return 0.0f; }
	virtual void Act(Entity& character, Entity* myself, Map* currentMap) = 0;
	virtual void DebugRender(Entity& myself, Map* currentMap) const { myself; currentMap; };
	virtual Behavior* Clone() const = 0;

	static	Behavior* CreateBehavior(const XMLNode behaviorXMLElement, int attributeStep, std::string name);
public:
};