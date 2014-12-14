
#include "Enemy.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "TurnManager.h"
#include <sstream>
#include "../Font/Font.h"
#include "GameData.h"
#include "../States/GamePlayState.h"


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
	if ( !alive )
		return;

	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
		{
		progress += GetSpeed() * dt;
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
			stepTime = .50f;
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
			if ( HasEffect( "Confused" ) )
				{

				// Do Confused Action (attack random target, anyone)
				int targets = TurnManager::GetInstance()->GetAll().size();
				int hit_this_guy = rand() % targets;

				Attack( this, TurnManager::GetInstance()->GetAll()[hit_this_guy] );

				if ( stepbackward == false && stepforward == false && progress != 0.0f )
					{
					stepbackward = true;
					stepTime = .50f;
					progress = 0.0f;
					}
				progress = 0.0f;
				TurnManager::GetInstance()->setProgressFullReached( false );
				TurnManager::GetInstance()->setTurnPause( true );
				}
			else if ( name == "Cat" )
				CatAI();
			else if ( name == "Raven" )
				RavenAI();
			else if ( name == "Dog" )
				DogAI();
			else if ( name == "Chef" )
				ChefAI();
			else if ( name == "Blacksmith" )
				BlacksmithAI();
			else if ( name == "Shopkeeper" )
				ShopkeeperAI();
			else if ( name == "Tailor" )
				TailorAI();
			else if ( name == "Priestess" )
				PriestAI();
			else if ( name == "Guard" )
				GuardAI();
			else if ( name == "Benevolent Wind Rogue" )
				BWRAI();
			else if ( name == "Ferocious Fire Warrior" )
				FFWAI();
			else if ( name == "Sturdy Earth Monk" )
				SEMAI();
			else if ( name == "Wise Water Wizard" )
				WWWAI();
			else if ( name == "Cecil" )
				CecilAI( CecilPhase );
			else if ( name == "Jane" )
				JaneAI();
			else if ( name == "John" )
				JohnAI();
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

				int atk = GetAttack();
				int dmg = rand() % atk + atk;
				dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[living[pool]]->GetDefense() );
				if ( dmg <= 0 )
					dmg = 0;
				TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[living[pool]], dmg );
				}

			if ( stepbackward == false && stepforward == false && progress != 0.0f )
				{
				stepbackward = true;
				stepTime = .50f;
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
	/*GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 51, position.y - 30, { 0, 0, 0 }, 1.6f );
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
	std::ostringstream se;
	if ( GetEType() == WIND )
		se << "Element: Wind";
	if ( GetEType() == FIRE )
		se << "Element: Fire";
	if ( GetEType() == EARTH )
		se << "Element: Earth";
	if ( GetEType() == WATER )
		se << "Element: Water";
	GameData::GetInstance()->GetFont()->DrawString( se.str(), position.x + 50, position.y + 55, { 0, 0, 0 } );*/

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
	return type::Enemies;
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

//Common Enemy
void Enemy::CatAI()
	{
	int lowestHp = 0;
	int tempLow;
	int target = 0;
	int tempAtk = (int)( GetAttack() * 1.3f );
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			tempLow = TurnManager::GetInstance()->GetAllies()[i]->GetHP();
			if ( lowestHp == 0 )
				lowestHp = tempLow;
			if( tempLow < lowestHp )
				{
				lowestHp = tempLow;
				target = i;
				}
			}
		}
	if ( lowestHp / (float)(TurnManager::GetInstance()->GetAllies()[target]->GetMaxHP()) < 0.25f
		 && TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		int dmg = rand() % tempAtk + tempAtk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		}
	else if ( lowestHp / (float)(TurnManager::GetInstance()->GetAllies()[target]->GetMaxHP()) < 0.65f
			  && TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[target] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		}
	else if ( TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		int atk = GetAttack();
		int dmg = rand() % atk + atk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		}
	}
