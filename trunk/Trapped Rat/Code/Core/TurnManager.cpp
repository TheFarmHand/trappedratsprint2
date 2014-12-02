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

	//Safe to add code here

	//TimelineImageload
	timelineImage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/timeline.png");

	// Particle Manager pointer assignment
	pPartMan = ParticleManager::GetInstance();
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


	//*The core updates to take place during the overall update. Add what code you need to the appropriate section
	//can modify order if needed, should not break
	CombatUpdate( dt );
	AnimationUpdate( dt );
	ParticleUpdate( dt );

}
void TurnManager::Render()
{
	//*Anything added below will render underneath of combat character

	//Timelinerendering
	if (timelineImage != SGD::INVALID_HANDLE)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(timelineImage, { 0, 70 }, 0.0f, {}, {}, {1.0f,.5f});
		for (unsigned int i = 0; i < AlliedUnits.size(); ++i)
		{
			if (AlliedUnits[i]->GetTimelineAnimation() != SGD::INVALID_HANDLE)
			{
				//87 = y
				//95 = start x
				//650 = end x
				//555
				float progress = AlliedUnits[i]->GetProgress() /100.0f;
				progress *= 555;
				SGD::GraphicsManager::GetInstance()->DrawTexture(AlliedUnits[i]->GetTimelineAnimation(), { 95+progress, 87 }, 0, {}, {}, {1.0f,1.0f});
			}
		}
		for (size_t i = 0; i < EnemyUnits.size(); i++)
		{
			if (EnemyUnits[i]->GetTimelineAnimation() != SGD::INVALID_HANDLE)
			{
				float progress = EnemyUnits[i]->GetProgress() / 100.0f;
				progress *= 555;
				SGD::GraphicsManager::GetInstance()->DrawTexture(EnemyUnits[i]->GetTimelineAnimation(), { 95 + progress, 87 }, 0, {}, {}, { 1.0f, 1.0f });
			}
		}
		if (AllCombatUnits[0]->GetProgress() >= 100.0f)
		{
			if (AllCombatUnits[0]->GetPortrait() != SGD::INVALID_HANDLE)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(AllCombatUnits[0]->GetPortrait(), { 650, 20 }, 0, {}, {}, { 1.0f, 1.0f });
			}
		}
	}

	//*


	//*Do not insert any code here
	for ( unsigned int i = 0; i < AlliedUnits.size(); ++i )
	{
		AlliedUnits[ i ]->Render();
	}
	for ( unsigned int i = 0; i < EnemyUnits.size(); ++i )
	{
		EnemyUnits[ i ]->Render();
	}
	//*

	//*Anything added below will render on top of combat characters
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

	// Clear up TurnManager's Particles
	if ( pPartMan != nullptr )
	{
		pPartMan->UnloadEmitter( "takedamage" );
		pPartMan->ClearAll();
		pPartMan = nullptr;
	}

	//Unload timeline image
	SGD::GraphicsManager::GetInstance()->UnloadTexture(timelineImage);
	timelineImage = SGD::INVALID_HANDLE;
}
void TurnManager::HealTarget( Character* target, int value )
{
	target->TakeDamage( -value);
}
void TurnManager::AttackTarget( Character* owner, Character* target, int value )
// Handles status effects React in here, passes along the finalized damage to the target to get murdered
{
	bool dodge = false;
	bool guard = false; // Cover
	bool firespike = false;
	bool counter = false;
	bool hedge = false;
	Character* Guard = nullptr; 
	StatusEffect* to_remove = nullptr;

	// Iterate status effect loops and look for special cases, setting appropriate bool to true to be hanled after loop
	for ( auto iter = target->GetEffects().begin(); iter != target->GetEffects().end(); iter++ )
	{
		// Check for Dodge abilities
		if ( ( *iter )->GetName() == "Dodging" )
		{
			// Found dodge!  
			dodge = true;
		}

		else if ( ( *iter )->GetName() == "Cover" )
		{
			// Found guard!
			guard = true;
			Guard = (*iter)->GetOwner();

		}

		else if ( ( *iter )->GetName() == "FireSpikes" )
		{
			firespike = true;
		}

		else if ( ( *iter )->GetName() == "Counter" )	
		{
			counter = true;
			to_remove = (*iter);
		}

		else if( (*iter)->GetName() == "Hedge")
		{
			// Hedge Guard reaction
			hedge = true;
		}
	}

	if ( firespike )
	{
		// Slap for a little damage
		
		owner->TakeDamage((int)(target->GetStats().magic * 0.2f));
	}

	if ( counter )
	{
		// Reduce incoming damage, attack the attacker
		value = value / 2;
		target->TakeDamage(value);
		target->Attack( target, owner );	// Oh, that's bad; circular counter attacks forever (actually should be fine, can't be countering on your turn)
		to_remove->Clear();
	}

	if ( dodge )
	{
		// Do some Dodge word magic instead of damage
		// For now:
		value = 0;
		target->TakeDamage(value);
	}

	if ( guard )
	{
		// Redirect attack to Guard
		owner->Attack(owner, Guard);
		
	}

	
	if ( !dodge && !counter && !guard)
	{
		target->TakeDamage( value );

		// This call places the emitter at the proper location
		pPartMan->GetEmitter( "takedamage" )->SetPosition( target->GetPosition().x, target->GetPosition().y );

		// This call creates a new instance of the emitter
		pPartMan->CreateEmitter( "takedamage" );
	}

}

