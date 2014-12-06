#include "CombatPlayer.h"
#include "TurnManager.h"
#include <string>
#include <sstream>
#include <vector>

#include "GameData.h"
#include "../States/GamePlayState.h"
#include "HelpText.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


CombatPlayer::CombatPlayer()
{
	myTarget = 0;
	states = 0;
	hudSelection = 0;
	menu = GamePlayState::GetInstance()->GetSelectableObjects();

}


CombatPlayer::~CombatPlayer()
{
	if ( ansys != nullptr )
		delete ansys;
}

void CombatPlayer::Update( float dt )
{


	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
	{
		progress += speed * dt;
	}
	else if ( progress < 100.0f )
	{
		if (stepbackward == true && stepforward == false)
		{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x -= stepvelocity*dt;
			SetPosition(pos);
			if (stepTime <= 0.0f)
				stepbackward = false;
		}
		if (stepbackward == false && stepforward == false)
		{
			TurnManager::GetInstance()->setProgressFullReached(false);
			TurnManager::GetInstance()->setTimeStop(false);
		}

		return;
	}

	if ( progress >= 100.0f )
	{
		SGD::InputManager *pInput = SGD::InputManager::GetInstance();
		TurnManager *pTurn = TurnManager::GetInstance();
		GamePlayState *game = GamePlayState::GetInstance();
		HelpText *help = game->GetHelpText();
		if ( TurnManager::GetInstance()->getTimeStop() == false )
		{
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
			if ( HasEffect( "Stun" ) )
			{
				// Lose your turn if stunned
				StatusTick();
				progress = 0;
				return;
			}

			StatusTick();
			if ( !alive )
			{
				progress = 0.0f;
				return;
			}

		}
		TurnManager::GetInstance()->setTimeStop( true );
		if ( !TurnManager::GetInstance()->getProgressFullReached() )
			TurnManager::GetInstance()->setProgressFullReached( true );
		// Here is where targeting happens

		if (stepforward == false && stepbackward == false && stepTime != 0.0f)
		{
			stepforward = true;
			stepTime = 2.0f;
		}
		if (stepforward == true && stepbackward == false)
		{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x += stepvelocity*dt;
			SetPosition(pos);
			if (stepTime <= 0.0f)
			{
				stepTime = 0.0f;
				stepforward = false;
			}
		}

		if (stepbackward == false && stepforward == false)
		{
			if (states == 0)
			{
				HomeUpdate(dt);
			}

			else if (states == 1)		// Attacking; Target selection is "enemy"
			{
				AttackUpdate(dt);
			}

			else if (states == 2) // Item Selection
			{
				if (item_choose == nullptr)
					ItemsUpdate(dt);
				else
					SelectingItems(dt);

			}
			else if (states == 3)// Ability Selection
			{
				AbilityUpdate(dt);
			}
			else if (states == 4) // Run update
			{
				RunUpdate(dt);
			}
			else if (states == 5)//Defensive Ability update
			{
				AllySelectUpdate(dt);
			}
			else if (states == 6) // Offensive ability update
			{
				EnemySelectUpdate(dt);
			}
			else if (states == 7) // Ternary Blast stuff
			{
				TernaryBlast(dt);
			}
		}
	}
}

void CombatPlayer::UpdateAnimation( float dt )
{
	if ( ansys != nullptr )
	{
		ansys->Update( dt );
		Character::Update( dt );
	}
}
void CombatPlayer::Render()

