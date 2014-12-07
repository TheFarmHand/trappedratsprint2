
#include "Enemy.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "TurnManager.h"
#include <sstream>
#include "../Font/Font.h"
#include "GameData.h"


Enemy::Enemy()
	{
	}


Enemy::~Enemy()
	{
	if ( ansys != nullptr )
		delete ansys;
	}
void Enemy::Update( float dt )
	{

	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
		{
		progress += speed * dt;
		}

	else if ( progress < 100.0f )
		{
		if ( stepbackward == true && stepforward == false )
			{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x += stepvelocity*dt;
			SetPosition( pos );
			if ( stepTime <= 0.0f )
				stepbackward = false;
			}
		if ( stepbackward == false && stepforward == false )
			{
			TurnManager::GetInstance()->setProgressFullReached( false );
			TurnManager::GetInstance()->setTimeStop( false );
			}
		return;
		}

	if ( progress >= 100.0f )
		{
		if ( !TurnManager::GetInstance()->getProgressFullReached() )
			{
			StatusTick();

			if ( !alive )
				{
				progress = 0.0f;
				return;
				}
			TurnManager::GetInstance()->setProgressFullReached( true );
			TurnManager::GetInstance()->setTimeStop( true );
			return;
			}

		if ( HasEffect( "Stun" ) )
			{
			// Lose your turn if stunned
			progress = 0.0f;
			return;
			}



		if ( stepforward == false && stepbackward == false )
			{
			stepforward = true;
			stepTime = 2.0f;
			}
		if ( stepforward == true && stepbackward == false )
			{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x -= stepvelocity*dt;
			SetPosition( pos );
			if ( stepTime <= 0.0f )
				stepforward = false;
			}

		if ( stepbackward == false && stepforward == false )
			{
			// Enemy Select Player
			if ( name == "Cecil" )
				CecilAI( CecilPhase );
			else if ( name == "Jane" )
				JaneAI();
			else if ( name == "John" )
				JohnAI();
			else
				{
				if ( HasEffect( "Confused" ) )
					{

					// Do Confused Action (attack random target, anyone)
					int targets = TurnManager::GetInstance()->GetAll().size();
					int hit_this_guy = rand() % targets;

					Attack( this, TurnManager::GetInstance()->GetAll()[hit_this_guy] );

					if ( stepbackward == false && stepforward == false && progress != 0.0f )
						{
						stepbackward = true;
						stepTime = 2.0f;
						progress = 0.0f;
						}
					progress = 0.0f;
					TurnManager::GetInstance()->setProgressFullReached( false );
					TurnManager::GetInstance()->setTurnPause( true );
					}

				else
					{
					int pool = TurnManager::GetInstance()->GetAllies().size();
					std::vector<int> living;

					for ( int i = 0; i < pool; i++ )
						{
						if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
							living.push_back( i );
						}

					if ( living.size() > 0 )
						pool = rand() % ( living.size() );
					else
						pool = -1;


					//if ( SGD::InputManager::GetInstance( )->IsKeyPressed( SGD::Key::Enter ) )
					if ( pool == -1 ) // Hack job to prevent enemies from murdering dead players/breaking code
						{
						progress = 0.0f;
						TurnManager::GetInstance()->setTimeStop( false );
						return;
						}

					Attack( this, TurnManager::GetInstance()->GetAllies()[living[pool]] );
					}
				}
			if ( stepbackward == false && stepforward == false && progress != 0.0f )
				{
				stepbackward = true;
				stepTime = 2.0f;
				progress = 0.0f;
				}
			//Attack( this, TurnManager::GetInstance()->GetAllies()[living[pool]] );
			progress = 0.0f;
			TurnManager::GetInstance()->setProgressFullReached( false );
			TurnManager::GetInstance()->setTurnPause( true );
			}
		}
	}
void Enemy::UpdateAnimation( float dt )
	{
	if ( ansys != nullptr )
		{
		ansys->Update( dt );
		}
	Character::Update( dt );
	}
