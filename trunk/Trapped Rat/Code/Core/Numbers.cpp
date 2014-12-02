
#include "Numbers.h"
#include "../Font/Font.h"
#include "GameData.h"
#include <string>

Numbers::Numbers()
{
}


Numbers::~Numbers()
{
}
void  Numbers::Update(float dt)
{
	lifespan--;
	if (lifespan <= 0)
		is_active = false;
	m_offset.y -= 10*dt;
}
void  Numbers::Render()
{
	if (is_active)
	{
		if (owner != nullptr)
		{
			GameData::GetInstance()->GetFont()->DrawString(std::to_string(value), owner->GetPosition().x + m_offset.x + 3, owner->GetPosition().y + m_offset.y+3,SGD::Color(0,0,0) , 3.0f);
			GameData::GetInstance()->GetFont()->DrawString(std::to_string(value), owner->GetPosition().x + m_offset.x, owner->GetPosition().y + m_offset.y,color, 3.0f);

		}
	}
}
Numbers::Numbers(int _value, SGD::Color _color, Character* _owner, SGD::Point _offset)
{
	value = _value;
	color = _color;
	m_offset = _offset;
	owner = _owner;
	lifespan = 90;
}
