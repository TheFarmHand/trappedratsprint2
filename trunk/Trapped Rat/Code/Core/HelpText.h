#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class SelectableObject;
class HelpText
{
private:
	int m_state = 5; //0Home State  1Ability State  2Item State  3Enemy Targetting State    4Ally Targetting State  5Waiting For Turn State 6 Run
	SGD::HTexture m_backimage = SGD::INVALID_HANDLE;
	SelectableObject* m_selectedObj = nullptr;

	void HomeDisplay();
	void AbilityDisplay();
	void ItemDisplay();
	void EnemyDisplay();
	void AllyDisplay();
	void WaitingDisplay();
	void RunDisplay();

public:
	HelpText();
	~HelpText();
	void Render();
	//0Home State  1Ability State  2Item State  3Enemy Targetting State    4Ally Targetting State  5Waiting For Turn State 6 Run
	void UpdateSelection(int currentState,SelectableObject* obj = nullptr);
	void SetImage(SGD::HTexture img){ m_backimage = img; }
};