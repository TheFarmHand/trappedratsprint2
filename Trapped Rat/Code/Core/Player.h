#pragma once
#include "Character.h"
#include "../Animation/AnimationSystem.h"
#include "../SGD Wrappers/SGD_Listener.h"
class Items;
class Player :
	public Character, public SGD::Listener
{
	AnimationSystem* ansys;
	
public:
	Player();
	virtual ~Player();

	void virtual Update(float dt);
	void virtual Render();
	bool IsMoving();
	void HandleEvent( const SGD::Event* pEvent );
	//accessors
	
};

