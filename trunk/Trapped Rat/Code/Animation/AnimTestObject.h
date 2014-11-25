#pragma once
#include "../SGD Wrappers/SGD_Listener.h"
#include "../Animation/AnimationSystem.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
class AnimTestObject :
	public SGD::Listener
{
	SGD::HAudio m_hAudio;
public:
	AnimationSystem* as;
	AnimTestObject();
	virtual ~AnimTestObject();
	void virtual Update(float dt);
	void virtual Render();
	void virtual Render(float x, float y);
	virtual void	HandleEvent(const SGD::Event* pEvent);
};