{
	SGD::GraphicsManager *pGraphics = SGD::GraphicsManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 51, position.y - 30, { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 50, position.y - 30, { 200, 0, 255 }, 1.6f );
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

	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), 301, 451 + ( order*50.0f ), { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), 300, 450 + ( order*50.0f ), { 255, 0, 0 }, 1.6f );

	//if ( TestAbility && progress >= 100.0f )
	//	tester.Render();
	std::string numbers = std::to_string( HP ) + "/" + std::to_string( GetMaxHP() ) + " HP";
	std::string bpNumbers = std::to_string( BP ) + "/" + std::to_string( GetMaxBP() ) + " BP";
	SGD::Point pdraw;

	if ( progress >= 100.0f )
	{
		if ( mySelection == enemy )
		{
			pdraw = pTurn->GetEnemies()[ myTarget ]->GetPosition();
			pdraw.x -= pTurn->GetEnemies()[ myTarget ]->GetSize().width + 20;
			pdraw.y -= pTurn->GetEnemies()[ myTarget ]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetEnemyArrow(), pdraw );
		}

		else if ( mySelection == player || mySelection == self || mySelection == deadAlly )
			// enforce self target by disallowing targeting input in update
		{
			pdraw = pTurn->GetAllies()[ myTarget ]->GetPosition();
			pdraw.x += pTurn->GetAllies()[ myTarget ]->GetSize().width + 20;
			pdraw.y -= pTurn->GetAllies()[ myTarget ]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetAllyArrow(), pdraw );
		}
		else if ( mySelection == allEnemy )
		{
			for ( size_t i = 0; i < pTurn->GetEnemies().size(); i++ )
			{
				pdraw = pTurn->GetEnemies()[ i ]->GetPosition();
				pdraw.x -= pTurn->GetEnemies()[ i ]->GetSize().width + 20;
				pdraw.y -= pTurn->GetEnemies()[ i ]->GetSize().height / 2;
				pGraphics->DrawTexture( pTurn->GetEnemyArrow(), pdraw );
			}
		}
		else if ( mySelection == allAlly )
		{
			for ( size_t i = 0; i < pTurn->GetAllies().size(); i++ )
			{
				pdraw = pTurn->GetAllies()[ i ]->GetPosition();
				pdraw.x += pTurn->GetAllies()[ i ]->GetSize().width + 20;
				pdraw.y -= pTurn->GetAllies()[ i ]->GetSize().height / 2;
				pGraphics->DrawTexture( pTurn->GetAllyArrow(), pdraw );
			}
		}

	}

	SGD::Color color;
	float hppercentage = GetHP() / (float)(GetMaxHP() );
	if ( hppercentage < .25f )
	{
		color = SGD::Color( 155, 0, 0 );
	}
	else if ( hppercentage < .60f )
	{
		color = SGD::Color( 155, 155, 0 );
	}
	else
	{
		color = SGD::Color( 0, 155, 0 );
	}
	if ( numbers <= "0" )
		numbers = "0";
	GameData::GetInstance()->GetFont()->DrawString( numbers, 491, 441 + ( order*50.0f ), { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( numbers, 490, 440 + ( order*50.0f ), color, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( bpNumbers, 491, 466 + ( order*50.0f ), SGD::Color( 0, 0, 255 ), 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( bpNumbers, 490, 465 + ( order*50.0f ), SGD::Color( 0, 100, 255 ), 1.6f );

	if ( ansys != nullptr )
	{
		ansys->Render( position.x, position.y );
	}

	Character::Render();

	if ( item_choose != nullptr )
		item_choose->Render();

}


void CombatPlayer::Attack( Character* owner, Character * target )
{
	if ( target != nullptr )
	{
		int atk = target->GetAttack();
		int dmg = rand() % atk + atk;
		dmg -= (int)(0.25f * target->GetStats().defense);
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( owner, target, dmg );
		//owner->TakeDamage( 100, target );
	}
}

void CombatPlayer::UseItem( Character * target, Items item )
{
	//here we take in the item check what it does, then send the effects to the target
}
bool CombatPlayer::GetActive()
{
	return active;
}

void CombatPlayer::SetActive( bool a )
{
	active = a;
}
void CombatPlayer::SetAnimations( AnimationSystem* _an )
{
	if ( ansys != nullptr )
		delete ansys;
	ansys = _an;
}
AnimationSystem* CombatPlayer::GetAnimations()
{
	return ansys;
}
void CombatPlayer::SetSelection( int selected )
{
	for ( unsigned int i = 0; i < menu.size(); i++ )
	{
		if ( i == selected )
			GamePlayState::GetInstance()->GetSelectableObjects()[ i ]->SetSelected( true );
		else
			GamePlayState::GetInstance()->GetSelectableObjects()[ i ]->SetSelected( false );
	}
	GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ selected ] );
}

void CombatPlayer::TargetUnit( std::vector<Character*> &targets )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if ( mySelection == self )
	{
		/*if ( pInput->IsKeyPressed( SGD::Key::Up )  )

		if(  pInput->IsKeyPressed( SGD::Key::Left )*/
	}

	// Input
	else if ( mySelection == deadAlly )
	{
		if ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Left ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			--myTarget;
			if ( myTarget < 0 )
				myTarget = targets.size() - 1;
			while ( targets[ myTarget ]->isAlive() )
			{
				--myTarget;
				if ( myTarget < 0 )
					myTarget = targets.size() - 1;
			}
		}
		else if ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Right ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			++myTarget;
			if ( myTarget >( int )targets.size() - 1 )
				myTarget = 0;
			while ( targets[ myTarget ]->isAlive() )
			{
				++myTarget;
				if ( myTarget > (int)targets.size() - 1 )
					myTarget = 0;
			}
		}

		if ( myTarget < 0 ) myTarget = 0;
	}
	else if ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Left ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		--myTarget;
		if ( myTarget < 0 )
			myTarget = targets.size() - 1;

		while ( !targets[ myTarget ]->isAlive() )
		{
			--myTarget;
			if ( myTarget < 0 )
				myTarget = targets.size() - 1;
		}

		if ( myTarget < 0 ) myTarget = 0;
	}

	else if ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Right ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		++myTarget;

		if ( myTarget >( int )targets.size() - 1 )
		{
			myTarget = 0;
		}
		while ( !targets[ myTarget ]->isAlive() )
		{
			++myTarget;
			if ( myTarget > (int)targets.size() - 1 )
			{
				myTarget = 0;
			}

		}

		// Shouldn't ever need this check, but safety first
		if ( myTarget > (int)targets.size() - 1 ) myTarget = (int)targets.size() - 1;


	}
}
void CombatPlayer::Reset()
{
	states = 0;
	mySelection = none;
	myTarget = 0;
	hudSelection = 0;
}

