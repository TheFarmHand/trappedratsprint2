
#include "GamePlayState.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../Core/Player.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../Core/CommonEnemy.h"

#include "../Messages/TestMessage.h"
#include "../Animation/AnimationSystem.h"
#include "../States/MainMenuState.h"
#include "../States/LoadGameState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include <sstream>
#include "../tinyxml/tinyxml.h"
#include <Windows.h>
#include <fstream>
#include "../Cutscenes/CutsceneManager.h"
#include "../Core/Boss.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "GameOverWinState.h"


GamePlayState* GamePlayState::GetInstance()
	{
	static GamePlayState data;

	return &data;
	}
void GamePlayState::Enter()
{
	GameData::GetInstance()->ResetPlayer();
	run_succeed = false;
	MinibossFight = false;
	FinalBossFight = false;
	ignore_game_over = false;

	townpoints[0] = SGD::Point(600, 400);
	townpoints[1] = SGD::Point(360, 130);
	townpoints[2] = SGD::Point(120, 345);
	townpoints[3] = SGD::Point(375, 490);
	townpoints[4] = SGD::Point(370, 317);

	WorldMapAnsys = new AnimationSystem();
	WorldMapAnsys->Load("RatAnimOverworld.xml");
	WorldMapAnsys->Play(0);
	WorldMap = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/world.png");
	PadLock = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/padlock.png");


	is_tutorial = true;
	//testFinalFight = true;
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/Scroll.png");
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
	Loading("Loading Tiles...");
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);
	//TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\TestTown.xml");
	Loading("Loading Images...");
	combathud = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/tempcombathud.png");
	enemytargeting = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/enemytargeticon.png" );
	allytargeting = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/allytargeticon.png" );
	buttonimg = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/button.png" );
	combatback = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/CombatTownBack.png" );
	button = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/button.png" );
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/cheese.png" );
	trapcursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/RatTrap.png" );
	cheesecursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/cheese.png" );
	TurnIndicator = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/playerSelect.png");
	Stats sts;
	sts.attack = 10;
	sts.defense = 10;
	sts.avoision = 10;
	sts.magic = 10;


	TurnManager::GetInstance()->SetArrow( allytargeting, enemytargeting );

	Loading("Loading Text...");

	SelectableObject* ob = CreateSelectableObject(buttonimg, { 70, 445 }, { 64, 64 }, "Attack");
	ob->SetSelected(true);
	ob->SetExplination("Deal damage to one enemy");
	m_vSelectableItems.push_back(ob);

	ob = CreateSelectableObject( buttonimg, { 10, 495 }, { 64, 64 }, "Item" );
	ob->SetExplination( "Use an Item to aide your party" );
	m_vSelectableItems.push_back( ob );

	ob = CreateSelectableObject( buttonimg, { 150, 495 }, { 64, 64 }, "Ability" );
	ob->SetExplination( "Use a unique ABILITY" );
	m_vSelectableItems.push_back( ob );

	ob = CreateSelectableObject( buttonimg, { 70, 545 }, { 64, 64 }, "Run" );
	ob->SetExplination( "FLEE from combat" );
	m_vSelectableItems.push_back( ob );

	helptextbox = new HelpText();
	helpback = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/HelpTextBox.png" );
	helptextbox->SetImage( helpback );

	ternary_bar = CreateBar({64,16}, {250,380}, nullptr, {255,255,0}, {0,0}, false);

	std::fstream fin;

	Loading("Loading Abilities...");
	fin.open( "Assets/Scripts/Abilities/AbilityList.txt" );
	if ( fin.is_open() )
		{
		int count;
		char buffer[64];
		std::string abilPath;

		fin >> count;

		for ( int i = 0; i < count; i++ )
			{
			abilPath = "Assets/Scripts/Abilities/";
			fin.ignore( INT_MAX, '\n' );
			fin.get( buffer, 64, '\n' );
			std::string temp( buffer );

			temp += ".xml";
			abilPath += temp;

			Ability* tempAbility = new Ability( abilPath.c_str() );
			MasterAbilityList[tempAbility->GetAbilityName()] = tempAbility;
			}

		}

	Loading("Loading Characters...");
	SGD::Point characterOrderPosition;
	CombatPlayer* p1 = nullptr;
	
	p1 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Ratsputin.xml" ) );
	p1->SetOrderPosition( 0 );
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p1->GetOrderPosition() * 100 + 150 );
	p1->SetPosition( characterOrderPosition );
	p1->SetSize( { 64, 64 } );
	std::vector<Ability*> partyAbilities;
	partyAbilities.push_back( MasterAbilityList["Burrow"] );
	partyAbilities.push_back( MasterAbilityList["Water Fang"] );
	partyAbilities.push_back( MasterAbilityList["Slow Claw"] );
	partyAbilities.push_back( MasterAbilityList["Earth Fang"] );
	partyAbilities.push_back( MasterAbilityList["Poison Fang"] );
	partyAbilities.push_back( MasterAbilityList["Fire Fang"] );
	partyAbilities.push_back( MasterAbilityList["Counter Claw"] );
	partyAbilities.push_back( MasterAbilityList["Wind Fang"] );
	p1->SetActive( true );
	p1->InitializeAbilities( partyAbilities );
	Party.push_back( p1 );

	partyAbilities.clear();

	//CombatPlayer* p2 = nullptr;
	//p2 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Slippy.xml" ) );
	//p2->SetOrderPosition( 1 );
	//characterOrderPosition.x = 100.0f;
	//characterOrderPosition.y = (float)( p2->GetOrderPosition() * 100 + 150 );
	//p2->SetPosition( characterOrderPosition );
	//p2->SetSize( { 64, 64 } );
	//partyAbilities.push_back( MasterAbilityList["Puddle"] );
	//partyAbilities.push_back( MasterAbilityList["Whirlpool"] );
	//partyAbilities.push_back( MasterAbilityList["Acid Rain"] );
	//partyAbilities.push_back( MasterAbilityList["Torrent"] );
	//partyAbilities.push_back( MasterAbilityList["Flood"] );
	//partyAbilities.push_back( MasterAbilityList["Squirt"] );
	//partyAbilities.push_back( MasterAbilityList["Dissolve"] );
	//partyAbilities.push_back( MasterAbilityList["Splash"] );

	//p2->SetActive( true );
	//p2->InitializeAbilities( partyAbilities );
	//Party.push_back( p2 );

	//partyAbilities.clear();

	//CombatPlayer* p3 = nullptr;
	//p3 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Checkers.xml" ) );
	//p3->SetOrderPosition( 2 );
	//characterOrderPosition.x = 100.0f;
	//characterOrderPosition.y = (float)( p3->GetOrderPosition() * 100 + 150 );
	//p3->SetPosition( characterOrderPosition );
	//p3->SetSize( { 64, 64 } );
	//partyAbilities.push_back( MasterAbilityList["Cover"] );
	//partyAbilities.push_back( MasterAbilityList["Geo Crush"] );
	//partyAbilities.push_back( MasterAbilityList["Pinch"] );
	//partyAbilities.push_back( MasterAbilityList["Quake"] );
	//partyAbilities.push_back( MasterAbilityList["Hedge Guard"] );
	//partyAbilities.push_back( MasterAbilityList["Rock Spike"] );
	//partyAbilities.push_back( MasterAbilityList["Rampart"] );
	//partyAbilities.push_back( MasterAbilityList["Tremor"] );
	//
	//p3->SetActive( true );
	//p3->InitializeAbilities( partyAbilities );
	//Party.push_back( p3 );

	//partyAbilities.clear();

	//CombatPlayer* p4 = nullptr;
	//p4 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Biggs.xml" ) );
	//p4->SetOrderPosition( 3 );
	//characterOrderPosition.x = 100.0f;
	//characterOrderPosition.y = (float)( p4->GetOrderPosition() * 100 + 150 );
	//p4->SetPosition( characterOrderPosition );
	//p4->SetSize( { 64, 64 } );
	//partyAbilities.push_back( MasterAbilityList["Zephyr"] );
	//partyAbilities.push_back( MasterAbilityList["Leaf on the Wind"] );
	//partyAbilities.push_back( MasterAbilityList["Second Wind"] );
	//partyAbilities.push_back( MasterAbilityList["Whispering Wind"] );
	//partyAbilities.push_back( MasterAbilityList["Tailwind"] );
	//partyAbilities.push_back( MasterAbilityList["Tempest"] );
	//partyAbilities.push_back( MasterAbilityList["Tornado"] );
	//partyAbilities.push_back( MasterAbilityList["Wind Vale"] );
	//p4->InitializeAbilities( partyAbilities );
	//p4->SetActive( false );
	//Party.push_back( p4 );

	//partyAbilities.clear();

	/*CombatPlayer* p5 = nullptr;
	p5 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/testcharacterJeeves.xml" ) );
	p5->SetOrderPosition( 4 );
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p5->GetOrderPosition() * 100 + 150 );
	p5->SetPosition( characterOrderPosition );
	p5->SetSize( { 64, 64 } );
	partyAbilities.push_back( MasterAbilityList["Collapse"] );
	partyAbilities.push_back( MasterAbilityList["Emblazon"] );
	partyAbilities.push_back( MasterAbilityList["Firefall"] );
	partyAbilities.push_back( MasterAbilityList["Fire Spikes"] );
	partyAbilities.push_back( MasterAbilityList["Ignite"] );
	partyAbilities.push_back( MasterAbilityList["Scorch"] );
	partyAbilities.push_back( MasterAbilityList["Rib-a-Rang"] );
	
	partyAbilities.push_back( MasterAbilityList["Incinerate"] );
	p5->InitializeAbilities( partyAbilities );
	p5->SetActive(false);
	Party.push_back( p5 );

	partyAbilities.clear();*/
	//build the shop inventory
	Items smallheal;
	smallheal.SetName("Small Heal");
	smallheal.SetPrice(10);
	smallheal.SetExplination("Heals the Target for_a Small Amount");
	Items largeheal;
	largeheal.SetName("Large Heal");
	largeheal.SetPrice(20);
	largeheal.SetExplination("Heals the Target for_a Large Amount");
	Items smallbp;
	smallbp.SetName("Small BP Restore");
	smallbp.SetPrice(10);
	smallbp.SetExplination("Restores a Small Amount of BP");
	Items largebp;
	largebp.SetName("Large BP Restore");
	largebp.SetPrice(20);
	largebp.SetExplination("Restores a Large Amount of BP");
	Items revive;
	revive.SetName("Revive");
	revive.SetPrice(40);
	revive.SetExplination("Brings the target Back_from the Dead");


	shopinv.push_back(smallheal);
	shopinv.push_back(largeheal);
	shopinv.push_back(smallbp);
	shopinv.push_back(largebp);
	shopinv.push_back(revive);
	//if this is a tutorial, we load in the stuff for it
	//if ( testFinalFight )
	//	{
	//	std::vector<Enemy*> bosses;
	//	//Boss* cecilFinal = new Boss( "Cecil" );
	//	//Boss* janeFinal = new Boss( "Jane" );
	//	//Boss* johnFinal = new Boss( "John" );
	//	Enemy* cecilFinal = nullptr;
	//	cecilFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Cecil.xml" );
	//	cecilFinal->SetOrderPosition( 1 );
	//	characterOrderPosition.x = 600.0f;
	//	characterOrderPosition.y = (float)( cecilFinal->GetOrderPosition() * 100 + 150 + 16 );
	//	cecilFinal->SetPosition( characterOrderPosition );
	//	partyAbilities.push_back( MasterAbilityList["Holy Shield"] );
	//	partyAbilities.push_back( MasterAbilityList["Holy Mace"] );
	//	partyAbilities.push_back( MasterAbilityList["Holy Flare"] );
	//	partyAbilities.push_back( MasterAbilityList["Retribution"] );
	//	partyAbilities.push_back( MasterAbilityList["Cover"] );
	//	cecilFinal->InitializeAbilities( partyAbilities );
	//	partyAbilities.clear();

	//	Enemy* janeFinal = nullptr;
	//	janeFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Jane.xml" );
	//	janeFinal->SetOrderPosition( 0 );
	//	janeFinal->SetLiving( false );
	//	janeFinal->SetHP( 0 );
	//	characterOrderPosition.x = 600.0f;
	//	characterOrderPosition.y = (float)( janeFinal->GetOrderPosition() * 100 + 150 + 16 );
	//	janeFinal->SetPosition( characterOrderPosition );
	//	partyAbilities.push_back( MasterAbilityList["Healing Light"] );
	//	partyAbilities.push_back( MasterAbilityList["Dia"] );
	//	partyAbilities.push_back( MasterAbilityList["Protect"] );
	//	janeFinal->InitializeAbilities( partyAbilities );
	//	partyAbilities.clear();

	//	Enemy* johnFinal = nullptr;
	//	johnFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final John.xml" );
	//	johnFinal->SetOrderPosition( 2 );
	//	johnFinal->SetLiving( false );
	//	johnFinal->SetHP( 0 );
	//	characterOrderPosition.x = 600.0f;
	//	characterOrderPosition.y = (float)( johnFinal->GetOrderPosition() * 100 + 150 + 16 );
	//	johnFinal->SetPosition( characterOrderPosition );
	//	partyAbilities.push_back( MasterAbilityList["Sure Shot"] );
	//	partyAbilities.push_back( MasterAbilityList["Barrage"] );
	//	partyAbilities.push_back( MasterAbilityList["Haste"] );
	//	johnFinal->InitializeAbilities( partyAbilities );
	//	partyAbilities.clear();

	//	bosses.push_back( janeFinal );
	//	bosses.push_back( cecilFinal );
	//	bosses.push_back( johnFinal );


	//	for ( unsigned int i = 0; i < Party.size(); i++ )
	//		{
	//		Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
	//		Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
	//		}
	//	TurnManager::GetInstance()->Initialize( Party, bosses );
	//	for ( size_t i = 0; i < Party.size(); i++ )
	//		{
	//		if ( Party[i]->GetActive() )
	//			{
	//			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
	//			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
	//			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
	//			}
	//		}

	//	for ( size_t i = 0; i < bosses.size(); i++ )
	//		{
	//		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
	//		}
	//	GameData::GetInstance()->SetIsInCombat( true );
	//	state = Combat;
	//	laststate = Combat;

	//	}
	if (is_tutorial)
	{
		std::vector<Enemy*> tempEnemy;
		std::vector<CombatPlayer*> tempParty;
		CombatPlayer* p6 = nullptr;
		p6 = (LoadCombatPlayer("../Trapped Rat/Assets/Scripts/mom.xml"));
		p6->SetOrderPosition(0);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(p6->GetOrderPosition() * 100 + 150);
		p6->SetPosition(characterOrderPosition);
		p6->SetSize({ 64, 64 });
		partyAbilities.push_back(MasterAbilityList["Burrow"]);
		partyAbilities.push_back(MasterAbilityList["Water Fang"]);
		partyAbilities.push_back(MasterAbilityList["Slow Claw"]);
		partyAbilities.push_back(MasterAbilityList["Earth Fang"]);
		partyAbilities.push_back(MasterAbilityList["Poison Fang"]);
		partyAbilities.push_back(MasterAbilityList["Fire Fang"]);
		partyAbilities.push_back(MasterAbilityList["Counter Claw"]);
		partyAbilities.push_back(MasterAbilityList["Wind Fang"]);
		p6->InitializeAbilities(partyAbilities);
		p6->SetActive(true);
		Parents.push_back(p6);

		partyAbilities.clear();

		CombatPlayer* p7 = nullptr;
		p7 = (LoadCombatPlayer("../Trapped Rat/Assets/Scripts/dad.xml"));
		p7->SetOrderPosition(1);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(p7->GetOrderPosition() * 100 + 150);
		p7->SetPosition(characterOrderPosition);
		p7->SetSize({ 64, 64 });
		partyAbilities.push_back(MasterAbilityList["Burrow"]);
		partyAbilities.push_back(MasterAbilityList["Water Fang"]);
		partyAbilities.push_back(MasterAbilityList["Slow Claw"]);
		partyAbilities.push_back(MasterAbilityList["Earth Fang"]);
		partyAbilities.push_back(MasterAbilityList["Poison Fang"]);
		partyAbilities.push_back(MasterAbilityList["Fire Fang"]);
		partyAbilities.push_back(MasterAbilityList["Counter Claw"]);
		partyAbilities.push_back(MasterAbilityList["Wind Fang"]);
		p7->InitializeAbilities(partyAbilities);
		p7->SetActive(true);
		Parents.push_back(p7);

		Enemy* cecil = nullptr;
		cecil = LoadEnemy("../Trapped Rat/Assets/Scripts/Cecil.xml");
		cecil->SetOrderPosition(0);
		characterOrderPosition.x = 600.0f;
		characterOrderPosition.y = (float)(cecil->GetOrderPosition() * 100 + 150 + 16);
		cecil->SetPosition(characterOrderPosition);
		tutorialenemy.push_back(cecil);

		for (unsigned int i = 0; i < Parents.size(); i++)
		{
		Parents[i]->GetAbility( 0 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 1 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 2 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 3 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 4 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 5 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 6 )->CalcluateBpScaledCost( Parents[i] );
		Parents[i]->GetAbility( 7 )->CalcluateBpScaledCost( Parents[i] );
		}
		TurnManager::GetInstance()->Initialize(Parents, tutorialenemy);
		for (size_t i = 0; i < Parents.size(); i++)
		{
			if (Parents[i]->GetActive())
			{
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(), Parents[i], SGD::Color(0, 255, 0), SGD::Point(-30, -25)));
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(630, 440 + (Parents[i]->GetOrderPosition()*50.0f)), Parents[i], SGD::Color(0, 255, 0), SGD::Point(0, 0)));
				m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(630, 465 + (Parents[i]->GetOrderPosition()*50.0f)), Parents[i], SGD::Color(0, 100, 255), SGD::Point(0, 0), false));
			}
		}


		for (size_t i = 0; i < tutorialenemy.size(); i++)
		{
			m_vhuditems.push_back(CreateBar({ 64, 16 }, SGD::Point(), tutorialenemy[i], SGD::Color(0, 255, 0), SGD::Point(-30, -45)));
		}
		state = Combat;
		laststate =Combat;
		GameData::GetInstance()->SetIsInCombat(true);
	}

	//Sets the abilities to unlocked if level is high enough (new game will only unlock the first ability)
	CheckAbilityUnlocked();

	dialogue = new Dialogue();
	//dialogue->Load( "../Trapped Rat/Assets/Scripts/testdialogue.xml" );
	//state = GPStates::Dia;
	Loading("Loading Sounds...");
	m_Audio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/req.xwm" );
	m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/ZeldaMetal.xwm" );
	entercombat = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/entercombat.wav" );
	SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio, true );
	SGD::AudioManager::GetInstance()->SetMasterVolume( SGD::AudioGroup::Music, GameData::GetInstance()->GetMusicVolume() );
	SGD::AudioManager::GetInstance()->SetMasterVolume( SGD::AudioGroup::SoundEffects, GameData::GetInstance()->GetEffectVolume() );

	GameData::GetInstance()->UpdateCamera( GameData::GetInstance()->GetOverworldPlayer() );
	pStatManager = StatusEffectManager::GetInstance();
	pStatManager->Initialize();

	CutsceneManager::GetInstance()->Initialize();
	//state = GPStates::Cuts;
	//CutsceneManager::GetInstance()->Play( 0 );
	Loading("Time to Play.......");

	MinibossFight = false;
	FinalBossFight = false;
	SGD::InputManager::GetInstance()->Update();
	state = Combat;
	laststate = Combat;


	//state = BattleSummary;
	}
