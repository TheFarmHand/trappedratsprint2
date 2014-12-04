
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
#include "../tinyxml/tinyxml.h"
#include <Windows.h>
#include <fstream>

GamePlayState* GamePlayState::GetInstance()
{
	static GamePlayState data;

	return &data;
}
void GamePlayState::Enter()
{
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);
	TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\TestTown.xml");
	combathud = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/tempcombathud.png");
	targeting = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/targeticon.png");
	buttonimg = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/button.png");
	combatback = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/CombatTownBack.png");
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
	button = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/button.png");
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/cheese.png");
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/Scroll.png");
	trapcursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/RatTrap.png");
	cheesecursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/cheese.png");

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


	std::fstream fin;

	fin.open("Assets/Scripts/Abilities/AbilityList.txt");
	if (fin.is_open())
	{
		int count;
		char buffer[64];
		std::string abilPath;

		fin >> count;

		for (int i = 0; i < count; i++)
		{
			abilPath = "Assets/Scripts/Abilities/";
			fin.ignore(INT_MAX, '\n');
			fin.get(buffer, 64, '\n');
			std::string temp(buffer);

			temp += ".xml";
			abilPath += temp;

			Ability* tempAbility = new Ability(abilPath.c_str());
			MasterAbilityList[tempAbility->GetAbilityName()] = tempAbility;
		}

	}

	SGD::Point characterOrderPosition;
	//Ability* temp = new Ability( "Assets/Scripts/Abilities/Flood.xml" );
	CombatPlayer* p1 = nullptr;
	p1 = (LoadCombatPlayer("../Trapped Rat/Assets/Scripts/testcharacter.xml"));
	p1->SetOrderPosition(1);
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p1->GetOrderPosition() * 100 + 150);
	p1->SetPosition(characterOrderPosition);
	p1->SetSize({ 64, 64 });
	std::vector<Ability*> partyAbilities;
	partyAbilities.push_back( MasterAbilityList["Burrow"] );
	partyAbilities.push_back( MasterAbilityList["Water Fang"] );
	partyAbilities.push_back( MasterAbilityList["Slow Claw"] );
	partyAbilities.push_back( MasterAbilityList["Earth Fang"] );
	partyAbilities.push_back( MasterAbilityList["Poison Fang"] );
	partyAbilities.push_back( MasterAbilityList["Fire Fang"] );
	partyAbilities.push_back( MasterAbilityList["Counter Claw"] );
	partyAbilities.push_back( MasterAbilityList["Wind Fang"] );
	p1->InitializeAbilities( partyAbilities );
	p1->GetAbility( 0 )->SetUnlocked( true );
	p1->GetAbility( 1 )->SetUnlocked( true );
	p1->GetAbility( 2 )->SetUnlocked( true );
	p1->GetAbility( 3 )->SetUnlocked( true );
	Party.push_back(p1);

	partyAbilities.clear();

	CombatPlayer* p2 = nullptr;
	p2 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/testcharacterSlippy.xml" ) );
	p2->SetOrderPosition( 0 );
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p2->GetOrderPosition() * 100 + 150);
	p2->SetPosition( characterOrderPosition );
	p2->SetSize( { 64, 64 } );
	partyAbilities.push_back( MasterAbilityList["Puddle"] );
	partyAbilities.push_back( MasterAbilityList["Squirt"] );
	partyAbilities.push_back( MasterAbilityList["Dissolve"] );
	partyAbilities.push_back( MasterAbilityList["Splash"] );
	partyAbilities.push_back( MasterAbilityList["Acid Rain"] );
	partyAbilities.push_back( MasterAbilityList["Whirlpool"] );
	partyAbilities.push_back( MasterAbilityList["Torrent"] );
	partyAbilities.push_back( MasterAbilityList["Flood"] );
	p2->InitializeAbilities( partyAbilities );
	p2->GetAbility( 0 )->SetUnlocked( true );
	p2->GetAbility( 1 )->SetUnlocked( true );
	p2->GetAbility( 2 )->SetUnlocked( true );
	p2->GetAbility( 3 )->SetUnlocked( true );
	Party.push_back( p2 );

	partyAbilities.clear();

	CombatPlayer* p3 = nullptr;
	p3 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/testcharacterJeeves.xml" ) );
	p3->SetOrderPosition( 2 );
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p3->GetOrderPosition() * 100 + 150 );
	p3->SetPosition( characterOrderPosition );
	p3->SetSize( { 64, 64 } );
	partyAbilities.push_back( MasterAbilityList["Collapse"] );
	partyAbilities.push_back( MasterAbilityList["Ignite"] );
	partyAbilities.push_back( MasterAbilityList["Scorch"] );
	partyAbilities.push_back( MasterAbilityList["Emblazon"] );
	partyAbilities.push_back( MasterAbilityList["Firefall"] );
	partyAbilities.push_back( MasterAbilityList["Fire Spikes"] );
	partyAbilities.push_back( MasterAbilityList["Incinerate"] );
	partyAbilities.push_back( MasterAbilityList["Rib-a-Rang"] );
	p3->InitializeAbilities( partyAbilities );
	p3->GetAbility( 0 )->SetUnlocked( true );
	p3->GetAbility( 1 )->SetUnlocked( true );
	p3->GetAbility( 2 )->SetUnlocked( true );
	p3->GetAbility( 3 )->SetUnlocked( true );
	Party.push_back( p3 );

	dialogue = new Dialogue();
	dialogue->Load("../Trapped Rat/Assets/Scripts/testdialogue.xml");
	state = GPStates::Dia;

	m_Audio = SGD::AudioManager::GetInstance()->LoadAudio("../Trapped Rat/Assets/Sounds/req.xwm");
	m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio("../Trapped Rat/Assets/Sounds/ZeldaMetal.xwm");
	entercombat = SGD::AudioManager::GetInstance()->LoadAudio("../Trapped Rat/Assets/Sounds/entercombat.wav");
	SGD::AudioManager::GetInstance()->PlayAudio(m_overAudio, true);
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, GameData::GetInstance()->GetMusicVolume());
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, GameData::GetInstance()->GetEffectVolume());

	GameData::GetInstance()->UpdateCamera(GameData::GetInstance()->GetOverworldPlayer());
	pStatManager = StatusEffectManager::GetInstance();
	pStatManager->Initialize();
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
	//pause the game if alt + tab
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt))
	{
		if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Tab))
		{
			state = GPStates::Menu;
		}
	}
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
	if (enemy1 != nullptr)
	{
		delete enemy1;
		enemy1 = nullptr;
	}
	if (dialogue != nullptr)
		delete dialogue;
	if (enemy2 != nullptr)
	{
		delete enemy2;
		enemy2 = nullptr;
	}

	if (enemy3 != nullptr)
	{
		delete enemy3;
		enemy3 = nullptr;
	}

	if (helptextbox != nullptr)
		delete helptextbox;


	for (unsigned int i = 0; i < Party.size(); i++)
	{
		delete Party[i];
		Party[i] = nullptr;
	}
	Party.clear();

	SGD::GraphicsManager::GetInstance()->UnloadTexture(combatback);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_Audio);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_overAudio);
	SGD::AudioManager::GetInstance()->UnloadAudio(entercombat);

	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(button);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cursor);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(scroll);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(trapcursor);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cheesecursor);

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

	auto iterAbil = MasterAbilityList.begin();
	for (; iterAbil != MasterAbilityList.end(); ++iterAbil)
	{
		delete iterAbil->second;
	}
	MasterAbilityList.clear();

	pStatManager->Terminate();
	pStatManager = nullptr;

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
	if (GameData::GetInstance()->GetOverworldPlayer()->IsMoving() && !GameData::GetInstance()->GetIsInCombat())
	{
		stepcounter++;
		//int sorandom = rand() % 250;
		//if (!sorandom)
		if (stepcounter >= 300)
		{
			stepcounter = rand() % 100;
			//play animation for entering random combat
			RandomAnimation();
			if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_overAudio))
				SGD::AudioManager::GetInstance()->StopAudio(m_overAudio);

			SGD::AudioManager::GetInstance()->PlayAudio(m_Audio, true);

			GameData::GetInstance()->SetIsInCombat(true);
			state = GPStates::Combat;




			std::vector<Enemy*> tempEnemy;



			if (enemy1 != nullptr)
				delete enemy1;
			if (enemy2 != nullptr)
				delete enemy2;
			if (enemy3 != nullptr)
				delete enemy3;

			Stats sts;
			sts.attack = 10;
			sts.defense = 10;
			sts.avoision = 10;
			sts.magic = 10;

			Character* en1 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 39.0f, 0.0f, nullptr, SGD::Point(600, 160), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy1 = dynamic_cast<Enemy*>(en1);
			enemy1->SetEtype( WIND );
			enemy1->SetOrderPosition(0);

			Character* en2 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 40.0f, 0.0f, nullptr, SGD::Point(600, 260), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy2 = dynamic_cast<Enemy*>(en2);
			enemy2->SetEtype( WATER );
			enemy2->SetOrderPosition(1);

			Character* en3 = CreateCommonEnemy("Dog", sts, 1, 50, 50, 42.0f, 0.0f, nullptr, SGD::Point(600, 360), SGD::Size(64, 64), "DogAnimBattle.xml");
			enemy3 = dynamic_cast<Enemy*>(en3);
			enemy3->SetEtype( EARTH );
			enemy3->SetOrderPosition(2);

			tempEnemy.push_back(enemy1);
			tempEnemy.push_back(enemy2);
			tempEnemy.push_back(enemy3);
			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost(Party[i]);
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			TurnManager::GetInstance()->Initialize(Party, tempEnemy);
			for (size_t i = 0; i < Party.size(); i++)
			{
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(), Party[i], SGD::Color(0, 255, 0), SGD::Point(-30, -25)));
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(630, 440 + (Party[i]->GetOrderPosition()*50.0f)), Party[i], SGD::Color(0, 255, 0), SGD::Point(0, 0)));
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
			}


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
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	switch (substate)
	{
	case None:
		maxindex = 4;
		if (input->IsKeyPressed(SGD::Key::Escape))
		{
			GameData::GetInstance()->PlaySelectionChange();

			menuindex = 0;
			state = laststate;
		}
		break;
	case Options:
		maxindex = 4;
		if (input->IsKeyPressed(SGD::Key::Escape))
		{
			GameData::GetInstance()->PlaySelectionChange();
			menuindex = 0;
			substate = MenuSubStates::None;
			
		}
		break;
	case Shop:
		maxindex = (int)shopinv.size() - 1;
		if (input->IsKeyPressed(SGD::Key::Escape))
		{
			GameData::GetInstance()->PlaySelectionChange();
			menuindex = 0;
			substate = MenuSubStates::None;
			shopinv.clear();
			
		}
		break;
	case HowTo:
		break;
	case MenuSubStates::Party:
		if (input->IsKeyPressed(SGD::Key::Escape))
		{
			GameData::GetInstance()->PlaySelectionChange();
			menuindex = 0;
			substate = MenuSubStates::None;
			

		}
		break;
	default:
		break;
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
			{
					  GameData::GetInstance()->PlaySelectionChange();
					  substate = MenuSubStates::Shop;
					  //build the shop inventory
					  Items smallheal;
					  smallheal.SetName("Small Heal");
					  smallheal.SetPrice(10);
					  Items largeheal;
					  largeheal.SetName("Large Heal");
					  largeheal.SetPrice(20);
					  Items smallbp;
					  smallbp.SetName("Small BP Restore");
					  smallbp.SetPrice(10);
					  Items largebp;
					  largebp.SetName("Large BP Restore");
					  largebp.SetPrice(20);
					  Items revive;
					  revive.SetName("Revive");
					  revive.SetPrice(40);
					  Items maxrevive;
					  maxrevive.SetName("Max Revive");
					  maxrevive.SetPrice(100);

					  shopinv.push_back(smallheal);
					  shopinv.push_back(largeheal);
					  shopinv.push_back(smallbp);
					  shopinv.push_back(largebp);
					  shopinv.push_back(revive);
					  shopinv.push_back(maxrevive);
					  break;
			}
			case 1:

				menuindex = 0;
				substate = MenuSubStates::Party;
				GameData::GetInstance()->PlaySelectionChange();
				
				break;
			case 2:
				GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
				state = GPStates::Town;
				laststate = state;
				substate = MenuSubStates::None;
				menuindex = 0;


				break;
			case 3:

				GameData::GetInstance()->PlaySelectionChange();
				menuindex = 0;
				substate = MenuSubStates::Options;
				break;
			case 4:
				GameData::GetInstance()->PlaySelectionChange();
				state = laststate;
				break;
			default:
				break;
			}
			break;
		case Options:
			switch (menuindex)
			{
			case 4:
				GameData::GetInstance()->PlaySelectionChange();
				menuindex = 0;
				substate = MenuSubStates::None;
				break;
			default:
				break;
			}
			break;
		case Shop:
			if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
			{
				if (shopinv[menuindex].GetPrice() <= gold)
				{
					inventory.push_back(shopinv[menuindex]);
					gold -= shopinv[menuindex].GetPrice();
				}
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
								  GameData::GetInstance()->PlaySelectionChange();
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
								  GameData::GetInstance()->PlaySelectionChange();
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
								  GameData::GetInstance()->PlaySelectionChange();
								  break;
						}
						case 3:
						{
								  GameData::GetInstance()->PlaySelectionChange();
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
		GameData::GetInstance()->PlaySelectionChange();
	}
	if (input->IsKeyPressed(SGD::Key::Down))
	{
		menuindex++;
		if (menuindex > maxindex)
			menuindex = 0;
		GameData::GetInstance()->PlaySelectionChange();
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
	graphics->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	
	switch (substate)
	{
	case None:
		GameData::GetInstance()->GetFont()->DrawString("Pause Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f);
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
		graphics->DrawTextureSection(cursor, { 10.0f, 95.0f + (menuindex * 80) }, { 0, 0, 238, 73 });
		GameData::GetInstance()->GetFont()->DrawString("Shop", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f);
		GameData::GetInstance()->GetFont()->DrawString("Party", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f);
		GameData::GetInstance()->GetFont()->DrawString("Main Menu", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f);
		GameData::GetInstance()->GetFont()->DrawString("Options", 100.0f, 360.0f, { 0, 0, 0 }, 2.0f);
		GameData::GetInstance()->GetFont()->DrawString("Exit", 100.0f, 440.0f, { 0, 0, 0 }, 2.0f);

		break;
	case Options:
	{
					SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
					GameData::GetInstance()->GetFont()->DrawString("Options Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f);
					SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
					SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
					SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
					SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
					SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });

					graphics->DrawTextureSection(cursor, { 10.0f, 95.0f + (menuindex * 80) }, { 0, 0, 238, 73 });

					GameData::GetInstance()->GetFont()->DrawString("Language:", 70.0f, 120.0f, { 155, 155, 155 }, 1.5f);
					if (GameData::GetInstance()->GetFont()->IsSpanish())
						GameData::GetInstance()->GetFont()->DrawString("Spanish", 150.0f, 125.0f, { 0, 0, 0 });
					else
						GameData::GetInstance()->GetFont()->DrawString("English", 175.0f, 125.0f, { 0, 0, 0 });

					GameData::GetInstance()->GetFont()->DrawString("Music Volume:", 70.0f, 200.0f, { 155, 155, 155 }, 1.5f);
					std::ostringstream mout;
					mout << GameData::GetInstance()->GetMusicVolume();
					GameData::GetInstance()->GetFont()->DrawString(mout.str(), 220.0f, 205.0f, { 0, 0, 0 });
					GameData::GetInstance()->GetFont()->DrawString("Effect Volume:", 70.0f, 280.0f, { 155, 155, 155 }, 1.5f);
					std::ostringstream eout;
					eout << GameData::GetInstance()->GetEffectVolume();
					GameData::GetInstance()->GetFont()->DrawString(eout.str(), 220.0f, 285.0f, { 0, 0, 0 });

					GameData::GetInstance()->GetFont()->DrawString("FullScreen:", 70.0f, 360.0f, { 155, 155, 155 }, 1.5f);
					if (GameData::GetInstance()->GetWindowed())
						GameData::GetInstance()->GetFont()->DrawString("Off", 200.0f, 360.0f, { 0, 0, 0 });
					else
						GameData::GetInstance()->GetFont()->DrawString("On", 200.0f, 360.0f, { 0, 0, 0 });

					GameData::GetInstance()->GetFont()->DrawString("Exit", 70.0f, 440.0f, { 155, 155, 155 }, 1.5f);

					break;
	}
	case Shop:
	{
				 //display each item and the price associated with it
				 std::ostringstream yourgold;
				 yourgold << "Your Gold: " << gold << "g";
				 GameData::GetInstance()->GetFont()->DrawString(yourgold.str(), 450.0f, 70.0f, { 0, 0, 0 });
				 GameData::GetInstance()->GetFont()->DrawString("Shop Inventory", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f);
				 GameData::GetInstance()->GetFont()->DrawString("Escape to Exit", 60.0f, 70.0f, { 0, 0, 0 });
				 SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 50.0f, 50.0f }, { 0, 0, 300, 540 });
				 for (unsigned int i = 0; i < shopinv.size(); i++)
				 {
					 GameData::GetInstance()->GetFont()->DrawString(shopinv[i].GetName(), 100.0f, 160.0f + (i * 55), { 0, 0, 0 }, 1.5f);
					 std::ostringstream price;
					 price << shopinv[i].GetPrice() << "g";
					 if (gold < shopinv[i].GetPrice())
						 GameData::GetInstance()->GetFont()->DrawString(price.str(), 200.0f, 185.0f + (i * 55), { 0, 0, 0 }, 1.5f);
					 else
						 GameData::GetInstance()->GetFont()->DrawString(price.str(), 200.0f, 185.0f + (i * 55), { 155, 155, 155 }, 1.5f);

				 }
				 SGD::GraphicsManager::GetInstance()->DrawTexture(trapcursor, { 240.0f, 145.0f + (menuindex * 55) });

				 //now display the players inventory
				 GameData::GetInstance()->GetFont()->DrawString("Your Inventory", 450.0f, 40.0f, { 0, 0, 0 }, 2.0f);
				 SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 });
				 for (unsigned int i = 0; i < inventory.size(); i++)
				 {
					 bool is_skip = false;
					 for (int j = i - 1; j > -1; j--)
					 {
						 if (inventory[i] == inventory[j])
						 {
							 is_skip = true;
						 }
					 }
					 if (is_skip)
						 continue;

					 GameData::GetInstance()->GetFont()->DrawString(inventory[i].GetName(), 490.0f, 160.0f + (i * 55), { 0, 0, 0 }, 1.5f);
					 int count = 0;
					 for (unsigned int j = 0; j < inventory.size(); j++)
					 {
						 if (inventory[i] == inventory[j])
							 count++;
					 }

					 std::ostringstream total;
					 total << count << "/5";
					 GameData::GetInstance()->GetFont()->DrawString(total.str(), 490.0f, 185.0f + (i * 55), { 155, 155, 155 }, 1.5f);
				 }
	}
		break;
	case MenuSubStates::Party:
		GameData::GetInstance()->GetFont()->DrawString("Party Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f);
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 50.0f, 50.0f }, { 0, 0, 300, 540 });
		for (unsigned int i = 0; i < Party.size(); i++)
		{
			GameData::GetInstance()->GetFont()->DrawString(Party[i]->GetName(), 100.0f, 185.0f + (i * 55), { 0, 0, 0 }, 1.5f);
			std::ostringstream info;
			info << "HP: " << Party[i]->GetHP() << "/" << Party[i]->GetMaxHP() << "  BP: " << Party[i]->GetBP() << "/" << Party[i]->GetMaxBP();
			GameData::GetInstance()->GetFont()->DrawString(info.str(), 100.0f, 200.0f + (i * 55), { 0, 0, 0 }, 1.5f);
		}
		break;
	default:
		break;
	}

}
void GamePlayState::CombatUpdate(float dt)
{

	laststate = GPStates::Combat;
	SGD::InputManager * input = SGD::InputManager::GetInstance();


	if (input->IsKeyPressed(SGD::Key::Backspace))
	{
		state = GPStates::Town;
		laststate = state;
		GameData::GetInstance()->SetIsInCombat(false);
		TurnManager::GetInstance()->Terminate();

	}
	if ( input->IsKeyPressed( SGD::Key::Eight ) )
		{
		Party[0]->SetBP( Party[0]->GetMaxBP() );
		}
	if ( input->IsKeyPressed( SGD::Key::One ) )
		{
		Party[0]->SetHP( Party[0]->GetMaxHP() );
		}
	if ( input->IsKeyPressed( SGD::Key::Zero ) )
		{
		Party[0]->SetProgress( 100.0f );
		}
	if( input->IsKeyPressed(SGD::Key::L) )
	{
		Party[0]->AddStatus(&pStatManager->GetStatus("Regen"));
	}

	if (state == GPStates::Town)
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Audio))
			SGD::AudioManager::GetInstance()->StopAudio(m_Audio);
		SGD::AudioManager::GetInstance()->PlayAudio(m_overAudio);
	}

	TurnManager::GetInstance()->Update(dt);

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


