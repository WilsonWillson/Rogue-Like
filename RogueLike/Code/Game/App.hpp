#pragma once
#include "Engine/Core/Console.hpp"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

class Console;

class App
{

public:
	App();
	~App();
	
	//--------------------Creation------------------------------------------------------
	void CreateGame(std::string DesieredAdventure);
	void CreateNewGameOverwriteOldGame(std::string DesieredAdventure);


	void RunFrame(HWND hwnd);

	//------------------------------------Keyboard Commands--------------------------------------------------
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnExitRequested();
	bool isQuitting() const;

	void NormalGameLogic(int asKey);
	void MainMenuKeyPressLogic(int asKey);


	//-----------------------Console Creation/Deletion---------------------------------------------
	void CreateConsoleWindow();
	void DeleteConsoleWindow();

	void RenderDebugHelper();

private:
	//----------------------------Update-----------------------------------------
	void Update(HWND hwnd);
	float GetDeltaSeconds();

	//----------------------Render-----------------------------------------------
	void Render() const;
	void RenderMainMenu() const;
	void RenderConsoleWindow() const;

public:
	//Console* console;
	bool m_isConsoleCommandOpen;

private:
	bool m_isQuitting;
	bool m_MainMenuShowing;
};

extern App* g_theApp;
extern Console* console;