#pragma once
#include "Character.h"
class AnimationSystem;
class Enemy :
	public Character
{
private:
	AnimationSystem* ansys = nullptr;
protected:
	float pausetimer;
public:
	Enemy();
	virtual ~Enemy();

	void virtual Update(float dt);
	void virtual UpdateAnimation( float dt );

	void virtual Render();
	void virtual BehaviorAI();

	void SetAnimations(AnimationSystem*);
	AnimationSystem* GetAnimations();

	int virtual GetType();
};

