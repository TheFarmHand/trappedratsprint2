#pragma once
#include "Enemy.h"
class Boss :
	public Enemy
{
public:
	Boss();
	virtual ~Boss();

	void virtual Update(float dt);
	void virtual Render();
	void virtual BehaviorAI();
	void Cecil();
	void Jane();
	void John();
};

