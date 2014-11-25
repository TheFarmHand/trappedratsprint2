#pragma once
#include "StatusEffect.h"
class DOTStatus :
	public StatusEffect
{
private:
	int value;
public:
	DOTStatus();
	~DOTStatus();


	void DoDamage();
};

