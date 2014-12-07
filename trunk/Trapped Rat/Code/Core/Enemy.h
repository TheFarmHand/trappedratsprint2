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
	bool firstBelowQuarter = false;
	bool firstBelowHalf = false;
	bool firstBelow3Quarter = false;
	

	//Common Enemy
	void CatAI();
	void RavenAI();
	void DogAI();
	void ChefAI();
	void BlacksmithAI();
	void ShopkeeperAI();
	void TailorAI();
	void PriestAI();
	//Mini Boss
	void GuardAI();
	void BWRAI();
	void FFWAI();
	void SEMAI();
	void WWWAI();
	//Final Boss
	void CecilAI( int phase );
	void CecilPhaseOne();
	void CecilPhaseTwo();
	void CecilPhaseThree();
	void JaneAI();
	void JohnAI();
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

};



