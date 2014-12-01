
#include "OptionsState.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "MainMenuState.h"
#include  <sstream>
OptionsState::OptionsState()
{

}

OptionsState::~OptionsState()
{

}
OptionsState* OptionsState::GetInstance()
{
	static OptionsState data;

	return &data;
}
void OptionsState::Enter()
{
	menuindex = 0;
	maxindex = 4;

	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
	button = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/menubutton.png", { 255, 255, 255 });
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/cursor.png", { 255, 255, 255 });
}
void const OptionsState::Render()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });

	graphics->DrawTextureSection(cursor, { 45.0f, 95.0f + (menuindex * 80) }, { 0, 0, 238, 73 });

	GameData::GetInstance()->GetFont()->DrawString("Language:", 70.0f, 120.0f, { 155, 155, 155 }, 1.5f);
	if (GameData::GetInstance()->GetFont()->IsSpanish())
		GameData::GetInstance()->GetFont()->DrawString("Spanish",150.0f, 125.0f, { 0,0,0 });
	else
		GameData::GetInstance()->GetFont()->DrawString("English", 175.0f, 125.0f, { 0,0,0 });

	GameData::GetInstance()->GetFont()->DrawString("Music Volume:", 70.0f, 200.0f, { 155, 155, 155 }, 1.5f);
	std::ostringstream mout;
	mout << GameData::GetInstance()->GetMusicVolume();
	GameData::GetInstance()->GetFont()->DrawString(mout.str(), 220.0f, 205.0f, { 0,0,0 });
	GameData::GetInstance()->GetFont()->DrawString("Effect Volume:", 70.0f, 280.0f, { 155, 155, 155 }, 1.5f);
	std::ostringstream eout;
	eout << GameData::GetInstance()->GetEffectVolume();
	GameData::GetInstance()->GetFont()->DrawString(eout.str(), 220.0f, 285.0f, { 0,0,0 });

	GameData::GetInstance()->GetFont()->DrawString("FullScreen:", 70.0f, 360.0f, { 155, 155, 155 }, 1.5f);
	if (GameData::GetInstance()->GetWindowed())
		GameData::GetInstance()->GetFont()->DrawString("Off", 200.0f, 360.0f, { 0,0,0 });
	else
		GameData::GetInstance()->GetFont()->DrawString("On", 200.0f, 360.0f, { 0,0,0 });

	GameData::GetInstance()->GetFont()->DrawString("Exit", 70.0f, 440.0f, { 155, 155, 155 }, 1.5f);


	//graphics->DrawRectangle({ 35.0f, 100.0f + (menuindex * 50), 45.0f, 110.0f + (menuindex * 50) }, { 155, 0, 155 }, { 155, 155, 155 });
}
void OptionsState::Update(float dt)
{
	if (dt <= 0.0)
		return;
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	if (input->IsKeyPressed(SGD::Key::Escape) || input->IsButtonPressed(0, 1))
	{
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}
	if (menuindex == 0)
	{
		if (input->IsKeyPressed(SGD::Key::Left) || input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0, SGD::DPad::Left) || input->IsDPadPressed(0, SGD::DPad::Right))
		{
			bool is = GameData::GetInstance()->GetFont()->IsSpanish();
			GameData::GetInstance()->GetFont()->SetSpanish(!is);
		}
	}
	if (menuindex == 1)
	{
		if (input->IsKeyPressed(SGD::Key::Left) || input->IsDPadPressed(0, SGD::DPad::Left))
		{
			GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() - 5);
		}
		else if (input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0, SGD::DPad::Right))
		{
			GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() + 5);
		}
	}
	if (menuindex == 2)
	{
		if (input->IsKeyPressed(SGD::Key::Left) || input->IsDPadPressed(0, SGD::DPad::Left))
		{
			GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() - 5);
		}
		else if (input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0, SGD::DPad::Right))
		{
			GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() + 5);
		}
	}
	if (menuindex == 3)
	{
		if (input->IsKeyPressed(SGD::Key::Left) || input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0, SGD::DPad::Left) || input->IsDPadPressed(0, SGD::DPad::Right))
		{
			bool is = GameData::GetInstance()->GetWindowed();
			GameData::GetInstance()->SetWindowed(!is);
			SGD::GraphicsManager::GetInstance()->Resize({ GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed());
		}
	}
	if (menuindex == 4)
	{
		if (input->IsKeyPressed(SGD::Key::Enter))
			GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}







	if (input->IsKeyPressed(SGD::Key::Up) || input->IsDPadPressed(0, SGD::DPad::Up))
	{
		menuindex--;
		if (menuindex < 0)
			menuindex = maxindex;
	}
	if (input->IsKeyPressed(SGD::Key::Down) || input->IsDPadPressed(0, SGD::DPad::Down))
	{
		menuindex++;
		if (menuindex > maxindex)
			menuindex = 0;
	}
}
void OptionsState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(button);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cursor);
	menuindex = 0;
}
