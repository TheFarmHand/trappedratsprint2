
#include "GamePlayState.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../Core/Player.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../Core/CommonEnemy.h"
#include "../Core/Bars.h"
#include "../Messages/TestMessage.h"
#include "../Animation/AnimationSystem.h"
#include "../States/MainMenuState.h"
#include "../States/LoadGameState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include <sstream>
GamePlayState* GamePlayState::GetInstance()
{
	static GamePlayState data;

	return &data;
}
void GamePlayState::Enter()
{
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);
	TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\TestTown.xml" );
	combathud = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/tempcombathud.png");
	targeting = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/targeticon.png");
	buttonimg = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/button.png");
	combatback = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/CombatTownBack.png");

	Stats sts;
	sts.attack = 10;
	sts.defense = 10;
	sts.avoision = 10;
	sts.magic = 10;



	TurnManager::GetInstance()->SetArrow(targeting);

	SelectableObject* ob = CreateSelectableObject(buttonimg, { 70, 445 }, { 64, 64 }, "Attack");
	ob->SetSelected(true);
	ob->SetExplination("Deal damage to one enemy");
	m_vSelectableItems.push_back(ob);

	ob = CreateSelectableObject(buttonimg, { 10, 495 }, { 64, 64 }, "Item");
	ob->SetExplination("Use an Item to aide your party");
	m_vSelectableItems.push_back(ob);

	ob = CreateSelectableObject(buttonimg, { 150, 495 }, { 64, 64 }, "Ability");
	ob->SetExplination("Use a unique ABILITY");
	m_vSelectableItems.push_back(ob);

	ob = CreateSelectableObject(buttonimg, { 70, 545 }, { 64, 64 }, "Run");
	ob->SetExplination("FLEE from combat");
	m_vSelectableItems.push_back(ob);

	helptextbox = new HelpText();
	helpback = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/HelpTextBox.png");
	helptextbox->SetImage(helpback);

	Ability* temp = new Ability( "Assets/Scripts/Abilities/Flood.xml" );

	p1 = dynamic_cast<CombatPlayer*>( CreateCombatPlayer( "Ratsputin", sts, 1, 50, 50, 32.0f, 0.0f, nullptr, SGD::Point( 100, 150 ), SGD::Size( 64, 64 ), "RatAnimBattle.xml" ));
	p1->SetOrderPosition(0);
	//GameData::GetInstance()->SetCombatParty(dynamic_cast<CombatPlayer*>(p1), 0);

	p2 = dynamic_cast<CombatPlayer*>(CreateCombatPlayer("Ratsputin", sts, 1, 50, 50, 31.0f, 0.0f, nullptr, SGD::Point(100, 250), SGD::Size(64, 64), "RatAnimBattle.xml"));
	p2->SetOrderPosition(1);
	//GameData::GetInstance()->SetCombatParty(dynamic_cast<CombatPlayer*>(p2), 1);



	dialogue = new Dialogue();
	dialogue->Load("../Trapped Rat/Assets/Scripts/testdialogue.xml");
	state = GPStates::Dia;

	m_Audio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/req.xwm" );
	m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/ZeldaMetal.xwm" );
	SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio, true );

	GameData::GetInstance()->UpdateCamera( GameData::GetInstance()->GetOverworldPlayer() );
}
void const GamePlayState::Render()
{
	switch (state)
	{
	case GPStates::Combat:
		CombatRender();
		break;
	case GPStates::Town:
		TownRender();
		break;
	case GPStates::Menu:
		MenuRender();
		break;
	case GPStates::Map:
		break;
	case GPStates::Dia:
		DialogueRender();
		break;
	default:
		break;
	}
}
void GamePlayState::Update(float dt)
{

SGD::EventManager::GetInstance()->Update();

	//put everything into functions here
	//its going to get packed

	switch (state)
	{
	case GPStates::Combat:
		CombatUpdate(dt);
		break;
	case GPStates::Town:
		//see if we enter combat
		Fight();
		TownUpdate(dt);

		break;
	case GPStates::Menu:
		MenuUpdate(dt);
		break;
	case GPStates::Map:
		break;
	case GPStates::Dia:
		DialogueUpdate(dt);
		break;
	default:
		break;
	}




}
void GamePlayState::Exit()
{
	for (size_t i = 0; i < m_vhuditems.size(); i++)
	{
		delete m_vhuditems[i];
	}
	m_vhuditems.clear();
	for (size_t i = 0; i < m_vSelectableItems.size(); i++)
	{
		delete m_vSelectableItems[i];
	}
	m_vSelectableItems.clear();
	if ( enemy1 != nullptr )
		{
		delete enemy1;
		enemy1 = nullptr;
		}
	if (dialogue != nullptr )
	delete dialogue;
	if ( enemy2 != nullptr )
		{
		delete enemy2;
		enemy2 = nullptr;
		}

	if ( enemy3 != nullptr )
		{
		delete enemy3;
		enemy3 = nullptr;
		}

	if (helptextbox != nullptr )
	delete helptextbox;
	if ( p1 != nullptr )
		delete p1;
	if ( p2 != nullptr )
		delete p2;

	for ( unsigned int i = 0; i < GameData::GetInstance()->GetCombatParty().size(); i++ )
		{
		delete GameData::GetInstance()->GetCombatParty()[i];
		GameData::GetInstance()->GetCombatParty()[i] = nullptr;
		}
	GameData::GetInstance()->GetCombatParty().clear();

	SGD::GraphicsManager::GetInstance()->UnloadTexture(combatback);

	SGD::AudioManager::GetInstance()->UnloadAudio( m_Audio );
	SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );


	SGD::GraphicsManager::GetInstance()->UnloadTexture(helpback);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(combathud);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(targeting);
	if (buttonimg != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->UnloadTexture(buttonimg);



	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::GetInstance()->DeleteInstance();

	TileSystem::GetInstance()->Exit();

	// Double Check ParticleManager in TurnManager (Particles need to be a singleton, blah)
	// Cleans up those leaks
	TurnManager::GetInstance()->Terminate();



}

