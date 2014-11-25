
#include "LoadGameState.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "MainMenuState.h"

LoadGameState::LoadGameState()
{
}


LoadGameState::~LoadGameState()
{
}
LoadGameState* LoadGameState::GetInstance()
{
	static LoadGameState data;

	return &data;
}
void  LoadGameState::Enter()
{

}
void const LoadGameState::Render()
{
	Font * font = GameData::GetInstance()->GetFont();

	font->DrawString("Load File...", 50.0f, 50.0f, { 155, 155, 155 });
	font->DrawString("File 1", 100.0f, 100.0f, { 155, 155, 155 });
	font->DrawString("File 2", 100.0f, 150.0f, { 155, 155, 155 });
	font->DrawString("File 3", 100.0f, 200.0f, { 155, 155, 155 });

	SGD::GraphicsManager::GetInstance()->DrawRectangle({ 60.0f, 100.0f + (index * 50), 70.0f, 110.0f + (index * 50) }, { 155, 0, 155 }, { 155, 155, 155 });
}
void LoadGameState::Update(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Up))
	{
		index--;
		if (index < 0)
			index = 2;
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Down))
	{
		index++;
		if (index > 2)
			index = 0;
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) || SGD::InputManager::GetInstance()->IsButtonPressed(0,0))
	{
		GameData::GetInstance()->SetSaveFile(index);
		GameData::GetInstance()->LoadSave();
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}
}
void LoadGameState::Exit()
{

}
