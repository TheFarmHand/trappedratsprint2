#pragma once
#include "GameState.h"
#include <list>
enum states{settings, howto, none};
class OptionsState :
	public GameState
{
	OptionsState();
	virtual ~OptionsState();
	OptionsState(OptionsState& rhs);
	OptionsState& operator=(OptionsState& rhs);

	int menuindex = 0;
	int maxindex = 0;
	std::list<states> _states;

public:
	static OptionsState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
};

