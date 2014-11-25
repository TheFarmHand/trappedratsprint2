#pragma once
#include "GameState.h"
class CreditsState :
	public GameState
{
	CreditsState() = default;
	virtual ~CreditsState() = default;
	CreditsState(CreditsState& rhs) = delete;
	CreditsState& operator=(CreditsState& rhs) = delete;
public:
	static CreditsState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	
};

