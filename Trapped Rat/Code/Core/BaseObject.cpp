
#include "BaseObject.h"


BaseObject::BaseObject()
{

}


BaseObject::~BaseObject()
{
//if (image != SGD::INVALID_HANDLE)
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
}
void BaseObject::Update(float dt)
{

}
void BaseObject::Render()
{

}
SGD::Rectangle BaseObject::GetRect()
{
	SGD::Rectangle rect;
	rect.left = position.x - size.width / 2;
	rect.top = position.y - size.height / 2;
	rect.right = position.x + size.width / 2;
	rect.bottom = position.y + size.height / 2;
	return rect;
}

void BaseObject::HandleCollision(BaseObject * collide)
{

}

int BaseObject::GetType()
{
	return type::None;
}
