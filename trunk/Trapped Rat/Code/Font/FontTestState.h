#pragma once
#include "../States/GameState.h"
#include "Font.h"
class FontTestState :
	public GameState
{
	FontTestState() = default;
	~FontTestState() = default;
	FontTestState(FontTestState& rhs) = delete;
	FontTestState& operator=(FontTestState& rhs) = delete;

	Font * font;
public:
	void virtual Enter();
	void virtual Update(float dt);
	static FontTestState * GetInstance();
	void virtual const Render();
	void virtual Exit();
};

