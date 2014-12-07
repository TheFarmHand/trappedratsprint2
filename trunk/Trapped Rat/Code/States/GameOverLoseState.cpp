
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
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
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
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	GameData::GetInstance()->GetFont()->DrawString("You Have been Defeated in Combat!", 50.0f, 200.0f, { 0, 0, 0 }, 3.0f);
	GameData::GetInstance()->GetFont()->DrawString("Hit Escape to Exit", 200.0f, 500.0f, { 0, 0, 0 }, 1.0f);


}
void  GameOverLoseState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
}