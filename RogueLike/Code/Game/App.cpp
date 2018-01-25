#include "Game/App.hpp"
#include "Game/Game.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Console.hpp"
#include "Engine/Audio/Audio.hpp"
#
#include <ctype.h>
#include <regex>
#include <cmath> 

const float MIN_FRAMES_PER_SECOND = 10.f; // Game time will slow down beneath this framerate
const float MAX_FRAMES_PER_SECOND = 60.f;
const float MIN_SECONDS_PER_FRAME = (1.f / MAX_FRAMES_PER_SECOND);
const float MAX_SECONDS_PER_FRAME = (1.f / MIN_FRAMES_PER_SECOND);
App* g_theApp = nullptr;
Console* console = nullptr;

//----------------------------------Constructor/Deconstructor---------------------------------
App::App()
{
	m_MainMenuShowing = true;
	g_inputSystem = new InputSystem();
	//g_AudioSystem = new AudioSystem();
	g_renderSystem = new Renderer();
	console = new Console();

	Texture* texture2 = g_renderSystem->CreateOrGetTexture("Fonts/SquirrelFixedFont.png");
	SpriteSheet* spirteSheet = new SpriteSheet(texture2, 16, 16);
	g_textSprites = new BitMapFont();
	g_textSprites->m_spriteSheet = spirteSheet;

	m_isConsoleCommandOpen = false;
}

App::~App()
{

}

void App::CreateGame(std::string DesieredAdventure)
{
	g_theGame = new Game(DesieredAdventure);
}

void App::CreateNewGameOverwriteOldGame(std::string DesieredAdventure)
{
	delete g_theGame;
	g_theGame = new Game(DesieredAdventure);
}

//--------------------------------------------Run Frame--------------------------------------------
void App::RunFrame(HWND hwnd)
{
	Update(hwnd);
	Render();
}
//----------------------------------------------Update-----------------------------------------
void App::Update(HWND hwnd)
{
	float deltaSeconds = GetDeltaSeconds();

	g_inputSystem->Update(hwnd);
	//g_AudioSystem->Update();

	if (g_theGame != nullptr && m_MainMenuShowing == false)
		g_theGame->Update((float)deltaSeconds);
}

float App::GetDeltaSeconds()
{
	double timeNow = GetCurrentTimeSeconds();
	static double lastFrameTime = timeNow;
	double deltaSeconds = timeNow - lastFrameTime;

	// Wait until [nearly] the minimum frame time has elapsed (limit framerate to within the max)
	while (deltaSeconds < MIN_SECONDS_PER_FRAME * .999f)
	{
		timeNow = GetCurrentTimeSeconds();
		deltaSeconds = timeNow - lastFrameTime;
	}
	lastFrameTime = timeNow;

	// Clamp deltaSeconds to be within the max time allowed (e.g. sitting at a debug break point)
	if (deltaSeconds > MAX_SECONDS_PER_FRAME)
	{
		deltaSeconds = MAX_SECONDS_PER_FRAME;
	}
	return (float)deltaSeconds;
}

//----------------------------------------------------Render---------------------------------------------------------
void App::Render() const
{
	g_renderSystem->LoadIdentity();
	g_renderSystem->Ortho(0.f, (float)g_WindowWidth, 0.f, (float)g_WindowHeight, 0.f, 1.f);

	g_renderSystem->ClearColor(0.0f, 0.f, 0.f, 1.f);
	g_renderSystem->ClearColorBufferBit();
	
	if(m_MainMenuShowing == true)
		RenderMainMenu();
	else if (g_theGame != nullptr)
		g_theGame->Render();
}

void App::RenderMainMenu() const
{
	g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 550.0f), "Rogue(like)", 100.0f, 600.0f, Rgba(255, 0, 0, 255), g_textSprites);

	if(g_theGame == nullptr)
		g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 350.0f), "1.) Continue Game", 50.0f, 300.0f, Rgba(68, 68, 68, 255), g_textSprites);
	else
		g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 300.0f), "1.) Continue Game", 50.0f, 300.0f, Rgba(255, 255,255, 255), g_textSprites);
	g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 250.0f), "2.) Debug a Level", 50.0f, 600.0f, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawCenteredText2D(Vector2(800.0f, 200.0f), "3.) Go on a Grand Adventure for a Pile of Gold", 50.0f, 600.0f, Rgba(255, 255, 255, 255), g_textSprites);
}

