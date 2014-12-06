#include "HelpText.h"
#include <string>
#include "GameData.h"
#include "../Font/Font.h"
#include "SelectableObject.h"
#include "Items.h"
HelpText::HelpText()
{

}
HelpText::~HelpText()
{

}
void HelpText::Render()
{
	if (m_backimage != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_backimage, {200,0});
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
	
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetExplination(), 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetExplination(), 215, 15, {0,0,100},1.45f);
}
void HelpText::AbilityDisplay()
{
	if (m_selectedObj == nullptr)
		return;
	if (m_selectedObj->GetAbility() == nullptr)
		return;
	GameData::GetInstance()->GetFont()->DrawString(std::to_string(m_selectedObj->GetAbility()->GetBPCost()), 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(std::to_string(m_selectedObj->GetAbility()->GetBPCost()), 215, 15, { 0, 0, 100 }, 1.45f);

	GameData::GetInstance()->GetFont()->DrawString("BP cost", 246, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("BP cost", 245, 15, { 0, 0, 100 }, 1.45f);

	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetAbility()->GetExplination(), 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetAbility()->GetExplination(), 215, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::ItemDisplay()
{
	if (m_selectedObj == nullptr)
		return;
	if (m_selectedObj->GetItem() == nullptr)
		return;

	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetItem()->GetExplination(), 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString(m_selectedObj->GetItem()->GetExplination(), 215, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::EnemyDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select an ENEMY for your _attack or ability", 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select an ENEMY for your _attack or ability", 215, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::AllyDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select an ALLY for your _item or ability", 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select an ALLY for your _item or ability", 215, 15, { 0, 0, 100 }, 1.45f);
}
void HelpText::WaitingDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("You are now planning your next move", 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("You are now planning your next move", 215, 15, {0,0,100},1.45f);
}
void HelpText::RunDisplay()
{
	GameData::GetInstance()->GetFont()->DrawString("Select yourself to confirm _your COWARDICE", 216, 16, { 0, 0, 0 }, 1.45f);
	GameData::GetInstance()->GetFont()->DrawString("Select yourself to confirm _your COWARDICE", 215, 15, { 0, 0, 100 }, 1.45f);
}