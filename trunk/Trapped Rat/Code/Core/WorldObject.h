#pragma once
#include "BaseObject.h"
class WorldObject :
	public BaseObject
{
public:
	WorldObject();
	virtual ~WorldObject();

	void virtual Update(float dt);
	void virtual Render();
};

