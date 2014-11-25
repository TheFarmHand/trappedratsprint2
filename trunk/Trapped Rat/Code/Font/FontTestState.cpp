#include "FontTestState.h"



void  FontTestState::Enter()
{
	font = new Font();
	font->LoadFont("Assets/Textures/testpng.png", "Assets/Scripts/BitMapFont.txt", {0,0,0});
}
void  FontTestState::Update(float dt)
{

}
void  const FontTestState::Render()
{
	font->DrawString("This is the story of Riley Wood \t he was a silly MaN\n!~.?", 50.0, 50.0, {155,0,0});
}
void  FontTestState::Exit()
{
	delete font;
	font = nullptr;
}
FontTestState* FontTestState::GetInstance()
{
	static FontTestState data;

	return &data;
}