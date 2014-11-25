#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Update(float dt);
	//void Render();			

	// Accessors For All Time
	float GetLifeEnd()		{ return LifeEnd; }
	float GetLifeCurrent()	{ return LifeCurrent; }
	unsigned char GetColorA()		{ return ColorA; }
	unsigned char GetColorR( )		{ return ColorR; }
	unsigned char GetColorG( )		{ return ColorG; }
	unsigned char GetColorB( )		{ return ColorB; }
	
	float GetRotation()		{ return Rotation; }
	float GetVelocity()		{ return Velocity; }

	SGD::Vector GetPos()	{ return Position; }
	SGD::Vector GetDir()	{ return Direction; }
	float GetScale( )		{ return Scale; }
	SGD::Color GetColor();
	

	// Mutate 
	void SetLife(float end, float current = 0);
	void SetColor( unsigned char a, unsigned char r, unsigned char g, unsigned char b );
	void SetColor(SGD::Color);
	void SetScale(float sca);
	void SetRotation(float rot);
	void SetVelocity(float vel);
	void SetPosition(SGD::Vector vec);
	void SetDirection(SGD::Vector vec);

private:
	SGD::Vector Direction; 
	SGD::Vector Position;
	//SGD::Size Scale;

	float
		LifeEnd,
		LifeCurrent,
		Rotation,
		Scale,
		Velocity;

	unsigned char
		ColorA,
		ColorR,
		ColorG,
		ColorB;
};

