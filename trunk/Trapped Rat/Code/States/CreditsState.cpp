
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
	 Font * font = GameData::GetInstance()->GetFont();
	 font->DrawString("Trapped Rat:", 50.0f, 30.0f, { 155, 155, 155 }, 3.0f);
	 //font->DrawString("Developers...", 50.0f, 80.0f, { 155, 155, 155 });
	 //font->DrawString("Riley Wood, Joe Thompson, Matthew Zanini, Grant Taylor", 100.0f, 110.0f, { 155, 155, 155 });
	 //font->DrawString("Artists...", 50.0f, 140.0f, { 155, 155, 155 });
	 //font->DrawString("Gregory Bey, Caris Frazier", 100.0f, 170.0f, { 155, 155, 155 });
	 //font->DrawString("Executive Producer...", 50.0f, 200.0f, { 155, 155, 155 });
	 //font->DrawString("John Oleske", 100.0f, 230.0f, { 155, 155, 155 });
	 //font->DrawString("Associate Producers...", 50.0f, 260.0f, { 155, 155, 155 });
	 //font->DrawString("Shawn Paris, Robert Martinez", 100.0f, 290.0f, { 155, 155, 155 });
	 //font->DrawString("Press Escape to Exit", 50.0f, 500.0f, { 155, 155, 155 });
	 font->DrawString("Executive Producer...", 50.0f, 80.0f, { 155, 155, 155 });
	 font->DrawString("John Oleske", 100.0f, 110.0f, { 155, 155, 155 });
	 font->DrawString("Associate Producers...", 50.0f, 140.0f, { 155, 155, 155 });
	 font->DrawString("Shawn Paris, Robert Martinez", 100.0f, 170.0f, { 155, 155, 155 });
	 font->DrawString("Artists...", 50.0f, 200.0f, { 155, 155, 155 });
	 font->DrawString("Gregory Bey, Caris Frazier", 100.0f, 230.0f, { 155, 155, 155 });
	 font->DrawString("Developers...", 50.0f, 260.0f, { 155, 155, 155 });
	 font->DrawString("Riley Wood, Joe Thompson, Matthew Zanini, Grant Taylor", 100.0f, 290.0f, { 155, 155, 155 });
	 font->DrawString("Press Escape to Exit", 50.0f, 500.0f, { 155, 155, 155 });
 }
void CreditsState::Enter()
{

}
void CreditsState::Update(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
}
void CreditsState::Exit()
{

}
