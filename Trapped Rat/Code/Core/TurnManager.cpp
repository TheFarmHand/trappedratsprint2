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

	GamePlayState::GetInstance()->runAway = false;
	//ElementalChartLoad
	elementalImage = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/eletable.png" );
	elementalgraphicactive = false;
	//TimelineImageload
	timelineImage = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/timeline.png" );

	// Particle Manager pointer assignment
	pPartMan = ParticleManager::GetInstance();
	pPartMan->LoadEmitter( "../Trapped Rat/Assets/Scripts/bloodparticle.xml", "takedamage" );
}

void TurnManager::CombatLoot()
{
	GamePlayState* pGPS = GamePlayState::GetInstance();
	int num_items = 0;
	int gold = 0;
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
	{
		int test = rand() % 100;
		if ( test < 35 ) num_items++;
		gold += rand() % 25;
	}

	for ( int i = 0; i < num_items; i++ )
	{
		// Roll for stuff!
		int item = rand() % 1000;

		if ( item < 300 )	// Small HP
		{
			pGPS->AddItem( SmallHP );
		}

		else if ( item < 600 )	// Small BP
		{
			pGPS->AddItem( SmallBP );
		}

		else if ( item < 725 )	// Large HP
		{
			pGPS->AddItem( LargeHP );
		}

		else if ( item < 850 )	// Large BP
		{
			pGPS->AddItem( LargeBP );
		}

		else if ( item < 950 )	// Revive
		{
			pGPS->AddItem( Revive );
		}
		else if ( item < 1000 )	// Max Revive (not a real thing right now)
		{
			pGPS->AddItem( Revive );
		}
	}


	int i = pGPS->tripped_trap;
	if ( i != -1 )
	{
		gold += pGPS->GetTraps()[ i ]->gold;
		pGPS->AddGold( gold );

		for ( int its = 0; its < 3; its++ )
		{
			if ( pGPS->GetTraps()[ i ]->items[ its ] < 5 && pGPS->GetTraps()[ i ]->items[ its ] >= 0 ) // proper item types
			{
				ItemType stupidcode = (ItemType)pGPS->GetTraps()[ i ]->items[ its ];
				pGPS->AddItem( stupidcode );
			}
		}
		//pGPS->tripped_trap = -1;  don't remember why this code got placed here, but it was bad
	}
	else

	{
		pGPS->AddGold(gold);
	}
}

void TurnManager::CombatXP()
{
	int xp = 0;
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
	{
		xp += dynamic_cast<Enemy*>( EnemyUnits[ i ] )->GetXPValue();
	}

	GamePlayState::GetInstance()->SetLootXP( xp );

	for ( unsigned int i = 0; i < AlliedUnits.size(); i++ )
	{
		if ( AlliedUnits[ i ]->isAlive() )
			dynamic_cast<CombatPlayer*>( AlliedUnits[ i ] )->AddXP( xp );
	}
}