void CombatPlayer::HomeUpdate( float dt )
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( states == 0 )
	{
		if ( pInput->IsKeyPressed( SGD::Key::Up ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 0 );
			hudSelection = 0;

		}
		else if ( pInput->IsKeyPressed( SGD::Key::Right ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 2 );
			hudSelection = 2;
		}
		else if ( pInput->IsKeyPressed( SGD::Key::Left ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 1 );
			hudSelection = 1;

		}
		else if ( pInput->IsKeyPressed( SGD::Key::Down ) )
		{
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 3 );
			hudSelection = 3;
		}


		// ****************** Ternary Blast Code added by Grant *******************
		else if ( GamePlayState::GetInstance()->GetGauge() >= MAXTG && pInput->IsKeyPressed( SGD::Key::O ) )
			// Ternary Blast activation
		{
			GameData::GetInstance()->PlaySelectionChange();
			// Ability Selecting copied from below, modified for TB
			help->UpdateSelection( 1 );
			states = 7;
			GamePlayState::GetInstance()->SetTernary( true );	// Ternary Blast time!

			for ( size_t i = 0; i < 4; i++ )
			{
				if ( abilityList[ i ]->GetUnlocked() )
				{
					menu[ i ]->SetAbility( abilityList[ i ] );
					menu[ i ]->SetObjectType( 1 );
					menu[ i ]->SetString( abilityList[ i ]->GetAbilityName() );
				}
				else
				{
					menu[ i ]->SetString( "" );
					menu[ i ]->SetObjectType( 0 );
				}
			}
			hudSelection = 0;
			help->UpdateSelection( 1, menu[ 0 ] );
			return;
		}
		// ****************** Ternary Blast Code END  *******************


		if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
		{
			unsigned int activeindex = 0;
			GameData::GetInstance()->PlaySelectionChange();
			switch ( hudSelection )
			{
				case 0: // Attack
					help->UpdateSelection( 3 );
					states = 1;
					for ( unsigned int i = 0; i < pTurn->GetEnemies().size(); ++i )
					{
						if ( pTurn->GetEnemies()[ i ]->isAlive() )
						{
							myTarget = i;
							break;

						}

					}


					mySelection = enemy;
					break;

				case 1: // Item
					myTarget = 0;
					help->UpdateSelection( 2 );
					states = 2;
					mySelection = none;
					help->UpdateSelection( 4 );
					item_choose = new ItemSelection();
					break;
				case 2: // Ability
					help->UpdateSelection( 1 );
					states = 3;
					for ( size_t i = 0; i < 4; i++ )
					{
						if ( abilityList[ i ]->GetUnlocked() )
						{
							menu[ i ]->SetAbility( abilityList[ i ] );
							menu[ i ]->SetObjectType( 1 );
							menu[ i ]->SetString( abilityList[ i ]->GetAbilityName() );
						}
						else
						{
							menu[ i ]->SetString( "" );
							menu[ i ]->SetObjectType( 0 );
						}
					}
					hudSelection = 0;
					help->UpdateSelection( 1, menu[ 0 ] );
					break;
				case 3: // RUN
					help->UpdateSelection( 6 );
					states = 4;
					mySelection = self;
					break;
			}
			return;
		}
		else if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
		{
			states = 0;
			myTarget = 0;
		}
	}
}
void CombatPlayer::AttackUpdate( float dt )
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	TargetUnit( pTurn->GetEnemies() );


	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		Attack( this, pTurn->GetEnemies()[ myTarget ] );
		//	}
		myTarget = 0;
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
		//TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		help->UpdateSelection( 5 );
		states = 0;
		mySelection = none;
		SetSelection( 0 );
	}
	else if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		SetSelection( 0 );
	}
}
void CombatPlayer::ItemsUpdate( float dt )
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	// **************** USER STORY COMPLETION FOR HEALING **************** //
	// **************** REPLACE WITH ITEM SELECTION CODE  **************** //
	// This code can be repurposed for actually using a healing item


	// Do player selection
	if ( item_choose != nullptr )
	{
		chosen = item_choose->chosen;
		delete item_choose;
		item_choose = nullptr;
		bool anyAllyDead = false;

	}
	//TargetUnit(pTurn->GetAllies());		// this is pretty sweet

	if ( chosen.GetName() == "nothing" )
	{
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		mySelection = none;
		states = 0;
		TurnManager::GetInstance()->setProgressFullReached( false );
		//pTurn->setTimeStop( false );
		pTurn->setTurnPause( true );
		hudSelection = 0;
		help->UpdateSelection( 5 );
		SetSelection( 0 );
		SetHomeButtons();
	}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
	{

		//pTurn->HealTarget(pTurn->GetAllies()[myTarget], 15);
		//this is where we say what the items do
		//Select(pTurn->GetAllies()[myTarget]);
		if ( chosen.GetName() == "Small Heal" )
		{
			pTurn->HealTarget( pTurn->GetAllies()[ myTarget ], 15 );
		}
		if ( chosen.GetName() == "Large Heal" )
		{
			pTurn->HealTarget( pTurn->GetAllies()[ myTarget ], 30 );
		}
		if ( chosen.GetName() == "Small BP Restore" )
		{
			int newBP = pTurn->GetAllies()[ myTarget ]->GetBP() + 3;
			if ( newBP > pTurn->GetAllies()[ myTarget ]->GetMaxBP() )
				newBP = pTurn->GetAllies()[ myTarget ]->GetMaxBP();
			pTurn->GetAllies()[ myTarget ]->SetBP( newBP );
		}
		if ( chosen.GetName() == "Large BP Restore" )
		{
			int newBP = pTurn->GetAllies()[ myTarget ]->GetBP() + 5;
			if ( newBP > pTurn->GetAllies()[ myTarget ]->GetMaxBP() )
				newBP = pTurn->GetAllies()[ myTarget ]->GetMaxBP();
			pTurn->GetAllies()[ myTarget ]->SetBP( newBP );
		}
		if ( chosen.GetName() == "Revive" )
		{
			pTurn->GetAllies()[ myTarget ]->SetLiving( true );
			pTurn->HealTarget( pTurn->GetAllies()[ myTarget ], (int)( pTurn->GetAllies()[ myTarget ]->GetHP() + 0.25 * MaxHP ) );

		}



		//reset chosen
		chosen = Items();

		// These three lines could be factored out to an EndTurn type function
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		mySelection = none;
		states = 0;
		TurnManager::GetInstance()->setProgressFullReached( false );
		//pTurn->setTimeStop( false );
		pTurn->setTurnPause( true );
		hudSelection = 0;
		help->UpdateSelection( 5 );
		SetSelection( 0 );
		SetHomeButtons();
		//ill keep this stuff here ^^^^
	}
	else if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();

	}
}
void CombatPlayer::AbilityUpdate( float dt )
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();
	bool anyAllyDead = false;

	if ( pInput->IsKeyPressed( SGD::Key::Up ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		hudSelection = 0;
		help->UpdateSelection( 1, menu[ 0 ] );

	}
	else if ( pInput->IsKeyPressed( SGD::Key::Right ) )
	{
		if ( menu[ 2 ]->GetObjectType() == 1 )
		{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 2;
			help->UpdateSelection( 1, menu[ 2 ] );

		}
	}
	else if ( pInput->IsKeyPressed( SGD::Key::Left ) )
	{
		if ( menu[ 1 ]->GetObjectType() == 1 )
		{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 1;
			help->UpdateSelection( 1, menu[ 1 ] );

		}
	}
	else if ( pInput->IsKeyPressed( SGD::Key::Down ) )
	{
		if ( menu[ 3 ]->GetObjectType() == 1 )
		{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 3;
			help->UpdateSelection( 1, menu[ 3 ] );

		}
	}

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();
		GamePlayState::GetInstance()->ClearTernary();

	}

	else if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
	{
		GameData::GetInstance()->PlaySelectionChange();
		if ( menu[ hudSelection ]->GetAbility() == nullptr )
		{
			return;
		}
		else if ( menu[ hudSelection ]->GetAbility()->GetBPCost() <= GetBP() )
		{
			if ( menu[ hudSelection ]->GetAbility()->GetOffensive() )//Enemy attack
			{
				if ( menu[ hudSelection ]->GetAbility()->GetAOE() ) // AOE attack
				{
					mySelection = allEnemy;
					myTarget = 0;
				}
				else
				{
					mySelection = enemy;
					myTarget = 0;
				}
				states = 6;
			}
			else // Ally Ability
			{
				if ( menu[ hudSelection ]->GetAbility()->GetAOE() ) // AOE attack
				{
					mySelection = allAlly;
					myTarget = 0;
				}
				else if ( menu[ hudSelection ]->GetAbility()->GetAbilityName() == "Second Wind" )
				{
					mySelection = deadAlly;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
					{
						if ( !TurnManager::GetInstance()->GetAllies()[ i ]->isAlive() )
						{
							myTarget = i;
							anyAllyDead = true;
							break;
						}
					}
					if ( !anyAllyDead )
					{
						mySelection = none;
						return;
					}
				}
				else if ( menu[ hudSelection ]->GetAbility()->GetSelfTarget() )
				{
					mySelection = self;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
					{
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[ i ]->GetName() )
						{
							myTarget = i;
							break;
						}
					}
				}
				else
				{
					mySelection = player;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
					{
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[ i ]->GetName() )
						{
							myTarget = i;
							break;
						}
					}
				}
				states = 5;
			}
		}
	}
	for ( unsigned int i = 0; i < menu.size(); i++ )
	{
		if ( i == hudSelection )
			GamePlayState::GetInstance()->GetSelectableObjects()[ i ]->SetSelected( true );
		else
			GamePlayState::GetInstance()->GetSelectableObjects()[ i ]->SetSelected( false );
	}
}
void CombatPlayer::RunUpdate( float dt )
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();

	}




	//if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) )
	//{
	//	// Here is where we decide Attack, or Ability etc
	//	Attack( pTurn->GetEnemies()[ myTarget ] );
	//	myTarget = 0;
	//	progress = 0.0f;
	//	TurnManager::GetInstance()->setTimeStop( false );
	//}
}
void CombatPlayer::AllySelectUpdate( float dt ) // Defensive ability use
{
	//we go into itemupdate if we need to

	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( mySelection == player || mySelection == deadAlly || mySelection == self)
	{
		TargetUnit( pTurn->GetAllies() );
	}

	if ( chosen.GetName() != "nothing" )
	{
		ItemsUpdate( dt );
		return;
	}
	if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();
		GamePlayState::GetInstance()->ClearTernary();

	}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
	{
		//Input Ability Use Here
		if ( mySelection == allAlly )
		{
			for ( size_t i = 0; i < pTurn->GetAllies().size(); i++ )
			{
				if ( pTurn->GetAllies()[ i ]->isAlive() )
				{
					if ( GamePlayState::GetInstance()->usingTernary() )
					{
						GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetAllies()[ i ] );
					}

					else
					{
						menu[ hudSelection ]->GetAbility()->CastAbility( this, pTurn->GetAllies()[ i ], i );
						GamePlayState::GetInstance()->AbilityUsed = true;
						GamePlayState::GetInstance()->CurrentAbilityUsed = menu[ hudSelection ]->GetAbility();
						GamePlayState::GetInstance()->abilityTimer = 2.0f;
						GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->ResetAll();
						GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->Play( 0 );
					}
				}
			}

			// After adding all the aoe casts to the Ternary vectors
			if ( GamePlayState::GetInstance()->usingTernary() )
			{
				GamePlayState::GetInstance( )->AddTarget( );
				//GamePlayState::GetInstance()->myTernTargets.num_targets -= ( numAlive - 1 );	// Should adjust for AOE
				// Reset certain targety things
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
			}
		}
		else if ( mySelection == player || mySelection == self || mySelection == deadAlly )
		{
			if ( GamePlayState::GetInstance()->usingTernary() )
			{
				// Add selection to vector
				GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetAllies()[ myTarget ] );
				GamePlayState::GetInstance( )->AddTarget( );
				// Reset certain targety things
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
			}

			else
			{
				menu[ hudSelection ]->GetAbility()->CastAbility( this, pTurn->GetAllies()[ myTarget ] );
				//TurnManager::GetInstance( )->UsingAbility( this, pTurn->GetAllies( )[ myTarget ], menu[ hudSelection ]->GetAbility( ) );
				GamePlayState::GetInstance()->AbilityUsed = true;
				GamePlayState::GetInstance()->CurrentAbilityUsed = menu[ hudSelection ]->GetAbility();
				GamePlayState::GetInstance()->abilityTimer = 2.0f;
				GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->ResetAll();
				GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->Play( 0 );
			}
		}
		myTarget = 0;
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
	//	TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		help->UpdateSelection( 5 );
		states = 0;
		mySelection = none;
		SetSelection( 0 );
		SetHomeButtons();

	}
}
void CombatPlayer::EnemySelectUpdate( float dt ) // Offensive Ability use
{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( mySelection == enemy )
	{
		TargetUnit( pTurn->GetEnemies() );
	}

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
	{
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );

		SetHomeButtons();
		GamePlayState::GetInstance( )->ClearTernary( );
	}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
	{
		//Input Ability Use Here
		if ( mySelection == allEnemy )
		{
			for ( size_t i = 0; i < pTurn->GetEnemies().size(); i++ )
			{
				if ( pTurn->GetEnemies()[ i ]->isAlive() )
				{	
					if ( GamePlayState::GetInstance()->usingTernary() )
					{
						// Add selection to vector
						GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetEnemies()[ i ] );
					}

					else
					{
						menu[ hudSelection ]->GetAbility()->CastAbility( this, pTurn->GetEnemies()[ i ], i );
						GamePlayState::GetInstance()->AbilityUsed = true;
						GamePlayState::GetInstance()->CurrentAbilityUsed = menu[ hudSelection ]->GetAbility();
						GamePlayState::GetInstance()->abilityTimer = 2.0f;
						GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->ResetAll();
						GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->Play( 0 );
					}
				}
			}

			if ( GamePlayState::GetInstance()->usingTernary() )
			{
				GamePlayState::GetInstance( )->AddTarget( );
				//GamePlayState::GetInstance()->myTernTargets.num_targets -= ( numAlive - 1 );	// Should adjust for AOE
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
			}
		}

		else if ( mySelection == enemy )
		{
			if ( GamePlayState::GetInstance()->usingTernary() )
			{
				// Add selection to vector
				GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetEnemies()[ myTarget ] );
				GamePlayState::GetInstance( )->AddTarget( );
				// Reset certain targety things
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
			}

			else
			{
				// Turn Manager will handle the go between for abilities and attacks for consistency and particles
				TurnManager::GetInstance()->UsingAbility( this, pTurn->GetEnemies()[ myTarget ], menu[ hudSelection ]->GetAbility() );
				GamePlayState::GetInstance()->AbilityUsed = true;
				GamePlayState::GetInstance()->CurrentAbilityUsed = menu[ hudSelection ]->GetAbility();
				GamePlayState::GetInstance()->abilityTimer = 2.0f;
				GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->ResetAll();
				GamePlayState::GetInstance()->CurrentAbilityUsed->GetAnimate()->Play( 0 );
			}
		}

		myTarget = 0;
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
		//TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		help->UpdateSelection( 5 );
		states = 0;
		mySelection = none;
		SetSelection( 0 );
		SetHomeButtons();

	}
}
void CombatPlayer::SetHomeButtons()
{
	for ( size_t i = 0; i < 4; i++ )
	{
		menu[ i ]->SetObjectType( 0 );
		menu[ i ]->SetAbility( nullptr );
		menu[ i ]->SetItem( nullptr );
	}
	menu[ 0 ]->SetString( "Attack" );
	menu[ 1 ]->SetString( "Item" );
	menu[ 2 ]->SetString( "Ability" );
	menu[ 3 ]->SetString( "Run" );

}

