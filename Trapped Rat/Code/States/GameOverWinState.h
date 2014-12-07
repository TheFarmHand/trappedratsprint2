#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class GameOverWinState :
	public GameState
{
	GameOverWinState();
	virtual ~GameOverWinState();
	GameOverWinState(GameOverWinState& rhs) = delete;
	GameOverWinState& operator=(GameOverWinState& rhs) = delete;

	SGD::HTexture background = SGD::INVALID_HANDLE;
public:
	static GameOverWinState * GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	
};

