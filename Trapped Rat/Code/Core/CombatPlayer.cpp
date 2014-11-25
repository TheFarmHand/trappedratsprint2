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
	//tester.SetOffensive( true );
	//tester.SetHealing( false );

	}


CombatPlayer::~CombatPlayer()
	{
	if ( ansys != nullptr )
		delete ansys;
	}

void CombatPlayer::Update( float dt )
	{

	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
		progress += speed * dt;
	else if ( progress < 100.0f )
		{
		return;
		}

	if ( progress >= 100.0f )
		{
		SGD::InputManager *pInput = SGD::InputManager::GetInstance();
		TurnManager *pTurn = TurnManager::GetInstance();
		GamePlayState *game = GamePlayState::GetInstance();
		HelpText *help = game->GetHelpText();
		if ( TurnManager::GetInstance()->getTimeStop() == false )
			help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
		TurnManager::GetInstance()->setTimeStop( true );
		// Here is where targeting happens




		if ( states == 0 )
			{
			if ( pInput->IsKeyPressed( SGD::Key::Up ) )
				{
				SetSelection( 0 );
				hudSelection = 0;

				}
			else if ( pInput->IsKeyPressed( SGD::Key::Right ) )
				{
				SetSelection( 2 );
				hudSelection = 2;
				}
			else if ( pInput->IsKeyPressed( SGD::Key::Left ) )
				{
				SetSelection( 1 );
				hudSelection = 1;

				}
			else if ( pInput->IsKeyPressed( SGD::Key::Down ) )
				{
				SetSelection( 3 );
				hudSelection = 3;

				}

			if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
				{
				switch ( hudSelection )
					{
					case 0:
						help->UpdateSelection( 3 );
						states = 1;
						//if ( abilityList[i].GetHealing() )
						//	{
						//	help->UpdateSelection( 4 );
						//	mySelection = player;
						//	}
						//else
							//{
							for ( unsigned int i = 0; i < pTurn->GetEnemies().size(); ++i )
								{
								if ( pTurn->GetEnemies()[i]->isAlive() )
									{
									myTarget = i;
									break;
									}
								}
							mySelection = enemy;
							//}
						break;
					case 1:
						help->UpdateSelection( 2 );
						states = 2;
						mySelection = player;	// ***************** HACKED CODE FOR USER STORY; BYPASSING ITEM SELECTION *****************
						help->UpdateSelection( 4 );		// *********  ALSO HACKED ATM ********** //
						break;
					case 2:
						help->UpdateSelection( 1 );
						states = 3;
						break;
					case 3:
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
				}
			}

		if ( states == 1 )		// Attacking; Target selection is "enemy"
			{
			//
			//if (TestAbility && tester.GetHealing())
			//	TargetUnit( pTurn->GetAllies() );
			//else
				TargetUnit( pTurn->GetEnemies() );
			

			if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) )
				{
				// Here is where we decide Attack, or Ability etc
				//if ( TestAbility )
				//	{
				//	if ( tester.GetOffensive() )
				//		{
				//		//TargetUnit( pTurn->GetEnemies() );
				//		tester.CalculateFormula( this, pTurn->GetEnemies()[myTarget] );
				//		tester.CastAbility( this, pTurn->GetEnemies()[myTarget] );
				//		tester.SetHealing( true );
				//		tester.SetOffensive( false );
				//		}
				//	else if ( tester.GetHealing() )
				//		{
				//		//TargetUnit( pTurn->GetAllies() );
				//		tester.CalculateFormula( this, pTurn->GetAllies()[myTarget] );
				//		tester.CastAbility( this, pTurn->GetAllies()[myTarget] );
				//		tester.SetHealing( false );
				//		tester.SetOffensive( true );
				//		}
				//	}
				//else
				//	{
					
					Attack( this, pTurn->GetEnemies()[myTarget] );
				//	}
				myTarget = 0;
				progress = 0.0f;
				TurnManager::GetInstance()->setProgressFullReached( false );
				hudSelection = 0;
				TurnManager::GetInstance()->setTimeStop( false );
				TurnManager::GetInstance()->setTurnPause( true );
				help->UpdateSelection( 5 );
				states = 0;
				mySelection = none;
				SetSelection( 0 );
				}
			else if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
				{
				states = 0;
				help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
				mySelection = none;
				SetSelection( 0 );
				}
			}

		else if ( states == 2 ) // Item Selection
			{
			// **************** USER STORY COMPLETION FOR HEALING **************** //
			// **************** REPLACE WITH ITEM SELECTION CODE  **************** //
			// This code can be repurposed for actually using a healing item
			// Factored it out into a function already, to cut back on work later

			// Do player selection

			TargetUnit( pTurn->GetAllies() );		// this is pretty sweet
			if ( pInput->IsKeyPressed( SGD::Key::Enter ) )
				{
				pTurn->HealTarget( pTurn->GetAllies()[myTarget], 15 );

				// These three lines could be factored out to an EndTurn type function
				progress = 0.0f;
				mySelection = none;
				states = 0;
				pTurn->setTimeStop( false );
				hudSelection = 0;
				help->UpdateSelection( 5 );
				SetSelection( 0 );
				}
			else if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
				{
				states = 0;
				help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
				mySelection = none;
				hudSelection = 0;
				SetSelection( 0 );
				}
			}
		else if ( states == 3 )
			{
			if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
				{
				states = 0;
				help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
				mySelection = none;
				hudSelection = 0;
				SetSelection( 0 );
				}
			}
		else if ( states == 4 )
			{
			if ( pInput->IsKeyPressed( SGD::Key::Escape ) )
				{
				states = 0;
				help->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[0] );
				mySelection = none;
				hudSelection = 0;
				SetSelection( 0 );
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
		}

	}
