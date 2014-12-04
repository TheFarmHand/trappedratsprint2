#include "ItemSelection.h"
#include "CombatPlayer.h"
#include "TurnManager.h"
#include <string>
#include <sstream>
#include <vector>

#include "GameData.h"
#include "../States/GamePlayState.h"
#include "HelpText.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


ItemSelection::ItemSelection()
{
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/itemscroll.png", { 255, 255, 255 });
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/cheese.png");
	inventory = GamePlayState::GetInstance()->GetInventory();
	PopulateUniqueItems();
	max_index = uniquenames.size() - 1;
	if (uniquenames.size() == 0)
		max_index = 0;
}
ItemSelection::~ItemSelection()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(scroll);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cursor);
}
bool ItemSelection::Update(float dt)
{
	SGD::InputManager * input = SGD::InputManager::GetInstance();

	if (input->IsKeyPressed(SGD::Key::Enter))
	{
		item_cursor;
		CollectItem(item_cursor);
		item_selected = true;
	}
	if (input->IsKeyPressed(SGD::Key::Up))
	{
		item_cursor--;
		if (item_cursor < 0)
			item_cursor = max_index;
	}
	if (input->IsKeyPressed(SGD::Key::Down))
	{
		item_cursor++;
		if (item_cursor > max_index)
			item_cursor = 0;
	}
	return item_selected;
}
void ItemSelection::Render()
{
	//display the scroll over the normal selection menu
	SGD::GraphicsManager::GetInstance()->DrawTexture(scroll, { 300.0f, 175.0f }, 0.0f, {}, {}, { 1.25f, 1.25f });
	//iterate throught the map to display the 
	for (unsigned int i = 0; i < uniquenames.size(); i++)
	{
		GameData::GetInstance()->GetFont()->DrawString(uniquenames[i], 335.0f, 225.0f + (i * 40), { 0, 0, 0 }, 1.0f);
		std::ostringstream total;
		total << uniquecounts[i] << "/5";
		GameData::GetInstance()->GetFont()->DrawString(total.str(), 335.0f, 245.0f + (i * 40), { 0, 0, 0 }, 1.0f);

	}
	if (uniquenames.size() == 0)
	{
		GameData::GetInstance()->GetFont()->DrawString("Empty", 335.0f, 225.0f, { 0, 0, 0 }, 1.0f);
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(cursor, { 290.0f, 215.0f + (item_cursor * 40) }, 0.0f, {}, {}, { .75f, .75f });
	}

}
void ItemSelection::PopulateUniqueItems()
{
	for (unsigned int i = 0; i < inventory->size(); i++)
	{
		int count = 0;
		for (unsigned int j = 0; j < inventory->size(); j++)
		{
			if ((*inventory)[i] == (*inventory)[j])
				count++;
		}
		bool is = true;
		for (unsigned int j = 0; j < uniquenames.size(); j++)
		{

			if ((*inventory)[i].GetName() == uniquenames[j])
				is = false;
		}
		if (is)
		{
			uniquecounts.push_back(count);
			uniquenames.push_back((*inventory)[i].GetName());
		}



	}
}
void ItemSelection::CollectItem(int _index)
{
	if (inventory->size() == 0)
	{
		Items temp;
		temp;
		chosen = temp;
	}
	else
	{

		for (unsigned int i = 0; i < uniquenames.size(); i++)
		{
			for (unsigned int j = 0; j < inventory->size(); j++)
			{
				if (uniquenames[i] == (*inventory)[j].GetName())
				{
					chosen = (*inventory)[j];
					inventory->erase(inventory->begin() + j);
					return;
				}
			}
		}
	}
}