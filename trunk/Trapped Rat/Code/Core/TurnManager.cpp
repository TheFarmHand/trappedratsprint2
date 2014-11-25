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
	/*switch ( playerParty.size() )
	{
		case 1:
			
			break;
		case 2:
			
			break;
		default:
			
			break;
	}*/

	for ( unsigned int i = 0; i < playerParty.size(); ++i )
	{
		if ( playerParty[ i ]->GetActive() )
		{
		playerParty[i]->SetActive( true );
			AlliedUnits[ playerParty[ i ]->GetOrderPosition() ] = playerParty[ i ];
			AllCombatUnits.push_back( playerParty[ i ] );
			dynamic_cast<CombatPlayer*>( AlliedUnits[i] )->Reset();
		}
	}
	for ( unsigned int i = 0; i < enemyParty.size(); ++i )
	{
		EnemyUnits.push_back( enemyParty[ i ] );
		AllCombatUnits.push_back( enemyParty[ i ] );
	}
	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
	{
		AllCombatUnits[ i ]->SetProgress( 0.0f );
	}
	timeStop = false;
	fullProgressReached = false;


	// Particle Manager Initialization fo Turn Manager (Kinda hacky)
	SetupFireFang();
	pPartMan = new ParticleManager();
	pPartMan->LoadEmitter( "../Trapped Rat/Assets/Scripts/bloodparticle.xml", "takedamage" );
}

void TurnManager::Update( float dt )
{
	bool enemiesDead = true;
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
	{
		if ( EnemyUnits[ i ]->isAlive() )
			enemiesDead = false;
	}

	if ( enemiesDead )
	{
		// End of Combat, enemies have died
		GameData::GetInstance()->SetIsInCombat( false );
		TurnManager::GetInstance()->Terminate();
		GamePlayState::GetInstance()->SetState( GPStates::Town );
		return;
	}
	bool alliesdead = true;
	for ( unsigned int i = 0; i < AlliedUnits.size(); i++ )
	{
		if ( AlliedUnits[ i ]->isAlive() ) alliesdead = false;
	}
	if ( alliesdead )
	{
		GameData::GetInstance()->SetIsInCombat( false );
		TurnManager::GetInstance()->Terminate();
		GamePlayState::GetInstance()->SetState( GPStates::Town );

		GameData::GetInstance()->SwapState( GameOverLoseState::GetInstance() );
		return;
	}
	if ( !turnPause )
	{
		if ( !timeStop )
		{
			std::sort( AllCombatUnits.begin(), AllCombatUnits.end(), sortByProgress );
			for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
			{
				AllCombatUnits[ i ]->Update( dt );
				if ( fullProgressReached || turnPause )
				{
					break;
				}
			}
		}
		else
		{
			AllCombatUnits[ 0 ]->Update( dt );
			pauseTime = 2.0f;
		}
	}
	else
		pauseTime -= dt;
	if ( pauseTime <= 0.0f )
		turnPause = false;

	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
		{
		AllCombatUnits[i]->UpdateAnimation( dt );
		}

	pPartMan->Update( dt );
}
void TurnManager::Render()
{
	for ( unsigned int i = 0; i < AlliedUnits.size(); ++i )
	{
		AlliedUnits[ i ]->Render();
	}
	for ( unsigned int i = 0; i < EnemyUnits.size(); ++i )
	{
		EnemyUnits[ i ]->Render();
	}
	if ( timeStop )
	{
		GameData::GetInstance()->GetFont()->DrawString( "Current Turn", 350.0f, 450.0f, SGD::Color( 255, 255, 255, 255 ) );
		GameData::GetInstance()->GetFont()->DrawString( AllCombatUnits[ 0 ]->GetName().c_str(), 375.0f, 500.0f, SGD::Color( 255, 255, 255, 255 ) );
	}

	pPartMan->Render();
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
		SGD::GraphicsManager::GetInstance()->UnloadTexture( FireFang.GetIcon() );
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

	// Blarrrrr
	StatusEffect *tempstat = new StatusEffect;
	*tempstat = FireFang;
	tempstat->SetOwner( target );
	target->AddStatus( tempstat );
}

bool sortByProgress( Character* a, Character* b )
{
	return a->GetProgress() > b->GetProgress();
}






// ** HACKY STUFF AGAIN ** //
void TurnManager::SetupFireFang()
{
	FireFang.SetName( "Burn" );
	FireFang.SetOwner( AlliedUnits[ 0 ] );
	FireFang.SetElement( Fire );
	FireFang.SetTick( 2 );
	FireFang.SetTickDmg( 15 );
	FireFang.SetIcon( SGD::GraphicsManager::GetInstance()->LoadTexture(
		"../Trapped Rat/Assets/Textures/TestParticleRed.png" ) );
}