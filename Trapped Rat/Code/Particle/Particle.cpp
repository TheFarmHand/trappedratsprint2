
#include "Particle.h"


Particle::Particle()
{
}


Particle::~Particle()
{
}


void Particle::Update(float dt)
// Updates LifeCurrent and Position only
{
	LifeCurrent += dt;
	Position += (Direction * Velocity *dt);
	
}


void Particle::SetLife( float end, float current )
{
	LifeEnd = end;
	LifeCurrent = current;
	
}

void Particle::SetColor( unsigned char a, unsigned char r, unsigned char g, unsigned char b )
{
	ColorA = a;
	ColorR = r;
	ColorG = g;
	ColorB = b;
}

void Particle::SetColor(SGD::Color col)
{
	ColorA = col.alpha;
	ColorR = col.red;
	ColorG = col.green;
	ColorB = col.blue;
}

void Particle::SetPosition( SGD::Vector pos )
{
	Position = pos;
}

void Particle::SetScale( float sca )
{
	Scale = sca;
}

void Particle::SetRotation( float rot )
{
	Rotation = rot;
}

void Particle::SetVelocity( float vel )
{
	Velocity = vel;
}

void Particle::SetDirection(SGD::Vector vec)
{
	Direction = vec;
}

SGD::Color Particle::GetColor()
{
	return SGD::Color(ColorA, ColorR, ColorG, ColorB);
}