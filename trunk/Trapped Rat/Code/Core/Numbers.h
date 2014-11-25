#pragma once
#include "OwnedHUD.h"
class Numbers :
	public OwnedHUD
{
private:
	int value;
	float lifespan;
public:
	Numbers();
	virtual ~Numbers();

	void virtual Update(float dt);
	void virtual Render();
};

