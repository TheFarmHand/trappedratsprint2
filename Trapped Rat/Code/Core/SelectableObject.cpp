
#include "SelectableObject.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"

SelectableObject::SelectableObject()
{
	selectedImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/highlight.png");
	is_selected = false;
}


SelectableObject::~SelectableObject()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(selectedImage);
}
void SelectableObject::Update(float dt)
{

}
void SelectableObject::Render()
{
	if (image != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->DrawTexture(image, { position.x - 13, position.y - 5 }, 0, {}, {}, {.6f,.6f});

	if (is_selected)
	{
		if (selectedImage != SGD::INVALID_HANDLE)
			SGD::GraphicsManager::GetInstance()->DrawTexture(selectedImage, { position.x, position.y-3 }, 0, {}, {}, {.6f,.6f});
	}
	if (GameData::GetInstance()->GetFont()->IsSpanish())
	{
		std::string str = string;
		str = GameData::GetInstance()->GetFont()->StringIfSpanish(str);
		if (str.length())
		{
			int xoffset = 40 - (str.length() * 4);
			if (xoffset < 0)
				xoffset = 0;
			GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset + 2, position.y + 7, { 0, 0, 0 }, 1.8f);
			GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset, position.y + 5, { 180, 0, 180 }, 1.8f);
		}

	}
	else
	{
		if (string.length())
		{
			float scale = 0.0f;
			int xoffset = 40 - (string.length() * 4);
			if (xoffset < 0)
			{
				xoffset = 0;
			}
			if (string.length() > 9)
			{
				scale = 2.5f - (string.length()*.09f);
			}
			if (scale == 0.0f)
			{
				GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset + 2, position.y + 7, { 0, 0, 0 }, 1.8f);
				GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset, position.y + 5, { 128, 128, 255 }, 1.8f);
			}
			else
			{
				GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset + 1, position.y + 6, { 0, 0, 0 }, scale);
				GameData::GetInstance()->GetFont()->DrawString(string, position.x + xoffset, position.y + 5, { 128, 128, 255 }, scale);
			}
		}
	}
}
std::string SelectableObject::GetExplination()
{
	return explination;
}
void SelectableObject::SetExplination(std::string _str)
{
	explination = _str;
}
bool SelectableObject::GetSelected()
{ return is_selected; 
}
void SelectableObject::SetSelected(bool _is)
{ is_selected = _is; 
}

Ability* SelectableObject::GetAbility()
{ return m_ability; }
void SelectableObject::SetAbility(Ability* abl)
{ m_ability = abl; }

Items* SelectableObject::GetItem()
{ return m_item; }
void  SelectableObject::SetItem(Items*it)
{ m_item = it; }

int SelectableObject::GetObjectType()
{ return m_objecttype; }
void SelectableObject::SetObjectType(int type)
{ m_objecttype = type; }