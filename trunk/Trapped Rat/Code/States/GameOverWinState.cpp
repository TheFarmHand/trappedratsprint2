
#include "GameOverWinState.h"
#include "../Core/GameData.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../Font/Font.h"
#include "MainMenuState.h"
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
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
}
void const GameOverWinState::Render()
{
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	GameData::GetInstance()->GetFont()->DrawString("You Have Defeated Cecil_and_Avenged your parents!", 50.0f, 200.0f, {0,0,0} ,3.0f);
	GameData::GetInstance()->GetFont()->DrawString("Hit Escape to Exit", 200.0f, 500.0f, { 0, 0, 0 }, 1.0f);
}
void  GameOverWinState::Update(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}
}
void  GameOverWinState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
}

