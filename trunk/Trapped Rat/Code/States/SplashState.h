#pragma once
#include "../Core/GameData.h"
class SplashState :
	public GameState
{
	SplashState() = default;
	virtual ~SplashState() = default;
	SplashState& operator=(SplashState& rhs) = delete;
	SplashState(SplashState& rhs) = delete;
public:
	static SplashState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	
};

