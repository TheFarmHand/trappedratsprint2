#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <string>
class SelectableObject;
class Character;
class HelpText
{
private:
	int m_state = 5; //0Home State  1Ability State  2Item State  3Enemy Targetting State    4Ally Targetting State  5Waiting For Turn State 6 Run  7 Manual Override
	SGD::HTexture m_backimage = SGD::INVALID_HANDLE;
	SelectableObject* m_selectedObj = nullptr;
	Character*		  m_activeCharacter = nullptr;
	std::string  m_overridestring;


	void HomeDisplay();
	void AbilityDisplay();
	void ItemDisplay();
	void EnemyDisplay();
	void AllyDisplay();
	void WaitingDisplay();
	void RunDisplay();
	void ManualDisplay();

public:
	HelpText();
	~HelpText();
	void Render();
	//0Home State  1Ability State  2Item State  3Enemy Targetting State    4Ally Targetting State  5Waiting For Turn State 6 Run   7 Manual Override
	void UpdateSelection(int currentState,SelectableObject* obj = nullptr);
	void SetImage(SGD::HTexture img){ m_backimage = img; }
	//String that will be displayed in the help text, A character pointer to obtain portrait information
	void ManualOverride(std::string _customstring, Character* _ownercharacter = nullptr);
};