void TurnManager::UsingAbility(Character* owner, Character* target, Ability* ability)
// Calculates and dishes out damage based on an ability
{
	// Room here for adding particle effects
	ability->CalculateFormula(owner, target);
	ability->CastAbility(owner, target);
}

int TurnManager::ElementalMod( Character* owner, Character* target, int damage, ETYPE element )
{
	// Modifies damage by elemental types
	{
		if ( element == FIRE )
		{
			if ( owner->GetEType( ) == WIND )	// Resist Damage
			{
				return damage / 2;
			}

			else if ( owner->GetEType( ) == EARTH )
			{
				return (int)( damage * 1.5f );
			}
		}

		else if ( element == WATER )
		{
			if ( owner->GetEType( ) == WIND )
			{
				return (int)(damage * 1.5f);
			}

			if ( owner->GetEType( ) == EARTH )
			{
				return damage / 2;
			}
		}

		else if ( element == WIND )
		{
			if ( owner->GetEType( ) == FIRE )	// Extra Damage
			{
				return (int)( damage * 1.5f );
			}

			if ( owner->GetEType( ) == WATER )
			{
				return damage / 2;
			}
		}

		else if ( element == EARTH )
		{
			if ( owner->GetEType( ) == FIRE )
			{
				return damage / 2;
			}

			if ( owner->GetEType( ) == WATER )
			{
				return (int)( damage * 1.5f );
			}
		}

		else if ( element == MULTI )		// is this still a thing?
		{
			// What do I do with this? phys + ele I think
			return damage;
		}

		// Physical
		return damage;
	}
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
		if ( playerParty[ i ]->GetActive() )
		{
			AlliedUnits[ playerParty[ i ]->GetOrderPosition() ] = playerParty[ i ];
			AllCombatUnits.push_back( playerParty[ i ] );
			dynamic_cast<CombatPlayer*>( AlliedUnits[ i ] )->Reset();
		}
	}
}
void TurnManager::SetupEnemyParty( std::vector<Enemy*> enemyParty )
{
	for ( unsigned int i = 0; i < enemyParty.size(); ++i )
	{
		EnemyUnits.push_back( enemyParty[ i ] );
		AllCombatUnits.push_back( enemyParty[ i ] );
	}
	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
	{
		AllCombatUnits[ i ]->SetProgress( 0.0f );
	}
}
bool TurnManager::CheckWin()
{
	bool enemiesDead = true;
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
	{
		if ( EnemyUnits[ i ]->isAlive() )
			enemiesDead = false;
	}

	return enemiesDead;
}
bool TurnManager::CheckLose()
{
	bool alliesdead = true;
	for ( unsigned int i = 0; i < AlliedUnits.size(); i++ )
	{
		if ( AlliedUnits[ i ]->isAlive() )
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
void TurnManager::CombatUpdate( float dt )
{
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
		AllCombatUnits[0]->Update( dt );
		pauseTime = 2.0f;
		}
	}
	else
		pauseTime -= dt;
	if (pauseTime <= 0.0f)
	{
		//AllCombatUnits[0]->SetProgress(0.0f);
		turnPause = false;
	}
}
void TurnManager::AnimationUpdate( float dt )
{
	for ( unsigned int i = 0; i < AllCombatUnits.size(); ++i )
	{
		AllCombatUnits[ i ]->UpdateAnimation( dt );
	}
}
void TurnManager::ParticleUpdate( float dt )
{
	pPartMan->Update( dt );
}