void GamePlayState::MessageProc(const SGD::Message * mess)
{
	switch (mess->GetMessageID())
	{
	case MessageID::NOTHING:
		break;
	case MessageID::TEST_MESSAGE:
		Test();
		break;
	default:
		break;
	}
}
void GamePlayState::Test()
{
	GamePlayState::GetInstance()->is_test_message = true;
}

void GamePlayState::Fight()
{
if ( GameData::GetInstance()->GetOverworldPlayer()->IsMoving() && !GameData::GetInstance()->GetIsInCombat())
	{
		int sorandom = rand() % 250;
		if (!sorandom)
		{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_overAudio );

		SGD::AudioManager::GetInstance()->PlayAudio( m_Audio, true );

			GameData::GetInstance()->SetIsInCombat(true);
			state = GPStates::Combat;
			/*p1.SetOrderPosition( 0 );
			p1.SetSpeed( 10.0f );
			p1.SetPosition( SGD::Point( 10.0f, 100.0f ) );
			p1.SetMaxHP(50);
			p1.SetHP(45);
			p1.SetActive( true );
			p1.SetName( "Jeeves" );
			p2.SetOrderPosition( 1 );
			p2.SetSpeed( 10.0f );
			p2.SetPosition( SGD::Point( 10.0f, 200.0f ) );
			p2.SetActive( true );
			p2.SetMaxHP(100);
			p2.SetHP(50);
			p2.SetName( "Ratsputin" );
			p3.SetOrderPosition( 2 );
			p3.SetSpeed( 12.0f );
			p3.SetPosition( SGD::Point( 10.0f, 300.0f ) );
			p3.SetActive( true );
			p3.SetMaxHP(115);
			p3.SetHP(5);
			p3.SetName( "Checkers" );*/
			/*e1.SetOrderPosition( 0 );
			e1.SetSpeed( 8.0f );
			e1.SetPosition( SGD::Point( 600.0f, 100.0f ) );
			e1.SetName( "Dog" );
			e2.SetOrderPosition( 1 );
			e2.SetSpeed( 11.0f );
			e2.SetPosition( SGD::Point( 600.0f, 200.0f ) );
			e2.SetName( "Cat" );
			e3.SetOrderPosition( 2 );
			e3.SetSpeed( 9.0f );
			e3.SetPosition( SGD::Point( 600.0f, 300.0f ) );
			e3.SetName( "Raven" );*/
			std::vector<CombatPlayer*> tempPlayer;
			std::vector<Enemy*> tempEnemy;
			tempPlayer.push_back(p1);
			tempPlayer.push_back(p2);
			tempPlayer[0]->TestAbility = false;
			tempPlayer[1]->TestAbility = true;

			//tempPlayer.push_back( &p2 );
			//tempPlayer.push_back( &p3 );
			//tempEnemy.push_back( &e1 );
			//tempEnemy.push_back( &e2 );
			//tempEnemy.push_back( &e3 );
			if (enemy1 != nullptr )
			delete enemy1;
			if (enemy2 != nullptr )
			delete enemy2;
			if (enemy3 != nullptr )
			delete enemy3;

			Stats sts;
			sts.attack = 10;
			sts.defense = 10;
			sts.avoision = 10;
			sts.magic = 10;

			Character* en1 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 39.0f, 0.0f, nullptr, SGD::Point(600, 160), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy1 = dynamic_cast<Enemy*>(en1);
			enemy1->SetOrderPosition(0);



			Character* en2 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 40.0f, 0.0f, nullptr, SGD::Point(600, 260), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy2 = dynamic_cast<Enemy*>(en2);
			enemy2->SetOrderPosition(1);

			Character* en3 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 42.0f, 0.0f, nullptr, SGD::Point(600, 360), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy3 = dynamic_cast<Enemy*>(en3);
			enemy3->SetOrderPosition(2);


			tempEnemy.push_back(enemy1);
			tempEnemy.push_back(enemy2);
			tempEnemy.push_back(enemy3);

			TurnManager::GetInstance()->Initialize(tempPlayer, tempEnemy);
			for (size_t i = 0; i < tempPlayer.size(); i++)
			{
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(), tempPlayer[i], SGD::Color(0, 255, 0), SGD::Point(-30, -25)));
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(630, 450 + (tempPlayer[i]->GetOrderPosition()*50.0f)), tempPlayer[i], SGD::Color(0, 255, 0), SGD::Point(0, 0)));
			}

			////this is a message test
			//TestMessage * Test = new TestMessage();
			//Test->SendMessageNow();
			//delete Test;
			for (size_t i = 0; i < tempEnemy.size(); i++)
			{
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(), tempEnemy[i], SGD::Color(0, 255, 0), SGD::Point(-30, -45)));
			}
		}
	}


}

