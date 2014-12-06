#pragma once
#include "Enemy.h"
#include "../Animation/AnimationSystem.h"
class CommonEnemy :
	public Enemy
{
private:
	//Cecil fight variables
	int CecilPhase = 1;
	
public:
	CommonEnemy();
	virtual ~CommonEnemy();

	void virtual Update(float dt);
	void virtual Render();
	void virtual BehaviorAI();

	void CatAI();
	void DogAI();
	void RavenAI();
	void ChefAI();
	void BlacksmithAI();
	void ShopKeeperAI();
	void TailorAI();
	void PriestAI();

	//Final boss fight
	void CecilAI( int phase );
	void CecilPhaseOne();
	void CecilPhaseTwo();
	void CecilPhaseThree();
	void JaneAI();
	void JohnAI();

};

