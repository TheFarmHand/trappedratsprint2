#include "TurnManager.h"
#include <algorithm>
#include "GameData.h"
#include "../Font/Font.h"
#include <sstream>
#include "../States/GamePlayState.h"
#include "../States/GameOverLoseState.h"
#include "../Particle/ParticleManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_InputManager.h"

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
	testCover = false;
	//Safe to add code here

	//ElementalChartLoad
	elementalImage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/eletable.png");
	elementalgraphicactive = false;
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
	//Need to do exp gain and level increase calculations prior to this check
	GamePlayState::GetInstance()->CheckAbilityUnlocked();
		EndCombat();
		return;
	}
	if ( CheckLose() )
	{
		EndCombat();
		GameData::GetInstance()->SwapState( GameOverLoseState::GetInstance() );
		return;
	}

	//Check to see if elemental table display input
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::E))
	{
		elementalgraphicactive = !elementalgraphicactive;
	}
	//*The core updates to take place during the overall update. Add what code you need to the appropriate section
	//can modify order if needed, should not break
	if (!elementalgraphicactive)
	{
		CombatUpdate(dt);
		AnimationUpdate(dt);
		ParticleUpdate(dt);
	}
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
				int offset = 0;
				for ( auto iter = AlliedUnits[ i ]->GetEffects( ).begin( ); iter != AlliedUnits[ i ]->GetEffects( ).end( ); iter++ )
				{
					SGD::GraphicsManager::GetInstance( )->DrawTexture( ( *iter )->GetIcon( ), { ( 95 + progress) + 10 * offset, 87 + 35 } );
					offset++;
				}
			}
		}
		for (size_t i = 0; i < EnemyUnits.size(); i++)
		{
			if (EnemyUnits[i]->GetTimelineAnimation() != SGD::INVALID_HANDLE)
			{
				float progress = EnemyUnits[i]->GetProgress() / 100.0f;
				progress *= 555;
				SGD::GraphicsManager::GetInstance()->DrawTexture(EnemyUnits[i]->GetTimelineAnimation(), { 95 + progress, 87 }, 0, {}, {}, { 1.0f, 1.0f });
				int offset = 0;
				for ( auto iter = EnemyUnits[ i ]->GetEffects( ).begin( ); iter != EnemyUnits[ i ]->GetEffects( ).end( ); iter++ )
				{
					SGD::GraphicsManager::GetInstance( )->DrawTexture( ( *iter )->GetIcon( ), { ( 95 + progress) + 10 * offset, 87 + 35 } );
					offset++;
				}
			}
		}
		/*if (AllCombatUnits[0]->GetProgress >= 100.0f)
		{
			if (AllCombatUnits[0]->GetPortrait() != SGD::INVALID_HANDLE)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(AllCombatUnits[0]->GetPortrait(), { 95 + progress, 87 }, 0, {}, {}, { 1.0f, 1.0f });
			}
		}*/
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
	if ( GamePlayState::GetInstance()->AbilityUsed )
		GamePlayState::GetInstance()->CurrentAbilityUsed->Render();
	if (testCover)
		SGD::GraphicsManager::GetInstance()->DrawString( L"Covered Ally!", SGD::Point( AlliedUnits[2]->GetPosition().x + 150, AlliedUnits[2]->GetPosition().y + 32 ), SGD::Color(255, 100, 0) );

	//Render Elemental Chart if Needed
	
		if (elementalgraphicactive)
		{
			SGD::GraphicsManager::GetInstance()->DrawTexture(elementalImage, {10,200});
		}
	

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

	//Unload timeline image and elemental image
	SGD::GraphicsManager::GetInstance()->UnloadTexture(timelineImage);
	timelineImage = SGD::INVALID_HANDLE;

	SGD::GraphicsManager::GetInstance()->UnloadTexture(elementalImage);
	elementalImage = SGD::INVALID_HANDLE;
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
	StatusEffect* Guard = nullptr; 
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
			Guard = ( *iter );
			testCover = true;
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

	if ( owner->HasEffect( "Enfire" ))
		{
		target->TakeDamage( (int)(owner->GetStats().magic * 0.3f ), true);
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
		int dmg = owner->GetStats().attack;
		dmg += rand()%(dmg/3);
		dmg -= Guard->GetGuard()->GetStats().defense/2;
		Guard->GetGuard()->TakeDamage(dmg);
		
		//owner->Attack(owner, Guard->GetGuard());
		
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
	else if ( playerParty.size() >= 3 )
	{
		AlliedUnits.resize( 3 );
	}

	for ( unsigned int i = 0; i < playerParty.size(); ++i )
	{
		if ( playerParty[ i ]->GetActive() )
		{
			AlliedUnits[ playerParty[ i ]->GetOrderPosition() ] = playerParty[ i ];
			AllCombatUnits.push_back( playerParty[ i ] );
			dynamic_cast<CombatPlayer*>( AlliedUnits[playerParty[i]->GetOrderPosition()] )->Reset();
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
	GamePlayState::GetInstance()->SetLastState(GPStates::Town);
}
void TurnManager::CombatUpdate( float dt )
{
if ( GamePlayState::GetInstance()->AbilityUsed)
	{
	GamePlayState::GetInstance()->abilityTimer -= dt;
	if ( GamePlayState::GetInstance()->abilityTimer <= 0.0f )
		GamePlayState::GetInstance()->AbilityUsed = false;
	GamePlayState::GetInstance()->CurrentAbilityUsed->Update( dt );
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
		testCover = false;
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

