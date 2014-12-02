#pragma once
#include <string>
#include "Definitions.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
//#include "../Core/Character.h"	// Doing this has circular inclusion

class Character;
//enum ETYPE { PHYS, WIND, FIRE, EARTH, WATER, MULTI };
enum ETYPE;

class StatusEffect
{
private:
	std::string name;
	ETYPE element;
	SGD::HTexture icon;
	int num_ticks;
	int curr_tick = 0;
	int dmg_tick;
	Character* owner;
	enum STYPE { DOT, STAT, SPECIAL };
	STYPE myType;
	float stat_value = 0;

	void virtual HandleDOT( );
	void virtual HandleSpecial( );
	void virtual HandleStat( );
	int virtual ElementalMod( );
	

public:
	StatusEffect();
	virtual ~StatusEffect();
	void virtual Initialize( );
	void virtual React(Character* attacker, ETYPE ele);
	void virtual Turntick();
	void virtual Clear();
	void virtual Recover();	// For Statup/down

	// Access Granted
	virtual Character* GetOwner();
	virtual std::string GetName();
	virtual ETYPE GetElement();
	virtual SGD::HTexture GetIcon();
	virtual int GetCurrTick();
	virtual int GetNumTicks();
	virtual int GetTickDmg()	{ return dmg_tick; }


	// Mutates
	void virtual SetOwner(Character* own);
	void virtual SetName(std::string name);
	void virtual SetElement( ETYPE ele );
	void virtual SetIcon(SGD::HTexture ico);
	void virtual SetTick(int tic);
	void virtual SetTickDmg(int dmg)	{ dmg_tick = dmg; }
	void virtual SetType(int type);

	StatusEffect& operator=(const StatusEffect& rhs);
};

