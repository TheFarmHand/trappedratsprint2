#pragma once
#include "Ability.h"
#include "Stats.h"
class DamAbility :
	public Ability
{
private:
	int dmg;
	Stats used;
	float statmodify;
public:
	DamAbility();
	~DamAbility();

	void Render();
	void CastAbility(Character* owner);
};

