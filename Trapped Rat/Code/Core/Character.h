

#pragma once
#include "BaseObject.h"
#include <string>
#include <vector>
#include <list>
#include "Stats.h"
#include "StatusEffect.h"
#include "Ability.h"
#include "../Animation/AnimationSystem.h"
class Numbers;
class OwnedHUD;
class Character :
	public BaseObject
	{
	private:
		std::vector<Numbers*> damagenumbers;
	protected:
		std::string name;
		Stats stats;
		int level;
		int HP;
		int MaxHP;
		int BP;
		int MaxBP;
		float speed;
		float progress;
		int order;
		bool stepforward = false;
		bool stepbackward = false;
		float stepTime = .50f;
		float stepvelocity = 200.0f;
		SGD::HAudio hurt = SGD::INVALID_HANDLE;
		SGD::HAudio death = SGD::INVALID_HANDLE;
		std::list<StatusEffect*> effects;
		std::vector<Ability*> abilityList;
		bool alive = true;
		ETYPE element;
		std::string alliance;

		SGD::HTexture portrait = SGD::INVALID_HANDLE;
	//	SGD::HTexture timelineanimation = SGD::INVALID_HANDLE;
		AnimationSystem* timelineanimation = nullptr;

		OwnedHUD* TurnIndicator = nullptr;
	public:
		Character();
		virtual ~Character();


		void virtual Update( float dt );
		void virtual UpdateAnimation( float dt );
		void virtual Render();
		void virtual Attack( Character* owner, Character * target );
		void virtual InitializeAbilities( std::vector<Ability*> toSet );

		void virtual UseAbility();
		void virtual StatusTick();
		void virtual TakeDamage( int dmg, bool firefall = false );
		void virtual TakeDamage(int dmg, bool firefall, bool statusDot );
		void virtual TakeDamage(int dmg, bool firefall, bool statusDot, bool dodgeing);
		void React();
		void RemoveEffect( std::string effect );

		bool virtual HasEffect( std::string effect );
		void UpdateToLevel();

		//accessors
		std::string GetName();
		Stats& GetStats();
		int GetAttack();
		int GetDefense();
		int GetMagic();
		int GetAvoision();
		int GetLevel();
		int GetHP();
		int GetMaxHP();
		int GetBP();
		int GetMaxBP();
		float GetSpeed();
		float GetProgress();
		int GetOrderPosition();
		Ability* GetAbility( int index );
		int GetAbilitiesSize();
		bool virtual isAlive();
		ETYPE GetEType();
		std::string GetAlliance();
		void SetEtype( ETYPE _type );
		SGD::HTexture GetPortrait();
		AnimationSystem* GetTimelineAnimation();

		//std::vector<StatusEffect*>& GetEffects();
		std::list<StatusEffect*>& GetEffects();


		//mutators
		void SetName( std::string _name );
		void SetStats( Stats _stats );
		void SetLevel( int _level );
		void SetHP( int _hp );
		void SetMaxHP( int _max );
		void SetBP( int _bp );
		void SetMaxBP( int _max );
		void SetSpeed( float _speed );
		void SetProgress( float _progress );
		void SetAbility( int index, Ability* _ability );
		void SetOrderPosition( int index );
		void SetLiving( bool buul );
		void AddStatus( StatusEffect *status, Character* theOwner = nullptr, bool ternary = false );
		void SetTurnIndicator( OwnedHUD* _indicator );
		void SetPortrait( SGD::HTexture _port );
		void SetTimelineAnimation( AnimationSystem* _timeline );
		void SetAlliance( std::string type );
		int dogTarget = -1;
		int lastAttacker = -1;
		bool WRsplit = false;
		bool FlameSpout = false;
		int FSCountdown;

		//Cecil fight variables
		int CecilPhase = 1;
		bool phase3 = false;
		bool HolyFlare = false;
		int HFCountdown;
		float damageDealt;
		bool HolyMace = false;
		bool HolyShield = false;
		bool EarthShield = false;
		bool EarthWard = false;
		bool critHealth = false;
		bool JaneDead = false;
		bool JohnDead = false;
		int JaneHit = -2;
		int JohnHit = -2;
	};