void Enemy::RavenAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int elementalTarget;
	if ( rand() % 5 > 3 || TurnManager::GetInstance()->GetAllies().size() < 2 )
		{
		int atk = GetAttack();
		int dmg = rand() % atk + atk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		}
	else
		{
		if ( GamePlayState::GetInstance()->GetTownSelected() == 0 )
			elementalTarget = 1;
		else if ( GamePlayState::GetInstance()->GetTownSelected() == 1 )
			elementalTarget = 2;
		else if ( GamePlayState::GetInstance()->GetTownSelected() == 2 )
			elementalTarget = 3;
		else if ( GamePlayState::GetInstance()->GetTownSelected() == 3 )
			elementalTarget = 0;
		else
			elementalTarget = rand() % 4;

		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( elementalTarget == TurnManager::GetInstance()->GetAllies()[i]->GetEType() &&
				 TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				{
				abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
				GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				return;
				}
			}
		abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[target] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		}
	}
void Enemy::DogAI()
	{
	bool pack = false;
	int packMate;
	float enemyPartyAvg = 0.0f;
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetEnemies()[i]->GetName() == "Dog" &&
			 i != GetOrderPosition() )
			{
			pack = true;
			packMate = i;
			break;
			}
		}
	if ( pack )
		{
		if ( TurnManager::GetInstance()->GetEnemies()[packMate]->dogTarget >= 0 )
			{
			dogTarget = TurnManager::GetInstance()->GetEnemies()[packMate]->dogTarget;
			if ( !TurnManager::GetInstance()->GetAllies()[dogTarget]->isAlive() )
				{
				dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
				while ( !TurnManager::GetInstance()->GetAllies()[dogTarget]->isAlive() )
					{
					dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
					}
				}
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					enemyPartyAvg += TurnManager::GetInstance()->GetAllies()[i]->GetHP();
				}
			enemyPartyAvg /= TurnManager::GetInstance()->GetAllies().size();

			if ( enemyPartyAvg > 0.7f )
				{
				abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[dogTarget] );
				GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				}
			else
				{
				int atk = GetAttack();
				int dmg = rand() % atk + atk;
				dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[dogTarget]->GetDefense() );
				if ( dmg <= 0 )
					dmg = 0;
				TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[dogTarget], dmg );
				}
			}
		}
	else if ( dogTarget >= 0 )
		{
		if ( !TurnManager::GetInstance()->GetAllies()[dogTarget]->isAlive() )
			{
			dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
			while ( !TurnManager::GetInstance()->GetAllies()[dogTarget]->isAlive() )
				{
				dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
				}
			}
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				enemyPartyAvg += TurnManager::GetInstance()->GetAllies()[i]->GetHP();
			}
		enemyPartyAvg /= TurnManager::GetInstance()->GetAllies().size();

		if ( enemyPartyAvg > 0.7f )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[dogTarget] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			}
		else
			{
			int atk = GetAttack();
			int dmg = rand() % atk + atk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[dogTarget]->GetDefense() );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[dogTarget], dmg );
			}
		}
	/*else
		{
		dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
		while ( !TurnManager::GetInstance()->GetAllies()[dogTarget]->isAlive() )
			{
			dogTarget = rand() % TurnManager::GetInstance()->GetAllies().size();
			}
		int atk = GetAttack();
		int dmg = rand() % atk + atk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[dogTarget]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[dogTarget], dmg );
		}*/
	}
void Enemy::ChefAI()
	{
	float healthRange;
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int tempAtk = (int)( GetAttack() * 1.25f );
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() ) < 0.75f &&
			 TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		}
	healthRange = GetHP() / (float)( GetMaxHP() );
	if ( healthRange < 0.75f && healthRange > 0.25f )
		{
		int dmg = rand() % tempAtk + tempAtk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		}
	else
		{
		int dmg = rand() % GetAttack() + GetAttack();
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		}
	}
void Enemy::BlacksmithAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int tempAtk = (int)( GetAttack() * 1.25f );

	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetDefense() > GetDefense() &&
			 TurnManager::GetInstance()->GetAllies()[i]->isAlive() &&
			 !TurnManager::GetInstance()->GetAllies()[i]->HasEffect("DefenseDown") )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		else if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() ) < 0.25f &&
				  TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			int dmg = rand() % tempAtk + tempAtk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[i]->GetStats().defense );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[i], dmg );
			return;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
