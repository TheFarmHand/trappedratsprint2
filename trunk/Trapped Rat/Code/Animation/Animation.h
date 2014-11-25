#pragma once
#include <string>
#include "Frame.h"
#include "AnimationSystem.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../Core/BaseObject.h"
class Animation
{
	friend class AnimationSystem;
	std::string m_sNextAnimation;
	bool m_bLooping;
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;

	float m_fTimeOnFrame;
	bool m_bPaused;
	AnimationSystem* OwnerSystem = nullptr;
	int m_nCurFrame;
	std::vector<Frame*> m_vframes;
	std::string m_sName;

	void Next();
	void PlayNext();
	void CalculateOffset(SGD::Point AP, SGD::Size& offset, SGD::Rectangle Rect);
	void SendEvent(std::string);
public:




	Animation();
	~Animation();
	bool isColliding(BaseObject* ob);
	bool isColliding(Animation* a);
	void Render(float x, float y);
	void Update(float dt);
	void Load(std::string filename);
	void Reset();
	void Play();
	void Pause();
	SGD::Rectangle GetColRect();


};

