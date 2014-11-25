#pragma once
#include "GameState.h"
class LoadGameState :
	public GameState
{
	LoadGameState();
	virtual ~LoadGameState();
	LoadGameState(LoadGameState& rhs) = delete;
	LoadGameState& operator=(LoadGameState& rhs) = delete;


	int index = 0;
public:
	static LoadGameState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
};

