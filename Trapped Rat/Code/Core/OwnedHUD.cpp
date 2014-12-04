
#include "OwnedHUD.h"


OwnedHUD::OwnedHUD()
{
}


OwnedHUD::~OwnedHUD()
{
	SetOwner(nullptr);

}

void  OwnedHUD::Update(float dt)
{

}
void  OwnedHUD::Render()
{
	if (owner != nullptr)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(image, { owner->GetPosition().x + m_offset.x, owner->GetPosition().y + m_offset.y }, 0, {}, {}, { 1.5f, 1.0f });
	}
}
Character* OwnedHUD::GetOwner()
{
	return owner;
}
void OwnedHUD::SetOwner(Character * _owner)
{
	//if (owner != nullptr)
	//{
	//	delete owner;
	//	owner = nullptr;
	//}
	owner = _owner;
}
void OwnedHUD::SetOffset(SGD::Point _offset)
{
	m_offset = _offset;
}