void GamePlayState::TownUpdate(float dt)
{
	GameData::GetInstance()->GetOverworldPlayer()->Update(dt);
	GameData::GetInstance()->UpdateCamera(GameData::GetInstance()->GetOverworldPlayer());

	SGD::InputManager * input = SGD::InputManager::GetInstance();
	if (input->IsKeyPressed(SGD::Key::Escape))
	{
		state = GPStates::Menu;
	}
}
void GamePlayState::MenuUpdate(float dt)
{
	//set the maxindex based on what menu we are in
	switch (substate)
	{
	case None:
		maxindex = 2;
		break;
	case Options:
		maxindex = 4;
		break;
	case HowTo:
		break;
	}

	SGD::InputManager* input = SGD::InputManager::GetInstance();
	if (input->IsKeyPressed(SGD::Key::Escape))
	{
		state = GPStates::Town;
	}

	//if they select a menu item
	if (input->IsKeyPressed(SGD::Key::Enter))
	{
		switch (substate)
		{
		case None:

			switch (menuindex)
			{
			case 0:
				GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
				state = GPStates::Town;
				substate = MenuSubStates::None;
				menuindex = 0;
				break;
			case 1:
				menuindex = 0;
				substate = MenuSubStates::Options;
				break;
			case 2:
				state = GPStates::Town;
			default:
				break;
			}
			break;
		case Options:
			switch (menuindex)
			{
			case 4:
				menuindex = 0;
				substate = MenuSubStates::None;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}


	}


	//changing the values for language


	if (input->IsKeyPressed(SGD::Key::Left) || input->IsKeyPressed(SGD::Key::Right))
	{
		switch (substate)
		{
		case None:
			switch (menuindex)
			{
			case 0:
				break;
			default:
				break;
			}
			break;
		case Options:
		{
						switch (menuindex)
						{

						case 0:
						{
								  bool is = GameData::GetInstance()->GetFont()->IsSpanish();
								  GameData::GetInstance()->GetFont()->SetSpanish(!is);
								  break;
						}
						case 1:
						{
								  if (input->IsKeyPressed(SGD::Key::Left))
								  {
									  GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() - 5);
								  }
								  else if (input->IsKeyPressed(SGD::Key::Right))
								  {
									  GameData::GetInstance()->SetMusicVolume(GameData::GetInstance()->GetMusicVolume() + 5);
								  }
								  break;
						}
						case 2:
						{
								  if (input->IsKeyPressed(SGD::Key::Left))
								  {
									  GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() - 5);
								  }
								  else if (input->IsKeyPressed(SGD::Key::Right))
								  {
									  GameData::GetInstance()->SetEffectVolume(GameData::GetInstance()->GetEffectVolume() + 5);
								  }
								  break;
						}
						case 3:
						{
								  bool is = GameData::GetInstance()->GetWindowed();
								  GameData::GetInstance()->SetWindowed(!is);
								  SGD::GraphicsManager::GetInstance()->Resize({ GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed());

								  break;
						}
						}
		}
			break;
		default:
			break;
		}
	}

	//navigating menu
	if (input->IsKeyPressed(SGD::Key::Up))
	{
		menuindex--;
		if (menuindex < 0)
			menuindex = maxindex;
	}
	if (input->IsKeyPressed(SGD::Key::Down))
	{
		menuindex++;
		if (menuindex > maxindex)
			menuindex = 0;
	}
}
void GamePlayState::TownRender()
{
	TileSystem::GetInstance()->Render();

	GameData::GetInstance()->GetOverworldPlayer()->Render();
	if (is_test_message)
		GameData::GetInstance()->GetFont()->DrawString("This is a message", 250.0, 50.0, { 155, 255, 155 });
}
void GamePlayState::MenuRender()
{

	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	GameData::GetInstance()->GetFont()->DrawString("Pause", 5.0f, 5.0f, { 155, 155, 155 });
	switch (substate)
	{
	case None:
		GameData::GetInstance()->GetFont()->DrawString("Main Menu", 50.0f, 100.0f, { 155, 155, 155 });
		GameData::GetInstance()->GetFont()->DrawString("Options", 50.0f, 150.0f, { 155, 155, 155 });
		GameData::GetInstance()->GetFont()->DrawString("Exit", 50.0f, 200.0f, { 155, 155, 155 });
		break;
	case Options:
	{
					GameData::GetInstance()->GetFont()->DrawString("Language:", 50.0f, 100.0f, { 155, 155, 155 });
					if (GameData::GetInstance()->GetFont()->IsSpanish())
						GameData::GetInstance()->GetFont()->DrawString("Spanish", 200.0f, 100.0f, { 155, 155, 155 });
					else
						GameData::GetInstance()->GetFont()->DrawString("English", 200.0f, 100.0f, { 155, 155, 155 });

					GameData::GetInstance()->GetFont()->DrawString("Music Volume:", 50.0f, 150.0f, { 155, 155, 155 });
					std::ostringstream mout;
					mout << GameData::GetInstance()->GetMusicVolume();
					GameData::GetInstance()->GetFont()->DrawString(mout.str(), 200.0f, 150.0f, { 155, 155, 155 });
					GameData::GetInstance()->GetFont()->DrawString("Effect Volume:", 50.0f, 200.0f, { 155, 155, 155 });
					std::ostringstream eout;
					eout << GameData::GetInstance()->GetEffectVolume();
					GameData::GetInstance()->GetFont()->DrawString(eout.str(), 200.0f, 200.0f, { 155, 155, 155 });

					GameData::GetInstance()->GetFont()->DrawString("FullScreen:", 50, 250.0f, { 155, 155, 155 });
					if (GameData::GetInstance()->GetWindowed())
						GameData::GetInstance()->GetFont()->DrawString("Off", 200.0f, 250.0f, { 155, 155, 155 });
					else
						GameData::GetInstance()->GetFont()->DrawString("On", 200.0f, 250.0f, { 155, 155, 155 });

					GameData::GetInstance()->GetFont()->DrawString("Exit", 50.0f, 300.0f, { 155, 155, 155 });
					break;
	}
	case HowTo:
		break;
	default:
		break;
	}

	graphics->DrawRectangle({ 35.0f, 100.0f + (menuindex * 50), 45.0f, 110.0f + (menuindex * 50) }, { 155, 0, 155 }, { 155, 155, 155 });
}
void GamePlayState::CombatUpdate(float dt)
{
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	TurnManager::GetInstance()->Update(dt);
	
	if (input->IsKeyPressed(SGD::Key::Backspace))
	{
		state = GPStates::Town;
		GameData::GetInstance()->SetIsInCombat(false);
		TurnManager::GetInstance()->Terminate();

	}
	if ( state == GPStates::Town )
		{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_Audio );
		SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio );
		}

}
void GamePlayState::CombatRender()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTexture(combatback, { 0, 0 });
	graphics->DrawTextureSection(combathud, { -0.1f, 425.0f }, { 0.0f, 0.0f, 800.0f, 175.0f });
	GameData::GetInstance()->GetFont()->DrawString("You are in Combat", 50.0, 50.0, { 155, 255, 155 });
	TurnManager::GetInstance()->Render();
	for (size_t i = 0; i < m_vhuditems.size(); i++)
	{
		if (m_vhuditems[i]->GetActive())
			m_vhuditems[i]->Render();
	}
	for (size_t i = 0; i < m_vSelectableItems.size(); i++)
	{
		if (m_vSelectableItems[i]->GetActive())
			m_vSelectableItems[i]->Render();
	}
	helptextbox->Render();
}