void Enemy::ShopkeeperAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	if ( lastAttacker >= 0 && TurnManager::GetInstance()->GetAllies()[lastAttacker]->isAlive() )
		{
		target = lastAttacker;
		}
	int tempAtk = (int)( GetAttack() * 1.25f );
	if ( GetHP() / (float)( GetMaxHP() ) > 0.5f && !TurnManager::GetInstance()->GetAllies()[target]->HasEffect("Poison"))
		{
		abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[target] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		return;
		}
	if ( ( rand() % 20 ) % 3 == 0 )
		{
		int dmg = rand() % tempAtk + tempAtk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		return;
		}
	else
		{
		int dmg = rand() % GetAttack() + GetAttack();
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		return;
		}
	}
void Enemy::TailorAI()
	{
	int target;
	float healthRange;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int tempAtk = (int)( GetAttack() * 1.25f );

	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		healthRange = TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() );
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetSpeed() > GetSpeed() &&
			 TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		else if ( healthRange < 0.75f && healthRange > 0.25f &&
				  TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			int dmg = rand() % tempAtk + tempAtk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[i]->GetStats().defense );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[i], dmg );
			return;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
void Enemy::PriestAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetEnemies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetEnemies()[i]->GetMaxHP() ) < 0.6f
			 && TurnManager::GetInstance()->GetEnemies()[i]->isAlive() )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() ) < 0.5f
			 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
