#pragma once
#include "../States/GameState.h"
#include "AnimationSystem.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../Animation/AnimTestBase.h"
#include "../Animation/AnimTestObject.h"
#include "../Core/Bars.h"
class AnimTestState :
	public GameState
{
	AnimTestState () = default;
	virtual ~AnimTestState() = default;
	AnimTestState &operator=(AnimTestState& rhs) = delete;
	AnimTestState(AnimTestState& rhs) = delete;


	AnimTestBase * testBase;
	AnimTestObject* testob;
	AnimTestObject* testob2;
	Bars* testbar;
public:
	static AnimTestState * GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	static void MessageProc(const SGD::Message * mess);
	//AnimationSystem *as;
	//AnimationSystem *as2;
	//Animation* x;
	//Animation* z;
	//Animation* x2;
	//Animation* z2;

	SGD::Point pos1;
	SGD::Point pos2;
	SGD::Point pos3;
	float testVel = 0;
	float testVel2 = 0;
	float testvel3 = 0;
};

