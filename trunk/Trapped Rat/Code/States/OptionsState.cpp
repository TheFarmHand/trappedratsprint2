
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
	_states.clear();
	_states.push_front(states::none);
}
void const OptionsState::Render()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	switch (_states.front())
	{
	case none:
		GameData::GetInstance()->GetFont()->DrawString("Settings", 50.0f, 100.0f, { 155, 155, 155 });
		GameData::GetInstance()->GetFont()->DrawString("Exit", 50.0f, 150.0f, { 155, 155, 155 });
		break;
	case settings:
	{
					 GameData::GetInstance()->GetFont()->DrawString("Language:", 50.0f, 100.0f, { 155, 155, 155 });
					 if (GameData::GetInstance()->GetFont()->IsSpanish())
						 GameData::GetInstance()->GetFont()->DrawString("Spanish", 200.0f, 100.0f, { 155, 155, 155 });
					 else
						 GameData::GetInstance()->GetFont()->DrawString("English", 200.0f, 100.0f, { 155, 155, 155 });

					 GameData::GetInstance()->GetFont()->DrawString("Music Volume:", 50.0f, 150.0f, { 155, 155, 155 });
					 std::ostringstream mout;
					 mout << GameData::GetInstance()->GetMusicVolume();
					 GameData::GetInstance()->GetFont()->DrawString(mout.str(), 200.0f, 150.0f, { 155, 155, 155 });
					 GameData::GetInstance()->GetFont()->DrawString("Effect Volume:", 50.0f, 200.0f, { 155, 155, 155 });
					 std::ostringstream eout;
					 eout << GameData::GetInstance()->GetEffectVolume();
					 GameData::GetInstance()->GetFont()->DrawString(eout.str(), 200.0f, 200.0f, { 155, 155, 155 });

					 GameData::GetInstance()->GetFont()->DrawString("FullScreen:", 50, 250.0f, { 155, 155, 155 });
					 if (GameData::GetInstance()->GetWindowed())
						 GameData::GetInstance()->GetFont()->DrawString("Off", 200.0f, 250.0f, { 155, 155, 155 });
					 else
						 GameData::GetInstance()->GetFont()->DrawString("On", 200.0f, 250.0f, { 155, 155, 155 });

					 GameData::GetInstance()->GetFont()->DrawString("Exit", 50.0f, 300.0f, { 155, 155, 155 });
	}
		break;
	case howto:
		break;
	default:
		break;
	}

	graphics->DrawRectangle({ 35.0f, 100.0f + (menuindex * 50), 45.0f, 110.0f + (menuindex * 50) }, { 155, 0, 155 }, { 155, 155, 155 });
}
void OptionsState::Update(float dt)
{
	if (dt <= 0.0)
		return;
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	switch (_states.front())
	{
	case none:
		maxindex = 1;
		if (input->IsKeyPressed(SGD::Key::Enter) || input->IsButtonPressed(0,0))
		{
			if (menuindex == 0)
			{
				_states.push_front(states::settings);
			}
			else if (menuindex == 1)
			{
				GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
			}
		}
		if (input->IsKeyPressed(SGD::Key::Escape) || input->IsButtonPressed(0,1))
		{
			GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
		}
		break;
	case howto:
		
		break;
	case settings:
		maxindex = 4;
		if (input->IsKeyPressed(SGD::Key::Escape) || input->IsButtonPressed(0, 1))
		{
			_states.pop_front();
			menuindex = 0;
		}
		if (input->IsKeyPressed(SGD::Key::Enter) || input->IsButtonPressed(0, 0))
		{
			
			if (menuindex == 4)
			{
				_states.pop_front();
				menuindex = 0;

			}
		}
		if (menuindex == 0)
		{
			if (input->IsKeyPressed(SGD::Key::Left) || input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0,SGD::DPad::Left) || input->IsDPadPressed(0,SGD::DPad::Right))
			{
				bool is = GameData::GetInstance()->GetFont()->IsSpanish();
				GameData::GetInstance()->GetFont()->SetSpanish(!is);
			}
		}
		if (menuindex == 1)
		{
			if (input->IsKeyPressed(SGD::Key::Left) || input->IsDPadPressed(0,SGD::DPad::Left))
			{
				GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() - 5);
			}
			else if (input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0,SGD::DPad::Right))
			{
				GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() + 5);
			}
		}
		if (menuindex == 2)
		{
			if (input->IsKeyPressed(SGD::Key::Left) || input->IsDPadPressed(0,SGD::DPad::Left))
			{
				GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() - 5);
			}
			else if (input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0,SGD::DPad::Right))
			{
				GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() + 5);
			}
		}
		if (menuindex == 3)
		{
			if (input->IsKeyPressed(SGD::Key::Left) || input->IsKeyPressed(SGD::Key::Right) || input->IsDPadPressed(0,SGD::DPad::Left) || input->IsDPadPressed(0,SGD::DPad::Right))
			{
				bool is = GameData::GetInstance()->GetWindowed();
				GameData::GetInstance()->SetWindowed(!is);
				SGD::GraphicsManager::GetInstance()->Resize({ GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed());
			}
		}
		break;
	default:
		break;
	}

	



	if (input->IsKeyPressed(SGD::Key::Up) || input->IsDPadPressed(0,SGD::DPad::Up))
	{
		menuindex--;
		if (menuindex < 0)
			menuindex = maxindex;
	}
	if (input->IsKeyPressed(SGD::Key::Down) || input->IsDPadPressed(0,SGD::DPad::Down))
	{
		menuindex++;
		if (menuindex > maxindex)
			menuindex = 0;
	}
}
void OptionsState::Exit()
{
	_states.clear();
	_states.push_front(states::none);
	menuindex = 0;
}
