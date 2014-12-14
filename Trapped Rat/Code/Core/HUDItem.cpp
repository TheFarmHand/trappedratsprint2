
#include "HUDItem.h"
#include "../Core/GameData.h"
#include "../States/GamePlayState.h"
#include "../Font/Font.h"

HUDItem::HUDItem()
{
}


HUDItem::~HUDItem()
{
}

void HUDItem::Update(float dt)
{

}
void HUDItem::Render()
{
	if (image != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->DrawTexture(image, position);
	if (string.length())
	{
		GameData::GetInstance()->GetFont()->DrawString(string, position.x, position.y, {});
	}
}

void HUDItem::SetActive(bool _is)
{
	is_active = _is;
}
bool HUDItem::GetActive()
{
	return is_active;
}

void HUDItem::SetString(std::string _string)
{
	string = _string;
}
std::string HUDItem::GetString()
{
	return string;
}
