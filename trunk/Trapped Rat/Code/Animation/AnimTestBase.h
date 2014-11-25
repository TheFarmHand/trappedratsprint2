#pragma once
#include "../Core/BaseObject.h"
class AnimTestBase :
	public BaseObject
{
public:
	AnimTestBase();
	~AnimTestBase();
	void Render();
	void Update(float dt);
};

