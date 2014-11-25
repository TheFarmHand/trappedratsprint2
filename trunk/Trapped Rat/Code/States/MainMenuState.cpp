
#include "MainMenuState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include <string>
#include "../Core/GameData.h"
#include "GameOverLoseState.h"
#include "GamePlayState.h"
#include "TileDebugState.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../Font/Font.h"
#include "../Font/FontTestState.h"
#include "../Animation/AnimTestState.h"
#include "../Font/FontTestState.h"
#include "OptionsState.h"
#include "CreditsState.h"


MainMenuState * MainMenuState::GetInstance()
{
	static MainMenuState data;
	return &data;
}
void MainMenuState::Enter()
{
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");

}
void MainMenuState::Update(float dt)
{
	if (dt <= 0)
		return;
	SGD::InputManager *input = SGD::InputManager::GetInstance();

	/*if (input->IsKeyPressed(SGD::Key::F))
	{
		GameData::GetInstance()->SwapState(FontTestState::GetInstance());
	}
	if ( input->IsKeyPressed( SGD::Key::D ) )
		{
		GameData::GetInstance()->SwapState( TileDebugState::GetInstance() );
		}
	if (input->IsKeyPressed(SGD::Key::A))
	{
		GameData::GetInstance()->SwapState(AnimTestState::GetInstance());
	}*/


	if (input->IsKeyPressed(SGD::Key::Enter) || input->IsButtonPressed(0,0))
	{

		switch (index)
		{
		case 0:
			{
				  GameData::GetInstance()->SwapState(GamePlayState::GetInstance());
				  break;
			}
			
		case 1:
			{
				  GameData::GetInstance()->SwapState(OptionsState::GetInstance());
				  break;
			}
		case 2:
		{
				  GameData::GetInstance()->SwapState(CreditsState::GetInstance());
				  break;
		}
		case 3:
		{
				 
				  GameData::GetInstance()->SetRunning(false);
				  break;
		}
		}
	}

	if (input->IsKeyPressed(SGD::Key::Up) || input->IsDPadPressed(0,SGD::DPad::Up))
	{
		index--;
		if (index < 0)
			index = 3;
	}
	if (input->IsKeyPressed(SGD::Key::Down) || input->IsDPadPressed(0, SGD::DPad::Down))
	{
		index++;
		if (index > 3)
			index = 0;
	}
}
void const MainMenuState::Render()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTexture(background, { 0.0, 0.0 });
	//display the menu items
	//graphics->DrawString("Play", { 50.0, 100.0 });
	GameData::GetInstance()->GetFont()->DrawString("Play",  50.0f, 100.0f , {0,0,0});
	//graphics->DrawString("Exit", { 50.0, 200.0 });
	GameData::GetInstance()->GetFont()->DrawString("Options", 50.0f, 150.0f, { 0,0,0});
	GameData::GetInstance()->GetFont()->DrawString("Credits", 50.0f, 200.0f, { 0,0,0 });
	GameData::GetInstance()->GetFont()->DrawString("Exit",  50.0f, 250.0f , { 0,0,0});

	//display the cursor
	//graphics->DrawString("*", { 45.0f, 100.0f + (index * 100) });
	graphics->DrawRectangle({ 35.0f, 100.0f + (index * 50), 45.0f, 110.0f + (index * 50) }, { 155, 0, 155 }, { 155, 155, 155 });
}
void MainMenuState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
}