void App::RenderConsoleWindow() const
{
	g_renderSystem->DrawAABB2(AABB2(0, 100, 1600, 900), Rgba(255, 255, 255, 150));
	g_renderSystem->DrawText2D(Vector2(200.0f, 150.0f), console->m_currentConsoleCommand, 30.0f, 25.0f * console->m_currentConsoleCommand.length(), Rgba(100, 200, 255, 255), g_textSprites);

	int currentPrintCounter = 0;
	for (int consoleCommandHistoryStep = console->m_CommandHistory.size() - 1; consoleCommandHistoryStep > -1; consoleCommandHistoryStep--)
	{
		g_renderSystem->DrawText2D(Vector2(200.0f, 200.0f + currentPrintCounter * 50.0f), console->m_CommandHistory[consoleCommandHistoryStep], 30.0f, 25.0f * console->m_CommandHistory[consoleCommandHistoryStep].length(), Rgba(0, 0, 0, 255), g_textSprites);
		currentPrintCounter++;
	}
}


//------------------------------------Keyboard Commands--------------------------------------------------
void App::OnKeyDown(int asKey)
{
	g_inputSystem->OnKeyDown(asKey);

	//Run Normal Commands
	if (g_theGame == nullptr || g_theGame->m_isConsoleCommandOpen == false)
		NormalGameLogic(asKey);

	//Run Console Commands
	else
	{
		if (asKey == Key_TILDE)
		{
			g_theGame->m_isConsoleCommandOpen = false;
			return;
		}
		console->ConsoleCommand(asKey);
	}

}

void App::NormalGameLogic(int asKey)
{
	
	MainMenuKeyPressLogic(asKey);

	if (asKey == Key_TILDE && g_theGame != nullptr)
		g_theGame->m_isConsoleCommandOpen = true;

	if (asKey == KEY_I && g_theGame != nullptr)
		g_theGame->ChangeGameStateToStatsScreen();

	if (asKey == KEY_F1 && g_theGame != nullptr)
		g_theGame->ChangeGameStateToInstructions();
	if (asKey == KEY_F2 && g_theGame != nullptr)
		g_theGame->ChangeGameStateToPathDebug();
	if (asKey == KEY_F3 && g_theGame != nullptr)
		g_theGame->ChangeGameStateToBehaviorDebugging();
	if (asKey == KEY_F4 && g_theGame != nullptr)
		g_theGame->ChangeGameStateToBlockInfo();
}

void App::MainMenuKeyPressLogic(int asKey)
{
	if (asKey == KEY_ESCAPE && m_MainMenuShowing == true)
		OnExitRequested();
	if (asKey == KEY_ESCAPE && g_theGame != nullptr && g_theGame->m_GameState == "")
		m_MainMenuShowing = true;
	if (asKey == KEY_ESCAPE && g_theGame != nullptr && g_theGame->m_GameState != "")
		g_theGame->CloseGameState();

	if (asKey == KEY_2 && g_theGame == nullptr)
	{
		m_MainMenuShowing = false;
		CreateGame("Exploring");
	}
	if (asKey == KEY_2 && g_theGame != nullptr && m_MainMenuShowing == true)
	{
		m_MainMenuShowing = false;
		CreateNewGameOverwriteOldGame("Exploring");
	}
	if (asKey == KEY_3 && g_theGame == nullptr)
	{
		m_MainMenuShowing = false;
		CreateGame("FireTemple");
	}
	if (asKey == KEY_3 && g_theGame != nullptr && m_MainMenuShowing == true)
	{
		m_MainMenuShowing = false;
		
		int random = GetRandomIntInRange(0, 2);

		switch (random)
		{
		case 0:
			CreateNewGameOverwriteOldGame("FireTemple");
			break;
		case 1:
			CreateNewGameOverwriteOldGame("IceCaveExploring");
			break;
		case 2:
			CreateNewGameOverwriteOldGame("CaveExploring");
			break;
		default:
			break;
		}
	}

	if (asKey == KEY_1 && g_theGame != nullptr && m_MainMenuShowing == true)
		m_MainMenuShowing = false;
}


void App::OnKeyUp(int asKey)
{
	g_inputSystem->OnKeyUp(asKey);
}

void App::OnExitRequested()
{
	m_isQuitting = true;
}

bool App::isQuitting() const
{
	return m_isQuitting;
}

//-----------------------Console Creation/Deletion---------------------------------------------
void App::CreateConsoleWindow()
{
	g_theGame->m_isConsoleCommandOpen = true;
}

void App::DeleteConsoleWindow()
{
	g_theGame->m_isConsoleCommandOpen = false;
}

void App::RenderDebugHelper()
{

}
