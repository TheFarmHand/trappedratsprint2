#pragma once
#include <string>
#include "../Animation/AnimationSystem.h"

class Character;

// Elemental Type
enum ETYPE { PHYS, WIND, FIRE, EARTH, WATER, MULTI };
class Ability
{
private:
	std::string explination;
	std::string noBPexplination;
	std::string abilityName;
	std::string statusName;
	std::string animationPath;
	std::string wavSoundPath;
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
	int unlockLevel;
	bool unlocked;
	bool access;
	bool selfTarget;
	AnimationSystem* animate;
	Character* Abilowner;
	Character* Abiltarget;

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
	bool GetUnlocked();
	int GetUnlockLevel();
	bool GetAccess();
	bool GetSelfTarget();
	AnimationSystem* GetAnimate();
	Character* GetOwner();
	void SetExplination(std::string str){ explination = str; }
	void SetAbilityName( std::string name );
	void SetOffensive( bool offense );
	void SetHealing( bool heal );
	void SetFormulaTotal( float total );
	void SetStatusID( int id );
	void SetAtkDam( float atk );
	void SetMgcDam( float mgc );
	void SetUnlocked( bool unlock );
	void SetAccess( bool combatUse );
	void virtual Render( );
	void virtual Update( float dt );
	void CastAbility( Character* owner, Character* target, int AoeCounter = 0, bool ternary = false );
	void CalculateFormula( Character* owner, Character* target );
	void CalcluateBpScaledCost( Character* owner );

	void RenderAnimation();

	int baseBP;
};

