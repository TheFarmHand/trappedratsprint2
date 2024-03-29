
#include "Bars.h"
#include "GameData.h"
#include "../Font/Font.h"


Bars::Bars()
{
}


Bars::~Bars()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(outlineimage);
	if (image != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
}

void  Bars::Update(float dt)
{

}
void  Bars::Render()
{
	if (!is_active)
		is_active = true;
	int width = 0;
	if (owner == nullptr)
	{
		width = (int)((float)size.width * percentage);
	}
	else
	{
		if (is_HP)
		{
			float temp = size.width * ((float)owner->GetHP() / owner->GetMaxHP());
			if (temp < 1.0f && temp > 0.0f)
				width = 1;
			else
				width = (int)((float)size.width * ((float)owner->GetHP() / owner->GetMaxHP()));
			if ((float)owner->GetHP() / owner->GetMaxHP() > .66f)
			{
				color = SGD::Color(0, 255, 0);
			}
			else if ((float)owner->GetHP() / owner->GetMaxHP() > .25f)
			{
				color = SGD::Color(255, 255, 0);
			}
			else
			{
				color = SGD::Color(255, 0, 0);
			}
		}
		else
		{
			width = (int)((float)size.width * ((float)owner->GetBP() / owner->GetMaxBP()));
		}
	}
	if (width < 5 && width > 0)
		width = 5;
	if (owner && (m_offset.x != 0 || m_offset.y != 0) && width > 0)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(image, SGD::Point(owner->GetPosition().x + m_offset.x, owner->GetPosition().y + m_offset.y), SGD::Rectangle(0, 0, (float)width, size.height), 0, {},color);
		SGD::GraphicsManager::GetInstance()->DrawTexture(outlineimage, SGD::Point(owner->GetPosition().x + m_offset.x,owner->GetPosition().y + m_offset.y));
	}
	else if (owner && (m_offset.x != 0 || m_offset.y != 0) && width <= 0)
	{
		//SGD::GraphicsManager::GetInstance()->DrawTexture(outlineimage, SGD::Point(owner->GetPosition().x + m_offset.x, owner->GetPosition().y + m_offset.y));
	}
	else if (width > 0)
	{
		if(owner)
		{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(image, position, SGD::Rectangle(0, 0, (float)width, size.height), 0, {}, color, {2.0f,1.0f});
		SGD::GraphicsManager::GetInstance()->DrawTexture(outlineimage, position, 0, {}, {}, {2.0f,1.0f});
		}
		else
		{
			SGD::GraphicsManager::GetInstance( )->DrawTextureSection( image, position, SGD::Rectangle( 0, 0, (float)width, size.height ), 0, { }, color, { 4.0f, 2.0f } );
			SGD::GraphicsManager::GetInstance( )->DrawTexture( outlineimage, position, 0, { }, { }, { 4.0f, 2.0f } );
			if(width >= (int)size.width)
			{
				// I think this means you have a full bar
				GameData::GetInstance()->GetFont()->DrawString("Ternary Blast Ready!  Press O", position.x + 10, position.y + 10, { 255,0,150}, 1.15f);	
				// Render Cheese!  Ok no not here.  Somewhere else is smarter.
			}
			else
			{
				GameData::GetInstance( )->GetFont( )->DrawString( "Charging Ternary Blast!", position.x + 25, position.y + 10, { 255, 0, 150 }, 1.1f );
			}

		}
	}
	else if (width <= 0)
	{
		if(owner)
			SGD::GraphicsManager::GetInstance()->DrawTexture(outlineimage, position, 0, {}, {}, { 2.0f, 1.0f });
		else
		{
			SGD::GraphicsManager::GetInstance( )->DrawTexture( outlineimage, position, 0, { }, { }, { 4.0f, 2.0f } );
			GameData::GetInstance( )->GetFont( )->DrawString( "Charging Ternary Blast!", position.x + 30, position.y + 10, { 255, 0, 150 }, 1.1f );
		}
	}
	else
	{
		is_active = false;
	}
}
void Bars::SetSize(float x, float y)
{
	size.width = x;
	size.height = y;
}
void Bars::SetPercentage(float p)
{
	percentage = p;
}
float Bars::GetPercentage()
{
	return percentage;
}
Bars::Bars(SGD::Size sz, SGD::Point pos, Character * _owner)
{
	size.width = sz.width;
	size.height = sz.height;
	position.x = pos.x;
	position.y = pos.y;
	owner = _owner;
	
	image = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/barfill.png");
	outlineimage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/baroutline.png");
}

void Bars::SetColor(SGD::Color _color)
{
	color = _color;
}
void Bars::SetisHP(bool _is_HP)
{
	is_HP = _is_HP;
}
bool Bars::GetIsHP()
{
	return is_HP;
}