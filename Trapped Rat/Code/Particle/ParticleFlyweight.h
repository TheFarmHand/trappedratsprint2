#pragma once

#include "..\\SGD Wrappers\SGD_GraphicsManager.h"

class ParticleFlyweight
{
public:
	ParticleFlyweight();
	~ParticleFlyweight();


	// Mutate
	// Sets take in all values related to individual types
	// Color start takes in ARGB all at once
	void SetColorStart( unsigned char a, unsigned char r, unsigned char g, unsigned char b );
	void SetColorEnd( unsigned char a, unsigned char r, unsigned char g, unsigned char b );
	void SetLife(float min, float max);
	void SetScale(float start, float end);
	void SetRoation(float start, float end);
	void SetVelocity(float start, float end);
	void SetImage(SGD::HTexture img);
	void SetRange(int x, int y);
	void SetOffset(float x, float y) { OffsetX = x; OffsetY = y; }


	// Access
	// Accessors are setup for singular data members
	float GetMinLife( )		{ return MinLife; }
	float GetMaxLife( )		{ return MaxLife; }

	unsigned char GetColorStartA( ) { return ColorStartA; }
	unsigned char GetColorStartR( ) { return ColorStartR; }
	unsigned char GetColorStartB( ) { return ColorStartB; }
	unsigned char GetColorStartG( ) { return ColorStartG; }

	unsigned char GetColorEndA( ) { return ColorEndA; }
	unsigned char GetColorEndR( ) { return ColorEndR; }
	unsigned char GetColorEndB( ) { return ColorEndB; }
	unsigned char GetColorEndG( ) { return ColorEndG; }

	int GetRangeX( )	  { return RangeX; }		// Unused functionality
	int GetRangeY( )	  { return RangeY; }

	float GetScaleStart( )	{ return ScaleStart; }
	float GetScaleEnd( )	{ return ScaleEnd; }
	float GetRotationStart( ) { return RotationStart; }
	float GetRotationEnd( )	  { return RotationEnd; }
	float GetVelocityStart( ) { return VelocityStart; }
	float GetVelocityEnd( )	  { return VelocityEnd; }
	float GetOffSetX()		  { return OffsetX; }
	float GetOffSetY()		  { return OffsetY; }
	
	void ClearImage();
	SGD::HTexture GetImage( ) { return Image; }

	// Overloads
	ParticleFlyweight& operator=(const ParticleFlyweight rhs);

private:
	float
		MinLife,
		MaxLife,
		ScaleStart,
		ScaleEnd,
		RotationStart,
		RotationEnd,
		VelocityStart,
		VelocityEnd,
		OffsetX,
		OffsetY;

	unsigned char
		ColorStartA,
		ColorStartR,
		ColorStartG,
		ColorStartB,
		ColorEndA,
		ColorEndR,
		ColorEndG,
		ColorEndB;

	int
		RangeX,
		RangeY;

	SGD::HTexture Image;
};