void Enemy::Render()
	{
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 51, position.y - 30, { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 50, position.y - 30, { 200, 0, 0 }, 1.6f );
	std::ostringstream sp;
	sp << "Progress: " << progress;
	GameData::GetInstance()->GetFont()->DrawString( sp.str(), position.x + 50, position.y - 10, { 255, 0, 0 } );
	std::ostringstream sa;
	sa << "Attack: " << GetAttack();
	GameData::GetInstance()->GetFont()->DrawString( sa.str(), position.x + 50, position.y + 5, { 0, 0, 0 } );
	std::ostringstream sd;
	sd << "Defense: " << GetDefense();
	GameData::GetInstance()->GetFont()->DrawString( sd.str(), position.x + 50, position.y + 15, { 0, 0, 0 } );
	std::ostringstream sm;
	sm << "Magic: " << GetMagic();
	GameData::GetInstance()->GetFont()->DrawString( sm.str(), position.x + 50, position.y + 25, { 0, 0, 0 } );
	std::ostringstream sav;
	sav << "Avoision: " << GetAvoision();
	GameData::GetInstance()->GetFont()->DrawString( sav.str(), position.x + 50, position.y + 35, { 0, 0, 0 } );
	std::ostringstream ss;
	ss << "Speed: " << GetSpeed();
	GameData::GetInstance()->GetFont()->DrawString( ss.str(), position.x + 50, position.y + 45, { 0, 0, 0 } );

	if ( ansys != nullptr )
		{
		ansys->Render( position.x, position.y );
		}


	Character::Render();
	}
void Enemy::BehaviorAI()
	{

	}

void Enemy::SetAnimations( AnimationSystem* _an )
	{
	if ( ansys != nullptr )
		delete ansys;
	ansys = _an;
	}
AnimationSystem* Enemy::GetAnimations()
	{
	return ansys;
	}
int Enemy::GetType()
	{
	return type::Enemy;
	}


int Enemy::GetXPValue()
	{
	return xp_value;
	}

void Enemy::SetLevel( int lev )
	{
	level = lev;
	}

void Enemy::SetXPVal( int val )
	{
	xp_value = val;
	}

//Cecil Fight
void Enemy::CecilAI( int phase )
	{
	switch ( phase )
		{
		case 1:
			CecilPhaseOne();
			break;
		case 2:
			CecilPhaseTwo();
			break;
		case 3:
			CecilPhaseThree();
			break;
		}
	}
void Enemy::CecilPhaseOne()
	{
	if ( progress >= 100.0f )
		{
		TurnManager::GetInstance()->GetEnemies()[0]->JaneDead = false;
		TurnManager::GetInstance()->GetEnemies()[2]->JohnDead = false;
		if ( progress >= 100.0f )
			{
			float tenthHealth = HP / (float)( GetMaxHP() );
			bool below30 = false;
			int target;
			if ( tenthHealth > 0.1f )
				{
				for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
					{
					if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() ) < 0.3f
						 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
						{
						below30 = true;
						target = i;
						break;
						}
					}
				if ( !below30 )
					{
					target = rand() % TurnManager::GetInstance()->GetAllies().size();
					while ( !TurnManager::GetInstance()->GetEnemies()[target]->isAlive() )
						{
						target = rand() % TurnManager::GetInstance()->GetAllies().size();
						}
					}
				int atk = GetStats().attack;
				int dmg = rand() % atk + atk;
				dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
				if ( dmg <= 0 )
					dmg = 0;
				TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
				}
			else
				{
				CecilPhase = 2;
				SetHP( GetMaxHP() + 100 );
				stats.attack += 5;
				stats.avoision += 5;
				stats.defense += 10;
				stats.magic += 5;

				TurnManager::GetInstance()->GetEnemies()[0]->SetLiving( true );
				TurnManager::GetInstance()->GetEnemies()[0]->SetHP( TurnManager::GetInstance()->GetEnemies()[0]->GetMaxHP() );
				TurnManager::GetInstance()->GetEnemies()[0]->SetProgress( 0.0f );
				TurnManager::GetInstance()->GetEnemies()[2]->SetLiving( true );
				TurnManager::GetInstance()->GetEnemies()[2]->SetHP( TurnManager::GetInstance()->GetEnemies()[2]->GetMaxHP() );
				TurnManager::GetInstance()->GetEnemies()[2]->SetProgress( 0.0f );
				}
			}
		}
	}
