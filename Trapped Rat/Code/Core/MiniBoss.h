#pragma once
#include "Enemy.h"
class MiniBoss :
	public Enemy
{
public:
	MiniBoss();
	virtual ~MiniBoss();

	void virtual Update(float dt);
	void virtual Render();
	void virtual BehaviorAI();

	void WindRougue();
	void FireWarrior();
	void EarthMonth();
	void WaterWizard();
};

