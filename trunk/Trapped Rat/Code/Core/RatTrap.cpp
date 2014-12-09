#include "RatTrap.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

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
	pos.y -= 32;
	pGraphics->DrawTexture(image, pos);

}