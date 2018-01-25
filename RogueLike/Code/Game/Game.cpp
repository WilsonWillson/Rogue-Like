#include "Game/Game.hpp"

Game* g_theGame = nullptr;

//---------------------------------Constructor/Destructors------------------------------------
Game::Game(std::string desieredAdventure)
{
	m_isConsoleCommandOpen = false;
	m_world = new World(desieredAdventure);
	m_GameState = "";
}


Game::Game()
{

}

Game::~Game()
{
}

//---------------------------------UPDATE--------------------------------------------------------------
void Game::Update(float deltaSeconds)
{
	deltaSeconds;
	if (m_isConsoleCommandOpen == false && m_GameState == "")
		m_world->Update(m_GameState, deltaSeconds);
	else if (m_isConsoleCommandOpen == false && m_GameState == "DebugPath")
		m_world->UpdateDebugPathing();
	else if (m_isConsoleCommandOpen == false && m_GameState == "StatsScreen")
		m_world->UpdateStatsScreen();
}


//----------------------------Game Actions-------------------------------------------------------------------
/*
void Game::PlayDemoSound()
{
	SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/Player_Shoot.wav");
	g_AudioSystem->PlaySound(playerShoot);
}
*/

//----------------------------------------------Game States-------------------------------------------------------

void Game::CloseGameState()
{
	if (m_GameState == "StatsScreen" || m_GameState == "BlockInfo")
		m_GameState = "";
	if (m_GameState == "DebugPath")
	{
		m_world->m_DoneDebugging = true;
		for (unsigned int entityStep = 0; entityStep < m_world->m_currentMap->m_entity.size(); entityStep++)
		{
			Path* generatedPath = m_world->m_currentMap->GenerateWalkablePath(&m_world->m_currentMap->m_entity[entityStep]->m_CurrentCords, &m_world->m_Player->m_CurrentCords, m_world->m_currentMap->m_entity[entityStep]);
			m_world->m_currentMap->m_entity[entityStep]->m_currentPath = generatedPath;
		}
	}
}

void Game::ChangeGameStateToStatsScreen()
{
	if (m_GameState == "StatsScreen")
		m_GameState = "";
	else
		m_GameState = "StatsScreen";
}

void Game::ChangeGameStateToBlockInfo()
{
	if (m_GameState == "BlockInfo")
		m_GameState = "";
	else
		m_GameState = "BlockInfo";
}

void Game::ChangeGameStateToInstructions()
{
	if (m_GameState == "Instructions")
		m_GameState = "";
	else
		m_GameState = "Instructions";
}

//---------------------------------------Rendering--------------------------------------------------------
void Game::Render()
{
	m_world->Render();
	m_world->RenderControlScheme(m_GameState);
	
	if (m_GameState == "DebugPath")
		m_world->RenderPathDebugging();
	if (m_GameState == "BlockInfo")
		m_world->RenderTooltip();
	if (m_GameState == "StatsScreen")
		m_world->RenderStatsScreen();
	if (m_GameState == "DebugBehavior")
		m_world->RenderBehaviorDebugging();
}


void Game::CreateConsoleWindow()
{
	g_renderSystem->DrawAABB2(AABB2(0, 0, 1600, 900), Rgba(255, 255, 255, 100));
}

//------------------------------Debugger Tools-----------------------------------------------
void Game::ChangeGameStateToPathDebug()
{
	if (m_GameState == "DebugPath")
	{
		m_GameState = "";
		m_world->m_DoneDebugging = true;
		for (unsigned int entityStep = 0; entityStep < m_world->m_currentMap->m_entity.size(); entityStep++)
		{
			Path* generatedPath = m_world->m_currentMap->GenerateWalkablePath(&m_world->m_currentMap->m_entity[entityStep]->m_CurrentCords, &m_world->m_Player->m_CurrentCords, m_world->m_currentMap->m_entity[entityStep]);
			m_world->m_currentMap->m_entity[entityStep]->m_currentPath = generatedPath;
		}
	}
	else
	{
		m_GameState = "DebugPath";
		m_world->m_DoneDebugging = false;
		for (unsigned int entityStep = 0; entityStep < m_world->m_currentMap->m_entity.size(); entityStep++)
		{
			delete m_world->m_currentMap->m_entity[entityStep]->m_currentPath;
			m_world->m_currentMap->m_entity[entityStep]->m_currentPath = nullptr;
		}
		m_world->m_currentMap->StartStepPeth(&m_world->m_currentMap->m_entity[0]->m_CurrentCords, &m_world->m_Player->m_CurrentCords, m_world->m_currentMap->m_entity[0]);
	}
}

void Game::ChangeGameStateToBehaviorDebugging()
{
	if (m_GameState == "DebugBehavior")
	{
		m_GameState = "";

	}
	else
	{
		m_GameState = "DebugBehavior";
		
	}
}