//Mini Boss
void Enemy::GuardAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int guardTally = 0;
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
		{
		if ( !TurnManager::GetInstance()->GetEnemies()[i]->isAlive() )
			++guardTally;
		}
	if ( guardTally == 1 )
		{
		abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[target] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		return;
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetAttack() > GetAttack()
			 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			target = i;
			break;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
void Enemy::BWRAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	int lowestHealth = TurnManager::GetInstance()->GetAllies()[0]->GetHP();
	if ( GetHP() / (float)( GetMaxHP() ) < 0.5f &&
		 TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		if ( !WRsplit )
			{
			abilityList[0]->CastAbility( this, this );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetSpeed() > GetSpeed()
			 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			target = i;
			break;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
void Enemy::FFWAI()
	{
	int target;
	std::ostringstream usingAbility;

	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	if ( GetHP() / (float)( GetMaxHP() ) < 0.55f )
		{
		if ( !FlameSpout )
			{
			FlameSpout = true;
			usingAbility << name << " starts casting Flame Spout";
			GamePlayState::GetInstance()->CurrentAbilityUsed = GamePlayState::GetInstance()->GetMasterList()["Flame Spout"];
			GamePlayState::GetInstance()->AbilityUsed = true;
			GamePlayState::GetInstance()->abilityTimer = 2.0f;
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			FSCountdown = 4;
			}
		else if ( FSCountdown > 1 )
			{
			--FSCountdown;
			usingAbility << FSCountdown << "...";
			GamePlayState::GetInstance()->CurrentAbilityUsed = GamePlayState::GetInstance()->GetMasterList()["Flame Spout"];
			GamePlayState::GetInstance()->AbilityUsed = true;
			GamePlayState::GetInstance()->abilityTimer = 2.0f;
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			return;
			}
		else
			{
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					abilityList[0]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
				}
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			FlameSpout = false;
			}
		}
	else
		{
		int dmg = rand() % GetAttack() + GetAttack();
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
		return;
		}
	}
void Enemy::SEMAI()
	{
	int target;
	target = rand() % TurnManager::GetInstance()->GetAllies().size();
	while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
		{
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / (float)( TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() )
			 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			target = i;
			break;
			}
		}
	if ( !EarthShield )
		{
		EarthShield = true;
		EarthWard = false;
		abilityList[0]->CastAbility( this, this );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		}
	else
		{
		EarthShield = false;
		EarthWard = true;
		abilityList[1]->CastAbility( this, this );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[1] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
void Enemy::WWWAI()
	{
	int target = 0;
	int lowestHp = 0;
	int tempLow;
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			tempLow = TurnManager::GetInstance()->GetAllies()[i]->GetHP();
			if ( lowestHp == 0 )
				lowestHp = tempLow;
			if ( tempLow < lowestHp )
				{
				lowestHp = tempLow;
				target = i;
				}
			}
		}
	float health = GetHP() / (float)( GetMaxHP() );
	if ( health < 0.25f && !firstBelowQuarter && !HasEffect("WaterWall"))
		{
		abilityList[0]->CastAbility( this, this );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		firstBelowQuarter = true;
		return;
		}
	else if ( health < 0.5f && !firstBelowHalf && !HasEffect( "WaterWall" ) )
		{
		abilityList[0]->CastAbility( this, this );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		firstBelowHalf = true;
		return;
		}
	else if ( health < 0.75f && !firstBelow3Quarter && !HasEffect( "WaterWall" ) )
		{
		abilityList[0]->CastAbility( this, this );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		firstBelow3Quarter = true;
		return;
		}
	for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
		{
		if ( TurnManager::GetInstance()->GetAllies()[i]->GetMagic() > GetMagic()
			 && TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
			{
			target = i;
			break;
			}
		}
	int dmg = rand() % GetAttack() + GetAttack();
	dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetDefense() );
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
	return;
	}
//Final Boss
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
					while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
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
				SetHP( GetMaxHP() + 200 );
				stats.attack += 5;
				stats.avoision += 5;
				stats.defense += 10;
				stats.magic += 5;

				TurnManager::GetInstance()->GetEnemies()[0]->SetLiving( true );
				TurnManager::GetInstance()->GetEnemies()[0]->SetHP( TurnManager::GetInstance()->GetEnemies()[0]->GetMaxHP() );
				TurnManager::GetInstance()->GetEnemies()[0]->SetProgress( 0.0f );
				TurnManager::GetInstance()->GetEnemies()[0]->SetPosition( SGD::Point( 600.0f, (float)( TurnManager::GetInstance()->GetEnemies()[0]->GetOrderPosition() * 100 + 150 + 16 ) ) );
				TurnManager::GetInstance()->GetEnemies()[2]->SetLiving( true );
				TurnManager::GetInstance()->GetEnemies()[2]->SetHP( TurnManager::GetInstance()->GetEnemies()[2]->GetMaxHP() );
				TurnManager::GetInstance()->GetEnemies()[2]->SetProgress( 0.0f );
				TurnManager::GetInstance()->GetEnemies()[2]->SetPosition( SGD::Point( 600.0f, (float)( TurnManager::GetInstance()->GetEnemies()[2]->GetOrderPosition() * 100 + 150 + 16 ) ) );
				}
			}
		}
	}
void Enemy::CecilPhaseTwo()
	{
	int tempAtk;
	int target;
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
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[3] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			}
		else
			//Cast Retribution
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
					}
				}
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[3] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[3] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			}
		else
			//Cast Retribution
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Guarding" );
				TurnManager::GetInstance()->GetAllies()[i]->RemoveEffect( "Dodging" );
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					abilityList[3]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
					}
				}
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[3] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		return;
		}
	if ( TurnManager::GetInstance()->GetEnemies()[0]->JaneHit % 3 == 0 )
		{
		//cover code of Jane
		TurnManager::GetInstance()->GetEnemies()[0]->JaneHit += 1;
		abilityList[4]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[0] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[4] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		return;
		}
	else if ( TurnManager::GetInstance()->GetEnemies()[0]->JohnHit % 3 == 0 )
		{
		//cover code of John
		TurnManager::GetInstance()->GetEnemies()[0]->JohnHit += 1;
		abilityList[4]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[2] );
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[4] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[1] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			target = rand() % TurnManager::GetInstance()->GetAllies().size();
			while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
				{
				target = rand() % TurnManager::GetInstance()->GetAllies().size();
				}
		int atk = GetStats().attack;
		int dmg = rand() % atk + atk;
		dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
			return;
			}
		else
			{
			HolyMace = false;
			HolyShield = true;
			//cast Holy Shield
			abilityList[0]->CastAbility( this, this );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
			target = rand() % TurnManager::GetInstance()->GetAllies().size();
			while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
				{
				target = rand() % TurnManager::GetInstance()->GetAllies().size();
				}
			int atk = GetStats().attack;
			int dmg = rand() % atk + atk;
			dmg -= (int)( 0.25f * TurnManager::GetInstance()->GetAllies()[target]->GetStats().defense );
			if ( dmg <= 0 )
				dmg = 0;
			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[target], dmg );
			return;
			}
		}
	}
