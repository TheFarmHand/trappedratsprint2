
#include "GameOverWinState.h"


GameOverWinState::GameOverWinState()
{
}


GameOverWinState::~GameOverWinState()
{
}
 GameOverWinState * GameOverWinState::GetInstance()
{
	 static GameOverWinState data;

	 return &data;
}
void  GameOverWinState::Enter()
{

}
void const GameOverWinState::Render()
{

}
void  GameOverWinState::Update(float dt)
{

}
void  GameOverWinState::Exit()
{

}

