#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
class GameOverLoseState :
	public GameState
{
	GameOverLoseState();
	virtual ~GameOverLoseState();
	GameOverLoseState& operator=(GameOverLoseState& rhs) = delete;
	GameOverLoseState(GameOverLoseState& rhs) = delete;

	SGD::HTexture background = SGD::INVALID_HANDLE;


public:
	static GameOverLoseState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	
};

