#pragma once

#include "ParticleFlyweight.h"
//#include "Particle.h"



#include "..\SGD Wrappers\SGD_Geometry.h"
#include <list>

class Particle;

class Emitter
{
public:
	Emitter();
	~Emitter();

	void Cleanup();

	void Update(float dt);
	void Render();
	void EmptyImage();

	bool isActive();
	void SetActive(bool set);

	void TestCase(bool loop);


	// Setem Up
	ParticleFlyweight& GetFly()				{ return myFly;}
	void SetPosition(float x, float y)		{ myPosition.x = x; myPosition.y = y; }
	void SetDirection( float x, float y )	{ myDirection.x = x; myDirection.y = y; }
	void SetSize(float siz)					{ mySize.width = siz; mySize.height = siz; }
	void SetLooping(bool loop)				{ myLooping = loop; }
	void SetDir(bool x, bool y)				{ myDirX = x; myDirY = y; }
	void SetMinParticles(int min)			{ myMinParticles = min; }
	void SetMaxParticles(int max)			{ myMaxParticles = max; }
	void SetTotalParticles(int tot)			{ myTotalParticles = tot; }
	void SetSpawnRate(float spa)			{ mySpawnRate = spa; }


	bool GetDirX()							{ return myDirX; }
	bool GetDirY()							{ return myDirY; }

	void SetShape(std::string shape);		// in .cpp line 176

	Emitter& operator=(const Emitter& rhs);


private:

	ParticleFlyweight myFly;

	std::list<Particle*> myLiveList;
	std::list<Particle*> myDeadList;

	SGD::Vector myDirection;
	SGD::Vector myPosition;
	SGD::Size	mySize;

	bool myLooping;
	bool active;
	bool myDirX, myDirY;		// Used for left/right, up/down duality.  
								// If true, myDirection randoms between negative & posative values

	int myMaxParticles, 
		myMinParticles,
		myTotalParticles = 0;
	int myShape;				// point, rectangle, circle, line

	float mySpawnRate;

	void UpdateLooping(float dt);
	void UpdateBurst(float dt);
	void SetParticle(Particle* &part);
};

