#pragma once
#include "Enemy.h"
class Guard :
	public Enemy
{
public:
	Guard();
	virtual ~Guard();


	void virtual Update(float dt);
	void virtual Render();
	void virtual BehaviorAI();
};

