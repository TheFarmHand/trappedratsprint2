#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class CreditsState :
	public GameState
{
	CreditsState() = default;
	virtual ~CreditsState() = default;
	CreditsState(CreditsState& rhs) = delete;
	CreditsState& operator=(CreditsState& rhs) = delete;

	SGD::HTexture background = SGD::INVALID_HANDLE;
	SGD::HTexture logo = SGD::INVALID_HANDLE;
	float increment = 0.0f;
public:
	static CreditsState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	
};

