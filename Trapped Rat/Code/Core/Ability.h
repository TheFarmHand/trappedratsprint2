#pragma once
//#include "Character.h"
#include <string>
class Character;
class Ability
{
private:
	std::string explination;
	std::string abilityName;
	bool offensive;
	bool healing;
	float formulaTotal;
	int statusID;
	float atkDam, mgcDam;

public:
	Ability();
	virtual ~Ability();

	std::string GetExplination(){ return explination; }
	std::string GetAbilityName();
	bool GetOffensive();
	bool GetHealing();
	float GetFormulaTotal();
	int GetStatusID();
	float GetAtkDam();
	float GetMgcDam();
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