void CombatPlayer::SelectingItems( float dt )
{
	HelpText *help = GamePlayState::GetInstance()->GetHelpText();


	if ( item_choose->Update( dt ) && item_choose != nullptr )
	{
		chosen = item_choose->chosen;
		delete item_choose;
		item_choose = nullptr;
		bool anyAllyDead = false;
		if ( chosen.GetName() == "Revive" )
		{

			mySelection = deadAlly;
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
				if ( !TurnManager::GetInstance()->GetAllies()[ i ]->isAlive() )
				{
					myTarget = i;
					anyAllyDead = true;
					states = 5;
					break;
				}
			}
			if ( !anyAllyDead )
			{
				mySelection = none;
				return;
			}
		}
		else
		{
			mySelection = player;
			states = 5;

		}
		if (chosen.GetName() == "nothing")
		{
			delete item_choose;
			item_choose = nullptr;
			GameData::GetInstance()->PlaySelectionChange();
			states = 0;
			help->UpdateSelection(0, GamePlayState::GetInstance()->GetSelectableObjects()[0]);
			mySelection = none;
			SetSelection(0);
		}

	}
	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Escape ) )
	{
		delete item_choose;
		item_choose = nullptr;
		GameData::GetInstance()->PlaySelectionChange();
		mySelection = none;
		states = 0;
		TurnManager::GetInstance()->setProgressFullReached(false);
		//pTurn->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause(true);
		hudSelection = 0;
		help->UpdateSelection(5);
		SetSelection(0);
		SetHomeButtons();
	}
	

}

