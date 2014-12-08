
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
	button = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/button.png");
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/cheese.png");
	logo = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/logo.png");

	

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


	if (input->IsKeyPressed(SGD::Key::Enter) || input->IsButtonPressed(0, 1))
	{

		switch (index)
		{
		case 0:
		{
				  GameData::GetInstance()->PlaySelectionChange();
				  GameData::GetInstance()->SwapState(GamePlayState::GetInstance());
				  break;
		}

		case 1:
		{
				  GameData::GetInstance()->PlaySelectionChange();
				  GameData::GetInstance()->SwapState(OptionsState::GetInstance());
				  break;
		}
		case 2:
		{
				  GameData::GetInstance()->PlaySelectionChange();
				  GameData::GetInstance()->SwapState(CreditsState::GetInstance());
				  break;
		}
		case 3:
		{
				  GameData::GetInstance()->PlaySelectionChange();
				  GameData::GetInstance()->SetRunning(false);
				  break;
		}
		}
	}

	if (input->IsKeyPressed(SGD::Key::Up) || input->IsDPadPressed(0, SGD::DPad::Up))
	{
		index--;
		if (index < 0)
			index = 3;
		GameData::GetInstance()->PlaySelectionChange();
	}
	if (input->IsKeyPressed(SGD::Key::Down) || input->IsDPadPressed(0, SGD::DPad::Down))
	{
		index++;
		if (index > 3)
			index = 0;
		GameData::GetInstance()->PlaySelectionChange();
	}
}
void const MainMenuState::Render()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	graphics->DrawTextureSection(logo, { 300.0f, 95.0f }, { 0.0f, 0.0f, 500.0f, 326.0f });
	//display the menu items
	//graphics->DrawString("Play", { 50.0, 100.0 });
	//display the button
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });


	//display the cursor
	//graphics->DrawString("*", { 45.0f, 100.0f + (index * 100) });
	graphics->DrawTextureSection(cursor, { 10.0f, 95.0f + (index * 80) }, { 0, 0, 238, 73 });

	GameData::GetInstance()->GetFont()->DrawString("Play", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("Options", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("Credits", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("Exit", 100.0f, 360.0f, { 0, 0, 0 }, 2.0f);
}
void MainMenuState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(button);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cursor);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(logo);
}
