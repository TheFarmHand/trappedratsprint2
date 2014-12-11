#pragma once
#include "OwnedHUD.h"
class Numbers :
	public OwnedHUD
{
private:
	int value;
	float lifespan;
	SGD::Color color;
	bool evaded;
public:
	Numbers();
	Numbers(int _value, SGD::Color _color, Character* _owner,SGD::Point _offset);
	Numbers(int _value, SGD::Color _color, Character* _owner, SGD::Point _offset, bool _evade);

	virtual ~Numbers();

	void virtual Update(float dt);
	void virtual Render();
};

