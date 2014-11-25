

#pragma once
#include "BaseObject.h"
#include <string>
#include <vector>
#include <list>
#include "Stats.h"
#include "StatusEffect.h"
#include "Ability.h"



class Character :
	public BaseObject
{
protected:
	std::string name;
	Stats stats;
	int level;
	int HP;
	int MaxHP;
	float speed;
	float progress;
	int order;
	SGD::HAudio hurt = SGD::INVALID_HANDLE;
	SGD::HAudio death = SGD::INVALID_HANDLE;
	std::list<StatusEffect*> effects;
	std::vector<Ability*> abilityList;
	bool alive = true;
	
	
public:
	Character();
	virtual ~Character();


	void virtual Update(float dt);
	void virtual UpdateAnimation( float dt );
	void virtual Render();
	void virtual Attack( Character* owner, Character * target );
	
	void UseAbility();
	void virtual StatusTick();
	void virtual TakeDamage(int dmg, Character* target);
	void React();


	//accessors
	std::string GetName();
	Stats GetStats();
	int GetLevel();
	int GetHP();
	int GetMaxHP();
	float GetSpeed();
	float GetProgress();
	int GetOrderPosition();
	Ability* GetAbility(int index);
	bool virtual isAlive( );
	//std::vector<StatusEffect*>& GetEffects();
	std::list<StatusEffect*>& GetEffects();
	

	//mutators
	void SetName(std::string _name);
	void SetStats(Stats _stats);
	void SetLevel(int _level);
	void SetHP(int _hp);
	void SetMaxHP(int _max);
	void SetSpeed(float _speed);
	void SetProgress(float _progress);
	void SetAbility(int index, Ability* _ability);
	void SetOrderPosition( int index );
	void SetLiving( bool buul );
	void AddStatus(StatusEffect *status);
};

