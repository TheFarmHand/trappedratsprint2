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
#include "../SGD Wrappers/SGD_AudioManager.h"


CombatPlayer::CombatPlayer()
	{
	myTarget = 0;
	states = 0;
	hudSelection = 0;
	menu = GamePlayState::GetInstance()->GetSelectableObjects();
	tempitem = new Items();
	helpobject = new SelectableObject();

	}


CombatPlayer::~CombatPlayer()
	{
	if ( ansys != nullptr )
		delete ansys;
	delete tempitem;
	delete helpobject;
	}

void CombatPlayer::Update( float dt )
	{
	if ( !alive )
		return;

	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
		progress += GetSpeed() * dt;
	

	else if ( progress < 100.0f )
		{
		if ( stepbackward == true && stepforward == false )
			{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x -= stepvelocity*dt;
			SetPosition( pos );
			if ( stepTime <= 0.0f )
				stepbackward = false;
			}
		if ( stepbackward == false && stepforward == false )
			{
			if ( GamePlayState::GetInstance()->tutorial_incrememnt < 4 )
				GamePlayState::GetInstance()->SetState( GPStates::Dia );
			TurnManager::GetInstance()->setProgressFullReached( false );
			TurnManager::GetInstance()->setTimeStop( false );
			if ( runaway )
				{
				GamePlayState::GetInstance()->run_succeed = true;
				runaway = false;
				}
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
				help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
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

			if ( stepforward == false && stepbackward == false && stepTime != 0.0f )
				{
				stepforward = true;
				stepTime = 0.50f;
				}
			if ( stepforward == true && stepbackward == false )
				{
				stepTime -= dt;
				SGD::Point pos = GetPosition();
				pos.x += stepvelocity*dt;
				SetPosition( pos );
				if ( stepTime <= 0.0f )
					{
					stepTime = 0.0f;
					stepforward = false;

					// Stun Goes here
					}
				}

			if ( stepbackward == false && stepforward == false )
				{
				if ( GamePlayState::GetInstance()->ignore_game_over && ( states != 5 && states != 6 && states != 7 && states != 3 ) )
					{
					switch ( GamePlayState::GetInstance()->tutorial_incrememnt )
						{
						case 0:
							//here we make sure they can only attack
							SetSelection( 0 );
							help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
							hudSelection = 0;
							break;
						case 1:
							//here we make sure they can only use abilities
							SetSelection( 2 );
							help->UpdateSelection( 2, GamePlayState::GetInstance()->GetSelectableObjects()[2] );
							hudSelection = 2;
							break;
						case 2:
							//here we make sure they can only use items
							SetSelection( 1 );
							help->UpdateSelection( 1, GamePlayState::GetInstance()->GetSelectableObjects()[1] );
							hudSelection = 1;
							break;
						case 3:
							//here we force them to use run
							SetSelection( 3 );
							help->UpdateSelection( 3, GamePlayState::GetInstance()->GetSelectableObjects()[3] );
							hudSelection = 3;
							break;
						case 4:
							//here they can do whatever they want
							//or maybe not i dunno
							break;
						}
					}
				if ( states == 0 )
					{
					HomeUpdate( dt );
					}

				else if ( states == 1 )		// Attacking; Target selection is "enemy"
					{
					AttackUpdate( dt );
					}

				else if ( states == 2 ) // Item Selection
					{
					if ( item_choose == nullptr )
						ItemsUpdate( dt );
					else
						SelectingItems( dt );

					}
				else if ( states == 3 )// Ability Selection
					{

					AbilityUpdate( dt );
					}
				else if ( states == 4 ) // Run update
					{
					RunUpdate( dt );
					}
				else if ( states == 5 )//Defensive Ability update
					{
					AllySelectUpdate( dt );
					}
				else if ( states == 6 ) // Offensive ability update
					{
					EnemySelectUpdate( dt );
					}
				else if ( states == 7 ) // Ternary Blast stuff
					{
					TernaryBlast( dt );
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

	/*GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 51, position.y - 30, { 0, 0, 0 }, 1.6f );
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
	GameData::GetInstance()->GetFont()->DrawString( ss.str(), position.x + 50, position.y + 45, { 0, 0, 0 } );*/

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
			pdraw = pTurn->GetEnemies()[myTarget]->GetPosition();
			pdraw.x -= pTurn->GetEnemies()[myTarget]->GetSize().width + 20;
			pdraw.y -= pTurn->GetEnemies()[myTarget]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetEnemyArrow(), pdraw );
			}

		else if ( mySelection == player || mySelection == self || mySelection == deadAlly )
			// enforce self target by disallowing targeting input in update
			{
			pdraw = pTurn->GetAllies()[myTarget]->GetPosition();
			pdraw.x += pTurn->GetAllies()[myTarget]->GetSize().width + 20;
			pdraw.y -= pTurn->GetAllies()[myTarget]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetAllyArrow(), pdraw );
			}
		else if ( mySelection == allEnemy )
			{
			for ( size_t i = 0; i < pTurn->GetEnemies().size(); i++ )
				{
				if ( pTurn->GetEnemies()[i]->isAlive() )
					{
					pdraw = pTurn->GetEnemies()[i]->GetPosition();
					pdraw.x -= pTurn->GetEnemies()[i]->GetSize().width + 20;
					pdraw.y -= pTurn->GetEnemies()[i]->GetSize().height / 2;
					pGraphics->DrawTexture( pTurn->GetEnemyArrow(), pdraw );
					}
				}
			}
		else if ( mySelection == allAlly )
			{
			for ( size_t i = 0; i < pTurn->GetAllies().size(); i++ )
				{
				if ( pTurn->GetAllies()[i]->isAlive() )
					{
					pdraw = pTurn->GetAllies()[i]->GetPosition();
					pdraw.x += pTurn->GetAllies()[i]->GetSize().width + 20;
					pdraw.y -= pTurn->GetAllies()[i]->GetSize().height / 2;
					pGraphics->DrawTexture( pTurn->GetAllyArrow(), pdraw );
					}
				}
			}

		}

	SGD::Color color;
	float hppercentage = GetHP() / (float)( GetMaxHP() );
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
		if ( GamePlayState::GetInstance()->AbilityUsed )
			{
			if ( ( GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName() == "Burrow" || GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName() == "Puddle" || GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName() == "Collapse" ) && GamePlayState::GetInstance()->CurrentAbilityUsed->GetOwner() == this )
				{

				}
			else
				{
				ansys->Render( position.x, position.y );
				}
			}
		else
			{
			ansys->Render( position.x, position.y );
			}

		}

	Character::Render();

	if ( item_choose != nullptr )
		item_choose->Render();
	}


