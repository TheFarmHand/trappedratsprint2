#include "RatTrap.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "GameData.h"

RatTrap::RatTrap()
{
	size.height = 32;
	size.width = 32;
}

void RatTrap::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::Point pos = position;
	//pos.x -= 16;
	float x = GameData::GetInstance( )->GetCamera( ).x;
	float y = GameData::GetInstance( )->GetCamera( ).y;
	pos.x -= x;
	pos.y -= y;
	//pos.y -= 32;
	if (image != SGD::INVALID_HANDLE)
		pGraphics->DrawTexture(image, pos);

}