Character * GamePlayState::CreateCommonEnemy(std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename)
{
	CommonEnemy * temp = new CommonEnemy();
	temp->SetName(name);
	temp->SetStats(_stats);
	temp->SetLevel(_lvl);
	temp->SetHP(_hp);
	temp->SetMaxHP(_maxhp);
	temp->SetSpeed(_speed);
	temp->SetProgress(_progress);
	if (abilityarr != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			temp->SetAbility(i, abilityarr[i]);
		}
	}
	temp->SetPosition(_position);
	temp->SetSize(_size);

	AnimationSystem* tempanimsys = new AnimationSystem;
	if (_animfilename.size())
	{
		tempanimsys->Load(_animfilename);
		tempanimsys->Play(0);
	}
	else
		tempanimsys = nullptr;

	temp->SetAnimations(tempanimsys);

	return temp;
}
Character * GamePlayState::CreateCombatPlayer(std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename)
{
	CombatPlayer * temp = new CombatPlayer();
	temp->SetName(name);
	temp->SetActive( true );
	temp->SetStats(_stats);
	temp->SetLevel(_lvl);
	temp->SetHP(_hp);
	temp->SetMaxHP(_maxhp);
	temp->SetSpeed(_speed);
	temp->SetProgress(_progress);
	if (abilityarr != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			temp->SetAbility(i, abilityarr[i]);
		}
	}
	temp->SetPosition(_position);
	temp->SetSize(_size);

	AnimationSystem* tempanimsys = new AnimationSystem;
	if (_animfilename.size())
	{
		tempanimsys->Load(_animfilename);
		tempanimsys->Play(0);
	}
	else
		tempanimsys = nullptr;

	temp->SetAnimations(tempanimsys);
	return temp;
}

