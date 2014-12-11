
#include "CreditsState.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include <string>
#include "../SGD Wrappers/SGD_InputManager.h"
#include "MainMenuState.h"


 CreditsState* CreditsState::GetInstance()
{
	 static CreditsState data;

	 return &data;
}

 void const CreditsState::Render()
 {

	 float offset = 500.0f;
	 float newy = offset - increment;
	 Font * font = GameData::GetInstance()->GetFont();
	 SGD::GraphicsManager::GetInstance()->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	font->DrawString("Trapped Rat:", 50.0f, 30.0f + newy, { 0,0,0 }, 3.0f);
	 //font->DrawString("Developers...", 50.0f, 80.0f, { 155, 155, 155 });
	 //font->DrawString("Riley Wood, Joe Thompson, Matthew Zanini, Grant Taylor", 100.0f, 110.0f, { 155, 155, 155 });
	 //font->DrawString("Artists...", 50.0f, 140.0f, { 155, 155, 155 });
	 //font->DrawString("Gregory Bey, Caris Frazier", 100.0f, 170.0f, { 155, 155, 155 });
	 //font->DrawString("Executive Producer...", 50.0f, 200.0f, { 155, 155, 155 });
	 //font->DrawString("John Oleske", 100.0f, 230.0f, { 155, 155, 155 });
	 //font->DrawString("Associate Producers...", 50.0f, 260.0f, { 155, 155, 155 });
	 //font->DrawString("Shawn Paris, Robert Martinez", 100.0f, 290.0f, { 155, 155, 155 });
	 //font->DrawString("Press Escape to Exit", 50.0f, 500.0f, { 155, 155, 155 });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(logo, { 150.0f, -395.0f + newy}, { 0.0f, 0.0f, 500.0f, 326.0f });
	 font->DrawString("Executive Producer...", 50.0f, 80.0f + newy, { 0,0,0},2.0f);
	 font->DrawString("John Oleske", 100.0f, 110.0f + newy, { 0,0,0 },2.0f);
	 font->DrawString("Associate Producers...", 50.0f, 140.0f + newy, { 0, 0, 0 },2.0f);
	 font->DrawString("Shawn Paris, Robert Martinez", 100.0f, 170.0f + newy, { 0,0,0 },2.0f);
	 font->DrawString("Artists...", 50.0f, 200.0f + newy, { 0,0,0 },2.0f);
	 font->DrawString("Gregory Bey, Caris Frazier, Erich Jaeger", 100.0f, 230.0f + newy, { 0,0,0 },2.0f);
	 font->DrawString("Developers...", 50.0f, 260.0f + newy, { 0,0,0 },2.0f);
	 font->DrawString("Riley Wood, Joe Thompson, Matthew Zanini, Grant Taylor", 100.0f, 290.0f + newy, { 0,0,0 },2.0f);
	 //font->DrawString("Press Escape to Exit", 50.0f, 500.0f, { 0,0,0 });
 }
void CreditsState::Enter()
{
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/MenuBackground.png");
	logo = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/logo.png");

}
void CreditsState::Update(float dt)
{
	increment += dt * 30;
	if (increment >= 820.0f)
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape) || SGD::InputManager::GetInstance()->IsButtonPressed(0,2))
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
}
void CreditsState::Exit()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(logo);
	increment = 0.0f;
}
