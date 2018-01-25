#pragma once
#include "Engine/Input/XboxControl.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Core/EngineConfig.hpp"
#include "Game/GameCommon.hpp"
#include "Game/GameConfig.hpp"
#include "Game/World.hpp"

#include <iostream>
#include <vector>


class Game
{
public:
	Game();
	Game(std::string desieredAdventureName);
	~Game();

	void Update(float deltaSeconds);
	void Render();

	void CreateConsoleWindow();

	void CloseGameState();
	void ChangeGameStateToPathDebug();
	void ChangeGameStateToBehaviorDebugging();
	void ChangeGameStateToBlockInfo();
	void ChangeGameStateToInstructions();
	void ChangeGameStateToStatsScreen();

	void PlayDemoSound();

public:
	bool m_isConsoleCommandOpen;
	World* m_world;
	std::string m_GameState;
	unsigned int m_TotalBytesSent;
};


//-----------------------------------------------------------------------------------------------
extern Game* g_theGame;