void Enemy::CecilPhaseTwo()
	{
	int tempAtk;
	if ( TurnManager::GetInstance()->GetEnemies()[0]->JaneDead )
		{
		TurnManager::GetInstance()->GetEnemies()[0]->JaneDead = false;
		if ( !TurnManager::GetInstance()->GetEnemies()[0]->isAlive() && !TurnManager::GetInstance()->GetEnemies()[2]->isAlive() )
			{
			CecilPhase = 3;
			phase3 = true;
			//Cast Super Retribution(wipes companions, weakens ratsputin
			//Wipe Dodging
			//Wipe Guarding
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->GetName() == "Ratsputin" )
					{
					tempAtk = GetAttack();
					GetStats().attack = 2;
					}
				else
					{
					tempAtk = GetAttack();
					GetStats().attack = 9999;
					}
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
					}
				GetStats().attack = tempAtk;
				}
			}
		else
			//Cast Retribution
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
				}
		return;
		}
	if ( TurnManager::GetInstance()->GetEnemies()[2]->JohnDead )
		{
		TurnManager::GetInstance()->GetEnemies()[2]->JohnDead = false;
		//Cast Retribution
		if ( !TurnManager::GetInstance()->GetEnemies()[0]->isAlive() && !TurnManager::GetInstance()->GetEnemies()[2]->isAlive() )
			{
			CecilPhase = 3;
			phase3 = true;
			//Cast Super Retribution(wipes companions, weakens ratsputin
			//Wipe Dodging
			//Wipe Guarding
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->GetName() == "Ratsputin" )
					{
					tempAtk = GetAttack();
					GetStats().attack = 2;
					}
				else
					{
					tempAtk = GetAttack();
					GetStats().attack = 9999;
					}
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
					}
				GetStats().attack = tempAtk;
				}
			}
		else
			//Cast Retribution
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
				}
		return;
		}
	if ( TurnManager::GetInstance()->GetEnemies()[0]->JaneHit % 3 == 0 )
		{
		//cover code of Jane
		abilityList[4]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[0] );
		return;
		}
	else if ( TurnManager::GetInstance()->GetEnemies()[0]->JohnHit % 3 == 0 )
		{
		//cover code of John
		abilityList[4]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[2] );
		return;
		}
	else
		{
		if ( !HolyMace )
			{
			HolyMace = true;
			HolyShield = false;
			//cast Holy Mace
			abilityList[1]->CastAbility( this, this );
			return;
			}
		else
			{
			HolyMace = false;
			HolyShield = true;
			//cast Holy Shield
			abilityList[0]->CastAbility( this, this );
			return;
			}
		}
	}
void Enemy::CecilPhaseThree()
	{
	if ( !HolyFlare )
		{
		HolyFlare = true;
		HFCountdown = 10;
		damageDealt = 0.0f;
		//Dialogue for casting/countdown
		}
	if ( HFCountdown > 0 )
		{
		--HFCountdown;
		//Dialogue for countdown
		return;
		}
	else
		{
		if ( damageDealt >= 400 )
			{
			//Cast weak Holy Flare
			TakeDamage( 9999 );
			}
		else
			{
			//Cast strong Holy Flare
			TakeDamage( 1 );
			}
		}

	}
void Enemy::JaneAI()
	{
	int target;
	if ( !HasEffect( "DefenseUp" ) )
		{
		//cast protect
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
			{
			abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i], i );
			}
		}
	else
		{
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetEnemies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetEnemies()[i]->GetMaxHP() ) < 0.6f
				 && TurnManager::GetInstance()->GetEnemies()[i]->isAlive() )
				{
				//cast healing light
				abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i] );
				return;
				}
			}
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		while ( !TurnManager::GetInstance()->GetEnemies()[target]->isAlive() )
			{
			target = rand() % TurnManager::GetInstance()->GetAllies().size();
			}
		if ( rand() % 10 > 8 )
			{
			//cast Dia
			abilityList[1]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[target] );
			}
		else
			{
			int atk = TurnManager::GetInstance()->GetAllies()[target]->GetStats().attack;
			int dmg = rand() % atk + atk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
			}
		}
	}
void Enemy::JohnAI()
	{
	int target;
	int lowestHP = TurnManager::GetInstance()->GetAllies()[0]->GetHP();
	float ratPartyAvgHealth = 0.0f;

	if ( !HasEffect( "SpeedUp" ) )
		{
		//cast Haste
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
			{
			abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i], i );
			}
		}
	else
		{
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() ) < 0.5f
				 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				{
				//cast Sure Shot
				abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
				return;
				}
			}
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() < lowestHP
				 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				{
				lowestHP = TurnManager::GetInstance()->GetAllies()[i]->GetHP();
				target = i;
				}
			ratPartyAvgHealth += TurnManager::GetInstance()->GetAllies()[i]->GetHP();
			}
		ratPartyAvgHealth /= TurnManager::GetInstance()->GetAllies().size();

		if ( ratPartyAvgHealth > 0.75f )
			{
			//cast Barrage
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					abilityList[1]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
				}
			return;
			}
		else
			{
			int atk = TurnManager::GetInstance()->GetAllies()[target]->GetStats().attack;
			int dmg = rand() % atk + atk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
			}
		}
	}