Enemy * GamePlayState::CreateCommonEnemy(std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename)
{
	CommonEnemy * temp = new CommonEnemy();
	temp->SetName(name);
	temp->SetStats(_stats);
	temp->SetLevel(_lvl);
	temp->SetHP(_hp);
	temp->SetMaxHP(_maxhp);
	temp->SetSpeed(_speed);
	temp->SetProgress(_progress);
	//temp->SetEtype(WIND);
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

	if (temp->GetName() == "Dog")
	{
		temp->SetTimelineAnimation(SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/DogTimeline.png"));
	}

	return temp;
}
CombatPlayer * GamePlayState::CreateCombatPlayer(std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, int _bp, int _maxbp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename)
{
	CombatPlayer * temp = new CombatPlayer();
	temp->SetName(name);
	temp->SetActive(true);
	temp->SetStats(_stats);
	temp->SetLevel(_lvl);
	temp->SetHP(_hp);
	temp->SetMaxHP(_maxhp);
	temp->SetBP( _bp );
	temp->SetMaxBP( _maxbp );
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
	temp->SetActive(true);
	temp->SetString(_string);
	temp->SetPosition(_position);
	temp->SetSize(_size);
	return temp;
}

HUDItem* GamePlayState::CreateBar(SGD::Size _size, SGD::Point _pos, Character * _owner, SGD::Color _color, SGD::Point _offset, bool isHP)
{
	Bars * temp = new Bars(_size, _pos, _owner);
	//temp->SetImage(_image);
	temp->SetisHP( isHP );
	temp->SetOffset(_offset);
	temp->SetActive(true);
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
	temp->SetActive(true);
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
CombatPlayer * GamePlayState::LoadCombatPlayer(std::string _path)
{
	CombatPlayer * toon = nullptr;
	TiXmlDocument doc;
	if (doc.LoadFile(_path.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string name = root->FirstChildElement("Name")->GetText();
		std::string element = root->FirstChildElement("Element")->GetText();
		std::string type = root->FirstChildElement("Type")->GetText();
		std::string random = root->FirstChildElement("IsRandom")->GetText();
		//load in the Stats
		Stats stats;
		int HP;
		int BP;
		int level;
		float speed;
		TiXmlElement * Stat = root->FirstChildElement("Stats");
		HP = std::stoi(Stat->FirstChildElement("HitPoints")->FirstChildElement("Base")->GetText());
		stats.hp_scale = std::stof(Stat->FirstChildElement("HitPoints")->FirstChildElement("Scale")->GetText());

		BP = std::stoi(Stat->FirstChildElement("BattlePoints")->FirstChildElement("Base")->GetText());
		stats.bp_scale = std::stof(Stat->FirstChildElement("BattlePoints")->FirstChildElement("Scale")->GetText());

		stats.attack = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.attack_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		stats.magic = std::stoi(Stat->FirstChildElement("Magic")->FirstChildElement("Base")->GetText());
		stats.magic_scale = std::stof(Stat->FirstChildElement("Magic")->FirstChildElement("Scale")->GetText());

		stats.defense = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.defense_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		speed = std::stof(Stat->FirstChildElement("Speed")->FirstChildElement("Base")->GetText());
		stats.speed_scale = std::stof(Stat->FirstChildElement("Speed")->FirstChildElement("Scale")->GetText());

		stats.avoision = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.avoision_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		level = std::stoi(root->FirstChildElement("Level")->GetText());

		std::string hurt = root->FirstChildElement("Sound")->FirstChildElement("Hurt")->GetText();
		std::string attack = root->FirstChildElement("Sound")->FirstChildElement("Attack")->GetText();
		std::string death = root->FirstChildElement("Sound")->FirstChildElement("Hurt")->GetText();

		std::string animation = root->FirstChildElement("Animation")->GetText();



		if (type == "Ally")
		{
			//create a player
			toon = CreateCombatPlayer(name, stats, level, HP, HP, BP, BP, speed, 0, nullptr, { 0.0f, 0.0f }, { 0.0f, 0.0f }, animation);
			toon->SetMaxBP(BP);
			toon->SetBP(BP);
			if (element == "Wind")
			{
				toon->SetEtype(ETYPE::WIND);
			}
			else if (element == "Fire")
			{
				toon->SetEtype(ETYPE::FIRE);
			}
			else if (element == "Earth")
			{
				toon->SetEtype(ETYPE::EARTH);
			}
			else if (element == "Water")
			{
				toon->SetEtype(ETYPE::WATER);
			}
			else
			{
				toon->SetEtype(ETYPE::MULTI);
			}
		}
		else if (type == "Enemy")
		{

			return nullptr;

		}
		else if (type == "Guard")
		{
			return nullptr;
		}
		else
		{
			return nullptr;
		}

		if (element == "Wind")
		{
			toon->SetEtype(ETYPE::WIND);
		}
		else if (element == "Fire")
		{
			toon->SetEtype(ETYPE::FIRE);
		}
		else if (element == "Earth")
		{
			toon->SetEtype(ETYPE::EARTH);
		}
		else if (element == "Water")
		{
			toon->SetEtype(ETYPE::WATER);
		}
		else
		{
			toon->SetEtype(ETYPE::MULTI);
		}

	}
	if (toon->GetName() == "Ratsputin")
	{
		toon->SetTimelineAnimation(SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/RatTimeline.png"));
	}
	if (toon->GetName() == "Ratsputin")
	{
		toon->SetPortrait(SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/Bowie.png"));

	}
	return toon;



}
Enemy* GamePlayState::LoadEnemy(std::string _path)
{

	Enemy * toon = nullptr;
	TiXmlDocument doc;
	if (doc.LoadFile(_path.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string name = root->FirstChildElement("Name")->GetText();
		std::string element = root->FirstChildElement("Element")->GetText();
		std::string type = root->FirstChildElement("Type")->GetText();
		std::string random = root->FirstChildElement("IsRandom")->GetText();
		//load in the Stats
		Stats stats;
		int HP;
		int BP;
		int level;
		float speed;
		TiXmlElement * Stat = root->FirstChildElement("Stats");
		HP = std::stoi(Stat->FirstChildElement("HitPoints")->FirstChildElement("Base")->GetText());
		stats.hp_scale = std::stof(Stat->FirstChildElement("HitPoints")->FirstChildElement("Scale")->GetText());

		BP = std::stoi(Stat->FirstChildElement("BattlePoints")->FirstChildElement("Base")->GetText());
		stats.bp_scale = std::stof(Stat->FirstChildElement("BattlePoints")->FirstChildElement("Scale")->GetText());

		stats.attack = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.attack_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		stats.magic = std::stoi(Stat->FirstChildElement("Magic")->FirstChildElement("Base")->GetText());
		stats.magic_scale = std::stof(Stat->FirstChildElement("Magic")->FirstChildElement("Scale")->GetText());

		stats.defense = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.defense_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		speed = std::stof(Stat->FirstChildElement("Speed")->FirstChildElement("Base")->GetText());
		stats.speed_scale = std::stof(Stat->FirstChildElement("Speed")->FirstChildElement("Scale")->GetText());

		stats.avoision = std::stoi(Stat->FirstChildElement("Attack")->FirstChildElement("Base")->GetText());
		stats.avoision_scale = std::stof(Stat->FirstChildElement("Attack")->FirstChildElement("Scale")->GetText());

		level = std::stoi(root->FirstChildElement("Level")->GetText());

		std::string hurt = root->FirstChildElement("Sound")->FirstChildElement("Hurt")->GetText();
		std::string attack = root->FirstChildElement("Attack")->FirstChildElement("Attack")->GetText();
		std::string death = root->FirstChildElement("Death")->FirstChildElement("Death")->GetText();

		std::string animation = root->FirstChildElement("Animation")->GetText();


		if (type == "Ally")
		{

			return nullptr;
		}
		else if (type == "Enemy")
		{
			//create a enemy
			toon = CreateCommonEnemy(name, stats, level, HP, HP, speed, 0, nullptr, { 0.0f, 0.0f }, { 0.0f, 0.0f }, animation);

		}
		else if (type == "Guard")
		{
			return nullptr;
		}
		else
		{
			return nullptr;
		}
		if (element == "Wind")
		{
			toon->SetEtype(ETYPE::WIND);
		}
		else if (element == "Fire")
		{
			toon->SetEtype(ETYPE::FIRE);
		}
		else if (element == "Earth")
		{
			toon->SetEtype(ETYPE::EARTH);
		}
		else if (element == "Water")
		{
			toon->SetEtype(ETYPE::WATER);
		}
		else
		{
			toon->SetEtype(ETYPE::MULTI);
		}
	}
	return toon;
}

void GamePlayState::RandomAnimation()
{
	SGD::AudioManager::GetInstance()->PlayAudio(entercombat);
	for (int i = 0; i < 20; i += 3)
	{
		for (int j = 0; j < 20; j += 3)
		{

			TownRender();
			SGD::GraphicsManager::GetInstance()->DrawRectangle({ 0.0f + (i * 40), 0.0f + (j * 40), (i * 40) + 40.0f, (j * 40) + 40.0f }, { 0, 0, 0 });
			SGD::GraphicsManager::GetInstance()->DrawRectangle({ 0.0f + (j * 40), 0.0f + (i * 40), (j * 40) + 40.0f, (i * 40) + 40.0f }, { 0, 0, 0 });
			SGD::GraphicsManager::GetInstance()->Update();

		}
	}
}
void GamePlayState::PauseGame()
{
	state = GPStates::Menu;
}
std::vector<Items> * GamePlayState::GetInventory()
{
	return &inventory;
}