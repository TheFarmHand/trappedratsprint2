#pragma once

#include "BaseObject.h"
#include "Items.h"



class RatTrap :
	public BaseObject
{
public:
	RatTrap( );
	~RatTrap( ) = default;

	int items[3];
	int gold = 0;

	void Render();

};