#pragma once
#include <string>
#include "Definitions.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Character;

enum Elements { Wind, Fire, Earth, Water, Multi, Physical };
class StatusEffect
{
private:
	std::string name;
	Elements element;
	SGD::HTexture icon;
	int num_ticks;
	int curr_tick = 0;
	int dmg_tick;
	Character* owner;

public:
	StatusEffect();
	virtual ~StatusEffect();

	void virtual React(Character* attacker);
	void virtual Turntick();
	void virtual Clear();

	// Access Granted
	virtual Character* GetOwner();
	virtual std::string GetName();
	virtual Elements GetElement();
	virtual SGD::HTexture GetIcon();
	virtual int GetCurrTick();
	virtual int GetNumTicks();
	virtual int GetTickDmg()	{ return dmg_tick; }


	// Mutates
	void virtual SetOwner(Character* own);
	void virtual SetName(std::string name);
	void virtual SetElement(Elements ele);
	void virtual SetIcon(SGD::HTexture ico);
	void virtual SetTick(int tic);
	void virtual SetTickDmg(int dmg)	{ dmg_tick = dmg; }

	StatusEffect& operator=(const StatusEffect& rhs);
};

