#include "Engine/Renderer/Renderer.hpp"
#include "Game/DialogBoxes.hpp"
#include "Game/GameCommon.hpp"


DialogBoxes::DialogBoxes()
{
	m_CurrentSpotInDialog = 0;
	m_DialogDone = false;
}

DialogBoxes::~DialogBoxes()
{

}

void DialogBoxes::PushBackDialogToScene(std::string typeOfDialog, std::string words, std::string image)
{
	Dialog* dialog = new Dialog();

	dialog->m_CharacterSprite = g_renderSystem->CreateOrGetTexture(image);
	dialog->m_dialog = words;


	if (typeOfDialog == "Start")
		dialog->m_WhenToPlayDialog = START;
	if (typeOfDialog == "Quest")
		dialog->m_WhenToPlayDialog = QUEST;
	if (typeOfDialog == "End")
		dialog->m_WhenToPlayDialog = END;

	m_DialogForScene.push_back(dialog);
}

bool DialogBoxes::UpdateDialogBox()
{
	m_CurrentSpotInDialog++;

	if (m_CurrentSpotInDialog >= m_DialogForScene.size())
	{
		m_DialogDone = true;
		return false;
	}


	return true;
}

void DialogBoxes::RenderDialogBox()
{
	if (m_DialogDone == true)
		return;

	g_renderSystem->DrawTexturedAABB2(AABB2(Vector2(0, 150), Vector2(200, 400)), *m_DialogForScene[m_CurrentSpotInDialog]->m_CharacterSprite, 0);

	g_renderSystem->DrawAABB2(AABB2(Vector2(0, 0), Vector2(1600, 200)), Rgba(200, 170, 130, 255));
	g_renderSystem->DrawAABB2(AABB2(Vector2(0, 190), Vector2(1600, 200)), Rgba(130, 100, 70, 255));
	g_renderSystem->DrawAABB2(AABB2(Vector2(0, 0), Vector2(1600, 10)), Rgba(130, 100, 70, 255));
	g_renderSystem->DrawAABB2(AABB2(Vector2(0, 0), Vector2(10, 200)), Rgba(130, 100, 70, 255));
	g_renderSystem->DrawAABB2(AABB2(Vector2(1590, 0), Vector2(1600, 200)), Rgba(130, 100, 70, 255));

	g_renderSystem->DrawCenteredText2D(Vector2(800, 100), m_DialogForScene[m_CurrentSpotInDialog]->m_dialog, 50, 1400, Rgba(255, 255, 255, 255), g_textSprites);

	g_renderSystem->DrawCenteredText2D(Vector2(1400, 25), "Press Enter to Proceed", 25, 200, Rgba(255, 255, 255, 255), g_textSprites);

}

whenToPlay DialogBoxes::GetCurrentPlayForDialog()
{
	return m_DialogForScene[m_CurrentSpotInDialog]->m_WhenToPlayDialog;
}
