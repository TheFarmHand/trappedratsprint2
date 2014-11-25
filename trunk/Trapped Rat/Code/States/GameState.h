#pragma once
class GameState
{
public:
	GameState() = default;
	virtual ~GameState() = default;

	
public:
	void virtual Enter() = 0;
	void virtual Update(float dt) = 0;
	void virtual const Render() = 0;
	void virtual Exit() = 0;
};

