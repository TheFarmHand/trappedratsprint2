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
	std::vector<Character*> GetAll()			{ return AllCombatUnits; }
	SGD::HTexture GetAllyArrow() { return allytargetArrow; }
	SGD::HTexture GetEnemyArrow() { return enemytargetArrow; }

	void SetArrow( SGD::HTexture allyarrow, SGD::HTexture enemyarrow ) { allytargetArrow = allyarrow; enemytargetArrow = enemyarrow; }

	void HealTarget(Character* target, int value);
	int ElementalMod( Character* target, int damage, ETYPE element );
	void AttackTarget(Character* owner, Character* target, int value);
	void UsingAbility( Character* owner, Character* attacker, Ability* ability, bool ternary = false );
private:
	std::vector<Character*> AllCombatUnits;
	std::vector<Character*> AlliedUnits;
	std::vector<Character*> EnemyUnits;

	SGD::HTexture allytargetArrow = SGD::INVALID_HANDLE;
	SGD::HTexture enemytargetArrow = SGD::INVALID_HANDLE;
	SGD::HTexture timelineImage = SGD::INVALID_HANDLE;
	SGD::HTexture elementalImage = SGD::INVALID_HANDLE;
	bool timeStop;
	bool fullProgressReached;
	bool turnPause;
	bool elementalgraphicactive;
	float pauseTime;

	bool testCover;
	

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

	void CombatLoot();
	void CombatXP();

	ParticleManager *pPartMan;

};

