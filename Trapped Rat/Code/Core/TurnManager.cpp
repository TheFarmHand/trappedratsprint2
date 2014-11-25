#include "TurnManager.h"
#include <algorithm>
#include "GameData.h"
#include "../Font/Font.h"
#include <sstream>
#include "../States/GamePlayState.h"
#include "../States/GameOverLoseState.h"
#include "../Particle/ParticleManager.h"

TurnManager* TurnManager::GetInstance()
	{
	static TurnManager data;

	return &data;
	}
void TurnManager::Initialize( std::vector<CombatPlayer*> playerParty, std::vector<Enemy*> enemyParty )
	{
	//*Do not add code to before or in this area, initializes the parties and necessary data members
	SetupAllyParty( playerParty );
	SetupEnemyParty( enemyParty );
	timeStop = false;
	fullProgressReached = false;
	//*

	//Safe to add code here

	// Particle Manager Initialization fo Turn Manager (Kinda hacky)
	//Change to singleton
	pPartMan = new ParticleManager();
	pPartMan->LoadEmitter( "../Trapped Rat/Assets/Scripts/bloodparticle.xml", "takedamage" );
	}
void TurnManager::Update( float dt )
	{
	//*Should be checked first each update, try not to add code here or before
	if ( CheckWin() )
		{
		// End of Combat, enemies have died
		EndCombat();
		return;
		}
	if ( CheckLose() )
		{
		EndCombat();
		GameData::GetInstance()->SwapState( GameOverLoseState::GetInstance() );
		return;
		}
	//*

	//*The core updates to take place during the overall update. Add what code you need to the appropriate section
	//can modify order if needed, should not break
	CombatUpdate(dt);
	AnimationUpdate(dt);
	ParticleUpdate( dt );
	//*
	
	pPartMan->Update( dt );
	}
void TurnManager::Render()
	{
	//*Anything added below will render underneath of combat character

	//*

	//*Do not insert any code here
	for ( unsigned int i = 0; i < AlliedUnits.size(); ++i )
		{
		AlliedUnits[i]->Render();
		}
	for ( unsigned int i = 0; i < EnemyUnits.size(); ++i )
		{
		EnemyUnits[i]->Render();
		}
	//*

	//*Anything added below will render on top of combat characters
	pPartMan->Render();
	//*
	}
void TurnManager::setTimeStop( bool stop )
	{
	timeStop = stop;
	}
bool TurnManager::getTimeStop()
	{
	return timeStop;
	}
void TurnManager::setProgressFullReached( bool reached )
	{
	fullProgressReached = reached;
	}
bool TurnManager::getProgressFullReached()
	{
	return fullProgressReached;
	}
void TurnManager::setTurnPause( bool pause )
	{
	turnPause = pause;
	}
bool TurnManager::getTurnPause()
	{
	return turnPause;
	}
void TurnManager::Terminate()
	{
	timeStop = false;
	AlliedUnits.clear();
	EnemyUnits.clear();
	AllCombatUnits.clear();

	// *** Hacky Stuf *** //
	if ( pPartMan != nullptr )
		{
		pPartMan->UnloadEmitter( "takedamage" );
		pPartMan->ClearAll();
		delete pPartMan; pPartMan = nullptr;
		}
	}
void TurnManager::HealTarget( Character* target, int value )
	{
	target->TakeDamage( -value, NULL );
	}
void TurnManager::AttackTarget( Character* owner, Character* target, int value )
	{
	target->TakeDamage( value, owner );
	// This call places the emitter at the proper location
	pPartMan->GetEmitter( "takedamage" )->SetPosition( target->GetPosition().x, target->GetPosition().y );
	// This call creates a new instance of the emitter
	pPartMan->CreateEmitter( "takedamage" );
	}
bool sortByProgress( Character* a, Character* b )
	{
	return a->GetProgress() > b->GetProgress();
	}
void TurnManager::SetupAllyParty( std::vector<CombatPlayer*> playerParty )
	{
	if ( playerParty.size() == 1 )
		{
		AlliedUnits.resize( 1 );
		}
	else if ( playerParty.size() == 2 )
		{
		AlliedUnits.resize( 2 );
		}
	else if ( playerParty.size() == 3 )
		{
		AlliedUnits.resize( 3 );
		}

	for ( unsigned int i = 0; i < playerParty.size(); ++i )
		{
		if ( playerParty[i]->GetActive() )
			{
			AlliedUnits[playerParty[i]->GetOrderPosition()] = playerParty[i];
			AllCombatUnits.push_back( playerParty[i] );
			dynamic_cast<CombatPlayer*>( AlliedUnits[i] )->Reset();
			}
		}
	}
void TurnManager::SetupEnemyParty( std::vector<Enemy*> enemyParty )
	{
	for ( unsigned int i = 0; i < enemyParty.size(); ++i )
		{
		EnemyUnits.push_back( enemyParty[i] );
		AllCombatUnits.push_back( enemyParty[i] );
		}
	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
		{
		AllCombatUnits[i]->SetProgress( 0.0f );
		}
	}
bool TurnManager::CheckWin()
	{
	bool enemiesDead = true;
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
		{
		if ( EnemyUnits[i]->isAlive() )
			enemiesDead = false;
		}

	return enemiesDead;
	}
bool TurnManager::CheckLose()
	{
	bool alliesdead = true;
	for ( unsigned int i = 0; i < AlliedUnits.size(); i++ )
		{
		if ( AlliedUnits[i]->isAlive() ) 
			alliesdead = false;
		}

	return alliesdead;
	}
void TurnManager::EndCombat()
	{
	GameData::GetInstance()->SetIsInCombat( false );
	TurnManager::GetInstance()->Terminate();
	GamePlayState::GetInstance()->SetState( GPStates::Town );
	}
void TurnManager::CombatUpdate(float dt)
	{
	if ( !turnPause )
		{
		if ( !timeStop )
			{
			std::sort( AllCombatUnits.begin(), AllCombatUnits.end(), sortByProgress );
			for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
				{
				AllCombatUnits[i]->Update( dt );
				if ( fullProgressReached || turnPause )
					{
					break;
					}
				}
			}
		else
			{
			AllCombatUnits[0]->Update( dt );
			pauseTime = 2.0f;
			}
		}
	else
		pauseTime -= dt;
	if ( pauseTime <= 0.0f )
		turnPause = false;
	}
void TurnManager::AnimationUpdate( float dt )
	{
	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
		{
		AllCombatUnits[i]->UpdateAnimation( dt );
		}
	}
void TurnManager::ParticleUpdate( float dt )
	{

	}
// Stuff to be deleted...used only for testing stories