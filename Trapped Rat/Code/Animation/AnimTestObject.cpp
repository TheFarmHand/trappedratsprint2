#include "AnimTestObject.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
AnimTestObject::AnimTestObject() : SGD::Listener(this)
{
	as = new AnimationSystem;
	//as->Load("testanim.xml");
	as->Load("RatAnimOverworld.xml");
	as->Play(0);
	RegisterForEvent("test");

	m_hAudio = SGD::AudioManager::GetInstance()->LoadAudio("../Trapped Rat/Assets/Sounds/Battle.xwm");
}
AnimTestObject::~AnimTestObject()
{

	delete as;
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hAudio);

}
void AnimTestObject::Update(float dt)
{

	as->Update(dt);
}
void AnimTestObject::Render()
{
	as->Render(300,300);
}
void  AnimTestObject::Render(float x, float y)
{
	as->Render(x, y);
}
void	AnimTestObject::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "test")
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hAudio,false);
		UnregisterFromEvent("test");
	}
}