void Enemy::CecilPhaseThree()
	{
	std::ostringstream usingAbility;
	if ( !HolyFlare )
		{
		HolyFlare = true;
		HFCountdown = 6;
		damageDealt = 0.0f;
		//Dialogue for casting/countdown
		usingAbility << name << " starts casting Holy Flare";
		GamePlayState::GetInstance()->AbilityUsed = true;
		GamePlayState::GetInstance()->abilityTimer = 2.0f;
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		}
	else if ( HFCountdown > 1 )
		{
		--HFCountdown;
		//Dialogue for countdown
		usingAbility << HFCountdown << "...";
		GamePlayState::GetInstance()->AbilityUsed = true;
		GamePlayState::GetInstance()->abilityTimer = 2.0f;
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
		return;
		}
	else
		{
		if ( damageDealt >= 400 )
			{
			//Cast weak Holy Flare
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				GetStats().magic = 0;
				if ( TurnManager::GetInstance()->GetAllies()[i]->GetName() == "Ratsputin" )
					{
					abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
					GamePlayState::GetInstance()->HoldOntoAbility( abilityList[2] );
					usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
					GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
					break;
					}
				}
			CecilPhase = -1;
			TakeDamage( 9999 );
			}
		else
			{
			//Cast strong Holy Flare
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->GetName() == "Ratsputin" )
					{
					abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i] );
					GamePlayState::GetInstance()->HoldOntoAbility( abilityList[2] );
					usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
					GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
					break;
					}
				}
			CecilPhase = -1;
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
			if ( TurnManager::GetInstance()->GetEnemies()[i]->isAlive() )
				abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i], i );
			}
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[2] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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
				GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				return;
				}
			}
		target = rand() % TurnManager::GetInstance()->GetAllies().size();
		while ( !TurnManager::GetInstance()->GetAllies()[target]->isAlive() )
			{
			target = rand() % TurnManager::GetInstance()->GetAllies().size();
			}
		if ( rand() % 10 > 8 )
			{
			//cast Dia
			abilityList[1]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[target] );
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[1] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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
	int lowestHp = 0;
	int tempLow;
	float ratPartyAvgHealth = 0.0f;

	if ( !HasEffect( "SpeedUp" ) )
		{
		//cast Haste
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetEnemies()[i]->isAlive())
				abilityList[2]->CastAbility( this, TurnManager::GetInstance()->GetEnemies()[i], i );
			}
		GamePlayState::GetInstance()->HoldOntoAbility( abilityList[2] );
		std::ostringstream usingAbility;
		usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
		GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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
				GamePlayState::GetInstance()->HoldOntoAbility( abilityList[0] );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				return;
				}
			}
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				{
				tempLow = TurnManager::GetInstance()->GetAllies()[i]->GetHP();
				if ( lowestHp == 0 )
					lowestHp = tempLow;
				if ( tempLow < lowestHp )
					{
					lowestHp = tempLow;
					target = i;
					}
				ratPartyAvgHealth += TurnManager::GetInstance()->GetAllies()[i]->GetHP();
				}
			}
		ratPartyAvgHealth /= TurnManager::GetInstance()->GetAllies().size();

		if ( ratPartyAvgHealth > 0.75f )
			{
			//cast Barrage
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					abilityList[1]->CastAbility( this, TurnManager::GetInstance()->GetAllies()[i], i );
					}
				}
			GamePlayState::GetInstance()->HoldOntoAbility( abilityList[1] );
			std::ostringstream usingAbility;
			usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
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