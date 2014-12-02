#pragma once
#include "OwnedHUD.h"
class Bars :
	public OwnedHUD
{
private:
	SGD::HTexture outlineimage = SGD::INVALID_HANDLE;
	float percentage = 1;
	SGD::Color color;
	bool is_HP;
public:
	Bars();
	Bars(SGD::Size sz, SGD::Point pos,Character * _owner);
	virtual ~Bars();

	void virtual Update(float dt);
	void virtual Render();
	void SetSize(float x, float y);
	void SetPercentage(float p);
	void SetColor(SGD::Color);
	float GetPercentage();

	void SetisHP(bool _is_HP);
	bool GetIsHP();
	
};

