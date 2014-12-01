#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class MainMenuState :
	public GameState
{
private:

	MainMenuState() = default;
	virtual ~MainMenuState() = default;
	MainMenuState& operator=(MainMenuState& rhs) = delete;
	MainMenuState(MainMenuState& rhs) = delete;
	SGD::HTexture background = SGD::INVALID_HANDLE;
	SGD::HTexture button = SGD::INVALID_HANDLE;
	SGD::HTexture cursor = SGD::INVALID_HANDLE;
	int index = 0;
public:
	static MainMenuState * GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
};