void const GamePlayState::Render()
	{
	switch ( state )
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
			MapRender();
			break;
		case GPStates::Dia:
			DialogueRender();
			break;
		case GPStates::Cuts:
			CutsceneRender();
			break;
		case GPStates::BattleSummary:
			SummaryRender();
			break;
		default:
			break;
		}
	}
void GamePlayState::Update( float dt )
	{
	//pause the game if alt + tab
	if ( SGD::InputManager::GetInstance()->IsKeyDown( SGD::Key::Alt ) )
		{
		if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Tab ) )
			{
			state = GPStates::Menu;
			}
		}
	SGD::EventManager::GetInstance()->Update();

	//put everything into functions here
	//its going to get packed

	switch ( state )
		{
		case GPStates::Combat:
			CombatUpdate( dt );
			break;
		case GPStates::Town:
			//see if we enter combat
			Fight();
			TownUpdate( dt );

			break;
		case GPStates::Menu:
			MenuUpdate( dt );
			break;
		case GPStates::Map:
			MapUpdate(dt);
			break;
		case GPStates::Dia:
			DialogueUpdate( dt );
			break;
		case GPStates::Cuts:
			CutsceneUpdate( dt );
			break;
		case GPStates::BattleSummary:
			SummaryUpdate(dt);
			break;
		default:
			break;
		}




	}
