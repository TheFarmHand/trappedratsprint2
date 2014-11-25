
#include "AnimTestState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include <string>
#include "../Core/GameData.h"
#include "Animation.h"
#include "AnimationSystem.h"
#include "../States/MainMenuState.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../Core/Player.h"
#include "../Core/GameData.h"


AnimTestState * AnimTestState::GetInstance()
{
	static AnimTestState data;
	return &data;
}
void AnimTestState::Enter()
{

	//as = new AnimationSystem;
	//as2 = new AnimationSystem;
	//x = new Animation;
	//z = new Animation;
//	x2 = new Animation;
//	z2 = new Animation;

	
	//x->Load("x");
	//z->Load("z");
	//x2->Load("../Trapped Rat/Assets/Scripts/testanim.xml");
	//x2->Load("x2");
	//z2->Load("z2");
	

	//as->Load(x);
	//as->Load(z);

	//as2->Load("../Trapped Rat/Assets/Scripts/testanim.xml");
	//as2->Load(x2);
	//as2->Load(z2);

	//as->Play(0);

	pos1 = SGD::Point(100, 100);
	pos2 = SGD::Point(500, 100);
	pos3 = SGD::Point(290, 500);
	testVel = 1;
	testVel2 = -1;
	testvel3 = -1;

	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);
	SGD::EventManager::GetInstance()->Initialize();
	testob = new AnimTestObject();
	testBase = new AnimTestBase();
	testob2 = new AnimTestObject();
	testbar = new Bars(SGD::Size(64,16), SGD::Point(200,200), nullptr);
	testbar->SetColor(SGD::Color(255, 255, 0));
}
void AnimTestState::Update(float dt)
{
	
	SGD::InputManager *input = SGD::InputManager::GetInstance();
	SGD::EventManager::GetInstance()->Update();
	SGD::MessageManager::GetInstance()->Update();
	if (testob->as->CheckCollision(testBase))
	{
		testVel = 0;
		pos2.x += 10;
		testVel2 *= -1;
		testbar->SetPercentage(testbar->GetPercentage()-.25f);
	}
	else if (testob->as->CheckCollision(testob2->as))
	{
		testVel = 1;
		testvel3 *= -1;
		testbar->SetPercentage(.1f);
	}

	pos1.x += testVel;
	pos2.x += testVel2;
	pos3.y += testvel3;
	testBase->SetPosition(pos2);
	testob->Update(dt);
	testob2->Update(dt);
	//as->Update(dt);
	//as2->Update(dt);
	if (input->IsKeyPressed(SGD::Key::M))
	{
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
		return;
	}
	if (input->IsKeyPressed(SGD::Key::N))
	{
		//as2->ResetAll();
		//as2->Play(0);
	}

	//pos1.x += testVel;
	//pos2.x += testVel2;

	//if (as->CheckCollision(as))
	//{
	//	testVel = 0;
	//	testVel2 = 0;
	//}
	
}
void const AnimTestState::Render()
{
	SGD::GraphicsManager *graphics = SGD::GraphicsManager::GetInstance();
	std::string name = "Animation Test State";
	//graphics->DrawString(name.c_str(), { 5.0f, 5.0f }, { 255,255,255});
	graphics->DrawRectangle({ 100, 100, 300, 300 }, { 255, 0, 0 }, { 155, 155, 155 }, 10);
	testob->Render(pos1.x,pos1.y);
	testob2->Render(pos3.x, pos3.y);
	testBase->Render();
	testbar->Render();
	//as->Render(pos1.x,pos1.y);
	//as2->Render(pos2.x,pos2.y);
}
void AnimTestState::Exit()
{
	//delete as;
	//delete as2;
	delete testob;
	delete testBase;
	delete testob2;
	delete testbar;
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::GetInstance()->DeleteInstance();
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::GetInstance()->DeleteInstance();
}
void AnimTestState::MessageProc(const SGD::Message * mess)
{
	switch (mess->GetMessageID())
	{
	case MessageID::NOTHING:
		break;
	default:
		break;
	}
}