void CombatPlayer::Attack( Character* owner, Character * target )
	{
	if ( target != nullptr )
		{
		int atk = owner->GetAttack();
		int dmg = rand() % atk + atk;
		dmg -= (int)( 0.25f * target->GetStats().defense );
		if ( dmg <= 0 )
			dmg = 0;
		TurnManager::GetInstance()->AttackTarget( owner, target, dmg );
		//owner->TakeDamage( 100, target );
		}
	}

void CombatPlayer::UseItem( Character * target, Items item )
	{
	// Here we take in the item check what it does, then send the effects to the target
	// Or we leave it blank
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
			GamePlayState::GetInstance()->GetSelectableObjects()[i]->SetSelected( true );
		else
			GamePlayState::GetInstance()->GetSelectableObjects()[i]->SetSelected( false );
		}
	//GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( , GamePlayState::GetInstance()->GetSelectableObjects()[ selected ] );
	}

void CombatPlayer::TargetUnit( std::vector<Character*> &targets )
	{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Find yoursel
	if ( mySelection == self )
		{
		myTarget = 0;
		while ( targets[myTarget] != this )
			myTarget++;
		}

	// Input
	else if ( mySelection == deadAlly )
		{
		if ( ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Left ) || pInput->GetLeftJoystick( 0 ).y < 0 || pInput->GetLeftJoystick( 0 ).x < 0 || pInput->IsDPadPressed( 0, SGD::DPad::Up ) || pInput->IsDPadPressed( 0, SGD::DPad::Left ) ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->GameData::GetInstance()->input_timer = 0.15f;
			GameData::GetInstance()->PlaySelectionChange();
			--myTarget;
			if ( myTarget < 0 )
				myTarget = targets.size() - 1;
			while ( targets[myTarget]->isAlive() )
				{
				--myTarget;
				if ( myTarget < 0 )
					myTarget = targets.size() - 1;
				}
			}
		else if ( ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Right ) || pInput->GetLeftJoystick( 0 ).y > 0 || pInput->GetLeftJoystick( 0 ).x > 0 || pInput->IsDPadPressed( 0, SGD::DPad::Down ) || pInput->IsDPadPressed( 0, SGD::DPad::Right ) ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->input_timer = 0.15f;;
			GameData::GetInstance()->PlaySelectionChange();
			++myTarget;
			if ( myTarget > ( int )targets.size() - 1 )
				myTarget = 0;
			while ( targets[myTarget]->isAlive() )
				{
				++myTarget;
				if ( myTarget > (int)targets.size() - 1 )
					myTarget = 0;
				}
			}

		if ( myTarget < 0 ) myTarget = 0;
		}

	else if ( ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Left ) || pInput->GetLeftJoystick( 0 ).y < 0 || pInput->GetLeftJoystick( 0 ).x < 0 || pInput->IsDPadPressed( 0, SGD::DPad::Up ) || pInput->IsDPadPressed( 0, SGD::DPad::Left ) ) && GameData::GetInstance()->input_timer < 0 )
		{
		GameData::GetInstance()->input_timer = 0.15f;
		GameData::GetInstance()->PlaySelectionChange();
		--myTarget;
		if ( myTarget < 0 )
			myTarget = targets.size() - 1;

		while ( !targets[myTarget]->isAlive() )
			{
			--myTarget;
			if ( myTarget < 0 )
				myTarget = targets.size() - 1;
			}

		if ( myTarget < 0 ) myTarget = 0;
		}

	else if ( ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Right ) || pInput->GetLeftJoystick( 0 ).y > 0 || pInput->GetLeftJoystick( 0 ).x > 0 || pInput->IsDPadPressed( 0, SGD::DPad::Down ) || pInput->IsDPadPressed( 0, SGD::DPad::Right ) ) && GameData::GetInstance()->input_timer < 0 )
		{
		GameData::GetInstance()->input_timer = 0.15f;
		GameData::GetInstance()->PlaySelectionChange();
		++myTarget;

		if ( myTarget > ( int )targets.size() - 1 )
			{
			myTarget = 0;
			}
		while ( !targets[myTarget]->isAlive() )
			{
			++myTarget;
			if ( myTarget > (int)targets.size() - 1 )
				{
				myTarget = 0;
				}

			}

		// Shouldn't ever need this check, but safety first
		if ( myTarget > (int)targets.size() - 1 ) myTarget = ( (int)targets.size() - 1 );


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

	if ( GamePlayState::GetInstance()->MinibossFight || GamePlayState::GetInstance()->FinalBossFight || GamePlayState::GetInstance()->ignore_game_over )
		{
		GamePlayState::GetInstance()->GetSelectableObjects()[3]->SetExplination( "You are unable to run from this battle" );
		}
	else
		{
		GamePlayState::GetInstance()->GetSelectableObjects()[3]->SetExplination( "Attempt to run from this battle" );
		}

	if ( states == 0 )
		{
		if ( ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsDPadPressed( 0, SGD::DPad::Up ) || pInput->GetLeftJoystick( 0 ).y < 0 ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->input_timer = 0.15f;
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 0 );
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			hudSelection = 0;

			}
		else if ( ( pInput->IsKeyPressed( SGD::Key::Right ) || pInput->IsDPadPressed( 0, SGD::DPad::Right ) || pInput->GetLeftJoystick( 0 ).x > 0 ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->input_timer = 0.15f;
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 2 );
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[2] );
			hudSelection = 2;
			}
		else if ( ( pInput->IsKeyPressed( SGD::Key::Left ) || pInput->IsDPadPressed( 0, SGD::DPad::Left ) || pInput->GetLeftJoystick( 0 ).x < 0 ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->input_timer = 0.15f;
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 1 );
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[1] );
			hudSelection = 1;

			}
		else if ( ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsDPadPressed( 0, SGD::DPad::Down ) || pInput->GetLeftJoystick( 0 ).y > 0 ) && GameData::GetInstance()->input_timer < 0 )
			{
			GameData::GetInstance()->input_timer = 0.15f;
			GameData::GetInstance()->PlaySelectionChange();
			SetSelection( 3 );
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[3] );
			hudSelection = 3;
			}


		// ****************** Ternary Blast Code added by Grant *******************
		else if ( GamePlayState::GetInstance()->GetGauge() >= MAXTG && pInput->IsKeyPressed( SGD::Key::O ) || pInput->IsButtonPressed( 0, 5 ) )
			// Ternary Blast activation
			{
			GameData::GetInstance()->PlaySelectionChange();
			// Ability Selecting copied from below, modified for TB
			help->UpdateSelection( 1 );
			states = 7;
			GamePlayState::GetInstance()->SetTernary( true );	// Ternary Blast time!

			for ( size_t i = 0; i < 4; i++ )
				{
				if ( abilityList[i]->GetUnlocked() )
					{
					menu[i]->SetAbility( abilityList[i] );
					menu[i]->SetObjectType( 1 );
					menu[i]->SetString( abilityList[i]->GetAbilityName() );
					}
				else
					{
					menu[i]->SetString( "" );
					menu[i]->SetObjectType( 0 );
					}
				}
			hudSelection = 0;
			help->UpdateSelection( 1, menu[0] );
			return;
			}
		// ****************** Ternary Blast Code END  *******************


		if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
			{
			unsigned int activeindex = 0;
			GamePlayState::GetInstance()->PlaySoundEffect( 6 );
			switch ( hudSelection )
				{
				case 0: // Attack
					help->UpdateSelection( 3 );
					states = 1;
					for ( unsigned int i = 0; i < pTurn->GetEnemies().size(); ++i )
						{
						if ( pTurn->GetEnemies()[i]->isAlive() )
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
					mySelection = player;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
						{
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[i]->GetName() )
							{
							myTarget = i;
							break;
							}
						}
					help->UpdateSelection( 4 );
					item_choose = new ItemSelection();
					break;
				case 2: // Ability
					help->UpdateSelection( 1 );
					states = 3;
					for ( size_t i = 0; i < 4; i++ )
						{
						if ( abilityList[i]->GetUnlocked() )
							{
							menu[i]->SetAbility( abilityList[i] );
							menu[i]->SetObjectType( 1 );
							menu[i]->SetString( abilityList[i]->GetAbilityName() );
							}
						else
							{
							menu[i]->SetString( "" );
							menu[i]->SetObjectType( 0 );
							}
						}

					hudSelection = 0;
					help->UpdateSelection( 1, menu[0] );
					break;
				case 3: // RUN
					help->UpdateSelection( 6 );
					states = 4;
					mySelection = self;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
						{
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[i]->GetName() )
							{
							myTarget = i;
							break;
							}
						}
					break;
				}
			return;
			}
		else if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
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


	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{
		if ( GamePlayState::GetInstance()->is_done&& GamePlayState::GetInstance()->ignore_game_over )
			{
			GamePlayState::GetInstance()->tutorial_incrememnt = 1;
			GamePlayState::GetInstance()->GetDialogue()->Load( "Assets/Scripts/abilitiestutorial.xml" );
			}
		//GamePlayState::GetInstance()->PlaySoundEffect(6);
		Attack( this, pTurn->GetEnemies()[myTarget] );
		//	}
		myTarget = 0;
		if ( stepbackward == false && stepforward == false && progress != 0.0f )
			{
			stepbackward = true;
			stepTime = .50f;
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
		GamePlayState::GetInstance()->PlaySoundEffect( 1 );

		}
	else if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		GameData::GetInstance()->PlaySelectionChange();
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
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
		if ( stepbackward == false && stepforward == false && progress != 0.0f )
			{
			stepbackward = true;
			stepTime = .50f;
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

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{

		//GamePlayState::GetInstance()->PlaySoundEffect(7);
		//pTurn->HealTarget(pTurn->GetAllies()[myTarget], 15);
		//this is where we say what the items do
		//Select(pTurn->GetAllies()[myTarget]);
		if ( chosen.GetName() == "Small Heal" )
			{
			pTurn->HealTarget( pTurn->GetAllies()[myTarget], (int)( pTurn->GetAllies()[myTarget]->GetMaxHP() * 0.20f ) );
			GamePlayState::GetInstance()->itemTimer = 2.0f;
			GamePlayState::GetInstance()->ItemUsed = true;
			std::ostringstream usingItem;
			usingItem << name << " uses " << chosen.GetName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingItem.str(), this );
			}
		if ( chosen.GetName() == "Large Heal" )
			{
			pTurn->HealTarget( pTurn->GetAllies()[myTarget], (int)( pTurn->GetAllies()[myTarget]->GetMaxHP() * 0.45f ) );
			GamePlayState::GetInstance()->itemTimer = 2.0f;
			GamePlayState::GetInstance()->ItemUsed = true;
			std::ostringstream usingItem;
			usingItem << name << " uses " << chosen.GetName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingItem.str(), this );
			}
		if ( chosen.GetName() == "Small BP Restore" )
			{
			int newBP = pTurn->GetAllies()[myTarget]->GetBP() + (int)( pTurn->GetAllies()[myTarget]->GetMaxBP() * 0.20f );
			if ( newBP > pTurn->GetAllies()[myTarget]->GetMaxBP() )
				newBP = pTurn->GetAllies()[myTarget]->GetMaxBP();
			pTurn->GetAllies()[myTarget]->SetBP( newBP );
			GamePlayState::GetInstance()->itemTimer = 2.0f;
			GamePlayState::GetInstance()->ItemUsed = true;
			std::ostringstream usingItem;
			usingItem << name << " uses " << chosen.GetName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingItem.str(), this );
			}
		if ( chosen.GetName() == "Large BP Restore" )
			{
			int newBP = pTurn->GetAllies()[myTarget]->GetBP() + (int)( pTurn->GetAllies()[myTarget]->GetMaxBP() * 0.45f );
			if ( newBP > pTurn->GetAllies()[myTarget]->GetMaxBP() )
				newBP = pTurn->GetAllies()[myTarget]->GetMaxBP();
			pTurn->GetAllies()[myTarget]->SetBP( newBP );
			GamePlayState::GetInstance()->itemTimer = 2.0f;
			GamePlayState::GetInstance()->ItemUsed = true;
			std::ostringstream usingItem;
			usingItem << name << " uses " << chosen.GetName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingItem.str(), this );
			}
		if ( chosen.GetName() == "Revive" )
			{
			pTurn->GetAllies()[myTarget]->SetLiving( true );
			dynamic_cast<CombatPlayer*>( pTurn->GetAllies()[myTarget] )->GetAnimations()->Play( 0 );
			pTurn->HealTarget( pTurn->GetAllies()[myTarget], (int)( pTurn->GetAllies()[myTarget]->GetHP() + 0.25 * GetMaxHP() ) );
			GamePlayState::GetInstance()->itemTimer = 2.0f;
			GamePlayState::GetInstance()->ItemUsed = true;
			std::ostringstream usingItem;
			usingItem << name << " uses " << chosen.GetName();
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingItem.str(), this );
			}
		GamePlayState::GetInstance()->PlaySoundEffect( 6 );


		//reset chosen
		chosen = Items();

		// These three lines could be factored out to an EndTurn type function
		if ( stepbackward == false && stepforward == false && progress != 0.0f )
			{
			stepbackward = true;
			stepTime = .50f;
			progress = 0.0f;
			}
		mySelection = none;
		states = 0;
		TurnManager::GetInstance()->setProgressFullReached( false );
		//pTurn->setTimeStop( false );
		pTurn->setTurnPause( true );
		hudSelection = 0;
		//help->UpdateSelection( 5 );
		SetSelection( 0 );
		SetHomeButtons();
		//ill keep this stuff here ^^^^
		}
	else if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
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

	if ( ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Up ) || pInput->GetLeftJoystick( 0 ).y < 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
		GameData::GetInstance()->PlaySelectionChange();
		hudSelection = 0;
		help->UpdateSelection( 1, menu[0] );
		GameData::GetInstance()->input_timer = 0.15f;
		}
	else if ( ( pInput->IsKeyPressed( SGD::Key::Right ) || pInput->IsKeyPressed( SGD::Key::Right ) || pInput->GetLeftJoystick( 0 ).x > 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
		if ( menu[2]->GetObjectType() == 1 )
			{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 2;
			help->UpdateSelection( 1, menu[2] );
			GameData::GetInstance()->input_timer = 0.15f;
			}
		}
	else if ( ( pInput->IsKeyPressed( SGD::Key::Left ) || pInput->IsKeyPressed( SGD::Key::Left ) || pInput->GetLeftJoystick( 0 ).x < 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
		if ( menu[1]->GetObjectType() == 1 )
			{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 1;
			help->UpdateSelection( 1, menu[1] );
			GameData::GetInstance()->input_timer = 0.15f;
			}
		}
	else if ( ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Down ) || pInput->GetLeftJoystick( 0 ).y > 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
		if ( menu[3]->GetObjectType() == 1 )
			{
			GameData::GetInstance()->PlaySelectionChange();
			hudSelection = 3;
			help->UpdateSelection( 1, menu[3] );
			GameData::GetInstance()->input_timer = 0.15f;
			}
		}

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		if ( GamePlayState::GetInstance()->usingTernary() )
			{
			if ( GamePlayState::GetInstance()->RemoveTarget() )
				{
				GamePlayState::GetInstance()->DeselectTernaryTarget();
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
				}
			}

		GameData::GetInstance()->PlaySelectionChange();
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();
		GamePlayState::GetInstance()->ClearTernary();
		GamePlayState::GetInstance()->SetTernary( false );

		}

	else if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{
		if ( GamePlayState::GetInstance()->is_done&& GamePlayState::GetInstance()->ignore_game_over )
			{
			GamePlayState::GetInstance()->tutorial_incrememnt = 2;
			GamePlayState::GetInstance()->GetDialogue()->Load( "Assets/Scripts/itemstutorial.xml" );
			GamePlayState::GetInstance()->PlaySoundEffect( 6 );
			}
		if ( menu[hudSelection]->GetAbility() == nullptr )
			{
			return;
			}
		else if ( menu[hudSelection]->GetAbility()->GetBPCost() <= GetBP() )
			{
			if ( menu[hudSelection]->GetAbility()->GetOffensive() )//Enemy attack
				{
				if ( menu[hudSelection]->GetAbility()->GetAOE() ) // AOE attack
					{
					mySelection = allEnemy;
					myTarget = 0;
					}
				else
					{
					mySelection = enemy;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
						{
						if ( TurnManager::GetInstance()->GetEnemies()[i]->isAlive() )
							{
							myTarget = i;
							break;
							}
						}
					}
				states = 6;
				}
			else // Ally Ability
				{
				if ( menu[hudSelection]->GetAbility()->GetAOE() ) // AOE attack
					{
					mySelection = allAlly;
					myTarget = 0;
					}
				else if ( menu[hudSelection]->GetAbility()->GetAbilityName() == "Second Wind" )
					{
					mySelection = deadAlly;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
						{
						if ( !TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
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
				else if ( menu[hudSelection]->GetAbility()->GetSelfTarget() )
					{
					mySelection = self;
					for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
						{
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[i]->GetName() )
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
						if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[i]->GetName() )
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
			GamePlayState::GetInstance()->GetSelectableObjects()[i]->SetSelected( true );
		else
			GamePlayState::GetInstance()->GetSelectableObjects()[i]->SetSelected( false );
		}
	}
void CombatPlayer::RunUpdate( float dt )
	{
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();

		}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{
		if ( GamePlayState::GetInstance()->is_done && GamePlayState::GetInstance()->ignore_game_over )
			{

			GamePlayState::GetInstance()->tutorial_incrememnt = 4;
			GamePlayState::GetInstance()->GetDialogue()->Load( "Assets/Scripts/cantrunlol.xml" );
			GamePlayState::GetInstance()->SetState( GPStates::Dia );
			GamePlayState::GetInstance()->is_done = false;
			}
		if ( GamePlayState::GetInstance()->MinibossFight || GamePlayState::GetInstance()->FinalBossFight || GamePlayState::GetInstance()->ignore_game_over )
			{
			// You can't run away
			GamePlayState::GetInstance()->PlaySoundEffect( 4 );
			mySelection = none;
			states = 0;
			//TurnManager::GetInstance()->setProgressFullReached(false);
			//pTurn->setTimeStop( false );
			//TurnManager::GetInstance()->setTurnPause(true);
			hudSelection = 0;
			//help->UpdateSelection(5);
			SetSelection( 0 );
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			SetHomeButtons();
			return;
			}
		int chance = rand() % 1000;
		if ( chance < GetAvoision() * 250 )
			{
			//GamePlayState::GetInstance()->run_succeed = true;
			runaway = true;

			GamePlayState::GetInstance()->runTimer = 2.0f;
			GamePlayState::GetInstance()->RunUsed = true;
			std::ostringstream runText;
			runText << name << " succeeds in running away!";
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( runText.str(), this );

			myTarget = 0;
			if ( stepbackward == false && stepforward == false && progress != 0.0f )
				{
				stepbackward = true;
				stepTime = .50f;
				progress = 0.0f;
				}

			TurnManager::GetInstance()->setProgressFullReached( false );
			hudSelection = 0;
			//TurnManager::GetInstance()->setTimeStop( false );
			TurnManager::GetInstance()->setTurnPause( true );
			//help->UpdateSelection( 5 );
			states = 0;
			mySelection = none;
			SetSelection( 0 );
			//help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			GamePlayState::GetInstance()->PlaySoundEffect( 5 );
			}

		else
			{
			GamePlayState::GetInstance()->runTimer = 2.0f;
			GamePlayState::GetInstance()->RunUsed = true;
			std::ostringstream runText;
			runText << name << " failed to run away!";
			GamePlayState::GetInstance()->GetHelpText()->ManualOverride( runText.str(), this );

			if ( stepbackward == false && stepforward == false && progress != 0.0f )
				{
				stepbackward = true;
				stepTime = .50f;
				progress = 0.0f;
				}
			myTarget = 0;
			hudSelection = 0;
			SetSelection( 0 );
			//help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			SetHomeButtons();

			Reset();

			}
		}

	}
void CombatPlayer::AllySelectUpdate( float dt ) // Defensive ability use
	{
	//we go into itemupdate if we need to

	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	GamePlayState *game = GamePlayState::GetInstance();
	HelpText *help = game->GetHelpText();

	if ( mySelection == player || mySelection == deadAlly || mySelection == self )
		{
		TargetUnit( pTurn->GetAllies() );
		}

	if ( chosen.GetName() != "nothing" )
		{
		ItemsUpdate( dt );
		return;
		}

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		if ( GamePlayState::GetInstance()->usingTernary() )
			{
			//if ( GamePlayState::GetInstance()->RemoveTarget() )
				{
				// If there was at least 1 queued up ability, stay in Ternary State
				// Nothing should need to happen here

				//GamePlayState::GetInstance()->DeselectTernaryTarget();
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
				}

			/*else
			{
			states = 0;
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[ 0 ] );
			mySelection = none;
			hudSelection = 0;
			SetSelection( 0 );
			SetHomeButtons();
			GamePlayState::GetInstance()->ClearTernary();
			GamePlayState::GetInstance()->SetTernary( false );
			}*/
			}

		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );
		SetHomeButtons();
		GamePlayState::GetInstance()->ClearTernary();

		}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{
		//Input Ability Use Here
		if ( mySelection == allAlly )
			{
			if ( GamePlayState::GetInstance()->usingTernary() )
				{
				/*GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetAllies()[ 0 ] );
				GamePlayState::GetInstance()->AddTarget();*/
				TernarySelection();
				return;
				}

			for ( size_t i = 0; i < pTurn->GetAllies().size(); i++ )
				{
				if ( pTurn->GetAllies()[i]->isAlive() )
					{

						{
						menu[hudSelection]->GetAbility()->CastAbility( this, pTurn->GetAllies()[i], i );
						GamePlayState::GetInstance()->HoldOntoAbility( menu[hudSelection]->GetAbility() );
						std::ostringstream usingAbility;
						usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
						GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
						}
					}
				}

			// After adding all the aoe casts to the Ternary vectors
			//if ( GamePlayState::GetInstance()->usingTernary() )
			//{
			//	TernarySelection();
			//	//GamePlayState::GetInstance()->AddTarget();
			//	//// Reset certain targety things
			//	//myTarget = 0;
			//	//mySelection = none;
			//	//states = 7;
			//	return;
			//}
			}

		else if ( mySelection == player || mySelection == self || mySelection == deadAlly )
			{
			if ( GamePlayState::GetInstance()->usingTernary() )
				{

				//Add selection to vector
				/*GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetAllies()[ myTarget ] );
				GamePlayState::GetInstance()->AddTarget();*/
				// Reset certain targety things
				TernarySelection();
				//myTarget = 0;
				//mySelection = none;
				//states = 7;
				return;
				}

			else
				{
				menu[hudSelection]->GetAbility()->CastAbility( this, pTurn->GetAllies()[myTarget] );
				GamePlayState::GetInstance()->HoldOntoAbility( menu[hudSelection]->GetAbility() );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				}
			}

		myTarget = 0;
		if ( stepbackward == false && stepforward == false && progress != 0.0f )
			{
			stepbackward = true;
			stepTime = .50f;
			progress = 0.0f;
			}

		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
		//	TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		//help->UpdateSelection( 5 );
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

	if ( pInput->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		if ( GamePlayState::GetInstance()->usingTernary() )
			{
			//if ( GamePlayState::GetInstance()->RemoveTarget() )
				{
				// If there was at least 1 queued up ability, stay in Ternary State
				// Reset targeting info and state

				//GamePlayState::GetInstance()->DeselectTernaryTarget();
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
				}

			}

		states = 0;
		help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
		mySelection = none;
		hudSelection = 0;
		SetSelection( 0 );

		SetHomeButtons();
		GamePlayState::GetInstance()->ClearTernary();
		GamePlayState::GetInstance()->SetTernary( false );

		}

	if ( pInput->IsKeyPressed( SGD::Key::Enter ) || pInput->IsButtonPressed( 0, 1 ) )
		{
		//Input Ability Use Here
		if ( mySelection == allEnemy )
			{

			if ( GamePlayState::GetInstance()->usingTernary() )
				{
				// Add selection to vector
				/*GamePlayState::GetInstance()->AddToTB( menu[ hudSelection ]->GetAbility(), pTurn->GetEnemies()[ 0 ] );
				GamePlayState::GetInstance()->AddTarget();*/
				TernarySelection();
				return;
				}
			for ( size_t i = 0; i < pTurn->GetEnemies().size(); i++ )
				{
				if ( pTurn->GetEnemies()[i]->isAlive() )
					{

						{
						menu[hudSelection]->GetAbility()->CastAbility( this, pTurn->GetEnemies()[i], i );
						GamePlayState::GetInstance()->HoldOntoAbility( menu[hudSelection]->GetAbility() );
						std::ostringstream usingAbility;
						usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
						GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
						}
					}
				}

			if ( GamePlayState::GetInstance()->usingTernary() )
				{

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
				GamePlayState::GetInstance()->AddToTB( menu[hudSelection]->GetAbility(), pTurn->GetEnemies()[myTarget] );
				GamePlayState::GetInstance()->AddTarget();
				// Reset certain targety things
				myTarget = 0;
				mySelection = none;
				states = 7;
				return;
				}

			else
				{
				// Turn Manager will handle the go between for abilities and attacks for consistency and particles
				TurnManager::GetInstance()->UsingAbility( this, pTurn->GetEnemies()[myTarget], menu[hudSelection]->GetAbility() );
				GamePlayState::GetInstance()->HoldOntoAbility( menu[hudSelection]->GetAbility() );
				std::ostringstream usingAbility;
				usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
				GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
				}
			}

		myTarget = 0;
		if ( stepbackward == false && stepforward == false && progress != 0.0f )
			{
			stepbackward = true;
			stepTime = .50f;
			progress = 0.0f;
			}
		TurnManager::GetInstance()->setProgressFullReached( false );
		hudSelection = 0;
		//TurnManager::GetInstance()->setTimeStop( false );
		TurnManager::GetInstance()->setTurnPause( true );
		//help->UpdateSelection( 5 );
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
		menu[i]->SetObjectType( 0 );
		menu[i]->SetAbility( nullptr );
		menu[i]->SetItem( nullptr );
		}
	menu[0]->SetString( "Attack" );
	menu[1]->SetString( "Item" );
	menu[2]->SetString( "Ability" );
	menu[3]->SetString( "Run" );

	}

void CombatPlayer::SelectingItems( float dt )
	{
	HelpText *help = GamePlayState::GetInstance()->GetHelpText();
	SGD::InputManager *pInput = SGD::InputManager::GetInstance();
	bool selected = item_choose->Update( dt );
	tempitem->SetExplination( item_choose->GetCurrentExplanation() );
	helpobject->SetItem( tempitem );
	help->UpdateSelection( 2, helpobject );

	if ( selected && item_choose != nullptr )
		{
		help->UpdateSelection( 4 );
		chosen = item_choose->chosen;
		delete item_choose;
		item_choose = nullptr;
		bool anyAllyDead = false;
		if ( chosen.GetName() == "Revive" )
			{

			mySelection = deadAlly;
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( !TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					{
					myTarget = i;
					anyAllyDead = true;
					states = 5;
					break;
					}
				}

			if ( !anyAllyDead )
				{
				GameData::GetInstance()->PlaySelectionChange();
				mySelection = none;
				states = 0;
				//TurnManager::GetInstance()->setProgressFullReached( false );
				//pTurn->setTimeStop( false );
				//TurnManager::GetInstance()->setTurnPause( true );
				hudSelection = 0;
				help->UpdateSelection( 5 );
				SetSelection( 0 );
				SetHomeButtons();
				return;
				}
			}

		else
			{
			mySelection = player;
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( this->GetName() == TurnManager::GetInstance()->GetAllies()[i]->GetName() )
					{
					myTarget = i;
					break;
					}
				}
			states = 5;

			}

		if ( chosen.GetName() == "nothing" )
			{
			delete item_choose;
			item_choose = nullptr;
			GameData::GetInstance()->PlaySelectionChange();
			states = 0;
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			mySelection = none;
			hudSelection = 0;
			SetSelection( 0 );
			SetHomeButtons();
			}

		}
	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Escape ) || pInput->IsButtonPressed( 0, 2 ) )
		{
		SGD::AudioManager::GetInstance()->PlayAudio( GamePlayState::GetInstance()->m_vsoundeffects[4] );
		delete item_choose;
		item_choose = nullptr;
		GameData::GetInstance()->PlaySelectionChange();
		mySelection = none;
		states = 0;
		//TurnManager::GetInstance()->setProgressFullReached( false );
		//pTurn->setTimeStop( false );
		//TurnManager::GetInstance()->setTurnPause( true );
		hudSelection = 0;
		help->UpdateSelection( 5 );
		SetSelection( 0 );
		SetHomeButtons();
		}


	}

