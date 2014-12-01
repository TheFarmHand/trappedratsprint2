#pragma once
#include "GameState.h"
#include <list>
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class OptionsState :
	public GameState
{
	OptionsState();
	virtual ~OptionsState();
	OptionsState(OptionsState& rhs);
	OptionsState& operator=(OptionsState& rhs);

	int menuindex = 0;
	int maxindex = 0;
	
	SGD::HTexture background = SGD::INVALID_HANDLE;
	SGD::HTexture button = SGD::INVALID_HANDLE;
	SGD::HTexture cursor = SGD::INVALID_HANDLE;

public:
	static OptionsState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
};

