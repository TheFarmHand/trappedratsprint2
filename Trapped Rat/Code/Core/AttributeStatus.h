#pragma once
#include "../Core/StatusEffect.h"
class AttributeStatus :
	public StatusEffect
{
private:
	int value;
	int changedstat;
public:
	AttributeStatus();
	~AttributeStatus();
};

