#include "AnimTestBase.h"


AnimTestBase::AnimTestBase()
{
	size.width = 50;
	size.height = 50;
}


AnimTestBase::~AnimTestBase()
{
}
void AnimTestBase::Render()
{
	SGD::GraphicsManager::GetInstance()->DrawRectangle(GetRect(), SGD::Color(255, 0, 0));
}
void AnimTestBase::Update(float dt)
{

}