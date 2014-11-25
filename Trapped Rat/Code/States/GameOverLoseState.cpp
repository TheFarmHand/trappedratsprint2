
#include "GameOverLoseState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "MainMenuState.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include <string>

GameOverLoseState::GameOverLoseState()
{
}


GameOverLoseState::~GameOverLoseState()
{
}

 GameOverLoseState* GameOverLoseState::GetInstance()
{
	 static GameOverLoseState data;
	 return &data;
}
void  GameOverLoseState::Enter()
{

}


void  GameOverLoseState::Update(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}
}
void const GameOverLoseState::Render()
{
	Font * font = GameData::GetInstance()->GetFont();
	font->DrawString("YOU HAVE LOST!", 50.0f, 50.0f, { 155, 155, 155 }, 3.0f);
	font->DrawString("Hit Escape to Exit",50.0f, 200.0f, { 155, 155, 155 });


}
void  GameOverLoseState::Exit()
{
	
}