
#include "CommonEnemy.h"
#include "TurnManager.h"


CommonEnemy::CommonEnemy()
	{
	}


CommonEnemy::~CommonEnemy()
	{
	}

void  CommonEnemy::Update( float dt )
	{
	Enemy::Update( dt );
	}
void  CommonEnemy::Render()
	{
	Enemy::Render();
	}
void  CommonEnemy::BehaviorAI()
	{

	}

void CommonEnemy::CatAI()
	{

	}
void CommonEnemy::DogAI()
	{

	}
void CommonEnemy::RavenAI()
	{

	}
void CommonEnemy::ChefAI()
	{

	}
void CommonEnemy::BlacksmithAI()
	{

	}
void CommonEnemy::ShopKeeperAI()
	{

	}
void CommonEnemy::TailorAI()
	{

	}
void CommonEnemy::PriestAI()
	{

	}
void CommonEnemy::CecilAI( int phase )
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
void CommonEnemy::CecilPhaseOne()
	{
	if ( progress >= 100.0f )
		{
		float tenthHealth = HP / (float)( GetMaxHP() );
		bool below30 = false;
		int target;
		if ( tenthHealth > 0.1f )
			{
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->GetHP() / TurnManager::GetInstance()->GetAllies()[i]->GetMaxHP() < 0.3f )
					{
					below30 = true;
					target = i;
					break;
					}
				}
			if ( !below30 )
				{
				target = rand() % TurnManager::GetInstance()->GetAllies().size();
				}
			int atk = TurnManager::GetInstance()->GetAllies()[target]->GetStats().attack;
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
			TurnManager::GetInstance()->GetEnemies()[2]->SetLiving( true );
			TurnManager::GetInstance()->GetEnemies()[2]->SetHP( TurnManager::GetInstance()->GetEnemies()[2]->GetMaxHP() );

			}
		}
	}
void CommonEnemy::CecilPhaseTwo()
	{

	}
void CommonEnemy::CecilPhaseThree()
	{

	}
void CommonEnemy::JaneAI()
	{

	}
void CommonEnemy::JohnAI()
	{

	}