void CombatPlayer::TernaryBlast( float dt )
// states 7
// Handles selection for ternary blast
	{
	// Ability Selection 3 times without ending turn or executing ability
	// Make sure Escape works correctly while selecting in here
	int tar = GamePlayState::GetInstance()->myTernTargets.num_targets;
	if ( tar < 3 )
		AbilityUpdate( dt );
	else
		// Selected all the targets, cast them and get outta dodge
		{
		if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Escape ) )
			{
			GamePlayState::GetInstance()->RemoveTarget();
			GamePlayState::GetInstance()->DeselectTernaryTarget();
			myTarget = 0;
			mySelection = none;
			states = 7;
			return;
			}

		if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) )
			tern_confirm = true;
		// Need to pick one ability out at a time, cast it, continue to update, and lalala.
		// Carry index of current ability cast somewhere global, check for AOE and then do the things
		if ( tern_confirm )
			{
			if ( !( GamePlayState::GetInstance()->abilityTimer > 0.0f ) )
				{
				if ( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAOE() )
					{
					// Hit em all

					if ( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetOffensive() )
						{

						for ( unsigned int targets = 0; targets < TurnManager::GetInstance()->GetEnemies().size(); targets++ )
							{
							if ( TurnManager::GetInstance()->GetEnemies()[targets]->isAlive() )
								{
								TurnManager::GetInstance()->UsingAbility(
									this,
									TurnManager::GetInstance()->GetEnemies()[targets],
									GamePlayState::GetInstance()->myTernTargets.abilities[tern_index],
									true );	// true for ternary stuff	

								ApplyTernary( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAbilityName(),
											  TurnManager::GetInstance()->GetEnemies()[targets] );
								}
							}
						}

					else
						{
						for ( unsigned int targets = 0; targets < TurnManager::GetInstance()->GetAllies().size(); targets++ )
							{
							if ( TurnManager::GetInstance()->GetAllies()[targets]->isAlive() || GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAbilityName() == "Second Wind" )
								{
								TurnManager::GetInstance()->UsingAbility(
									this,
									TurnManager::GetInstance()->GetAllies()[targets],
									GamePlayState::GetInstance()->myTernTargets.abilities[tern_index],
									true );	// true for ternary stuff	
								GamePlayState::GetInstance()->HoldOntoAbility( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index] );
								std::ostringstream usingAbility;
								usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
								GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );
								ApplyTernary( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAbilityName(),
											  TurnManager::GetInstance()->GetAllies()[targets] );
								}
							}
						}

					GamePlayState::GetInstance()->AbilityUsed = true;
					GamePlayState::GetInstance()->CurrentAbilityUsed = GamePlayState::GetInstance()->myTernTargets.abilities[tern_index];
					GamePlayState::GetInstance()->abilityTimer = 2.0f;
					tern_index++;
					return;
					}

				// If Not AOE, do one thing
				if ( GamePlayState::GetInstance()->myTernTargets.targets[tern_index]->isAlive() || GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAbilityName() == "Second Wind" )
					{
					TurnManager::GetInstance()->UsingAbility(
						this,
						GamePlayState::GetInstance()->myTernTargets.targets[tern_index],
						GamePlayState::GetInstance()->myTernTargets.abilities[tern_index],
						true );	// true for ternary stuff	
					GamePlayState::GetInstance()->HoldOntoAbility( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index] );
					std::ostringstream usingAbility;
					usingAbility << name << " uses " << GamePlayState::GetInstance()->CurrentAbilityUsed->GetAbilityName();
					GamePlayState::GetInstance()->GetHelpText()->ManualOverride( usingAbility.str(), this );


					ApplyTernary( GamePlayState::GetInstance()->myTernTargets.abilities[tern_index]->GetAbilityName(),
								  GamePlayState::GetInstance()->myTernTargets.targets[tern_index] );

					GamePlayState::GetInstance()->AbilityUsed = true;
					GamePlayState::GetInstance()->CurrentAbilityUsed = GamePlayState::GetInstance()->myTernTargets.abilities[tern_index];
					GamePlayState::GetInstance()->abilityTimer = 2.0f;

					}
				tern_index++;
				}
			}

		if ( tern_index >= 3 )
			{

			GamePlayState::GetInstance()->ClearTernary();
			myTarget = 0;
			if ( stepbackward == false && stepforward == false && progress != 0.0f )
				{
				stepbackward = true;
				stepTime = .50f;
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
			GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
			SetHomeButtons();
			GamePlayState::GetInstance()->SetTernary( false );
			GamePlayState::GetInstance()->SetGauge( 0 );
			tern_index = 0;
			tern_confirm = false;
			}
		}
	}

