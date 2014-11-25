#pragma once

#include <string>
#include "Animation.h"
#include "../Core/BaseObject.h"
class AnimationSystem
{
	friend class Animation;
	int m_nCurrentAnimation = -1;
	std::vector<Animation*> m_vAnimations;


public:
	AnimationSystem();
	~AnimationSystem();
	SGD::Point m_position;

	void Render(float PositionX, float PositionnY);
	void Update(float dt);
	void Load(Animation* Anim);
	void Load(std::string filename);
	void ResetAll();
	void Play(std::string name);
	void Play(int position);
	void Pause(bool is_pause);
	void UnloadAll();
	bool CheckCollision(AnimationSystem* as);
	bool CheckCollision(BaseObject* ob);
	SGD::Rectangle GetColRect();
};

