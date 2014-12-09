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
	pGraphics->DrawTexture(image, position);

}