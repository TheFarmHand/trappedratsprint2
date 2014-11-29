#pragma once
#include <string>

class Character;

// Elemental Type
enum ETYPE { PHYS, WIND, FIRE, EARTH, WATER, MULTI };
class Ability
{
private:
	std::string explination;
	std::string abilityName;
	std::string statusName;
	int elementType;
	bool status;
	bool offensive;
	bool healing;
	bool areaOfEffect;
	float formulaTotal;
	int statusID;
	float atkMod, mgcMod;
	float hpMod;
	int bpCost;
	bool access;

public:
	Ability( const char* path );
	virtual ~Ability();

	std::string GetExplination(){ return explination; }
	std::string GetAbilityName();
	bool GetOffensive();
	bool GetHealing();
	float GetFormulaTotal();
	int GetStatusID();
	float GetAtkMod();
	float GetMgcMod();
	int GetBPCost();
	float GetHPMod();
	bool GetAOE();
	void SetExplination(std::string str){ explination = str; }
	void SetAbilityName( std::string name );
	void SetOffensive( bool offense );
	void SetHealing( bool heal );
	void SetFormulaTotal( float total );
	void SetStatusID( int id );
	void SetAtkDam( float atk );
	void SetMgcDam( float mgc );
	void virtual Render();
	void CastAbility(Character* owner, Character* target);
	void CalculateFormula( Character* owner, Character* target );
};