int CombatPlayer::GetXP()
	{
	return XP;
	}

int CombatPlayer::GetLevel()
	{
	return level;
	}

int CombatPlayer::GetNextlvl()
	{
	return nextlvl;
	}

void CombatPlayer::AddXP( int xp )
	{
	leveledup = false;
	XP += xp;
	if ( XP >= nextlvl )
		{
		LevelUp();
		}
	}

void CombatPlayer::LevelUp()
	{
	level++;
	XP -= nextlvl;
	nextlvl = level * 150 + 100;
	leveledup = true;
	}



// Do Ternary Blast Specials
void CombatPlayer::ApplyTernary( std::string abil, Character* target )
	{
	if ( abil == "Fire Fang" )
		{
		if ( !target->HasEffect( "Burn" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Burn" ) );
		}

	else if ( abil == "Water Fang" )
		{
		if ( !target->HasEffect( "Drown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Drown" ) );
		}

	else if ( abil == "Earth Fang" )
		{
		if ( !target->HasEffect( "Muck" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Muck" ) );
		}

	else if ( abil == "Wind Fang" )
		{
		if ( !target->HasEffect( "Wisp" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Wisp" ) );
		}

	else if ( abil == "Poison Fang" )
		{
		// add delerium once it works
		if ( !target->HasEffect( "Confused" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Confused" ) );
		}

	else if ( abil == "Slow Claw" )
		{
		// Knock them back on the timeline
		float prog = target->GetProgress();
		prog -= 33;
		if ( prog < 0 ) prog = 0.0f;
		target->SetProgress( prog );
		}

	else if ( abil == "Splash" )
		{
		if ( !target->HasEffect( "SpeedDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "SpeedDown" ) );
		}

	else if ( abil == "Flood" )
		{
		if ( !target->HasEffect( "AvoisionDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "AvoisionDown" ) );
		}

	else if ( abil == "Squirt" )
		{
		if ( !target->HasEffect( "Stun" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Stun" ) );
		}

	else if ( abil == "Torrent" )
		{
		if ( !target->HasEffect( "AttackDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "AttackDown" ) );
		}

	else if ( abil == "Puddle" )
		{
		if ( !target->HasEffect( "DefenseUp" ) )
			this->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "DefenseUp" ) );
		}

	else if ( abil == "Disolve" )
		{
		if ( !target->HasEffect( "AttackUp" ) )
			this->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "AttackUp" ) );
		}

	else if ( abil == "Ignite" )
		{
		if ( !target->HasEffect( "Burn" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Burn" ) );
		}

	else if ( abil == "Incinerate" )
		{
		if ( !target->HasEffect( "Burn" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Burn" ) );
		}

	else if ( abil == "Scorch" )
		{
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				{
				if ( !target->HasEffect( "AttackUp" ) )
					TurnManager::GetInstance()->GetAllies()[i]->AddStatus(
					&StatusEffectManager::GetInstance()->GetStatus( "AttackUp" ) );
				}
			}
		}

	else if ( abil == "Tornado" )
		{
		target->SetProgress( 0.0f );
		}

	else if ( abil == "Tempest" )
		{
		target->SetProgress( 0.0f );
		}

	else if ( abil == "Zephyr" )
		{
		if ( !target->HasEffect( "Regen" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Regen" ) );
		}

	else if ( abil == "Rock Spike" )
		{
		if ( !target->HasEffect( "MagicDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "MagicDown" ) );
		}

	else if ( abil == "Geo Crush" )
		{
		if ( !target->HasEffect( "SpeedDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "SpeedDown" ) );
		}

	else if ( abil == "Tremor" )
		{
		if ( !target->HasEffect( "DefenseDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "DefenseDown" ) );
		}

	else if ( abil == "Quake" )
		{
		if ( !target->HasEffect( "AttackDown" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "AttackDown" ) );
		}

	else if ( abil == "Pinch" )
		{
		if ( !target->HasEffect( "Muck" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Muck" ) );
		}

	else if ( abil == "Rampart" )
		{
		if ( !target->HasEffect( "MagicUp" ) )
			target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "MagicUp" ) );
		}
	}

void CombatPlayer::TernarySelection()
	{
	GamePlayState::GetInstance()->AddToTB( menu[hudSelection]->GetAbility(), TurnManager::GetInstance()->GetAllies()[myTarget] );
	GamePlayState::GetInstance()->AddTarget();
	// Reset certain targety things
	myTarget = 0;
	mySelection = none;
	states = 7;
	return;
	}