#pragma once
#include "Ability.h"
class OtherAbility :
	public Ability
{
public:
	OtherAbility();
	~OtherAbility();

	void CastAbility(Character* target);
};