void GamePlayState::Exit()
	{
	Loading("Dumping...");
	for ( size_t i = 0; i < m_vhuditems.size(); i++ )
		{
		delete m_vhuditems[i];
		}
	m_vhuditems.clear();
	for ( size_t i = 0; i < m_vSelectableItems.size(); i++ )
		{
		delete m_vSelectableItems[i];
		}
	m_vSelectableItems.clear();
	if ( enemy1 != nullptr )
		{
		delete enemy1;
		enemy1 = nullptr;
		}
	if ( dialogue != nullptr )
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

	if ( helptextbox != nullptr )
		delete helptextbox;

	TurnManager::GetInstance( )->Terminate( );
	delete ternary_bar;
	for ( unsigned int i = 0; i < Party.size(); i++ )
		{
		delete Party[i];
		Party[i] = nullptr;
		}
	for (unsigned int i = 0; i < Parents.size(); i++)
	{
		delete Parents[i];
		Parents[i] = nullptr;

	}
	for (unsigned int i = 0; i < guards.size(); i++)
	{
		delete guards[i];
		guards[i] = nullptr;
	}
	tutorialenemy.clear();
	guards.clear();
	shopinv.clear();
	Party.clear();
	Parents.clear();
	delete WorldMapAnsys;
	SGD::GraphicsManager::GetInstance()->UnloadTexture( combatback );

	SGD::AudioManager::GetInstance()->UnloadAudio( m_Audio );
	SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
	SGD::AudioManager::GetInstance()->UnloadAudio( entercombat );

	SGD::GraphicsManager::GetInstance()->UnloadTexture(WorldMap);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(PadLock);

	SGD::GraphicsManager::GetInstance()->UnloadTexture( background );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( button );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( cursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( scroll );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( trapcursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( cheesecursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture(TurnIndicator);
	SGD::GraphicsManager::GetInstance()->UnloadTexture( helpback );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( combathud );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( allytargeting );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( enemytargeting );
	if ( buttonimg != SGD::INVALID_HANDLE )
		SGD::GraphicsManager::GetInstance()->UnloadTexture( buttonimg );

	CutsceneManager::GetInstance()->Terminate();

	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::GetInstance()->DeleteInstance();

	TileSystem::GetInstance()->Exit();

	// Double Check ParticleManager in TurnManager (Particles need to be a singleton, blah)
	// Cleans up those leaks
	

	auto iterAbil = MasterAbilityList.begin();
	for ( ; iterAbil != MasterAbilityList.end(); ++iterAbil )
		{
		delete iterAbil->second;
		}
	MasterAbilityList.clear();

	pStatManager->Terminate();
	pStatManager = nullptr;

	}

void GamePlayState::MessageProc( const SGD::Message * mess )
	{
	switch ( mess->GetMessageID() )
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
	if ( GameData::GetInstance()->GetOverworldPlayer()->IsMoving() && !GameData::GetInstance()->GetIsInCombat() )
		{
		stepcounter++;
		//int sorandom = rand() % 250;
		//if (!sorandom)
		if ( stepcounter >= 300 )
			{
			stepcounter = rand() % 100;
			//play animation for entering random combat
			RandomAnimation();
			if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
				SGD::AudioManager::GetInstance()->StopAudio( m_overAudio );

			SGD::AudioManager::GetInstance()->PlayAudio( m_Audio, true );

			GameData::GetInstance()->SetIsInCombat( true );
			state = GPStates::Combat;




			std::vector<Enemy*> tempEnemy;



			if ( enemy1 != nullptr )
				delete enemy1;
			if ( enemy2 != nullptr )
				delete enemy2;
			if ( enemy3 != nullptr )
				delete enemy3;

			Stats sts;
			sts.attack = 10;
			sts.defense = 10;
			sts.avoision = 10;
			sts.magic = 10;

			SGD::Point characterOrderPosition;
			int randomEnemyIndex;
			int randomEnemyParySize;

			randomEnemyParySize = rand() % 3 + 1;
			Enemy* tempRandomEnemy;
			std::vector<Ability*> partyAbilities;

			for ( int i = 0; i < randomEnemyParySize; i++ )
				{
				randomEnemyIndex = rand() % 8;

				switch ( randomEnemyIndex )
					{
					case 0:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Dog.xml" );
						partyAbilities.push_back( MasterAbilityList["Feral Bite"] );
						break;
					case 1:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cat.xml" );
						partyAbilities.push_back( MasterAbilityList["Scratch"] );
						break;
					case 2:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Raven.xml" );
						partyAbilities.push_back( MasterAbilityList["Feathered Flurry"] );
						break;
					case 3:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Chef.xml" );
						partyAbilities.push_back( MasterAbilityList["Fresh Meat"] );
						break;
					case 4:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Blacksmith.xml" );
						partyAbilities.push_back( MasterAbilityList["Bellows Blast"] );
						break;
					case 5:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Shopkeeper.xml" );
						partyAbilities.push_back( MasterAbilityList["Elemental Poison"] );
						break;
					case 6:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Tailor.xml" );
						partyAbilities.push_back( MasterAbilityList["Stitch in Time"] );
						break;
					case 7:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Priest.xml" );
						partyAbilities.push_back( MasterAbilityList["Healing Light"] );
						break;
					default:
						break;
					}
				tempRandomEnemy->SetHP(tempRandomEnemy->GetMaxHP());

				tempRandomEnemy->SetOrderPosition( i );
				characterOrderPosition.x = 600.0f;
				characterOrderPosition.y = (float)( tempRandomEnemy->GetOrderPosition() * 100 + 150 + 16 );
				tempRandomEnemy->SetPosition( characterOrderPosition );
				tempRandomEnemy->InitializeAbilities( partyAbilities );
				partyAbilities.clear();
				tempEnemy.push_back( tempRandomEnemy );
				}
			//Character* en1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Dog.xml" );
			////Character* en1 = CreateCommonEnemy( "Dog", sts, 1, 50, 50, 7.0f, 0.0f, nullptr, SGD::Point( 600, 160 ), SGD::Size( 64, 64 ), "DogAnimBattle.xml" );
			//enemy1 = dynamic_cast<Enemy*>( en1 );
			////enemy1->SetEtype( WIND );
			//enemy1->SetOrderPosition( 0 );
			//characterOrderPosition.x = 600.0f;
			//characterOrderPosition.y = (float)( enemy1->GetOrderPosition() * 100 + 150 + 16 );
			//enemy1->SetPosition( characterOrderPosition );

			//Character* en2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cat.xml" );
			//	//CreateCommonEnemy( "Dog", sts, 1, 50, 50, 8.0f, 0.0f, nullptr, SGD::Point( 600, 260 ), SGD::Size( 64, 64 ), "DogAnimBattle.xml" );
			//enemy2 = dynamic_cast<Enemy*>( en2 );
			////enemy2->SetEtype( WATER );
			//enemy2->SetOrderPosition( 1 );
			//characterOrderPosition.x = 600.0f;
			//characterOrderPosition.y = (float)( enemy2->GetOrderPosition() * 100 + 150 + 16 );
			//enemy2->SetPosition( characterOrderPosition );

			//Character* en3 = CreateCommonEnemy( "Dog", sts, 1, 50, 50, 7.0f, 0.0f, nullptr, SGD::Point( 600, 360 ), SGD::Size( 64, 64 ), "DogAnimBattle.xml" );
			//enemy3 = dynamic_cast<Enemy*>( en3 );
			//enemy3->SetEtype( EARTH );
			//enemy3->SetOrderPosition( 2 );
			//characterOrderPosition.x = 600.0f;
			//characterOrderPosition.y = (float)( enemy3->GetOrderPosition() * 100 + 150 + 16 );
			//enemy3->SetPosition( characterOrderPosition );

			//tempEnemy.push_back( enemy1 );
			//tempEnemy.push_back( enemy2 );
			//tempEnemy.push_back( enemy3 );
			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			
			TurnManager::GetInstance()->Initialize( Party, tempEnemy );
			for ( size_t i = 0; i < Party.size(); i++ )
				{
				if ( Party[i]->GetActive() )
					{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}


			for ( size_t i = 0; i < tempEnemy.size(); i++ )
				{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), tempEnemy[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
			}
		}


	}

void GamePlayState::TownUpdate( float dt )
	{
	GameData::GetInstance()->GetOverworldPlayer()->Update( dt );
	GameData::GetInstance()->UpdateCamera( GameData::GetInstance()->GetOverworldPlayer() );

	SGD::InputManager * input = SGD::InputManager::GetInstance();
	if ( input->IsKeyPressed( SGD::Key::Escape ) )
		{
		CheckAbilityUnlocked();
		state = GPStates::Menu;
		}

	for (unsigned int i = 0; i < guards.size(); i++)
	{
		guards[i]->Update(dt);
		
		//if(guards[i]->GetRect().IsIntersecting(GameData::GetInstance()->GetOverworldPlayer()->GetRect()))
		if(TileSystem::GetInstance()->GetTileIndex(guards[i]->GetPosition().x, guards[i]->GetPosition().y)
			== TileSystem::GetInstance()->GetTileIndex(
			 GameData::GetInstance( )->GetOverworldPlayer( )->GetPosition().x,
			 GameData::GetInstance( )->GetOverworldPlayer( )->GetPosition( ).y))

		{
			Enemy* temp;
			guard_index = i;
			std::vector<Ability*> partyAbilities;
			
			temp = LoadEnemy( "../Trapped Rat/Assets/Scripts/Guard.xml" );
			SGD::Point characterOrderPosition;
			temp->SetOrderPosition( 1 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( temp->GetOrderPosition( ) * 100 + 150 + 16 );
			temp->SetPosition( characterOrderPosition );
			std::vector<Enemy*> moreguards;
			partyAbilities.push_back( MasterAbilityList["Rush"] );
			temp->InitializeAbilities( partyAbilities );
			moreguards.push_back(temp);
			partyAbilities.clear();

			GameData::GetInstance( )->SetIsInCombat( true );
			state = GPStates::Combat;
			TurnManager::GetInstance( )->Initialize( Party, moreguards );
			for ( size_t i = 0; i < Party.size( ); i++ )
			{
				if ( Party[ i ]->GetActive( ) )
				{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition( )*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition( )*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
				}
			}


			for ( size_t i = 0; i < moreguards.size( ); i++ )
			{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( ), moreguards[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
			}
		}
	}
}
void GamePlayState::MenuUpdate( float dt )
	{
	//set the maxindex based on what menu we are in
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	switch ( substate )
		{
		case None:
			maxindex = 4;
			if (laststate == Combat)
				maxindex = 2;
			if ( input->IsKeyPressed( SGD::Key::Escape ) )
				{
				GameData::GetInstance()->PlaySelectionChange();
				menuindex = 0;
				state = laststate;
				}
			break;
		case Options:
			maxindex = 4;
			if ( input->IsKeyPressed( SGD::Key::Escape ) )
				{
				GameData::GetInstance()->PlaySelectionChange();
				menuindex = 0;
				substate = MenuSubStates::None;

				}
			break;
		case Shop:
			maxindex = (int)shopinv.size() - 1;
			if ( input->IsKeyPressed( SGD::Key::Escape ) )
				{
				GameData::GetInstance()->PlaySelectionChange();
				menuindex = 0;
				substate = MenuSubStates::None;
				//shopinv.clear();

				}
			break;
		case HowTo:
			break;
		case MenuSubStates::Party:
			maxindex = Party.size() - 1;
			if (select_first && selecting_party == false)
				maxindex = 1;
			if (selecting_ability)
				maxindex = Party[character_index]->GetAbilitiesSize() - 1;
			if ( input->IsKeyPressed( SGD::Key::Escape ) )
				{
				if (selecting_ability || selecting_party || select_first)
				{
					selecting_ability = false;
					selecting_party = false;
					select_first = false;
					select_new = false;
					menuindex = 0;
					oldindex = -1;
				}
				else
				{
					GameData::GetInstance()->PlaySelectionChange();
					menuindex = 0;
					substate = MenuSubStates::None;
				}
				}
			
			break;
		default:
			break;
		}




	//if they select a menu item
	if ( input->IsKeyPressed( SGD::Key::Enter ) )
		{
		switch ( substate )
			{
			case None:
		{

					 
					 switch (menuindex)
					{
					case 0:
						{
						GameData::GetInstance()->PlaySelectionChange();
							   if (laststate == Combat)
							   {
								   GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
								   state = GPStates::Town;
								   laststate = state;
								   substate = MenuSubStates::None;
								   menuindex = 0;
							   }
							   else
						substate = MenuSubStates::Shop;


						break;
						}
					case 1:
						 if (laststate == Combat)
						 {
							 GameData::GetInstance()->PlaySelectionChange();
						menuindex = 0;
							 substate = MenuSubStates::Options;
						 }
						 else
						 {
							 menuindex = 0;
						selecting_ability = false;
						selecting_party = false;
						select_first = false;
						select_new = false;
						substate = MenuSubStates::Party;
						GameData::GetInstance()->PlaySelectionChange();
						 }

						break;
					case 2:
						if (laststate == Combat)
						{
							GameData::GetInstance()->PlaySelectionChange();
							state = laststate;
							substate = MenuSubStates::None;
							menuindex = 0;
						}
						else
						{
							GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
							state = GPStates::Town;
							laststate = state;
							substate = MenuSubStates::None;
							menuindex = 0;
						}
						


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
		}
			case Options:
				switch ( menuindex )
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
				
					if ( shopinv[menuindex].GetPrice() <= gold )
						{
						inventory.push_back( shopinv[menuindex] );
						gold -= shopinv[menuindex].GetPrice();
						}
					
				break;
			case MenuSubStates::Party:

				if (selecting_ability == false && selecting_party == false && select_first == false)
				{
					select_first = true;
					character_index = menuindex;
					menuindex = 0;
					maxindex = 2;
				}
				else if (selecting_ability == false && selecting_party == false && select_first == true)
				{
					if (menuindex == 0)
					{
						//Hovering over Swap character
						selecting_party = true;
					}
					if (menuindex == 1)
					{
						//Hovering Over Swap Ability
						selecting_ability = true;
					}
				}
				/*else if (selecting_ability == false)
				{
					selecting_ability = true;
					character_index = menuindex;
					menuindex = 0;
					}*/
				else if (selecting_ability && !select_new)
				{
					if (!Party[character_index]->GetAbility(menuindex)->GetUnlocked())
						return;
					select_new = true;
					//Party[character_index]->GetAbility(menuindex)->SetUnlocked(false);
					oldindex = menuindex;

				}
				else if (select_new && selecting_ability)
				{
					if (!Party[character_index]->GetAbility(menuindex)->GetUnlocked())
						return;
						//Party[character_index]->GetAbility(menuindex)->SetUnlocked(true);
					if (Party[character_index]->GetLevel() >= Party[character_index]->GetAbility(menuindex)->GetUnlockLevel())
					{

						Ability* temp = Party[character_index]->GetAbility(menuindex);
						Ability *temp2 = Party[character_index]->GetAbility(oldindex);
						Party[character_index]->SetAbility(menuindex, temp2);
						Party[character_index]->SetAbility(oldindex, temp);
					}
					
					select_new = false;
					selecting_ability = false;
					select_first = false;
					menuindex = character_index;
					oldindex = -1;
				}
				else if (selecting_party && !select_new)
				{
					/*select_new = true;
					oldindex = menuindex;*/
					//Actually Move stuff
					bool ratactive = false;
					int ratindex = 0;
					CombatPlayer* temp = Party[character_index];
					Party[character_index] = Party[menuindex];
					Party[menuindex] = temp;
					for (unsigned int i = 0; i < Party.size(); i++)
					{
						if (Party[i]->GetName() == "Ratsputin")
						{
							ratindex = i;
						}
						if (i <= 2)
						{
							Party[i]->SetActive(true);
							if (Party[i]->GetName() == "Ratsputin")
							{
								ratactive = true;
							}
						}
						else
						{
							Party[i]->SetActive(false);
						}
						Party[i]->SetOrderPosition(i);

						SGD::Point characterOrderPosition;
						characterOrderPosition.x = 100.0f;
						characterOrderPosition.y = (float)(Party[i]->GetOrderPosition() * 100 + 150);
						Party[i]->SetPosition(characterOrderPosition);
					}
					if (!ratactive)
					{
						CombatPlayer* temp2 = Party[ratindex];
						Party[ratindex] = Party[1];
						Party[1] = temp2;
						Party[1]->SetActive(true);
						Party[1]->SetOrderPosition(1);
						Party[ratindex]->SetActive(false);
					}

					select_new = false;
					selecting_party = false;
					select_first = false;
					menuindex = character_index;
					oldindex = -1;

				}
				break;
			default:
				break;
			}


		}


	//changing the values for language


	if ( input->IsKeyPressed( SGD::Key::Left ) || input->IsKeyPressed( SGD::Key::Right ) )
		{
		switch ( substate )
			{
			case None:
				switch ( menuindex )
					{
					case 0:
						break;
					default:
						break;
					}
				break;
			case Options:
				{
				switch ( menuindex )
					{

					case 0:
						{
						bool is = GameData::GetInstance()->GetFont()->IsSpanish();
						GameData::GetInstance()->GetFont()->SetSpanish( !is );
						GameData::GetInstance()->PlaySelectionChange();
						break;
						}
					case 1:
						{

						if ( input->IsKeyPressed( SGD::Key::Left ) )
							{
							GameData::GetInstance()->SetMusicVolume( GameData::GetInstance()->GetMusicVolume() - 5 );
							}
						else if ( input->IsKeyPressed( SGD::Key::Right ) )
							{
							GameData::GetInstance()->SetMusicVolume( GameData::GetInstance()->GetMusicVolume() + 5 );
							}
						GameData::GetInstance()->PlaySelectionChange();
						break;
						}
					case 2:
						{
						if ( input->IsKeyPressed( SGD::Key::Left ) )
							{
							GameData::GetInstance()->SetEffectVolume( GameData::GetInstance()->GetEffectVolume() - 5 );
							}
						else if ( input->IsKeyPressed( SGD::Key::Right ) )
							{
							GameData::GetInstance()->SetEffectVolume( GameData::GetInstance()->GetEffectVolume() + 5 );
							}
						GameData::GetInstance()->PlaySelectionChange();
						break;
						}
					case 3:
						{
						GameData::GetInstance()->PlaySelectionChange();
						bool is = GameData::GetInstance()->GetWindowed();
						GameData::GetInstance()->SetWindowed( !is );
						SGD::GraphicsManager::GetInstance()->Resize( { GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed() );

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
	if ( input->IsKeyPressed( SGD::Key::Up ) )
		{
		menuindex--;
		if ( menuindex < 0 )
			menuindex = maxindex;
		GameData::GetInstance()->PlaySelectionChange();
		}
	if ( input->IsKeyPressed( SGD::Key::Down ) )
		{
		menuindex++;
		if ( menuindex > maxindex )
			menuindex = 0;
		GameData::GetInstance()->PlaySelectionChange();
		}
	}
void GamePlayState::TownRender()
	{
	TileSystem::GetInstance()->Render();

	GameData::GetInstance()->GetOverworldPlayer()->Render();
	for (unsigned int i = 0; i < guards.size(); i++)
	{
		guards[i]->Render();
		SGD::Point xy = guards[i]->GetPosition();
		int x = 0;
	}
	}
void GamePlayState::MenuRender()
	{

	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTextureSection( background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f } );

	switch ( substate )
		{
		case None:
			GameData::GetInstance()->GetFont()->DrawString( "Pause Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			if (laststate != Combat)
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
			}
			graphics->DrawTextureSection( cursor, { 10.0f, 95.0f + ( menuindex * 80 ) }, { 0, 0, 238, 73 } );
			if (laststate == Combat)
			{
				
				GameData::GetInstance()->GetFont()->DrawString("Main Menu", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Options", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Exit", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f);
			}
			else
			{
				GameData::GetInstance()->GetFont()->DrawString("Shop", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Party", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Main Menu", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Options", 100.0f, 360.0f, { 0, 0, 0 }, 2.0f);
				GameData::GetInstance()->GetFont()->DrawString("Exit", 100.0f, 440.0f, { 0, 0, 0 }, 2.0f);
			}


			break;
		case Options:
			{
			SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
			GameData::GetInstance()->GetFont()->DrawString( "Options Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );

			graphics->DrawTextureSection( cursor, { 10.0f, 95.0f + ( menuindex * 80 ) }, { 0, 0, 238, 73 } );

			GameData::GetInstance()->GetFont()->DrawString( "Language:", 70.0f, 120.0f, { 155, 155, 155 }, 1.5f );
			if ( GameData::GetInstance()->GetFont()->IsSpanish() )
				GameData::GetInstance()->GetFont()->DrawString( "Spanish", 150.0f, 125.0f, { 0, 0, 0 } );
			else
				GameData::GetInstance()->GetFont()->DrawString( "English", 175.0f, 125.0f, { 0, 0, 0 } );

			GameData::GetInstance()->GetFont()->DrawString( "Music Volume:", 70.0f, 200.0f, { 155, 155, 155 }, 1.5f );
			std::ostringstream mout;
			mout << GameData::GetInstance()->GetMusicVolume();
			GameData::GetInstance()->GetFont()->DrawString( mout.str(), 220.0f, 205.0f, { 0, 0, 0 } );
			GameData::GetInstance()->GetFont()->DrawString( "Effect Volume:", 70.0f, 280.0f, { 155, 155, 155 }, 1.5f );
			std::ostringstream eout;
			eout << GameData::GetInstance()->GetEffectVolume();
			GameData::GetInstance()->GetFont()->DrawString( eout.str(), 220.0f, 285.0f, { 0, 0, 0 } );

			GameData::GetInstance()->GetFont()->DrawString( "FullScreen:", 70.0f, 360.0f, { 155, 155, 155 }, 1.5f );
			if ( GameData::GetInstance()->GetWindowed() )
				GameData::GetInstance()->GetFont()->DrawString( "Off", 200.0f, 360.0f, { 0, 0, 0 } );
			else
				GameData::GetInstance()->GetFont()->DrawString( "On", 200.0f, 360.0f, { 0, 0, 0 } );

			GameData::GetInstance()->GetFont()->DrawString( "Exit", 70.0f, 440.0f, { 155, 155, 155 }, 1.5f );

			break;
			}
		case Shop:
			{
			//display each item and the price associated with it
			std::ostringstream yourgold;
			yourgold << "Your Gold: " << gold << "g";
			GameData::GetInstance()->GetFont()->DrawString( yourgold.str(), 450.0f, 70.0f, { 0, 0, 0 } );
			GameData::GetInstance()->GetFont()->DrawString( "Shop Inventory", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			GameData::GetInstance()->GetFont()->DrawString( "Escape to Exit", 60.0f, 70.0f, { 0, 0, 0 } );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 50.0f, 50.0f }, { 0, 0, 300, 540 } );
			for ( unsigned int i = 0; i < shopinv.size(); i++ )
				{
				GameData::GetInstance()->GetFont()->DrawString( shopinv[i].GetName(), 100.0f, 160.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
				std::ostringstream price;
				price << shopinv[i].GetPrice() << "g";
				if ( gold < shopinv[i].GetPrice() )
					GameData::GetInstance()->GetFont()->DrawString( price.str(), 200.0f, 185.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
				else
					GameData::GetInstance()->GetFont()->DrawString( price.str(), 200.0f, 185.0f + ( i * 55 ), { 155, 155, 155 }, 1.5f );

				}
			SGD::GraphicsManager::GetInstance()->DrawTexture( trapcursor, { 240.0f, 145.0f + ( menuindex * 55 ) } );

			//now display the players inventory
			GameData::GetInstance()->GetFont()->DrawString( "Your Inventory", 450.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
			for ( unsigned int i = 0; i < inventory.size(); i++ )
				{
				bool is_skip = false;
				for ( int j = i - 1; j > -1; j-- )
					{
					if ( inventory[i] == inventory[j] )
						{
						is_skip = true;
						}
					}
				if ( is_skip )
					continue;

				GameData::GetInstance()->GetFont()->DrawString( inventory[i].GetName(), 490.0f, 160.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
				int count = 0;
				for ( unsigned int j = 0; j < inventory.size(); j++ )
					{
					if ( inventory[i] == inventory[j] )
						count++;
					}

				std::ostringstream total;
				total << count << "/5";
				GameData::GetInstance()->GetFont()->DrawString( total.str(), 490.0f, 185.0f + ( i * 55 ), { 155, 155, 155 }, 1.5f );
				}
			}
			break;
		case MenuSubStates::Party:
			GameData::GetInstance()->GetFont()->DrawString( "Party Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 50.0f, 50.0f }, { 0, 0, 300, 540 } );
			for ( unsigned int i = 0; i < Party.size(); i++ )
			{ 
				if (i < 3)
				{
					GameData::GetInstance()->GetFont()->DrawString(Party[i]->GetName(), 100.0f, 185.0f + (i * 55), { 0, 150, 0 }, 1.5f);
				}
				else
				{
					GameData::GetInstance()->GetFont()->DrawString(Party[i]->GetName(), 100.0f, 185.0f + (i * 55), { 150, 0, 0 }, 1.5f);
				}
				std::ostringstream info;
				info << "HP: " << Party[i]->GetHP() << "/" << Party[i]->GetMaxHP() << "  BP: " << Party[i]->GetBP() << "/" << Party[i]->GetMaxBP();
				GameData::GetInstance()->GetFont()->DrawString( info.str(), 100.0f, 200.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
				SGD::GraphicsManager::GetInstance()->DrawRectangle({ 80.0f, 180.0f, 320.0f, 330.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2);
				}
			
			if (selecting_ability)
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 });
				for (int i = 0; i < Party[character_index]->GetAbilitiesSize(); i++)
				{
					std::string name = Party[character_index]->GetAbility(i)->GetAbilityName();
					if (oldindex == i)
					{
						GameData::GetInstance()->GetFont()->DrawString(name, 490.0f, 185.0f + (i * 35), { 155, 155, 155 }, 1.5f);
					}
					else
					{
						if (Party[character_index]->GetAbility(i)->GetUnlocked() && i <= 3)
							GameData::GetInstance()->GetFont()->DrawString(name, 490.0f, 185.0f + (i * 35), { 0, 255, 0 }, 1.5f);
						else if (!Party[character_index]->GetAbility(i)->GetUnlocked())
							GameData::GetInstance()->GetFont()->DrawString(name, 490.0f, 185.0f + (i * 35), { 255,0,0 }, 1.5f);
						else
						GameData::GetInstance()->GetFont()->DrawString(name, 490.0f, 185.0f + (i * 35), { 0, 0, 0 }, 1.5f);

					}
				}
				graphics->DrawTextureSection(cursor, { 400.0f, 160.0f + (menuindex * 35) }, { 0, 0, 238, 73 });
				SGD::GraphicsManager::GetInstance()->DrawRectangle({ 480.0f, 180.0f, 715.0f, 320.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2);	
			}
			else if (selecting_party)
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 240 });
				GameData::GetInstance()->GetFont()->DrawString("Swap Position_With Who?", 490.0f, 185.0f, { 0, 0, 0 }, 1.5f);
				graphics->DrawTextureSection(cursor, { 10.0f, 165.0f + (menuindex * 55) }, { 0, 0, 238, 73 });
			}
			else if (select_first)
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 240 });
				GameData::GetInstance()->GetFont()->DrawString("Swap Character", 490.0f, 185.0f , { 0, 0, 0 }, 1.5f);
				GameData::GetInstance()->GetFont()->DrawString("Swap Ability", 490.0f, 220.0f , { 0, 0, 0 }, 1.5f);
				graphics->DrawTextureSection(cursor, { 400.0f, 160.0f + (menuindex * 35) }, { 0, 0, 238, 73 });

			}
			else
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 });
				for (int i = 0; i < Party[menuindex]->GetAbilitiesSize(); i++)
				{
					std::string name = Party[menuindex]->GetAbility(i)->GetAbilityName();
					GameData::GetInstance()->GetFont()->DrawString(name, 490.0f, 185.0f + (i * 35), { 0,0,0 }, 1.5f);
				}
				graphics->DrawTextureSection(cursor, { 10.0f, 165.0f + (menuindex * 55) }, { 0, 0, 238, 73 });
				SGD::GraphicsManager::GetInstance()->DrawRectangle({ 480.0f, 180.0f, 715.0f, 320.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2);
			}
			break;
		default:
			break;
		}
	}
	
void GamePlayState::CombatUpdate( float dt )
	{

	if (is_tutorial)
	{
		dialogue->Load("Assets/Scripts/tutorialdialogue.xml");
		state = Dia;
		is_tutorial = false;
		ignore_game_over = true;
	}
	//run_succeed = false;
	laststate = GPStates::Combat;
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	dynamic_cast<Bars*>(ternary_bar)->SetPercentage((float)ternary_gauge/MAXTG);


	if (run_succeed  || input->IsKeyPressed( SGD::Key::Backspace ) )
		{
		state = GPStates::Town;
		laststate = state;
		GameData::GetInstance()->SetIsInCombat( false );
		TurnManager::GetInstance()->Terminate();
		run_succeed = false;
		}

	/*if ( input->IsKeyPressed( SGD::Key::Eight ) )
		{
		Party[0]->SetBP( Party[0]->GetMaxBP() );
		}
	if ( input->IsKeyPressed( SGD::Key::One ) )
		{
		Party[0]->SetHP( Party[0]->GetMaxHP() );
		}
	if ( input->IsKeyPressed( SGD::Key::Z ) )
		{
		Party[0]->SetProgress( 100.0f );
		}
	if ( input->IsKeyPressed( SGD::Key::X ) )
		{
		Party[1]->SetProgress( 100.0f );
		}
	if ( input->IsKeyPressed( SGD::Key::C ) )
		{
		Party[2]->SetProgress( 100.0f );
		}
	if ( input->IsKeyPressed( SGD::Key::L ) )
		{
		Party[0]->AddStatus( &pStatManager->GetStatus( "Regen" ) );
		}*/

	if(input->IsKeyPressed(SGD::Key::Nine))
	{
		ternary_gauge = MAXTG;
	}

	if (state == GPStates::Town)
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Audio))
			SGD::AudioManager::GetInstance()->StopAudio(m_Audio);
		SGD::AudioManager::GetInstance()->PlayAudio(m_overAudio);
	}

	TurnManager::GetInstance()->Update( dt );
	

	}
void GamePlayState::CombatRender()
	{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTexture( combatback, { 0, 0 } );
	graphics->DrawTextureSection( combathud, { -0.1f, 425.0f }, { 0.0f, 0.0f, 800.0f, 175.0f } );
	//GameData::GetInstance()->GetFont()->DrawString( "You are in Combat", 50.0, 50.0, { 155, 255, 155 } );
	TurnManager::GetInstance()->Render();
	for ( size_t i = 0; i < m_vhuditems.size(); i++ )
		{
		if ( m_vhuditems[i]->GetActive() )
			m_vhuditems[i]->Render();
		}
	for ( size_t i = 0; i < m_vSelectableItems.size(); i++ )
		{
		if ( m_vSelectableItems[i]->GetActive() )
			m_vSelectableItems[i]->Render();
		}
	helptextbox->Render();

	// Let's put the ternary bar here
	ternary_bar->Render();
	
	}


Enemy * GamePlayState::CreateCommonEnemy( std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename )
	{
	CommonEnemy * temp = new CommonEnemy();
	
	OwnedHUD* turnindicator = new OwnedHUD;
	turnindicator->SetActive(true);
	turnindicator->SetImage(TurnIndicator);
	turnindicator->SetOwner(temp);
	turnindicator->SetOffset(SGD::Point(-10, -64));
	temp->SetTurnIndicator(turnindicator);

	temp->SetName( name );
	temp->SetStats( _stats );
	temp->SetLevel( _lvl );
	temp->SetMaxHP( _maxhp );
	temp->SetHP( temp->GetMaxHP() );
	temp->SetSpeed( _speed );
	temp->SetProgress( _progress );
	//temp->SetEtype(WIND);
	if ( abilityarr != nullptr )
		{
		for ( int i = 0; i < 4; i++ )
			{
			temp->SetAbility( i, abilityarr[i] );
			}
		}
	temp->SetPosition( _position );
	temp->SetSize( _size );

	AnimationSystem* tempanimsys = new AnimationSystem;
	if ( _animfilename.size() )
		{
		tempanimsys->Load( _animfilename );
		tempanimsys->Play( 0 );
		}
	else
		tempanimsys = nullptr;

	temp->SetAnimations( tempanimsys );

//	if ( temp->GetName() == "Dog" )
		{
		temp->SetTimelineAnimation( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/DogTimeline.png" ) );
		}

	return temp;
	}
CombatPlayer * GamePlayState::CreateCombatPlayer( std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, int _bp, int _maxbp, float _speed, float _progress, Ability* abilityarr[], SGD::Point _position, SGD::Size _size, std::string _animfilename )
	{
	CombatPlayer * temp = new CombatPlayer();

	OwnedHUD* turnindicator = new OwnedHUD;
	turnindicator->SetActive(true);
	turnindicator->SetImage(TurnIndicator);
	turnindicator->SetOwner(temp);
	turnindicator->SetOffset(SGD::Point(-10, -42));
	temp->SetTurnIndicator(turnindicator);

	temp->SetName( name );
	temp->SetActive( true );
	temp->SetStats( _stats );
	temp->SetLevel( _lvl );
	temp->SetMaxHP( _maxhp );
	temp->SetHP( temp->GetMaxHP() );
	temp->SetMaxBP( _maxbp );
	temp->SetBP( temp->GetMaxBP() );
	temp->SetSpeed( _speed );
	temp->SetProgress( _progress );
	if ( abilityarr != nullptr )
		{
		for ( int i = 0; i < 4; i++ )
			{
			temp->SetAbility( i, abilityarr[i] );
			}
		}
	temp->SetPosition( _position );
	temp->SetSize( _size );

	AnimationSystem* tempanimsys = new AnimationSystem;
	if ( _animfilename.size() )
		{
		tempanimsys->Load( _animfilename );
		tempanimsys->Play( 0 );
		}
	else
		tempanimsys = nullptr;

	temp->SetAnimations( tempanimsys );
	return temp;
	}

HUDItem * GamePlayState::CreateHudItem( SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string _string )
	{
	HUDItem* temp = new HUDItem();
	temp->SetImage( _image );
	temp->SetActive( true );
	temp->SetString( _string );
	temp->SetPosition( _position );
	temp->SetSize( _size );
	return temp;
	}

HUDItem* GamePlayState::CreateBar( SGD::Size _size, SGD::Point _pos, Character * _owner, SGD::Color _color, SGD::Point _offset, bool isHP )
	{
	Bars * temp = new Bars( _size, _pos, _owner );
	//temp->SetImage(_image);
	temp->SetisHP( isHP );
	temp->SetOffset( _offset );
	temp->SetActive( true );
	temp->SetColor( _color );
	return temp;
	}


void GamePlayState::DialogueUpdate( float dt )
	{
	if ( !dialogue->Update( dt ) )
		{
		state = laststate;
		if (FinalBossFight && laststate == BattleSummary)
		{
			
			FinalBossFight = false;
			laststate = Town;
			state = Town;
			GameData::GetInstance()->SwapState(GameOverWinState::GetInstance());
		}
		}
	}
void GamePlayState::DialogueRender()
	{
	if (laststate == Combat)
	{
		CombatRender();
	}
	else
	{
		TownRender();
	}
	dialogue->Render();
	}
SelectableObject * GamePlayState::CreateSelectableObject( SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string _string )
	{
	SelectableObject* temp = new SelectableObject();
	temp->SetImage( _image );
	temp->SetString( _string );
	temp->SetActive( true );
	temp->SetPosition( _position );
	temp->SetSize( _size );
	return temp;
	}
std::vector<SelectableObject*> GamePlayState::GetSelectableObjects()
	{
	return m_vSelectableItems;
	}
void GamePlayState::SetSelectableObject( std::vector<SelectableObject*>_vec )
	{
	m_vSelectableItems = _vec;
	}
void GamePlayState::SetSelectableObject( SelectableObject* _obj, int _index )
	{
	if ( _index < (int)m_vSelectableItems.size() )
		{
		m_vSelectableItems[_index] = _obj;
		}
	}
HelpText* GamePlayState::GetHelpText()
	{
	return helptextbox;
	}
void GamePlayState::SetHelpText( HelpText* help )
	{
	helptextbox = help;
	}
CombatPlayer * GamePlayState::LoadCombatPlayer( std::string _path )
	{
	CombatPlayer * toon = nullptr;
	TiXmlDocument doc;
	if ( doc.LoadFile( _path.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8 ) )
		{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string name = root->FirstChildElement( "Name" )->GetText();
		std::string element = root->FirstChildElement( "Element" )->GetText();
		std::string type = root->FirstChildElement( "Type" )->GetText();
		std::string random = root->FirstChildElement( "IsRandom" )->GetText();
		//load in the Stats
		Stats stats;
		int HP;
		int BP;
		int level;
		float speed;
		TiXmlElement * Stat = root->FirstChildElement( "Stats" );
		HP = std::stoi( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Base" )->GetText() );
		stats.hp_scale = std::stof( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Scale" )->GetText() );

		BP = std::stoi( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Base" )->GetText() );
		stats.bp_scale = std::stof( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Scale" )->GetText() );

		stats.attack = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		stats.attack_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		stats.magic = std::stoi( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Base" )->GetText() );
		stats.magic_scale = std::stof( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Scale" )->GetText() );

		stats.defense = std::stoi( Stat->FirstChildElement( "Defense" )->FirstChildElement( "Base" )->GetText() );
		stats.defense_scale = std::stof( Stat->FirstChildElement( "Defense" )->FirstChildElement( "Scale" )->GetText() );

		speed = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Base" )->GetText() );
		stats.speed_scale = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Scale" )->GetText() );

		stats.avoision = std::stoi( Stat->FirstChildElement( "Avoision" )->FirstChildElement( "Base" )->GetText() );
		stats.avoision_scale = std::stof( Stat->FirstChildElement( "Avoision" )->FirstChildElement( "Scale" )->GetText() );

		level = std::stoi( root->FirstChildElement( "Level" )->GetText() );

		std::string hurt = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();
		std::string attack = root->FirstChildElement( "Sound" )->FirstChildElement( "Attack" )->GetText();
		std::string death = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();

		std::string animation = root->FirstChildElement( "Animation" )->GetText();



		if ( type == "Ally" )
			{
			//create a player
			toon = CreateCombatPlayer( name, stats, level, HP, HP, BP, BP, speed, 0, nullptr, { 0.0f, 0.0f }, { 0.0f, 0.0f }, animation );
			if ( element == "Wind" )
				{
				toon->SetEtype( ETYPE::WIND );
				}
			else if ( element == "Fire" )
				{
				toon->SetEtype( ETYPE::FIRE );
				}
			else if ( element == "Earth" )
				{
				toon->SetEtype( ETYPE::EARTH );
				}
			else if ( element == "Water" )
				{
				toon->SetEtype( ETYPE::WATER );
				}
			else
				{
				toon->SetEtype( ETYPE::MULTI );
				}
			}
		else if ( type == "Enemy" )
			{

			return nullptr;

			}
		else if ( type == "Guard" )
			{
			return nullptr;
			}
		else
			{
			return nullptr;
			}

		if ( element == "Wind" )
			{
			toon->SetEtype( ETYPE::WIND );
			}
		else if ( element == "Fire" )
			{
			toon->SetEtype( ETYPE::FIRE );
			}
		else if ( element == "Earth" )
			{
			toon->SetEtype( ETYPE::EARTH );
			}
		else if ( element == "Water" )
			{
			toon->SetEtype( ETYPE::WATER );
			}
		else
			{
			toon->SetEtype( ETYPE::MULTI );
			}

		}
	//if ( toon->GetName() == "Ratsputin" )
		{
		toon->SetTimelineAnimation( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/RatTimeline.png" ) );
		}
	//if ( toon->GetName() == "Ratsputin" )
		{
		toon->SetPortrait( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/Bowie.png" ) );

		}
	return toon;



	}
Enemy* GamePlayState::LoadEnemy( std::string _path )
	{

	Enemy * toon = nullptr;
	TiXmlDocument doc;
	if ( doc.LoadFile( _path.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8 ) )
		{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string name = root->FirstChildElement( "Name" )->GetText();
		std::string element = root->FirstChildElement( "Element" )->GetText();
		std::string type = root->FirstChildElement( "Type" )->GetText();
		std::string random = root->FirstChildElement( "IsRandom" )->GetText();
		//load in the Stats
		Stats stats;
		int HP;
		int BP;
		int level;
		float speed;
		TiXmlElement * Stat = root->FirstChildElement( "Stats" );
		HP = std::stoi( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Base" )->GetText() );
		stats.hp_scale = std::stof( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Scale" )->GetText() );

		BP = std::stoi( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Base" )->GetText() );
		stats.bp_scale = std::stof( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Scale" )->GetText() );

		stats.attack = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		stats.attack_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		stats.magic = std::stoi( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Base" )->GetText() );
		stats.magic_scale = std::stof( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Scale" )->GetText() );

		stats.defense = std::stoi( Stat->FirstChildElement( "Defense" )->FirstChildElement( "Base" )->GetText() );
		stats.defense_scale = std::stof( Stat->FirstChildElement( "Defense" )->FirstChildElement( "Scale" )->GetText() );

		speed = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Base" )->GetText() );
		stats.speed_scale = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Scale" )->GetText() );

		stats.avoision = std::stoi( Stat->FirstChildElement( "Avoision" )->FirstChildElement( "Base" )->GetText() );
		stats.avoision_scale = std::stof( Stat->FirstChildElement( "Avoision" )->FirstChildElement( "Scale" )->GetText() );

		level = std::stoi( root->FirstChildElement( "Level" )->GetText() );

		std::string hurt = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();
		std::string attack = root->FirstChildElement( "Sound" )->FirstChildElement( "Attack" )->GetText();
		std::string death = root->FirstChildElement( "Sound" )->FirstChildElement( "Death" )->GetText();

		std::string animation = root->FirstChildElement( "Animation" )->GetText();


		if ( type == "Ally" )
			{

			return nullptr;
			}
		else if ( type == "Enemy" || type == "Boss" || type == "Mini Boss")
			{
			//create a enemy
			toon = CreateCommonEnemy( name, stats, level, HP, HP, speed, 0, nullptr, { 0.0f, 0.0f }, { 64.0f, 64.0f }, animation );

			}
		else if ( type == "Guard" )
			{
			return nullptr;
			}
		else
			{
			return nullptr;
			}
		if ( element == "Wind" )
			{
			toon->SetEtype( ETYPE::WIND );
			}
		else if ( element == "Fire" )
			{
			toon->SetEtype( ETYPE::FIRE );
			}
		else if ( element == "Earth" )
			{
			toon->SetEtype( ETYPE::EARTH );
			}
		else if ( element == "Water" )
			{
			toon->SetEtype( ETYPE::WATER );
			}
		else
			{
			toon->SetEtype( ETYPE::MULTI );
			}
		}
	return toon;
	}

void GamePlayState::RandomAnimation()
	{
	SGD::AudioManager::GetInstance()->PlayAudio( entercombat );
	for ( int i = 0; i < 20; i += 3 )
		{
		for ( int j = 0; j < 20; j += 3 )
			{

			TownRender();
			SGD::GraphicsManager::GetInstance()->DrawRectangle( { 0.0f + ( i * 40 ), 0.0f + ( j * 40 ), ( i * 40 ) + 40.0f, ( j * 40 ) + 40.0f }, { 0, 0, 0 } );
			SGD::GraphicsManager::GetInstance()->DrawRectangle( { 0.0f + ( j * 40 ), 0.0f + ( i * 40 ), ( j * 40 ) + 40.0f, ( i * 40 ) + 40.0f }, { 0, 0, 0 } );
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


// Grant Added This Stuff for Ternary

void GamePlayState::SetGauge( int val )
{
	ternary_gauge = val;
}

int GamePlayState::GetGauge()
{
	return ternary_gauge;
}

int GamePlayState::GetMaxGauge()
{
	return MAXTG;
}

bool GamePlayState::usingTernary()
{
	return ternary;
}

void GamePlayState::SetTernary(bool buul)
{
	ternary = buul;
}

void GamePlayState::AddToTB( Ability* abi, Character* target )
{
	// Don't do this so many times
	if ( myTernTargets.num_targets >= 3 ) return;

	myTernTargets.targets.push_back( target );
	myTernTargets.abilities.push_back( abi );
	//myTernTargets.num_targets++;
}

void GamePlayState::AddTarget( )
{
	myTernTargets.num_targets++;
}

void GamePlayState::ClearTernary( )
{
	myTernTargets.abilities.clear( );
	myTernTargets.targets.clear( );
	myTernTargets.num_targets = 0;
}

bool GamePlayState::AddToGauge(int val)
// Adds value to the gauge
// returns true if gauge has been filled
{
	ternary_gauge += val;
	if(ternary_gauge >= MAXTG)
	{
		ternary_gauge = MAXTG;
		return true;
	}
	return false;
}

//std::vector<Items> * GamePlayState::GetInventory()
//{
//	return &inventory;
//}

void GamePlayState::CutsceneUpdate(float dt)
{
	CutsceneManager::GetInstance( )->Update(dt );
}

void GamePlayState::CutsceneRender()
{
	CutsceneManager::GetInstance()->Render();
}
void GamePlayState::Loading(std::string _loading)
{
	//here we..
	//clear the screen
	//display _loading
	//
	//SGD::GraphicsManager::GetInstance()->Update();
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 520.0f, 200.0f }, { 0, 0, 300, 540 }, SGD::PI / 2, { 112.5f, 27.5f });
	GameData::GetInstance()->GetFont()->DrawString(_loading, 250.0f, 250.0f, { 0, 0, 0 },2.0f);
	SGD::GraphicsManager::GetInstance()->Update();
	Sleep(250);
	
}

void GamePlayState::CheckAbilityUnlocked(bool EOC)
	{
	for ( unsigned int i = 0; i < Party.size(); i++ )
		{
		for ( unsigned int k = 0; k < 8; k++ )
			{
			if ( !Party[i]->GetAbility( k )->GetUnlocked() && Party[i]->GetLevel() >= Party[i]->GetAbility( k )->GetUnlockLevel() )
				{
				Party[i]->GetAbility( k )->SetUnlocked( true );
				if ( EOC )
					{

						//Enter code to notify loot/exp screen that a new ability was unlocked(i.e. level gained...show stat gains and new ability unlocked)
						// Print to Window Screen the details
					}
				}
			}
		}
	}

void GamePlayState::MapUpdate(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Six))
	{
		if (unlockedTowns >= 1)
		{
			unlockedTowns--;
		}
	}
	else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Seven))
	{
		if (unlockedTowns <= 3)
		{
			unlockedTowns++;
		}
	}

	if (WorldMapAnsys != nullptr)
	{
		GameData::GetInstance()->SetCamera({ 0, 0 });
		WorldMapAnsys->Update(dt);
	}
	if (unlockedTowns == 4)
	{
		SelectedTown = 4;
	}
	else
	{
		if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right))
		{
			SelectedTown = 0;
		}
		else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up))
		{
			if (unlockedTowns >= 1)
				SelectedTown = 1;
		}
		else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left))
		{
			if (unlockedTowns >= 2)
				SelectedTown = 2;
		}
		else if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down))
		{
			if (unlockedTowns >= 3)
				SelectedTown = 3;
		}
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		switch (SelectedTown)
		{
		case 0: //Wind
			Loading("Loading Map");
			TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\WindTown.xml");
			break;
		case 1://Fire
			Loading("Loading Map");
			TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\firetown.xml");
			break;
		case 2://Earth
			Loading("Loading Map");
			TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\EarthTown.xml");
			break;
		case 3://Water
			Loading("Loading Map");
			TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\WaterTown.xml");
			break;
		case 4://Final Town
			Loading("Loading Map");
			TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\herotown.xml");
			break;
		default:
			//TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\TestTown.xml");
			break;
		}
		laststate = Town;
		state = Town;
	}
}

void GamePlayState::MapRender()
{
	if (WorldMap != SGD::INVALID_HANDLE)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(WorldMap, { 0, 0 });
	}
	int i = unlockedTowns + 1;
	for (; i < 5; i++)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(PadLock, townpoints[i]);
	}
	if (WorldMapAnsys != nullptr)
	{
		WorldMapAnsys->Render(townpoints[SelectedTown].x + 32, townpoints[SelectedTown].y + 32);
	}
}	

void GamePlayState::AddItem( ItemType it )
{
	inventory.push_back(shopinv[it]);
	loot.push_back(it);

}

void GamePlayState::AddGold(int val)
{
	gold += val;
	loot_gold = val;
}

int GamePlayState::GetGold(void) const
{
	return gold;
}

void GamePlayState::SummaryUpdate(float dt)
{
	if (ignore_game_over)
	{
		ignore_game_over = false;
		state = Map;
		laststate = Map;
		for (unsigned int i = 0; i < m_vhuditems.size(); i++)
		{
			delete m_vhuditems[i];
			m_vhuditems[i] = nullptr;
		}
		m_vhuditems.clear();
	}
	GameData::GetInstance()->SetCamera({ 0, 0 });
	for (size_t i = 0; i < Party.size(); i++)
	{
		Party[i]->UpdateAnimation(dt);
	}

	//if (loot.size() == 0)
	//{
	//	loot.push_back(ItemType::SmallHP);
	//	loot.push_back(ItemType::SmallBP);
	//	loot_xp = 100;
	//	loot_gold = 5;
	//}

	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		
		state = Town;
		loot.clear();
		loot_xp = 0;
		loot_gold = 0;
		
		if (MinibossFight)
		{
			MinibossFight = false;
			if (unlockedTowns == SelectedTown)
				unlockedTowns++;


			for (unsigned int i = 0; i < guards.size(); i++)
			{
				delete guards[i];
				guards[i] = nullptr;
			}
			guards.clear();


			TileSystem::GetInstance()->Exit();

			state = Map;
		}
		if (FinalBossFight)
		{
			//Trigger Ending Cutscene or Dialouge
			//for now we will have to just do dialogue
			dialogue->Load("../Trapped Rat/Assets/Scripts/cecildefeateddialogue.xml");
			state = Dia;
			laststate = BattleSummary;
			//FinalBossFight = false;
		}

		// Guard Code
		if(guard_index != -1)
			// We killed a guard, guard_index is the index of the array
		{
			auto iter = guards.begin();
			while((*iter) != guards[guard_index])
				iter++;
			
			delete (*iter);
			guards.erase(iter);

			guard_index = -1;
		}

		if ( guards.size( ) == 0 )
		{
			// Open the keep code goes here
		}
		for (size_t i = 0; i < m_vhuditems.size(); i++)
		{
			delete m_vhuditems[i];
		}
		m_vhuditems.clear();
	}

}
void GamePlayState::SummaryRender()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(background, { 0, 0 });
	std::vector<CombatPlayer*> tempparty;
	for (unsigned int i = 0; i < Party.size(); i++)
	{
		tempparty.push_back(Party[i]);
		tempparty[i]->SetPosition({ 150.0f + (100.0f * i), 200.0f });
		tempparty[i]->Render();

		SGD::Point characterOrderPosition;
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(Party[i]->GetOrderPosition() * 100 + 150);
		tempparty[i]->SetPosition(characterOrderPosition);
	}
	SGD::GraphicsManager::GetInstance()->DrawTexture(scroll, { 800, 310 }, SGD::PI / 2, {}, {}, {1.0f,1.4f});
	std::string str = "Experience Gained:  " + std::to_string(loot_xp) +"_Gold Gained:  "+std::to_string(loot_gold)+"_Items Gained:  _  ";
	for (size_t i = 0; i < loot.size(); i++)
	{
		str += shopinv[loot[i]].GetName() + " _  ";
	}
	GameData::GetInstance()->GetFont()->DrawString(str, 250, 350, { 20, 0, 20 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("Press Enter to Continue", 250, 300, { 50, 50, 50 }, 2.0f);
}

void GamePlayState::AddToParty(CombatPlayer*_player)
{
	Party.push_back(_player);
	state = Dia;
	laststate = Town;
	dialogue->Load("Assets/Scripts/gainedcompanion.xml");
}

std::map<std::string, Ability*> GamePlayState::GetMasterList()
{
	return MasterAbilityList;
}

void GamePlayState::CreateFinalFight()
{

	FinalBossFight = true;
SGD::Point characterOrderPosition;
std::vector<Enemy*> bosses;
std::vector<Ability*> partyAbilities;

Enemy* cecilFinal = nullptr;
cecilFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Cecil.xml" );
cecilFinal->SetOrderPosition( 1 );
characterOrderPosition.x = 600.0f;
characterOrderPosition.y = (float)( cecilFinal->GetOrderPosition() * 100 + 150 + 16 );
cecilFinal->SetPosition( characterOrderPosition );
partyAbilities.push_back( MasterAbilityList["Holy Shield"] );
partyAbilities.push_back( MasterAbilityList["Holy Mace"] );
partyAbilities.push_back( MasterAbilityList["Holy Flare"] );
partyAbilities.push_back( MasterAbilityList["Retribution"] );
partyAbilities.push_back( MasterAbilityList["Cover"] );
cecilFinal->InitializeAbilities( partyAbilities );
partyAbilities.clear();

Enemy* janeFinal = nullptr;
janeFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Jane.xml" );
janeFinal->SetOrderPosition( 0 );
janeFinal->SetLiving( false );
janeFinal->SetHP( 0 );
characterOrderPosition.x = 600.0f;
characterOrderPosition.y = (float)( janeFinal->GetOrderPosition() * 100 + 150 + 16 );
janeFinal->SetPosition( characterOrderPosition );
partyAbilities.push_back( MasterAbilityList["Healing Light"] );
partyAbilities.push_back( MasterAbilityList["Dia"] );
partyAbilities.push_back( MasterAbilityList["Protect"] );
janeFinal->InitializeAbilities( partyAbilities );
partyAbilities.clear();

Enemy* johnFinal = nullptr;
johnFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final John.xml" );
johnFinal->SetOrderPosition( 2 );
johnFinal->SetLiving( false );
johnFinal->SetHP( 0 );
characterOrderPosition.x = 600.0f;
characterOrderPosition.y = (float)( johnFinal->GetOrderPosition() * 100 + 150 + 16 );
johnFinal->SetPosition( characterOrderPosition );
partyAbilities.push_back( MasterAbilityList["Sure Shot"] );
partyAbilities.push_back( MasterAbilityList["Barrage"] );
partyAbilities.push_back( MasterAbilityList["Haste"] );
johnFinal->InitializeAbilities( partyAbilities );
partyAbilities.clear();

bosses.push_back( janeFinal );
bosses.push_back( cecilFinal );
bosses.push_back( johnFinal );


for ( unsigned int i = 0; i < Party.size(); i++ )
	{
	Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
	Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
	}
TurnManager::GetInstance()->Initialize( Party, bosses );
for ( size_t i = 0; i < Party.size(); i++ )
	{
	if ( Party[i]->GetActive() )
		{
		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
		}
	}

for ( size_t i = 0; i < bosses.size(); i++ )
	{
	m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
	}
GameData::GetInstance()->SetIsInCombat( true );
	state = Combat;
	laststate = Combat;


	state = Dia;
	laststate = Combat;
	dialogue->Load("Assets/Scripts/bossdialogue.xml");
	FinalBossFight = true;
}

void GamePlayState::SetLootXP( int val )
{
	loot_xp = val;
}



void GamePlayState::CreateMinibossFight()
{
	//Stub
	MinibossFight = true;
SGD::Point characterOrderPosition;
std::vector<Enemy*> bosses;
std::vector<Ability*> partyAbilities;

	switch (SelectedTown)  //NOTE FOR JOE****Make sure to increment "unlockedTowns" after you win the fight against these Minibosses
	{
	case 0: //Wind <---
			{
			Enemy* BWR1 = nullptr;
			BWR1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Benevolent Wind Rogue.xml" );
			BWR1->SetOrderPosition( 1 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( BWR1->GetOrderPosition() * 100 + 150 + 16 );
			BWR1->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Wind Split"] );
			BWR1->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			Enemy* BWR2 = nullptr;
			BWR2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Benevolent Wind Rogue.xml" );
			BWR2->SetOrderPosition( 0 );
			BWR2->SetLiving( false );
			BWR2->SetHP( 0 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( BWR2->GetOrderPosition() * 100 + 150 + 16 );
			BWR2->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Wind Split"] );
			BWR2->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			bosses.push_back( BWR1 );
			bosses.push_back( BWR2 );

			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			TurnManager::GetInstance()->Initialize( Party, bosses );
			for ( size_t i = 0; i < Party.size(); i++ )
				{
				if ( Party[i]->GetActive() )
					{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}

			for ( size_t i = 0; i < bosses.size(); i++ )
				{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
			GameData::GetInstance()->SetIsInCombat( true );
			state = Combat;
			laststate = Combat;

			state = Dia;
			laststate = Combat;
			dialogue->Load( "Assets/Scripts/bossdialogue.xml" );
		break;
			}
	case 1: //Fire <----
			{
			Enemy* FFW1 = nullptr;
			FFW1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Ferocious Fire Warrior.xml" );
			FFW1->SetOrderPosition( 1 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( FFW1->GetOrderPosition() * 100 + 150 + 16 );
			FFW1->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Flame Spout"] );
			FFW1->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			Enemy* FFW2 = nullptr;
			FFW2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Ferocious Fire Warrior.xml" );
			FFW2->SetOrderPosition( 0 );
			FFW2->SetLiving( false );
			FFW2->SetHP( 0 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( FFW2->GetOrderPosition() * 100 + 150 + 16 );
			FFW2->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Flame Spout"] );
			FFW2->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			bosses.push_back( FFW1 );
			bosses.push_back( FFW2 );

			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			TurnManager::GetInstance()->Initialize( Party, bosses );
			for ( size_t i = 0; i < Party.size(); i++ )
				{
				if ( Party[i]->GetActive() )
					{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}

			for ( size_t i = 0; i < bosses.size(); i++ )
				{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
			GameData::GetInstance()->SetIsInCombat( true );
			state = Combat;
			laststate = Combat;

			state = Dia;
			laststate = Combat;
			dialogue->Load( "Assets/Scripts/bossdialogue.xml" );
		break;
			}
	case 2: //Earth <---
			{
			Enemy* SEM1 = nullptr;
			SEM1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Sturdy Earth Monk.xml" );
			SEM1->SetOrderPosition( 1 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( SEM1->GetOrderPosition() * 100 + 150 + 16 );
			SEM1->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Earthen Shield"] );
			partyAbilities.push_back( MasterAbilityList["Earthen Ward"] );
			SEM1->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			Enemy* SEM2 = nullptr;
			SEM2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Sturdy Earth Monk.xml" );
			SEM2->SetOrderPosition( 0 );
			SEM2->SetLiving( false );
			SEM2->SetHP( 0 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( SEM2->GetOrderPosition() * 100 + 150 + 16 );
			SEM2->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Earthen Shield"] );
			partyAbilities.push_back( MasterAbilityList["Earthen Ward"] );
			SEM2->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			bosses.push_back( SEM1 );
			bosses.push_back( SEM2 );

			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			TurnManager::GetInstance()->Initialize( Party, bosses );
			for ( size_t i = 0; i < Party.size(); i++ )
				{
				if ( Party[i]->GetActive() )
					{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}

			for ( size_t i = 0; i < bosses.size(); i++ )
				{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
			GameData::GetInstance()->SetIsInCombat( true );
			state = Combat;
			laststate = Combat;

			state = Dia;
			laststate = Combat;
			dialogue->Load( "Assets/Scripts/bossdialogue.xml" );
		break;
			}
	case 3: //Water <----
			{
			Enemy* WWW1 = nullptr;
			WWW1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Wise Water Wizard.xml" );
			WWW1->SetOrderPosition( 1 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( WWW1->GetOrderPosition() * 100 + 150 + 16 );
			WWW1->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Water Wall"] );
			WWW1->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			Enemy* WWW2 = nullptr;
			WWW2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Wise Water Wizard.xml" );
			WWW2->SetOrderPosition( 0 );
			WWW2->SetLiving( false );
			WWW2->SetHP( 0 );
			characterOrderPosition.x = 600.0f;
			characterOrderPosition.y = (float)( WWW2->GetOrderPosition() * 100 + 150 + 16 );
			WWW2->SetPosition( characterOrderPosition );
			partyAbilities.push_back( MasterAbilityList["Water Wall"] );
			WWW2->InitializeAbilities( partyAbilities );
			partyAbilities.clear();

			bosses.push_back( WWW1 );
			bosses.push_back( WWW2 );

			for ( unsigned int i = 0; i < Party.size(); i++ )
				{
				Party[i]->GetAbility( 0 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 1 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 2 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 3 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 4 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 5 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 6 )->CalcluateBpScaledCost( Party[i] );
				Party[i]->GetAbility( 7 )->CalcluateBpScaledCost( Party[i] );
				}
			TurnManager::GetInstance()->Initialize( Party, bosses );
			for ( size_t i = 0; i < Party.size(); i++ )
				{
				if ( Party[i]->GetActive() )
					{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[i]->GetOrderPosition()*50.0f ) ), Party[i], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}

			for ( size_t i = 0; i < bosses.size(); i++ )
				{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[i], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
			GameData::GetInstance()->SetIsInCombat( true );
			state = Combat;
			laststate = Combat;

			state = Dia;
			laststate = Combat;
			dialogue->Load( "Assets/Scripts/bossdialogue.xml" );
		break;
			}
	case 4://Cecils HomeTown **Should Not Be Used because no miniboss in that town
		break;
	}

}