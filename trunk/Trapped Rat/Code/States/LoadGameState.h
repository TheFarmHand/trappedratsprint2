#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <vector>
struct data
{
	float playtime = 0.0f;
	int town = 0;
	int partysize = 0;
	int gold = 0;
};
class LoadGameState :
	public GameState
{
	LoadGameState();
	virtual ~LoadGameState();
	LoadGameState(LoadGameState& rhs) = delete;
	LoadGameState& operator=(LoadGameState& rhs) = delete;

	data files[3];

	int index = 0;
	int maxindex = 0;
	bool is_saving = false;
	SGD::HTexture background = SGD::INVALID_HANDLE;
	SGD::HTexture button = SGD::INVALID_HANDLE;
	SGD::HTexture cursor = SGD::INVALID_HANDLE;
	SGD::HTexture scroll = SGD::INVALID_HANDLE;
public:
	static LoadGameState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
};