void TurnManager::Update( float dt )
{
	//*Should be checked first each update, try not to add code here or before
	if ( getTimeStop() == false )
	{
		if ( CheckWin() )
		{
			// End of Combat, enemies have died

			// Gain XP
			CombatXP();

			// Gain Items
			CombatLoot();

			// Abilities Unlocked?  This will become redundant I'm sure
			GamePlayState::GetInstance()->CheckAbilityUnlocked( true );
			//GamePlayState::GetInstance()->EndOfCombatScreen();

			//GamePlayState::GetInstance()->CheckAbilityUnlocked();
			GamePlayState::GetInstance()->SetLastState(GPStates::BattleSummary);
			GamePlayState::GetInstance()->SetState( GPStates::BattleSummary );
			EndCombat();
			return;
		}

		if ( CheckLose() )
		{
			EndCombat();
			if ( GamePlayState::GetInstance()->ignore_game_over )
			{

				return;
			}
			else
				GameData::GetInstance()->SwapState( GameOverLoseState::GetInstance() );
			return;
		}
	}

	//Check to see if elemental table display input
	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::E ) || SGD::InputManager::GetInstance()->IsButtonPressed(0,0) )
	{
		elementalgraphicactive = !elementalgraphicactive;
	}
	//Debug Keys
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::A) && SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::D))
	{
		AlliedUnits[0]->SetHP(1);
	}
	//*The core updates to take place during the overall update. Add what code you need to the appropriate section
	//can modify order if needed, should not break
	if ( !elementalgraphicactive )
	{
		CombatUpdate( dt );
		AnimationUpdate( dt );
		ParticleUpdate( dt );
	}
}
void TurnManager::Render()
{
	//*Anything added below will render underneath of combat character

	//Timelinerendering
	if ( timelineImage != SGD::INVALID_HANDLE )
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture( timelineImage, { 0, 70 }, 0.0f, { }, { }, { 1.0f, .5f } );
		for ( unsigned int i = 0; i < AlliedUnits.size(); ++i )
		{
			if ( AlliedUnits[ i ]->GetTimelineAnimation() != nullptr && AlliedUnits[ i ]->isAlive() )
			{
				//87 = y
				//95 = start x
				//650 = end x
				//555
				float progress = AlliedUnits[ i ]->GetProgress() / 100.0f;
				progress *= 555;
//				SGD::GraphicsManager::GetInstance()->DrawTexture( AlliedUnits[ i ]->GetTimelineAnimation(), { 95 + progress, 87 }, 0, { }, { }, { 1.0f, 1.0f } );
				AlliedUnits[i]->GetTimelineAnimation()->Render(111 + progress, 87);
				int offset = 0;
				for ( auto iter = AlliedUnits[ i ]->GetEffects().begin(); iter != AlliedUnits[ i ]->GetEffects().end(); iter++ )
				{
					SGD::GraphicsManager::GetInstance()->DrawTexture( ( *iter )->GetIcon(), { ( 95 + progress ) + 10 * offset, 87 + 35 } );
					offset++;
				}
			}
		}
		for ( size_t i = 0; i < EnemyUnits.size(); i++ )
		{
			if ( EnemyUnits[ i ]->GetTimelineAnimation() !=nullptr && EnemyUnits[ i ]->isAlive() )
			{
				float progress = EnemyUnits[ i ]->GetProgress() / 100.0f;
				progress *= 555;
				//SGD::GraphicsManager::GetInstance()->DrawTexture( EnemyUnits[ i ]->GetTimelineAnimation(), { 95 + progress, 87 }, 0, { }, { }, { 1.0f, 1.0f } );
				EnemyUnits[i]->GetTimelineAnimation()->Render(111 + progress, 87);
				int offset = 0;
				for ( auto iter = EnemyUnits[ i ]->GetEffects().begin(); iter != EnemyUnits[ i ]->GetEffects().end(); iter++ )
				{
					SGD::GraphicsManager::GetInstance()->DrawTexture( ( *iter )->GetIcon(), { ( 95 + progress ) + 10 * offset, 87 + 35 } );
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
		//if ( EnemyUnits[ i ]->isAlive() )
			EnemyUnits[ i ]->Render();
	}
	//*

	//*Anything added below will render on top of combat characters
	if ( pPartMan != nullptr )
		pPartMan->Render();
	if ( GamePlayState::GetInstance()->AbilityUsed && GamePlayState::GetInstance()->CurrentAbilityUsed != nullptr)
		GamePlayState::GetInstance()->CurrentAbilityUsed->Render();
	//if ( testCover )
	//SGD::GraphicsManager::GetInstance()->DrawString( L"Covered Ally!", SGD::Point( AlliedUnits[ 0 ]->GetPosition().x + 150, AlliedUnits[ 0 ]->GetPosition().y + 32 ), SGD::Color( 255, 100, 0 ) );

	//Render Elemental Chart if Needed

	if ( elementalgraphicactive )
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture( elementalImage, { 250, 100 } );
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
	for ( unsigned int i = 0; i<AlliedUnits.size(); i++ )
	{
		// Empty out active status effects on party

		while ( AlliedUnits[ i ]->GetEffects().size() > 0 )
		{
			auto iter = AlliedUnits[ i ]->GetEffects().begin();
			if ( ( *iter ) != nullptr )
				( *iter )->Clear();
		}
	}
	AlliedUnits.clear();
	for ( unsigned int i = 0; i < EnemyUnits.size(); i++ )
	{
		delete EnemyUnits[ i ];
	}

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
	SGD::GraphicsManager::GetInstance()->UnloadTexture( timelineImage );
	timelineImage = SGD::INVALID_HANDLE;

	SGD::GraphicsManager::GetInstance()->UnloadTexture( elementalImage );
	elementalImage = SGD::INVALID_HANDLE;
}
void TurnManager::HealTarget( Character* target, int value )
{
	target->TakeDamage( -value );
}

int TurnManager::ElementalMod( Character* target, int damage, ETYPE element )
{
	if ( element == FIRE )
	{
		if ( target->GetEType() == WIND )	// Resist Damage
		{
			return damage / 2;
		}

		else if ( target->GetEType() == EARTH )
		{
			return (int)( damage * 1.5f );
		}
	}

	else if ( element == WATER )
	{
		if ( target->GetEType() == WIND )
		{
			return (int)( damage * 1.5f );
		}

		if ( target->GetEType() == EARTH )
		{
			return damage / 2;
		}
	}

	else if ( element == WIND )
	{
		if ( target->GetEType() == FIRE )	// Extra Damage
		{
			return (int)( damage * 1.5f );
		}

		if ( target->GetEType() == WATER )
		{
			return damage / 2;
		}
	}

	else if ( element == EARTH )
	{
		if ( target->GetEType() == FIRE )
		{
			return damage / 2;
		}

		if ( target->GetEType() == WATER )
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

void TurnManager::AttackTarget( Character* owner, Character* target, int value )
// Handles status effects React in here, passes along the finalized damage to the target to get murdered
{
	bool dodge = false;
	bool guard = false; // Cover
	bool firespike = false;
	bool counter = false;
	bool hedge = false;
	bool enfire = false;
	StatusEffect* Guard = nullptr;
	StatusEffect* to_remove = nullptr;

	for ( auto iter = owner->GetEffects().begin(); iter != owner->GetEffects().end(); iter++ )
	{
		if ( ( *iter )->GetName() == "Enfire" )
		{
			if ( ( *iter )->GetTernEffect() ) enfire = true;

		}
	}

	// Iterate status effect loops and look for special cases, setting appropriate bool to true to be handled after loop
	for ( auto iter = target->GetEffects().begin(); iter != target->GetEffects().end(); iter++ )
	{
		// Check for Dodge abilities
		if ( ( *iter )->GetName() == "Dodging" )
		{
			// Found dodge!  
			if ( ( *iter )->GetTernEffect() )
			{
				if ( ( *iter )->GetOwner()->GetName() == "Ratsputin" )
					target->SetProgress( 100.0f );
				else if ( ( *iter )->GetOwner()->GetName() == "Jeeves" )
					owner->TakeDamage( 10 );
				else if ( ( *iter )->GetOwner()->GetName() == "Biggs" )
					target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "SpeedUp" ) );
				else if ( ( *iter )->GetOwner()->GetName() == "Slippy" )
					// Slippy don't do what you think he do
				{

				}

			}
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
			int dmg = (int)( target->GetMagic() * 0.2f );
			owner->TakeDamage( dmg );

			if ( ( *iter )->GetTernEffect() )
				target->TakeDamage( -dmg );
		}

		else if ( ( *iter )->GetName() == "Counter" )
		{
			counter = true;
			to_remove = ( *iter );
		}

		else if ( ( *iter )->GetName() == "Hedge" )
		{
			// Hedge Guard reaction
			int atk = owner->GetAttack();
			int dmg = rand() % atk + atk;
			dmg -= (int)( 0.25f * target->GetDefense() );
			if ( dmg <= 0 )
				dmg = 0;
			else
				dmg /= 2;
			if ( ( *iter )->GetTernEffect() )
				dmg = 0;
			target->TakeDamage( dmg );
		}

		// Primarily (only) Cecil fight
		else if ( ( ( *iter )->GetName() == "PhysicalReflect" ) )
			// Do your damage to yourself
		{
			int dmg = value;
			dmg -= (int)( 0.25f * target->GetDefense() );
			owner->TakeDamage( value );
			return;	// Don't allow other settings to fire
		}

		else if ( ( *iter )->GetName() == "WaterWall" )
		{
			( *iter )->SetTickDmg( ( *iter )->GetTickDmg() - value );
			target->TakeDamage( value, true );
			target->TakeDamage( -(value / 4) );
			if ( ( *iter )->GetTickDmg() <= 0 )
			{
				// Clear it
				to_remove = ( *iter );
				to_remove->Clear();

			}
			return;
		}
	}


	if ( firespike )
	{
		// Should no longer be useful		
	}

	if ( owner->HasEffect( "Enfire" ) )
	{
		int dmg = (int)( owner->GetMagic() * 0.3f );
		if ( enfire ) dmg = (int)( dmg * 1.75f );		// Ternary Blast style
		target->TakeDamage( dmg, true );
	}


	if ( dodge )
	{
		// Do some Dodge word magic instead of damage
		// For now:
		value = 0;
		target->TakeDamage( value ,false,false,true);
	}

	if ( guard )
	{
		// Redirect attack to Guard


		/*int dmg = owner->GetAttack();
		dmg += rand() % ( dmg / 3 );
		dmg -= Guard->GetGuard()->GetDefense() / 2;*/
		/*if ( dmg <= 0 )
			dmg = 0;*/
		if ( Guard->GetTernEffect() )
			owner->TakeDamage( value );
		else
			Guard->GetGuard()->TakeDamage( value );
	}

	if ( counter && !guard )
	{
		// Reduce incoming damage, attack the attacker
		if ( to_remove->GetTernEffect() )
			value = 0;
		else
			value = value / 2;
		target->TakeDamage( value );
		target->Attack( target, owner );
		to_remove->Clear();
		to_remove = nullptr;
	}

	if ( to_remove != nullptr )
	{
		to_remove->Clear();
		return;
	}

	if ( !dodge && !counter && !guard )
		// I think this value needs Defense modification
	{
		target->TakeDamage( value );

		// This call places the emitter at the proper location
		pPartMan->GetEmitter( "takedamage" )->SetPosition( target->GetPosition().x, target->GetPosition().y );

		// This call creates a new instance of the emitter
		pPartMan->CreateEmitter( "takedamage" );
	}

}

void TurnManager::UsingAbility( Character* owner, Character* target, Ability* ability, bool ternary, int aoeCounter)
// Calculates and dishes out damage based on an ability
{
	// Room here for adding particle effects && Cecil!


	if ( target->HasEffect( "Cover" ) )
	{
		if ( ability->GetOffensive() )
		{
			for ( auto iter = target->GetEffects().begin(); iter != target->GetEffects().end(); iter++ )
			{
				if ( ( *iter )->GetName() == "Cover" )
				{
					ability->CalculateFormula( owner, ( *iter )->GetGuard() );
					ability->CastAbility( owner, ( *iter )->GetGuard() );
					break;
				}
			}
		}
	}

	else
	{
		ability->CalculateFormula( owner, target );
		// Reflect ability damage back to caster if MacialReflect exists
		if ( target->HasEffect( "MagicalReflect" ) && ability->GetOffensive() )
		{
			ability->CastAbility( target, owner );
		}
		else
			ability->CastAbility( owner, target, aoeCounter, ternary );
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
			dynamic_cast<CombatPlayer*>( AlliedUnits[ playerParty[ i ]->GetOrderPosition() ] )->Reset();
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
	GamePlayState::GetInstance()->SetLastState( GPStates::Town );
	TurnManager::GetInstance()->Terminate();
}
void TurnManager::CombatUpdate( float dt )
{
	if ( GamePlayState::GetInstance()->AbilityUsed && GamePlayState::GetInstance()->CurrentAbilityUsed != nullptr )
	{
		GamePlayState::GetInstance()->abilityTimer -= dt;
		if ( GamePlayState::GetInstance()->abilityTimer <= 0.0f )
		{
			GamePlayState::GetInstance()->AbilityUsed = false;
			GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 5 );
		}
		GamePlayState::GetInstance()->CurrentAbilityUsed->Update( dt );
	}
	else if ( GamePlayState::GetInstance()->ItemUsed )
		{
		GamePlayState::GetInstance()->itemTimer -= dt;
		if ( GamePlayState::GetInstance()->itemTimer <= 0.0f )
			{
			GamePlayState::GetInstance()->ItemUsed = false;
			GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 5 );
			}
		}
	else if ( GamePlayState::GetInstance()->RunUsed )
		{
		GamePlayState::GetInstance()->runTimer -= dt;
		if ( GamePlayState::GetInstance()->runTimer <= 0.0f )
			{
			GamePlayState::GetInstance()->RunUsed = false;
			GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 5 );
			}
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
					timeStop = true;
					break;
				}
			}
		}
		else
		{
			AllCombatUnits[ 0 ]->Update( dt );
			pauseTime = 1.0f;
		}
	}
	else
		pauseTime -= dt;
	if ( pauseTime <= 0.0f )
	{
		//AllCombatUnits[0]->SetProgress(0.0f);
		turnPause = false;
		//testCover = false;
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