void CombatPlayer::TernaryBlast( float dt )
// states 7
// Handles selection for ternary blast
{
	// Ability Selection 3 times without ending turn or executing ability
	// Make sure Escape works correctly while selecting in here
	int tar = GamePlayState::GetInstance( )->myTernTargets.num_targets;
	if ( tar < 3 )
		AbilityUpdate( dt );
	else
		// Selected all the targets, cast them and get outta dodge
	{
		// For ability pausing type stuff, need to refactor this function, for now all abilities blitz the field
		for ( unsigned int i = 0; i < GamePlayState::GetInstance()->myTernTargets.abilities.size(); i++ )
		{
			TurnManager::GetInstance()->UsingAbility(
				this,
				GamePlayState::GetInstance()->myTernTargets.targets[ i ],
				GamePlayState::GetInstance()->myTernTargets.abilities[ i ],
				true);	// true for ternary stuff	
			ApplyTernary( GamePlayState::GetInstance( )->myTernTargets.abilities[ i ]->GetAbilityName(),
						  GamePlayState::GetInstance( )->myTernTargets.targets[ i ]);

			GamePlayState::GetInstance()->AbilityUsed = true;
			GamePlayState::GetInstance()->CurrentAbilityUsed = menu[ hudSelection ]->GetAbility();
			GamePlayState::GetInstance()->abilityTimer = 2.0f;
		}

		GamePlayState::GetInstance()->ClearTernary();
		myTarget = 0;
		if (stepbackward == false && stepforward == false && progress != 0.0f)
		{
			stepbackward = true;
			stepTime = 2.0f;
			progress = 0.0f;
		}
		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
		//TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 5 );
		states = 0;
		mySelection = none;
		SetSelection( 0 );
		SetHomeButtons();
		GamePlayState::GetInstance()->SetTernary( false );
		GamePlayState::GetInstance()->SetGauge( 0 );
	}
}

