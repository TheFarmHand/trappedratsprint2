#include "TileDebugState.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "..\SGD Wrappers\SGD_InputManager.h"
#include "../Core/GameData.h"
#include "../SGD Wrappers/SGD_EventManager.h"


TileDebugState * TileDebugState::GetInstance()
{
	static TileDebugState data;
	return &data;
}
void TileDebugState::Enter()
{
	SGD::EventManager::GetInstance()->Initialize();
}
void TileDebugState::Update(float dt)
{
	GameData::GetInstance()->GetOverworldPlayer()->Update(dt);
	GameData::GetInstance()->UpdateCamera(GameData::GetInstance()->GetOverworldPlayer());
}
void const TileDebugState::Render()
{
	tiler.GetInstance()->Render();
	GameData::GetInstance()->GetOverworldPlayer()->Render();
}
void TileDebugState::Exit()
{
	tiler.GetInstance()->Exit();
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::GetInstance()->DeleteInstance();
}
