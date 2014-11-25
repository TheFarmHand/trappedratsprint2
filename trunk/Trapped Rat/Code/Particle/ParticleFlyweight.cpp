
#include "ParticleFlyweight.h"


ParticleFlyweight::ParticleFlyweight()
{
}


ParticleFlyweight::~ParticleFlyweight()
{
}

void ParticleFlyweight::SetColorStart( unsigned char a, unsigned char r, unsigned char g, unsigned char b )
// Sets all 4 color values in one Mutator
{
	ColorStartA = a;
	ColorStartR = r;
	ColorStartG = g;
	ColorStartB = b;
}

void ParticleFlyweight::SetColorEnd( unsigned char a, unsigned char r, unsigned char g, unsigned char b )
// Sets all 4 color values in one Mutator
{
	ColorEndA = a;
	ColorEndR = r;
	ColorEndG = g;
	ColorEndB = b;
}

void ParticleFlyweight::SetLife( float min, float max )
// Sets min and max life values in one Mutator
{
	MinLife = min;
	MaxLife = max;
}

void ParticleFlyweight::SetScale( float start, float end )
{
	ScaleStart = start;
	ScaleEnd = end;
}

void ParticleFlyweight::SetRoation( float start, float end )
{
	RotationStart = start;
	RotationEnd = end;
}

void ParticleFlyweight::SetVelocity( float start, float end )
{
	VelocityStart = start;
	VelocityEnd = end;
}

void ParticleFlyweight::SetImage( SGD::HTexture img )
{
	Image = img;
}

void ParticleFlyweight::ClearImage()
{
	if ( Image != SGD::INVALID_HANDLE )
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture( Image );
		Image = SGD::INVALID_HANDLE;
	}
}

void ParticleFlyweight::SetRange( int x, int y )
{
	RangeX = x;
	RangeY = y;
}

ParticleFlyweight& ParticleFlyweight::operator=( const ParticleFlyweight rhs )
{
	// Super paranoid check
	if ( this == &rhs ) return *this;


	MinLife = rhs.MinLife;
	MaxLife = rhs.MaxLife;
	ColorStartA = rhs.ColorStartA;
	ColorStartR = rhs.ColorStartR;
	ColorStartG = rhs.ColorStartG;
	ColorStartB = rhs.ColorStartB;
	ColorEndA = rhs.ColorEndA;
	ColorEndR = rhs.ColorEndR;
	ColorEndG = rhs.ColorEndG;
	ColorEndB = rhs.ColorEndB;
	ScaleStart = rhs.ScaleStart;
	ScaleEnd = rhs.ScaleEnd;
	RotationStart = rhs.RotationStart;
	RotationEnd = rhs.RotationEnd;
	VelocityStart = rhs.VelocityStart;
	VelocityEnd = rhs.VelocityEnd;
	Image = rhs.Image;
	RangeX = rhs.RangeX;
	RangeY = rhs.RangeY;
	OffsetX = rhs.OffsetX;
	OffsetY = rhs.OffsetY;


	return *this;
}