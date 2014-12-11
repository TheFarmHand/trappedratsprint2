#include "HelpText.h"
#include "GameData.h"
#include "../Font/Font.h"
#include "SelectableObject.h"
#include "Items.h"
#include "Character.h"
HelpText::HelpText()
{

}
HelpText::~HelpText()
{

}
void HelpText::Render()
{
	if (m_backimage != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_backimage, {100,0});
	std::string str;
	switch (m_state)
	{
	case 0://Home
		HomeDisplay();
		break;
	case 1://Ability Selection
		AbilityDisplay();
		break;
	case 2://Item Selection
		ItemDisplay();
		break;
	case 3://Enemy Targetting State
		EnemyDisplay();
		break;
	case 4://Ally Targetting State
		AllyDisplay();
		break;
	case 5://Waiting For Turn State
		WaitingDisplay();
		break;
	case 6:
		RunDisplay();
		break;
	case 7:
		ManualDisplay();
		break;
	}
}
//0Home State  1Ability State  2Item State  3Enemy Targetting State    4Ally Targetting State  5Waiting For Turn State  6Run
//This function will be called on almost every input inside of combat
void HelpText::UpdateSelection(int currentState, SelectableObject* obj)
{
	m_state = currentState;
	m_selectedObj = obj;
}
void HelpText::HomeDisplay()
{
	if (m_selectedObj == nullptr)
		return;
	
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetExplination(), 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetExplination(), 115, 15, {0,0,100},1.45f);
}
void HelpText::AbilityDisplay()
{
	if (m_selectedObj == nullptr)
		return;
	if (m_selectedObj->GetAbility() == nullptr)
		return;
	GameData::GetInstance()->GetFont()->DrawString(std::to_string(m_selectedObj->GetAbility()->GetBPCost()), 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(std::to_string(m_selectedObj->GetAbility()->GetBPCost()), 115, 15, { 0, 0, 100 }, 1.45f);

	GameData::GetInstance()->GetFont()->DrawString("BP cost", 146, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("BP cost", 145, 15, { 0, 0, 100 }, 1.45f);

	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetAbility()->GetExplination(), 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetAbility()->GetExplination(), 115, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::ItemDisplay()
{
	if (m_selectedObj == nullptr)
		return;
	if (m_selectedObj->GetItem() == nullptr)
		return;

	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetItem()->GetExplination(), 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetItem()->GetExplination(), 115, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::EnemyDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select an ENEMY for your _attack or ability", 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select an ENEMY for your _attack or ability", 115, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::AllyDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select an ALLY for your _item or ability", 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select an ALLY for your _item or ability", 115, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::WaitingDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("You are now planning your next move", 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("You are now planning your next move", 115, 15, {0,0,100},1.45f);
}
void HelpText::RunDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select yourself to confirm _your COWARDICE", 116, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select yourself to confirm _your COWARDICE", 115, 15, { 0, 0, 100 }, 1.45f);
}

void HelpText::ManualOverride(std::string _customstring, Character* _ownercharacter)
{
	m_state = 7;
	m_overridestring = _customstring;
	m_activeCharacter = _ownercharacter;
}
void HelpText::ManualDisplay()
{

	//Render Portrait
	
	if (m_activeCharacter != nullptr)
	{
		if (m_activeCharacter->GetPortrait() != SGD::INVALID_HANDLE)
			{
			if (m_activeCharacter->GetAlliance() == "Ally")
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_activeCharacter->GetPortrait(), { 110, 10 });
				GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 180, 16, { 0, 0, 0 }, 1.45f);
				GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 179, 15, { 0, 0, 100 }, 1.45f);
			}
			else
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_activeCharacter->GetPortrait(), {606,10});
				GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 116, 16, { 0, 0, 0 }, 1.45f);
				GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 115, 15, { 0, 0, 100 }, 1.45f);
			}
			}
		
	}
	else
	{
		GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 116, 16, { 0, 0, 0 }, 1.45f);
		GameData::GetInstance()->GetFont()->DrawString(m_overridestring, 115, 15, { 0, 0, 100 }, 1.45f);
	
	}



}