HUDItem * GamePlayState::CreateHudItem(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string _string)
{
	HUDItem* temp = new HUDItem();
	temp->SetImage(_image);
	temp->SetActive( true );
	temp->SetString(_string);
	temp->SetPosition(_position);
	temp->SetSize(_size);
	return temp;
}

HUDItem* GamePlayState::CreateBar(SGD::Size _size, SGD::Point _pos, Character * _owner, SGD::Color _color, SGD::Point _offset)
{
	Bars * temp = new Bars(_size, _pos, _owner);
	//temp->SetImage(_image);
	temp->SetOffset(_offset);
	temp->SetActive( true );
	temp->SetColor(_color);
	return temp;
}


void GamePlayState::DialogueUpdate(float dt)
{
	if (!dialogue->Update(dt))
	{
		state = GPStates::Town;
	}
}
void GamePlayState::DialogueRender()
{
	TileSystem::GetInstance()->Render();
	GameData::GetInstance()->GetOverworldPlayer()->Render();
	dialogue->Render();
}
SelectableObject * GamePlayState::CreateSelectableObject(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string _string)
{
	SelectableObject* temp = new SelectableObject();
	temp->SetImage(_image);
	temp->SetString(_string);
	temp->SetActive( true );
	temp->SetPosition(_position);
	temp->SetSize(_size);
	return temp;
}
std::vector<SelectableObject*> GamePlayState::GetSelectableObjects()
{
	return m_vSelectableItems;
}
void GamePlayState::SetSelectableObject(std::vector<SelectableObject*>_vec)
{
	m_vSelectableItems = _vec;
}
void GamePlayState::SetSelectableObject(SelectableObject* _obj, int _index)
{
	if (_index < (int)m_vSelectableItems.size())
	{
		m_vSelectableItems[_index] = _obj;
	}
}
HelpText* GamePlayState::GetHelpText()
{
	return helptextbox;
}
void GamePlayState::SetHelpText(HelpText* help)
{
	helptextbox = help;
}