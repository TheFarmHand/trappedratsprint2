#pragma once
#include "BaseObject.h"
#include "../Animation/AnimationSystem.h"
class WorldObject :
	public BaseObject
{
AnimationSystem* ansys;

public:
	WorldObject( std::string imagePath, std::string aniPath, bool allied );
	virtual ~WorldObject();

	bool active = true;

	void virtual Update(float dt);
	void virtual Render();
};

