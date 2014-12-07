#pragma once
#include "Character.h"
class AnimationSystem;
class Enemy :
	public Character
{
private:
	AnimationSystem* ansys = nullptr;
	int xp_value = 100;
	//int level;
	
	
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

	// XP Stuffs
	int GetXPValue();
	int GetLevel();

	void SetLevel(int lev);
	void SetXPVal(int val);

	//Final boss fight
	void CecilAI( int phase );
	void CecilPhaseOne();
	void CecilPhaseTwo();
	void CecilPhaseThree();
	void JaneAI();
	void JohnAI();

};



