
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