int CombatPlayer::GetXP( )
{
	return XP;
}

int CombatPlayer::GetLevel( )
{
	return level;
}

//int CombatPlayer::GetNextlvl( )
//{
//	return nextlvl;
//}

void CombatPlayer::AddXP( int xp )
{
	XP += xp;
	if(XP >= nextlvl)
	{
		LevelUp();
	}
}

void CombatPlayer::LevelUp( )
{
	level++;
	XP -= nextlvl;
	nextlvl = level * 150 + 100;
}



// Do Ternary Blast Specials
void CombatPlayer::ApplyTernary(std::string abil, Character* target)
{
	if(abil == "Fire Fang")
	{
		target->AddStatus(&StatusEffectManager::GetInstance()->GetStatus("Burn"));
	}

	else if(abil == "Water Fang")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Drown" ) );
	}

	else if(abil == "Earth Fang")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Muck" ) );
	}

	else if(abil == "Wind Fang")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Wisp" ) );
	}

	else if(abil == "Poison Fang")
	{
		// add delerium once it works
	}

	else if(abil == "Slow Claw")
	{
		// Knock them back on the timeline
		float prog = target->GetProgress();
		prog -= 33;
		if(prog < 0) prog = 0.0f;
		target->SetProgress(prog);
	}

	else if(abil == "Splash")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "SpeedDown" ) );
	}

	else if(abil == "Flood")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "AvoisionDown" ) );
	}

	else if(abil == "Squirt")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Stun" ) );
	}

	else if(abil == "Torrent")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "AttackDown" ) );
	}

	else if(abil == "Puddle")
	{
		this->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "DefenseUp" ) );
	}
	
	else if(abil == "Disolve")
	{
		this->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "AttackUp" ) );
	}

	else if(abil == "Ignite")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Burn" ) );
	}

	else if(abil == "Incinerate")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Burn" ) );
	}

	else if(abil == "Scorch")
	{
		for(unsigned int i=0; i<TurnManager::GetInstance()->GetAllies().size(); i++)
		{
			if(TurnManager::GetInstance( )->GetAllies( )[i]->isAlive())
			{
				TurnManager::GetInstance( )->GetAllies( )[i]->AddStatus(
					&StatusEffectManager::GetInstance()->GetStatus("AttackUp"));
			}
		}
	}

	else if(abil == "Tornado")
	{
		target->SetProgress(0.0f);
	}

	else if(abil == "Tempest")
	{
		target->SetProgress(0.0f);
	}

	else if(abil == "Zephyr")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Regen" ) );
	}

	else if(abil == "Rock Spike")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "MagicDown" ) );
	}

	else if(abil == "Geo Crush")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "SpeedDown" ) );
	}

	else if(abil == "Tremor")
	{
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "DefenseDown" ) );
	}

	else if(abil == "Quake")
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "AttackDown" ) );

	else if(abil == "Pinch")
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Muck" ) );

	else if(abil == "Rampart")
		target->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "MagicUp" ) );
}