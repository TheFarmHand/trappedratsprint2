#pragma once
#include "Character.h"
#include "CombatPlayer.h"
#include "Enemy.h"
#include <vector>
#include "StatusEffect.h"

class ParticleManager;

class TurnManager
{
public:
	static TurnManager* GetInstance();
	void Initialize( std::vector<CombatPlayer*> playerParty, std::vector<Enemy*> enemyParty );
	void Update( float dt );
	void Render();
	void Terminate();

	void setTimeStop( bool stop );
	bool getTimeStop();
	void setProgressFullReached( bool reached );
	bool getProgressFullReached();
	void setTurnPause( bool pause );
	bool getTurnPause();

	std::vector<Character*> GetEnemies()		{ return EnemyUnits; }
	std::vector<Character*> GetAllies()			{ return AlliedUnits; }
	SGD::HTexture GetArrow()					{ return targetArrow; }
	
	void SetArrow(SGD::HTexture arrow)		{ targetArrow = arrow; }

	void HealTarget(Character* target, int value);
	void AttackTarget(Character* owner, Character* target, int value);

private:
	std::vector<Character*> AllCombatUnits;
	std::vector<Character*> AlliedUnits;
	std::vector<Character*> EnemyUnits;

	SGD::HTexture targetArrow = SGD::INVALID_HANDLE;
	bool timeStop;
	bool fullProgressReached;
	bool turnPause;
	float pauseTime;

	TurnManager() = default;
	virtual ~TurnManager() = default;
	TurnManager( TurnManager& rhs ) = delete;
	TurnManager& operator=( TurnManager& rhs ) = delete;
		
	friend bool sortByProgress( Character* a, Character* b );
	void SetupAllyParty( std::vector<CombatPlayer*> playerParty );
	void SetupEnemyParty( std::vector<Enemy*> enemyParty );
	bool CheckWin();
	bool CheckLose();
	void EndCombat();
	void CombatUpdate(float dt);
	void AnimationUpdate( float dt );
	void ParticleUpdate( float dt );

	ParticleManager *pPartMan;

	// ****** HACKY CODE FOR STATUS EFFECT FRAMEWORK ****** //
	StatusEffect FireFang;
	void SetupFireFang();
	StatusEffect& GetFireFang() { return FireFang; }
};

