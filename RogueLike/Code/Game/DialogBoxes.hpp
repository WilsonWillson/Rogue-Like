#pragma once
#include <vector>
#include <string>

class Texture;


enum whenToPlay
{
	START,
	QUEST,
	END
};

struct Dialog
{
	Texture* m_CharacterSprite;
	std::string m_dialog;
	whenToPlay m_WhenToPlayDialog;
};

class DialogBoxes
{
public:
	DialogBoxes();
	~DialogBoxes();

	void PushBackDialogToScene(std::string typeOfDialog, std::string dialog, std::string image);

	bool UpdateDialogBox();
	void RenderDialogBox();

	whenToPlay GetCurrentPlayForDialog();

public:
	std::vector<Dialog*> m_DialogForScene;
	int m_CurrentSpotInDialog;
	bool m_DialogDone;

};