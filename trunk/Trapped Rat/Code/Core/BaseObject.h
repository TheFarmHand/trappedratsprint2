#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class BaseObject
{
protected:
	SGD::Point position;
	SGD::HTexture image;
	SGD::Size size;
	SGD::Vector velocity;

	enum type { None, Rat, Enemy, Ally };
	type AllyCode;
public:

	BaseObject();
	virtual ~BaseObject();

	void virtual Update(float dt);
	void virtual Render();
	void virtual HandleCollision(BaseObject * collide);

	//accessors
	SGD::Point GetPosition(){ return position; }
	SGD::Rectangle GetRect();
	SGD::Size GetSize() { return size; }
	SGD::Vector GetVelocity(){ return velocity; }
	type getAllyCode() { return AllyCode; }
	int virtual GetType();

	//mutators
	void SetPosition(SGD::Point point){ position = point; }
	void SetSize(SGD::Size _size){ size = _size; }
	void SetImage(SGD::HTexture _image){ image = _image; }
	void SetVelocity(SGD::Vector velocity);
};