void CombatPlayer::UpdateAnimation( float dt )
	{
	if ( ansys != nullptr )
		{
		ansys->Update( dt );
		}
	}
void CombatPlayer::Render()
	{
	SGD::GraphicsManager *pGraphics = SGD::GraphicsManager::GetInstance();
	TurnManager *pTurn = TurnManager::GetInstance();
	std::ostringstream ss;
	ss << "Speed: " << speed;
	SGD::GraphicsManager::GetInstance()->DrawString( name.c_str(), { position.x + 50, position.y }, SGD::Color( 255, 255, 0, 0 ) );
	GameData::GetInstance()->GetFont()->DrawString( ss.str(), position.x + 50, position.y + 30, { 0, 255, 0 } );
	std::ostringstream sp;
	sp << "Progress: " << progress;
	GameData::GetInstance()->GetFont()->DrawString( sp.str(), position.x + 50, position.y + 50, { 0, 255, 255 } );

	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), 301, 451 + ( order*50.0f ), { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), 300, 450 + ( order*50.0f ), { 255, 0, 0 }, 1.6f );
	//if ( TestAbility && progress >= 100.0f )
	//	tester.Render();
	std::string numbers = std::to_string( HP ) + "/" + std::to_string( MaxHP ) + " HP";
	SGD::Point pdraw;

	if ( progress >= 100.0f )
		{
		if ( !TurnManager::GetInstance()->getProgressFullReached() )
			TurnManager::GetInstance()->setProgressFullReached( true );
		if ( mySelection == enemy )
			{
			pdraw = pTurn->GetEnemies()[myTarget]->GetPosition();
			pdraw.x -= pTurn->GetEnemies()[myTarget]->GetSize().width + 20;
			pdraw.y -= pTurn->GetEnemies()[myTarget]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetArrow(),
									pdraw );
			}

		else if ( mySelection == player || mySelection == self )
			// enforce self target by disallowing targeting input in update
			{
			pdraw = pTurn->GetAllies()[myTarget]->GetPosition();
			pdraw.x += pTurn->GetAllies()[myTarget]->GetSize().width + 20;
			pdraw.y -= pTurn->GetAllies()[myTarget]->GetSize().height / 2;
			pGraphics->DrawTexture( pTurn->GetArrow(), pdraw );
			}

		}

	SGD::Color color;
	float hppercentage = HP / (float)MaxHP;
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
	GameData::GetInstance()->GetFont()->DrawString( numbers, 491, 451 + ( order*50.0f ), { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( numbers, 490, 450 + ( order*50.0f ), color, 1.6f );

	if ( ansys != nullptr )
		{
		ansys->Render( position.x, position.y );
		}
	}
void CombatPlayer::Attack( Character* owner, Character * target )
	{
	if ( target != nullptr )
		{
		int atk = target->GetStats().attack;
		int dmg = rand() % atk + 3;
		TurnManager::GetInstance()->AttackTarget( owner, target, 20 );
		//owner->TakeDamage( 100, target );
		}
	}

void CombatPlayer::UseItem( Character * target, Items item )
	{

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
	GamePlayState::GetInstance()->GetHelpText()->UpdateSelection( 0, GamePlayState::GetInstance()->GetSelectableObjects()[selected] );
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

	else if ( pInput->IsKeyPressed( SGD::Key::Up ) || pInput->IsKeyPressed( SGD::Key::Left ) )
		{
		--myTarget;
		if ( myTarget < 0 ) 
			myTarget = targets.size() - 1;

		while ( !targets[myTarget]->isAlive() )
			{
			--myTarget;
			if ( myTarget < 0 )
				myTarget = targets.size() - 1;
			}
		// If the player pushes DOWN, select a later enemy in the list; wrap to bottom if we are at the top
		/*if ( myTarget < 0 )
			{
			myTarget = targets.size() - 1;
			}
		else
			myTarget--;*/

		// Shouldn't ever need this check, but safety first
		if ( myTarget < 0 ) myTarget = 0;
		}

	else if ( pInput->IsKeyPressed( SGD::Key::Down ) || pInput->IsKeyPressed( SGD::Key::Right ) )
		{
		++myTarget;
		
		if ( myTarget > (int)targets.size() - 1 )
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
		if ( myTarget > (int)targets.size() - 1 ) myTarget = (int)targets.size() - 1;
		}

	}
void CombatPlayer::Reset()
	{
	states = 0;
	mySelection = none;
	}
