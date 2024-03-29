
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
#include <shlwapi.h>
#include "../Animation/AnimationSystem.h"
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <ctime>

GamePlayState* GamePlayState::GetInstance()
{
	static GamePlayState data;

	return &data;
}
void GamePlayState::Enter()
{
	GameData::GetInstance()->ResetPlayer();
	dialogue = new Dialogue();
	run_succeed = false;
	MinibossFight = false;
	FinalBossFight = false;
	ignore_game_over = false;

	townpoints[ 0 ] = SGD::Point( 594, 388 );
	townpoints[ 1 ] = SGD::Point( 348, 118 );
	townpoints[ 2 ] = SGD::Point( 124, 365 );
	townpoints[ 3 ] = SGD::Point( 365, 474 );
	townpoints[ 4 ] = SGD::Point( 362, 298 );

	WorldMapAnsys = new AnimationSystem();
	WorldMapAnsys->Load( "RatAnimOverworld.xml" );
	WorldMapAnsys->Play( "Up" );
	WorldMap = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/world.png" );
	PadLock = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/padlock.png" );

	abilityunlocked = true;


	//testFinalFight = true;
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/Scroll.png" );
	background = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/MenuBackground.png" );
	Loading( "Loading Tiles..." );
	SGD::MessageManager::GetInstance()->Initialize( &MessageProc );
	//TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\TestTown.xml");
	Loading( "Loading Images..." );
	rattrap = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/RatTrap32.png" );
	combathud = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/tempcombathud.png" );
	enemytargeting = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/enemytargeticon.png" );
	allytargeting = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/allytargeticon.png" );
	buttonimg = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/button.png" );
	combatback = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/CombatTownBack.png" );
	button = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/button.png" );
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/cheese.png" );
	trapcursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/RatTrap.png" );
	cheesecursor = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/cheese.png" );
	TurnIndicator = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/playerSelect.png" );
	Stats sts;
	sts.attack = 10;
	sts.defense = 10;
	sts.avoision = 10;
	sts.magic = 10;



	TurnManager::GetInstance()->SetArrow( allytargeting, enemytargeting );

	Loading( "Loading Text..." );

	SelectableObject* ob = CreateSelectableObject( buttonimg, { 70, 445 }, { 64, 64 }, "Attack" );
	ob->SetSelected( true );
	ob->SetExplination( "Deal damage to one enemy" );
	m_vSelectableItems.push_back( ob );

	ob = CreateSelectableObject( buttonimg, { 10, 495 }, { 64, 64 }, "Item" );
	ob->SetExplination( "Use an Item to aide your party" );
	m_vSelectableItems.push_back( ob );

	ob = CreateSelectableObject( buttonimg, { 150, 495 }, { 64, 64 }, "Ability" );
	ob->SetExplination( "Use a unique ABILITY" );
	m_vSelectableItems.push_back( ob );

	ob = CreateSelectableObject( buttonimg, { 70, 545 }, { 64, 64 }, "Run" );
	ob->SetExplination( "Attempt to run from this battle" );
	m_vSelectableItems.push_back( ob );

	helptextbox = new HelpText();
	helpback = SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/HelpTextBox.png" );
	helptextbox->SetImage( helpback );

	ternary_bar = CreateBar( { 64, 16 }, { 250, 390 }, nullptr, { 255, 255, 0 }, { 0, 0 }, false );

	std::ifstream fin;

	Loading( "Loading Abilities..." );
	fin.open( "Assets/Scripts/Abilities/AbilityList.txt" );
	if ( fin.is_open() )
	{
		int count;
		char buffer[ 64 ];
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
			MasterAbilityList[ tempAbility->GetAbilityName() ] = tempAbility;
		}

	}

	Loading( "Loading Characters..." );
	SGD::Point characterOrderPosition;
	CombatPlayer* p1 = nullptr;

	p1 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Ratsputin.xml" ) );
	p1->SetOrderPosition( 0 );
	characterOrderPosition.x = 100.0f;
	characterOrderPosition.y = (float)( p1->GetOrderPosition() * 100 + 150 );
	p1->SetPosition( characterOrderPosition );
	p1->SetSize( { 64, 64 } );
	std::vector<Ability*> partyAbilities;
	partyAbilities.push_back( MasterAbilityList[ "Burrow" ] );
	partyAbilities.push_back( MasterAbilityList[ "Water Fang" ] );
	partyAbilities.push_back( MasterAbilityList[ "Slow Claw" ] );
	partyAbilities.push_back( MasterAbilityList[ "Earth Fang" ] );
	partyAbilities.push_back( MasterAbilityList[ "Poison Fang" ] );
	partyAbilities.push_back( MasterAbilityList[ "Fire Fang" ] );
	partyAbilities.push_back( MasterAbilityList[ "Counter Claw" ] );
	partyAbilities.push_back( MasterAbilityList[ "Wind Fang" ] );
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
	smallheal.SetName( "Small Heal" );
	smallheal.SetPrice( 10 );
	smallheal.SetExplination( "Heals the Target for_a Small Amount" );
	Items largeheal;
	largeheal.SetName( "Large Heal" );
	largeheal.SetPrice( 20 );
	largeheal.SetExplination( "Heals the Target for_a Large Amount" );
	Items smallbp;
	smallbp.SetName( "Small BP Restore" );
	smallbp.SetPrice( 10 );
	smallbp.SetExplination( "Restores a Small Amount of BP" );
	Items largebp;
	largebp.SetName( "Large BP Restore" );
	largebp.SetPrice( 20 );
	largebp.SetExplination( "Restores a Large Amount of BP" );
	Items revive;
	revive.SetName( "Revive" );
	revive.SetPrice( 40 );
	revive.SetExplination( "Brings the target Back_from the Dead" );


	shopinv.push_back( smallheal );
	shopinv.push_back( largeheal );
	shopinv.push_back( smallbp );
	shopinv.push_back( largebp );
	shopinv.push_back( revive );
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


	//Sets the abilities to unlocked if level is high enough (new game will only unlock the first ability)
	CheckAbilityUnlocked();


	Loading( "Loading Sounds..." );
	m_Audio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/req.xwm" );
	m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/ZeldaMetal.xwm" );
	entercombat = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/entercombat.wav" );
	m_Map = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Map.xwm" );
	chime = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Chime.wav" );
	if ( m_Credits != SGD::INVALID_HANDLE )
	{
		m_Credits = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Credits.xwm" );
	}
	m_SummaryAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Victory.xwm" );
	SGD::AudioManager::GetInstance()->PlayAudio( m_Map, true );
	SGD::AudioManager::GetInstance()->SetMasterVolume( SGD::AudioGroup::Music, GameData::GetInstance()->GetMusicVolume() );
	SGD::AudioManager::GetInstance()->SetMasterVolume( SGD::AudioGroup::SoundEffects, GameData::GetInstance()->GetEffectVolume() );


	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/dooropen.wav" ) );//0
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Pain.wav" ) );//1
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/trap.wav" ) );//2
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/walking.wav" ) );//3
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/cancel.wav" ) );//4
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/run.wav" ) );//5
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/confirm.wav" ) );//6
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/itemuse.wav" ) );//7
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Rampart.wav" ) );//8
	m_vsoundeffects.push_back( SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/no.wav" ) );//9


	GameData::GetInstance()->UpdateCamera( GameData::GetInstance()->GetOverworldPlayer() );
	pStatManager = StatusEffectManager::GetInstance();
	pStatManager->Initialize();


	//state = GPStates::Cuts;
	//CutsceneManager::GetInstance()->Play( 0 );
	Loading( "Time to Play......." );

	MinibossFight = false;
	FinalBossFight = false;
	SGD::InputManager::GetInstance()->Update();
	//LoadGame(1);
	state = Map;
	laststate = Map;


	if ( is_tutorial )
	{
		TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalLanding.xml" );
		CutsceneManager::GetInstance()->Terminate();
		CutsceneManager::GetInstance()->Initialize( 1 );
		CutsceneManager::GetInstance()->Play( 0 );
		state = Cuts;
		laststate = Cuts;
	}
	//TutorialStart();
	//state = BattleSummary;

	CutscenesPlayed = 0;
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
	//update playtim
	playtime += dt / 60.0f;
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
			MapUpdate( dt );
			break;
		case GPStates::Dia:
			DialogueUpdate( dt );
			break;
		case GPStates::Cuts:
			CutsceneUpdate( dt );
			break;
		case GPStates::BattleSummary:
			SummaryUpdate( dt );
			break;
		default:
			break;
	}

}
void GamePlayState::Exit()
{
	Loading( "Dumping..." );
	GameData::GetInstance()->SetIsInCombat( false );
	for ( size_t i = 0; i < m_vsoundeffects.size(); i++ )
	{
		SGD::AudioManager::GetInstance()->UnloadAudio( m_vsoundeffects[ i ] );
	}
	m_vsoundeffects.clear();
	ternary_gauge = 0;

	for ( size_t i = 0; i < m_vhuditems.size(); i++ )
	{
		delete m_vhuditems[ i ];
	}
	m_vhuditems.clear();
	for ( size_t i = 0; i < m_vSelectableItems.size(); i++ )
	{
		delete m_vSelectableItems[ i ];
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

	GameData::GetInstance()->SetIsInCombat( false );

	TurnManager::GetInstance()->Terminate();
	delete ternary_bar;
	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		delete Party[ i ];
		Party[ i ] = nullptr;
	}
	for ( unsigned int i = 0; i < Parents.size(); i++ )
	{
		delete Parents[ i ];
		Parents[ i ] = nullptr;

	}
	for ( unsigned int i = 0; i < guards.size(); i++ )
	{
		delete guards[ i ];
		guards[ i ] = nullptr;
	}
	tutorialenemy.clear();
	guards.clear();
	shopinv.clear();
	inventory.clear();
	Party.clear();
	Parents.clear();
	delete WorldMapAnsys;
	SGD::GraphicsManager::GetInstance()->UnloadTexture( combatback );

	SGD::AudioManager::GetInstance()->UnloadAudio( m_Audio );
	SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
	SGD::AudioManager::GetInstance()->UnloadAudio( entercombat );
	if ( m_Credits != SGD::INVALID_HANDLE )
	{
		SGD::AudioManager::GetInstance()->UnloadAudio( m_Credits );
		m_Credits = SGD::INVALID_HANDLE;
	}
	SGD::AudioManager::GetInstance()->UnloadAudio( chime );
	SGD::AudioManager::GetInstance()->UnloadAudio( m_Map );
	SGD::AudioManager::GetInstance()->UnloadAudio( m_SummaryAudio );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( WorldMap );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( PadLock );

	SGD::GraphicsManager::GetInstance()->UnloadTexture( background );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( button );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( cursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( scroll );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( trapcursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( cheesecursor );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( TurnIndicator );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( helpback );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( combathud );
	SGD::GraphicsManager::GetInstance()->UnloadTexture( rattrap );
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
	// These comments feel pretty old


	auto iterAbil = MasterAbilityList.begin();
	for ( ; iterAbil != MasterAbilityList.end(); ++iterAbil )
	{
		delete iterAbil->second;
	}
	MasterAbilityList.clear();

	for ( unsigned int i = 0; i < overworldObjects.size(); ++i )
	{
		if ( overworldObjects[ i ] != nullptr )
		{
			delete overworldObjects[ i ];
		}
	}
	overworldObjects.clear();

	pStatManager->Terminate();
	pStatManager = nullptr;

	for ( unsigned int i = 0; i < traps.size(); i++ )
	{
		delete traps[ i ];
	}
	traps.clear();
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
		int sorandom = rand() % 250;

		if ( stepcounter >= 500 && sorandom < 20 )
		{
			stepcounter = 0;
			//play animation for entering random combat
			RandomAnimation();
			//if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
			//	SGD::AudioManager::GetInstance()->StopAudio( m_overAudio );

			if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
			{
				StopAllBackgroundMusic();
				SGD::AudioManager::GetInstance()->PlayAudio( m_Audio, true );
			}

			GameData::GetInstance()->SetIsInCombat( true );
			state = GPStates::Combat;
			std::vector<Enemy*> tempEnemy;
			//Party[0]->SetBP( 6 );


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
						tempRandomEnemy->dogTarget = 0;
						partyAbilities.push_back( MasterAbilityList[ "Feral Bite" ] );
						break;
					case 1:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cat.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Scratch" ] );
						break;
					case 2:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Raven.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Feathered Flurry" ] );
						break;
					case 3:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Chef.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Fresh Meat" ] );
						break;
					case 4:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Blacksmith.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Bellows Blast" ] );
						break;
					case 5:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Shopkeeper.xml" );
						tempRandomEnemy->lastAttacker = -1;
						partyAbilities.push_back( MasterAbilityList[ "Elemental Poison" ] );
						break;
					case 6:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Tailor.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Stitch in Time" ] );
						break;
					case 7:
						tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Priestess.xml" );
						partyAbilities.push_back( MasterAbilityList[ "Healing Light" ] );
						break;
					default:
						break;
				}

				// Setup Enemy levels
				int templevel = Party[ 0 ]->GetLevel();
				templevel -= 2;
				if ( templevel <= 0 ) templevel = 1;
				templevel += ( rand() % 5 );
				tempRandomEnemy->SetLevel( templevel );
				tempRandomEnemy->SetXPVal( 50 + ( ( templevel - Party[ 0 ]->GetLevel() ) * 5 ) );
				tempRandomEnemy->SetHP( tempRandomEnemy->GetMaxHP() );

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
				Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
				Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
			}

			TurnManager::GetInstance()->Initialize( Party, tempEnemy );
			for ( size_t i = 0; i < Party.size(); i++ )
			{
				if ( Party[ i ]->GetActive() )
				{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
				}
			}


			for ( size_t i = 0; i < tempEnemy.size(); i++ )
			{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), tempEnemy[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
			}
		}
	}


}


void GamePlayState::TownUpdate( float dt )
{
	std::vector<Enemy*> moreguards;

	GameData::GetInstance()->GetOverworldPlayer()->Update( dt );

	//if(guard_index >= 0 && guard_index < (int)guards.size())
	//	// A guard was triggered for a fight and defeated
	//{
	//	// We killed a guard, guard_index is the index of the array
	//	{
	//		auto iter = guards.begin();
	//		while ( ( *iter ) != guards[ guard_index ] )
	//			iter++;

	//		delete ( *iter );
	//		guards.erase( iter );

	//		guard_index = -1;
	//	}

	//}

	no_combat_timer -= dt;
	if ( no_combat_timer < -1.0f )
		no_combat_timer = -1.0f;

	for ( unsigned int i = 0; i < overworldObjects.size(); ++i )
	{
		if ( overworldObjects[ i ] != nullptr )
			overworldObjects[ i ]->Update( dt );
	}
	GameData::GetInstance()->UpdateCamera( GameData::GetInstance()->GetOverworldPlayer() );
	if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
	{
		StopAllBackgroundMusic();
		SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio, true );
	}
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
	{
		CheckAbilityUnlocked();
		state = GPStates::Menu;
	}
	if ( input->IsKeyPressed( SGD::Key::F ) && input->IsKeyPressed( SGD::Key::D ) )
	{
		for ( unsigned int i = 0; i < guards.size(); ++i )
		{
			if ( guards[ i ] != nullptr )
			{
				delete guards[ i ];
			}
		}
		guards.clear();
	}
	if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::One ) )
		CreateEnemyFight( 0 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Two ) )
		CreateEnemyFight( 1 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Three ) )
		CreateEnemyFight( 2 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Four ) )
		CreateEnemyFight( 3 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Five ) )
		CreateEnemyFight( 4 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Six ) )
		CreateEnemyFight( 5 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Seven ) )
		CreateEnemyFight( 6 );
	else if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::Eight ) )
		CreateEnemyFight( 7 );
	//if ( input->IsKeyPressed( SGD::Key::D ) && input->IsKeyDown( SGD::Key::G ) )
	//{
	//	GamePlayState::GetInstance()->CreateMinibossFight();
	//}
	for ( unsigned int t = 0; t < traps.size(); t++ )
	{
		if ( TileSystem::GetInstance()->GetTileIndex( traps[ t ]->GetPosition().x, traps[ t ]->GetPosition().y )
			 == TileSystem::GetInstance()->GetTileIndex(
			 GameData::GetInstance()->GetOverworldPlayer()->GetPosition().x,
			 GameData::GetInstance()->GetOverworldPlayer()->GetPosition().y ) )

			 /*SGD::Point posi = GameData::GetInstance( )->GetOverworldPlayer()->GetPosition();
			 if ( TileSystem::GetInstance( )->TileEvent( posi.x, posi.y, GameData::GetInstance( )->GetOverworldPlayer( )->GetRect(), "Trap") )*/
		{
			tripped_trap = t;


			// Just Trigger Combat on a %
			int x = rand() % 1000;
			if ( x < 325 )
			{

				trap_combat = true;

				GameData::GetInstance()->SetIsInCombat( true );

				/*if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_overAudio))
					SGD::AudioManager::GetInstance()->StopAudio(m_overAudio);*/

				if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
				{
					StopAllBackgroundMusic();
					SGD::AudioManager::GetInstance()->PlayAudio( m_Audio, true );
				}


				state = GPStates::Combat;
				std::vector<Enemy*> tempEnemy;
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
							partyAbilities.push_back( MasterAbilityList[ "Feral Bite" ] );
							break;
						case 1:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cat.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Scratch" ] );
							break;
						case 2:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Raven.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Feathered Flurry" ] );
							break;
						case 3:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Chef.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Fresh Meat" ] );
							break;
						case 4:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Blacksmith.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Bellows Blast" ] );
							break;
						case 5:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Shopkeeper.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Elemental Poison" ] );
							break;
						case 6:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Tailor.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Stitch in Time" ] );
							break;
						case 7:
							tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Priestess.xml" );
							partyAbilities.push_back( MasterAbilityList[ "Healing Light" ] );
							break;
						default:
							break;
					}

					// Setup Enemy levels
					int templevel = Party[ 0 ]->GetLevel();
					templevel -= 2;
					if ( templevel <= 0 ) templevel = 1;
					templevel += ( rand() % 5 );
					tempRandomEnemy->SetLevel( templevel );
					tempRandomEnemy->SetXPVal( 50 + ( ( templevel - Party[ 0 ]->GetLevel() ) * 5 ) );
					tempRandomEnemy->SetHP( tempRandomEnemy->GetMaxHP() );
					tempRandomEnemy->SetOrderPosition( i );
					characterOrderPosition.x = 600.0f;
					characterOrderPosition.y = (float)( tempRandomEnemy->GetOrderPosition() * 100 + 150 + 16 );
					tempRandomEnemy->SetPosition( characterOrderPosition );
					tempRandomEnemy->InitializeAbilities( partyAbilities );
					partyAbilities.clear();
					tempEnemy.push_back( tempRandomEnemy );
				}

				for ( unsigned int i = 0; i < Party.size(); i++ )
				{
					Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
					Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
				}

				TurnManager::GetInstance()->Initialize( Party, tempEnemy );
				for ( size_t i = 0; i < Party.size(); i++ )
				{
					if ( Party[ i ]->GetActive() )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
					}
				}
				dialogue->Load( "Assets/Scripts/trapfailure.xml" );
				state = Dia;
				laststate = GPStates::Combat;


				for ( size_t i = 0; i < tempEnemy.size(); i++ )
				{
					m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), tempEnemy[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
				}
				return;
			}

			else
				// no Combat
			{
				dialogue->Load( "Assets/Scripts/trapsuccess.xml" );
				state = Dia;
				laststate = GPStates::Town;
				// Get loots bro
				// Event?
				int gold = traps[ tripped_trap ]->gold;

				for ( int its = 0; its < 3; its++ )
				{
					if ( traps[ tripped_trap ]->items[ its ] < 5 && traps[ tripped_trap ]->items[ its ] >= 0 ) // proper item types
					{
						ItemType stupidcode = (ItemType)traps[ tripped_trap ]->items[ its ];
						int itemCounter = 0;
						for ( unsigned int i = 0; i < inventory.size(); i++ )
						{
							if ( inventory[ i ].GetName() == shopinv[ (int)( stupidcode ) ].GetName() )
							{
								++itemCounter;
							}
						}
						if ( itemCounter < 5 )
						{
							inventory.push_back( shopinv[ (int)( stupidcode ) ] );
						}
						else
							gold += 50;
					}
				}
				AddGold( gold );

				loot.clear();
				loot_gold = 0;
				loot_xp = 0;
				RemoveTrap( tripped_trap );
				break;
			}
		}
		if ( invincTimer > 0.0f )
		{
			invincTimer -= dt;
		}
		else
			tempInvinc = false;

		if ( !tempInvinc )
		{
			for ( unsigned int i = 0; i < guards.size(); i++ )
			{
				guards[ i ]->Update( dt );

				//if(guards[i]->GetRect().IsIntersecting(GameData::GetInstance()->GetOverworldPlayer()->GetRect()))
				if ( TileSystem::GetInstance()->GetTileIndex( guards[ i ]->GetPosition().x, guards[ i ]->GetPosition().y )
					 == TileSystem::GetInstance()->GetTileIndex(
					 GameData::GetInstance()->GetOverworldPlayer()->GetPosition().x,
					 GameData::GetInstance()->GetOverworldPlayer()->GetPosition().y )
					 && state != GPStates::Combat )

				{
					GuardAnimation();
					Enemy* temp;
					guard_index = i;
					std::vector<Ability*> partyAbilities;
					SGD::Point characterOrderPosition;
					int templevel = Party[ 0 ]->GetLevel();
					templevel -= 2;
					if ( templevel <= 0 )
					{
						templevel = 1;
						templevel += ( rand() % 2 );
					}
					else
						templevel += ( rand() % 5 );


					temp = LoadEnemy( "../Trapped Rat/Assets/Scripts/Guard.xml" );
					temp->SetOrderPosition( 0 );
					characterOrderPosition.x = 600.0f;
					characterOrderPosition.y = (float)( temp->GetOrderPosition() * 100 + 150 + 16 );
					temp->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Rush" ] );
					temp->InitializeAbilities( partyAbilities );
					temp->SetLevel( templevel );
					temp->SetHP( temp->GetMaxHP() );
					moreguards.push_back( temp );
					partyAbilities.clear();

					temp = LoadEnemy( "../Trapped Rat/Assets/Scripts/Guard.xml" );
					temp->SetOrderPosition( 1 );
					characterOrderPosition.x = 600.0f;
					characterOrderPosition.y = (float)( temp->GetOrderPosition() * 100 + 150 + 16 );
					temp->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Rush" ] );
					temp->InitializeAbilities( partyAbilities );
					temp->SetLevel( templevel );
					temp->SetHP( temp->GetMaxHP() );
					moreguards.push_back( temp );
					partyAbilities.clear();

					GameData::GetInstance()->SetIsInCombat( true );
					state = GPStates::Combat;

					/*	if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_overAudio))
							SGD::AudioManager::GetInstance()->StopAudio(m_overAudio);
							*/
					if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
					{
						StopAllBackgroundMusic();
						SGD::AudioManager::GetInstance()->PlayAudio( m_Audio, true );
					}

					TurnManager::GetInstance()->Initialize( Party, moreguards );
					for ( size_t i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetActive() )
						{
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
						}
					}
					for ( size_t i = 0; i < moreguards.size(); i++ )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), moreguards[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
					}
					return;
				}

			}

		}

	}
}

void GamePlayState::RemoveTrap( int index )
{
	// Safety first kids
	if ( index == -1 ) return;
	if ( index < (int)traps.size() )
	{
		for ( auto iter = traps.begin(); iter != traps.end(); iter++ )
		{
			if ( traps[ index ] == ( *iter ) )
			{
				delete traps[ index ];
				traps.erase( iter );
				break;
			}
		}

	}
	tripped_trap = -1;
	trap_combat = false;
}

void GamePlayState::MenuUpdate( float dt )
{
	//set the maxindex based on what menu we are in
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	switch ( substate )
	{
		case None:
			maxindex = 5;
			if ( laststate == Combat )
				maxindex = 2;

			if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
			{
				SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ 4 ] );
				menuindex = 0;
				state = laststate;
			}
			break;
		case Options:
			maxindex = 4;
			if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
			{
				SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ 4 ] );
				menuindex = 0;
				substate = MenuSubStates::None;

			}
			break;
		case Shop:
			maxindex = (int)shopinv.size() - 1;
			if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
			{
				SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ 4 ] );
				menuindex = 0;
				substate = MenuSubStates::None;
				uniquecounts.clear();
				uniquenames.clear();
				//shopinv.clear();

			}
			break;
		case HowTo:
			break;
		case MenuSubStates::Party:
			maxindex = Party.size() - 1;
			if ( select_first && selecting_item == false && selecting_party == false )
				maxindex = 2;
			if ( selecting_ability )
			{
				GamePlayState::GetHelpText()->ManualOverride( Party[ character_index ]->GetAbility( menuindex )->GetExplination() );
				maxindex = Party[ character_index ]->GetAbilitiesSize() - 1;
			}
			if ( selecting_item )
			{
				PopulateUniqueNames();
				maxindex = uniquenames.size() - 1;
			}

			if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
			{
				SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ 4 ] );
				if ( selecting_ability || selecting_party || select_first )
				{
					selecting_ability = false;
					selecting_party = false;
					select_first = false;
					select_new = false;
					selecting_item = false;
					menuindex = 0;
					oldindex = -1;
				}
				else
				{

					menuindex = 0;
					substate = MenuSubStates::None;
				}
			}

			break;
		case Save:
			maxindex = 2;
			if ( maxindex < 0 )
				maxindex = 0;
			if ( input->IsKeyPressed( SGD::Key::Escape ) || input->IsButtonPressed( 0, 2 ) )
			{
				SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ 4 ] );
				menuindex = 0;
				substate = MenuSubStates::None;


			}
			break;
		default:
			break;
	}


	//if they select a menu item
	if ( input->IsKeyPressed( SGD::Key::Enter ) || input->IsButtonPressed( 0, 1 ) )
	{
		switch ( substate )
		{
			case None:
			{
						 switch ( menuindex )
						 {
							 case 0:
							 {
									   PlaySoundEffect( 6 );
									   state = laststate;
									   break;
							 }
							 case 1:
								 if ( laststate == Combat )
								 {
									 PlaySoundEffect( 6 );
									 menuindex = 0;
									 substate = MenuSubStates::Options;
								 }
								 else
								 {
									 PlaySoundEffect( 6 );
									 substate = MenuSubStates::Shop;
								 }

								 break;
							 case 2:
								 if ( laststate == Combat )
								 {
									 PlaySoundEffect( 6 );
									 GameData::GetInstance()->SwapState( MainMenuState::GetInstance() );
									 state = GPStates::Town;
									 laststate = state;
									 substate = MenuSubStates::None;
									 menuindex = 0;
								 }
								 else
								 {
									 menuindex = 0;
									 selecting_ability = false;
									 selecting_party = false;
									 select_first = false;
									 select_new = false;
									 substate = MenuSubStates::Party;
									 PlaySoundEffect( 6 );
								 }
								 break;
							 case 3:
							 {
									   PlaySoundEffect( 6 );
									   TCHAR path[ MAX_PATH ];
									   if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path ) ) )
									   {
										   PathAppend( path, TEXT( "basicinfo.txt" ) );
									   }
									   std::ifstream fin;
									   fin.open( path );
									   if ( fin.is_open() )
									   {
										   for ( int i = 0; i < 3; i++ )
										   {
											   data temp;
											   fin >> temp.playtime >> temp.town >> temp.partysize >> temp.gold;
											   files[ i ] = temp;
											   fin.ignore( INT_MAX, '\n' );
										   }

									   }
									   //SaveGame();
									   menuindex = 0;
									   substate = Save;
									   //we should display something if the save is successful
									   //we should have them select which file to save to
									   break;
							 }
							 case 4:
								 PlaySoundEffect( 6 );
								 menuindex = 0;
								 substate = MenuSubStates::Options;

								 break;
							 case 5:
								 PlaySoundEffect( 6 );
								 GameData::GetInstance()->SwapState( MainMenuState::GetInstance() );
								 state = GPStates::Town;
								 laststate = state;
								 substate = MenuSubStates::None;
								 menuindex = 0;
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
						PlaySoundEffect( 6 );
						menuindex = 0;
						substate = MenuSubStates::None;
						break;
					default:
						break;
				}
				break;
			case Shop:

				if ( shopinv[ menuindex ].GetPrice() <= gold )
				{
					PlaySoundEffect( 6 );
					int itemCounter = 0;
					for ( unsigned int i = 0; i < inventory.size(); i++ )
					{
						if ( inventory[ i ].GetName() == shopinv[ menuindex ].GetName() )
						{
							++itemCounter;
						}
					}
					if ( itemCounter < 5 )
					{
						inventory.push_back( shopinv[ menuindex ] );
						gold -= shopinv[ menuindex ].GetPrice();
					}
				}
				uniquecounts.clear();
				uniquenames.clear();
				break;
			case MenuSubStates::Party:

				if ( selecting_ability == false && selecting_item == false && selecting_party == false && select_first == false )
				{
					PlaySoundEffect( 6 );
					select_first = true;
					character_index = menuindex;
					menuindex = 0;
					maxindex = 2;
				}
				else if ( selecting_ability == false && selecting_item == false && selecting_party == false && select_first == true )
				{
					PlaySoundEffect( 6 );
					if ( menuindex == 0 )
					{
						//Hovering over Swap character
						selecting_party = true;
						menuindex = 0;
					}
					if ( menuindex == 1 )
					{
						//Hovering Over Swap Ability
						selecting_ability = true;
						menuindex = 0;
					}
					if ( menuindex == 2 )
					{
						selecting_item = true;
						menuindex = 0;
					}
				}

				else if ( selecting_item )
				{
					if ( uniquenames[ menuindex ] == "Small Heal" )
					{
						if ( !Party[ character_index ]->isAlive() )
							break;


						int newhp = (int)( Party[ character_index ]->GetMaxHP() * .20f );
						if ( newhp < 20 ) newhp = 20;
						newhp += Party[ character_index ]->GetHP() ;

						if ( newhp < Party[ character_index ]->GetMaxHP() )
						{
							Party[ character_index ]->SetHP( newhp );
						}
						else
						{
							Party[ character_index ]->SetHP( Party[ character_index ]->GetMaxHP() );
						}

					}

					else if ( uniquenames[ menuindex ] == "Large Heal" )
					{
						if ( !Party[ character_index ]->isAlive() )
							break;

						int newhp = (int)( Party[ character_index ]->GetMaxHP() * .5f );
						if ( newhp < 50 ) newhp = 50;
						newhp += Party[ character_index ]->GetHP() ;

						if ( newhp < Party[ character_index ]->GetMaxHP() )
						{
							//int newhp = (int)( Party[ character_index ]->GetMaxHP() * .5f + Party[ character_index ]->GetHP() );
							Party[ character_index ]->SetHP( newhp );
						}
						else
						{
							Party[ character_index ]->SetHP( Party[ character_index ]->GetMaxHP() );
						}

					}

					if ( uniquenames[ menuindex ] == "Small BP Restore" )
					{
						if ( !Party[ character_index ]->isAlive() )
							break;

						int newbp = (int)( Party[ character_index ]->GetMaxBP() * .20f );
						if ( newbp < 10 ) newbp = 10;
						newbp += Party[ character_index ]->GetBP() ;

						//int newbp = (int)( Party[ character_index ]->GetMaxBP() * .20f + Party[ character_index ]->GetBP() );
						if ( newbp < Party[ character_index ]->GetMaxBP() )
						{
							Party[ character_index ]->SetBP( newbp );
						}
						else
						{
							Party[ character_index ]->SetBP( Party[ character_index ]->GetMaxBP() );
						}
					}

					if ( uniquenames[ menuindex ] == "Large BP Restore" )
					{
						if ( !Party[ character_index ]->isAlive() )
							break;
						int newbp = (int)( Party[ character_index ]->GetMaxBP() * .40f );
						if ( newbp < 25 ) newbp = 25;
						newbp += Party[ character_index ]->GetBP() ;
						//if ( newbp <= Party[ character_index ]->GetMaxBP() )
						{
							//int newbp = (int)( Party[ character_index ]->GetMaxBP() * .40f + Party[ character_index ]->GetBP() );
							if ( newbp <= Party[ character_index ]->GetMaxBP() )
							{
								Party[ character_index ]->SetBP( newbp );
							}
							else
							{
								Party[ character_index ]->SetBP( Party[ character_index ]->GetMaxBP() );
							}
						}
						if ( uniquenames[ menuindex ] == "Revive" )
						{
							if ( Party[ character_index ]->isAlive() )
								return;
							Party[ character_index ]->SetLiving( true );
							Party[ character_index ]->SetHP( (int)( Party[ character_index ]->GetMaxHP() * 0.25 ) );
							Party[ character_index ]->GetAnimations()->Play( 0 );
						}
					}
					if ( uniquenames[ menuindex ] == "Revive" )
					{
						if ( Party[ character_index ]->isAlive() )
							return;
						Party[ character_index ]->SetLiving( true );
						Party[ character_index ]->SetHP( (int)( Party[ character_index ]->GetMaxHP() * 0.25 ) );
						Party[ character_index ]->GetAnimations()->Play( 0 );
					}

					for ( unsigned int i = 0; i < inventory.size(); i++ )
					{
						if ( uniquenames[ menuindex ] == inventory[ i ].GetName() )
						{
							inventory.erase( inventory.begin() + i );
							break;
						}
						selecting_ability = false;
						selecting_party = false;
						select_first = false;
						select_new = false;
						selecting_item = false;
						menuindex = 0;
						oldindex = -1;
					}
				}

				else if ( selecting_ability && !select_new )
				{
					PlaySoundEffect( 6 );
					if ( !Party[ character_index ]->GetAbility( menuindex )->GetUnlocked() )
						return;
					select_new = true;
					//Party[character_index]->GetAbility(menuindex)->SetUnlocked(false);
					oldindex = menuindex;

				}
				else if ( select_new && selecting_ability )
				{
					PlaySoundEffect( 6 );
					if ( !Party[ character_index ]->GetAbility( menuindex )->GetUnlocked() )
						return;
					//Party[character_index]->GetAbility(menuindex)->SetUnlocked(true);
					if ( Party[ character_index ]->GetLevel() >= Party[ character_index ]->GetAbility( menuindex )->GetUnlockLevel() )
					{

						Ability* temp = Party[ character_index ]->GetAbility( menuindex );
						Ability *temp2 = Party[ character_index ]->GetAbility( oldindex );
						Party[ character_index ]->SetAbility( menuindex, temp2 );
						Party[ character_index ]->SetAbility( oldindex, temp );
					}

					select_new = false;
					selecting_ability = false;
					select_first = false;
					menuindex = character_index;
					oldindex = -1;
				}

				else if ( selecting_party && !select_new )
				{
					PlaySoundEffect( 6 );
					/*select_new = true;
					oldindex = menuindex;*/
					//Actually Move stuff
					bool ratactive = false;
					int ratindex = 0;
					CombatPlayer* temp = Party[ character_index ];
					Party[ character_index ] = Party[ menuindex ];
					Party[ menuindex ] = temp;
					for ( unsigned int i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetName() == "Ratsputin" )
						{
							ratindex = i;
						}
						if ( i <= 2 )
						{
							Party[ i ]->SetActive( true );
							if ( Party[ i ]->GetName() == "Ratsputin" )
							{
								ratactive = true;
							}
						}
						else
						{
							Party[ i ]->SetActive( false );
						}
						Party[ i ]->SetOrderPosition( i );

						SGD::Point characterOrderPosition;
						characterOrderPosition.x = 100.0f;
						characterOrderPosition.y = (float)( Party[ i ]->GetOrderPosition() * 100 + 150 );
						Party[ i ]->SetPosition( characterOrderPosition );
					}

					if ( !ratactive )
					{
						CombatPlayer* temp2 = Party[ ratindex ];
						Party[ ratindex ] = Party[ 1 ];
						Party[ 1 ] = temp2;
						Party[ 1 ]->SetActive( true );
						Party[ 1 ]->SetOrderPosition( 1 );
						Party[ ratindex ]->SetActive( false );
					}

					select_new = false;
					selecting_party = false;
					select_first = false;
					menuindex = character_index;
					oldindex = -1;

				}
				break;
			case Save:
				PlaySoundEffect( 6 );
				saveindex = menuindex;
				menuindex = 3;
				substate = None;
				SaveGame();
				break;
			default:
				break;
		}


	}


	//changing the values for language


	if ( ( input->IsKeyPressed( SGD::Key::Left ) || input->IsKeyPressed( SGD::Key::Right ) || input->IsDPadPressed( 0, SGD::DPad::Left ) || input->IsDPadPressed( 0, SGD::DPad::Right ) || input->GetLeftJoystick( 0 ).x < 0 || input->GetLeftJoystick( 0 ).x > 0 ) )
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

										  if ( ( input->IsKeyPressed( SGD::Key::Left ) || input->IsDPadPressed( 0, SGD::DPad::Left ) || input->GetLeftJoystick( 0 ).x < 0 ) && GameData::GetInstance()->input_timer < 0 )
										  {
											  GameData::GetInstance()->SetMusicVolume( GameData::GetInstance()->GetMusicVolume() - 5 );
											  GameData::GetInstance()->input_timer = 0.15f;
										  }
										  else if ( ( input->IsKeyPressed( SGD::Key::Right ) || input->IsDPadPressed( 0, SGD::DPad::Right ) || input->GetLeftJoystick( 0 ).x > 0 ) && GameData::GetInstance()->input_timer < 0 )
										  {
											  GameData::GetInstance()->SetMusicVolume( GameData::GetInstance()->GetMusicVolume() + 5 );
											  GameData::GetInstance()->input_timer = 0.15f;
										  }
										  GameData::GetInstance()->PlaySelectionChange();
										  break;
								}
								case 2:
								{
										  if ( ( input->IsKeyPressed( SGD::Key::Left ) || input->IsDPadPressed( 0, SGD::DPad::Left ) || input->GetLeftJoystick( 0 ).x < 0 ) && GameData::GetInstance()->input_timer < 0 )
										  {
											  GameData::GetInstance()->SetEffectVolume( GameData::GetInstance()->GetEffectVolume() - 5 );
											  GameData::GetInstance()->input_timer = 0.15f;
										  }
										  else if ( ( input->IsKeyPressed( SGD::Key::Right ) || input->IsDPadPressed( 0, SGD::DPad::Right ) || input->GetLeftJoystick( 0 ).x > 0 ) && GameData::GetInstance()->input_timer < 0 )
										  {
											  GameData::GetInstance()->SetEffectVolume( GameData::GetInstance()->GetEffectVolume() + 5 );
											  GameData::GetInstance()->input_timer = 0.15f;
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
	if ( ( input->IsKeyPressed( SGD::Key::Up ) || input->IsDPadPressed( 0, SGD::DPad::Up ) || input->GetLeftJoystick( 0 ).y < 0 ) && GameData::GetInstance()->input_timer < 0 )
	{
		menuindex--;
		if ( menuindex < 0 )
			menuindex = maxindex;
		GameData::GetInstance()->PlaySelectionChange();
		GameData::GetInstance()->input_timer = 0.15f;
	}
	if ( ( input->IsKeyPressed( SGD::Key::Down ) || input->IsDPadPressed( 0, SGD::DPad::Down ) || input->GetLeftJoystick( 0 ).y > 0 ) && GameData::GetInstance()->input_timer < 0 )
	{
		menuindex++;
		if ( menuindex > maxindex )
			menuindex = 0;
		GameData::GetInstance()->PlaySelectionChange();
		GameData::GetInstance()->input_timer = 0.15f;
	}
}



void GamePlayState::TownRender()
{
	TileSystem::GetInstance()->Render();

	for ( unsigned int i = 0; i < overworldObjects.size(); i++ )
	{
		if ( overworldObjects[ i ] != nullptr )
			overworldObjects[ i ]->Render();
	}

	GameData::GetInstance()->GetOverworldPlayer()->Render();
	for ( unsigned int i = 0; i < guards.size(); i++ )
	{
		guards[ i ]->Render();
		SGD::Point xy = guards[ i ]->GetPosition();
		int x = 0;
	}

	for ( unsigned int i = 0; i < traps.size(); i++ )
	{
		traps[ i ]->Render();
	}

	// Render num Guards left!
	if ( guards.size() )
	{
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 600, 41, { 0, 0, 0 }, 2.0f );
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 598, 39, { 125, 0, 0 }, 2.0f );
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 599, 40, { 255, 0, 0 }, 2.0f );
	}
	else
	{
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 600, 41, { 0, 0, 0 }, 2.0f );
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 598, 39, { 0, 125, 0 }, 2.0f );
		GameData::GetInstance()->GetFont()->DrawString( "Guards Left:", 599, 40, { 0, 255, 0 }, 2.0f );
	}
	GameData::GetInstance()->GetFont()->DrawString( std::to_string( guards.size() ), 781, 41, { 0, 0, 0 }, 2.0f );
	GameData::GetInstance()->GetFont()->DrawString( std::to_string( guards.size() ), 779, 39, { 0, 125, 85 }, 2.0f );
	GameData::GetInstance()->GetFont()->DrawString( std::to_string( guards.size() ), 780, 40, { 0, 255, 175 }, 2.0f );
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
			if ( laststate != Combat )
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 335.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 415.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 495.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
			}
			graphics->DrawTextureSection( cursor, { 10.0f, 95.0f + ( menuindex * 80 ) }, { 0, 0, 238, 73 } );
			if ( laststate == Combat )
			{

				GameData::GetInstance()->GetFont()->DrawString( "Resume", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Options", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Main Menu", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f );

			}
			else
			{
				GameData::GetInstance()->GetFont()->DrawString( "Resume", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Shop", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Party", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Save Game", 100.0f, 360.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Options", 100.0f, 440.0f, { 0, 0, 0 }, 2.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Main Menu", 100.0f, 520.0f, { 0, 0, 0 }, 2.0f );

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
						 GameData::GetInstance()->GetFont()->DrawString( shopinv[ i ].GetName(), 100.0f, 160.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
						 std::ostringstream price;
						 price << shopinv[ i ].GetPrice() << "g";
						 if ( gold < shopinv[ i ].GetPrice() )
							 GameData::GetInstance()->GetFont()->DrawString( price.str(), 200.0f, 185.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
						 else
							 GameData::GetInstance()->GetFont()->DrawString( price.str(), 200.0f, 185.0f + ( i * 55 ), { 155, 155, 155 }, 1.5f );

					 }
					 SGD::GraphicsManager::GetInstance()->DrawTexture( trapcursor, { 240.0f, 145.0f + ( menuindex * 55 ) } );

					 //now display the players inventory
					 GameData::GetInstance()->GetFont()->DrawString( "Your Inventory", 450.0f, 40.0f, { 0, 0, 0 }, 2.0f );
					 SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
					 PopulateUniqueNames();
					 for ( unsigned int i = 0; i < uniquenames.size(); i++ )
					 {



						 std::ostringstream total;
						 total << uniquecounts[ i ] << "/5";
						 GameData::GetInstance()->GetFont()->DrawString( uniquenames[ i ], 490, 160.0f + ( i * 55 ), { 0, 0, 0 }, 1.5f );
						 GameData::GetInstance()->GetFont()->DrawString( total.str(), 490.0f, 185.0f + ( i * 55 ), { 155, 155, 155 }, 1.5f );
					 }
		}
			break;
		case MenuSubStates::Party:
			GameData::GetInstance()->GetFont()->DrawString( "Party Menu", 60.0f, 40.0f, { 0, 0, 0 }, 2.0f );
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 50.0f, 50.0f }, { 0, 0, 300, 540 } );
			for ( unsigned int i = 0; i < Party.size(); i++ )
			{
				if ( i < 3 )
				{
					GameData::GetInstance()->GetFont()->DrawString( Party[ i ]->GetName(), 90.0f, 185.0f + ( i * 55 ), { 0, 150, 0 }, 1.5f );
					std::ostringstream lvl;
					lvl << "Level: " << Party[ i ]->GetLevel();
					GameData::GetInstance()->GetFont()->DrawString( lvl.str(), 250.0f, 185.0f + ( i * 55 ), { 0, 0, 0 }, 1.0f );
				}
				else
				{
					GameData::GetInstance()->GetFont()->DrawString( Party[ i ]->GetName(), 90.0f, 185.0f + ( i * 55 ), { 150, 0, 0 }, 1.5f );
				}
				std::ostringstream info;
				info << "HP: " << Party[ i ]->GetHP() << "/" << Party[ i ]->GetMaxHP() << "  BP: " << Party[ i ]->GetBP() << "/" << Party[ i ]->GetMaxBP() << "  XP:" << Party[ i ]->GetXP() << "/" << Party[ i ]->GetNextlvl();
				GameData::GetInstance()->GetFont()->DrawString( info.str(), 90.0f, 200.0f + ( i * 55 ), { 0, 0, 0 }, 1.0f );
				SGD::GraphicsManager::GetInstance()->DrawRectangle( { 80.0f, 180.0f, 320.0f, 330.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2 );

			}

			if ( selecting_ability )
			{
				GamePlayState::GetHelpText()->Render();
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
				for ( int i = 0; i < Party[ character_index ]->GetAbilitiesSize(); i++ )
				{
					std::string name = Party[ character_index ]->GetAbility( i )->GetAbilityName();
					if ( oldindex == i )
					{
						GameData::GetInstance()->GetFont()->DrawString( name, 490.0f, 185.0f + ( i * 35 ), { 155, 155, 155 }, 1.5f );
					}
					else
					{
						if ( Party[ character_index ]->GetAbility( i )->GetUnlocked() && i <= 3 )
							GameData::GetInstance()->GetFont()->DrawString( name, 490.0f, 185.0f + ( i * 35 ), { 0, 255, 0 }, 1.5f );
						else if ( !Party[ character_index ]->GetAbility( i )->GetUnlocked() )
							GameData::GetInstance()->GetFont()->DrawString( name, 490.0f, 185.0f + ( i * 35 ), { 255, 0, 0 }, 1.5f );
						else
							GameData::GetInstance()->GetFont()->DrawString( name, 490.0f, 185.0f + ( i * 35 ), { 0, 0, 0 }, 1.5f );

					}
				}
				graphics->DrawTextureSection( cursor, { 400.0f, 160.0f + ( menuindex * 35 ) }, { 0, 0, 238, 73 } );
				SGD::GraphicsManager::GetInstance()->DrawRectangle( { 480.0f, 180.0f, 715.0f, 320.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2 );
			}
			else if ( selecting_item )
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
				for ( unsigned int i = 0; i < uniquenames.size(); i++ )
				{
					GameData::GetInstance()->GetFont()->DrawString( uniquenames[ i ], 490.0f, 225.0f + ( i * 40 ), { 0, 0, 0 }, 1.0f );
					std::ostringstream total;
					total << uniquecounts[ i ] << "/5";
					GameData::GetInstance()->GetFont()->DrawString( total.str(), 490.0f, 245.0f + ( i * 40 ), { 0, 0, 0 }, 1.0f );

				}
				if ( uniquenames.size() == 0 )
				{
					GameData::GetInstance()->GetFont()->DrawString( "Empty", 490.0f, 225.0f, { 0, 0, 0 }, 1.0f );
				}
				else
				{
					SGD::GraphicsManager::GetInstance()->DrawTexture( cursor, { 400.0f, 225.0f + ( menuindex * 40 ) }, 0.0f, { }, { }, { .75f, .75f } );
				}
			}
			else if ( selecting_party )
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 240 } );
				GameData::GetInstance()->GetFont()->DrawString( "Swap Position_With Who?", 490.0f, 185.0f, { 0, 0, 0 }, 1.5f );
				graphics->DrawTextureSection( cursor, { 10.0f, 165.0f + ( menuindex * 55 ) }, { 0, 0, 238, 73 } );
			}
			else if ( select_first )
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 240 } );
				GameData::GetInstance()->GetFont()->DrawString( "Swap Character", 490.0f, 185.0f, { 0, 0, 0 }, 1.5f );
				GameData::GetInstance()->GetFont()->DrawString( "Swap Ability", 490.0f, 220.0f, { 0, 0, 0 }, 1.5f );
				GameData::GetInstance()->GetFont()->DrawString( "Use Item", 490.0f, 255.0f, { 0, 0, 0 }, 1.5f );
				graphics->DrawTextureSection( cursor, { 400.0f, 160.0f + ( menuindex * 35 ) }, { 0, 0, 238, 73 } );

			}
			else
			{
				SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
				for ( int i = 0; i < Party[ menuindex ]->GetAbilitiesSize(); i++ )
				{
					std::string name = Party[ menuindex ]->GetAbility( i )->GetAbilityName();
					GameData::GetInstance()->GetFont()->DrawString( name, 490.0f, 185.0f + ( i * 35 ), { 0, 0, 0 }, 1.5f );
				}
				graphics->DrawTextureSection( cursor, { 10.0f, 165.0f + ( menuindex * 55 ) }, { 0, 0, 238, 73 } );
				SGD::GraphicsManager::GetInstance()->DrawRectangle( { 480.0f, 180.0f, 715.0f, 320.0f }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2 );
			}
			break;
		case Save:
		{
					 Font * font = GameData::GetInstance()->GetFont();


					 graphics->DrawTextureSection( background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f } );
					 SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
					 if ( maxindex >= 1 )
						 SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );
					 if ( maxindex == 2 )
						 SGD::GraphicsManager::GetInstance()->DrawTextureSection( button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f } );

					 graphics->DrawTextureSection( cursor, { 10.0f, 95.0f + ( menuindex * 80 ) }, { 0, 0, 238, 73 } );
					 GameData::GetInstance()->GetFont()->DrawString( "File 1", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f );
					 if ( maxindex >= 1 )
						 GameData::GetInstance()->GetFont()->DrawString( "File 2", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f );
					 if ( maxindex == 2 )
						 GameData::GetInstance()->GetFont()->DrawString( "File 3", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f );


					 SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 } );
					 //have a file that has quick info to load in
					 //here we display a scroll to show how well the game is going
					 //we want
					 //time of last save
					 //who was in the main party
					 //the town they are currently in
					 if ( files[ menuindex ].playtime != 0 )
					 {
						 std::string townwords;
						 if ( files[ menuindex ].town == 0 )
						 {
							 townwords = "Windy Woods";
						 }
						 if ( files[ menuindex ].town == 1 )
						 {
							 townwords = "Magma Falls";
						 }
						 if ( files[ menuindex ].town == 2 )
						 {
							 townwords = "Earthy Town";
						 }
						 if ( files[ menuindex ].town == 3 )
						 {
							 townwords = "New Water City";
						 }
						 if ( files[ menuindex ].town == 4 )
						 {
							 townwords = "Hero's Landing";
						 }
						 std::ostringstream playtime;
						 playtime << "Playtime: " << ceilf( files[ menuindex ].playtime * 10 ) / 10 << " minutes";
						 GameData::GetInstance()->GetFont()->DrawString( playtime.str(), 475.0f, 225.0f, { 0, 0, 0 } );
						 std::ostringstream town;
						 town << "Town: " << townwords;
						 GameData::GetInstance()->GetFont()->DrawString( town.str(), 475.0f, 275.0f, { 0, 0, 0 } );
						 std::ostringstream party;
						 party << "Party Size: " << files[ menuindex ].partysize;
						 GameData::GetInstance()->GetFont()->DrawString( party.str(), 475.0f, 325.0f, { 0, 0, 0 } );
						 std::ostringstream gold;
						 gold << "Gold: " << files[ menuindex ].gold << 'g';
						 GameData::GetInstance()->GetFont()->DrawString( gold.str(), 475.0f, 375.0f, { 0, 0, 0 } );
					 }
					 else
					 {
						 GameData::GetInstance()->GetFont()->DrawString( "EMPTY", 475.0f, 175.0f, { 0, 0, 0 } );
					 }
					 break;
		}
		default:
			break;
	}
}

void GamePlayState::CombatUpdate( float dt )
{

	if ( is_tutorial )
	{
		//dialogue->Load( "Assets/Scripts/tutorialdialogue.xml" );
		//this is bad now everything needs to change
		state = Dia;
		is_tutorial = false;
		ignore_game_over = true;
		dialogue->Load( "../Trapped Rat/Assets/Scripts/attacktutorial.xml" );

	}
	//run_succeed = false;
	laststate = GPStates::Combat;
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	dynamic_cast<Bars*>( ternary_bar )->SetPercentage( (float)ternary_gauge / MAXTG );


	if ( run_succeed || input->IsKeyPressed( SGD::Key::Backspace ) )
	{
		guard_index = -1;
		no_combat_timer = 4.0f;
		state = GPStates::Town;
		laststate = state;
		GameData::GetInstance()->SetIsInCombat( false );
		TurnManager::GetInstance()->Terminate();
		run_succeed = false;
	}

	if ( input->IsKeyPressed( SGD::Key::Eight ) )
	{
		Party[ 0 ]->SetBP( Party[ 0 ]->GetMaxBP() );
	}
	if ( input->IsKeyPressed( SGD::Key::One ) )
	{
		Party[ 0 ]->SetHP( Party[ 0 ]->GetMaxHP() );
	}
	if ( input->IsKeyPressed( SGD::Key::Z ) )
	{
		Party[ 0 ]->SetProgress( 100.0f );
	}
	if ( input->IsKeyPressed( SGD::Key::X ) )
	{
		Party[ 1 ]->SetProgress( 100.0f );
	}
	if ( input->IsKeyPressed( SGD::Key::C ) )
	{
		Party[ 2 ]->SetProgress( 100.0f );
	}

	if ( input->IsKeyPressed( SGD::Key::Nine ) )
	{
		ternary_gauge = MAXTG;
	}

	if ( state == GPStates::Town )
	{
		//	if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
		//	SGD::AudioManager::GetInstance()->StopAudio( m_Audio );
		if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
		{
			StopAllBackgroundMusic();
			SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio );
		}
	}

	for ( size_t i = 0; i < m_vhuditems.size(); i++ )
	{
		m_vhuditems[ i ]->Update( dt );
	}

	if ( ternary_gauge == MAXTG && triptern )
	{
		SGD::AudioManager::GetInstance()->PlayAudio( chime );
		triptern = false;
	}



	TurnManager::GetInstance()->Update( dt );


}
void GamePlayState::CombatRender()
{
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	graphics->DrawTexture( combatback, { 0, 0 } );
	graphics->DrawTextureSection( combathud, { -0.1f, 425.0f }, { 0.0f, 0.0f, 800.0f, 175.0f } );
	//GameData::GetInstance()->GetFont()->DrawString( "You are in Combat", 50.0, 50.0, { 155, 255, 155 } );

	for ( size_t i = 0; i < m_vhuditems.size(); i++ )
	{
		if ( m_vhuditems[ i ]->GetActive() )
			m_vhuditems[ i ]->Render();
	}
	for ( size_t i = 0; i < m_vSelectableItems.size(); i++ )
	{
		if ( m_vSelectableItems[ i ]->GetActive() )
			m_vSelectableItems[ i ]->Render();
	}
	helptextbox->Render();
	TurnManager::GetInstance()->Render();
	// Let's put the ternary bar here
	ternary_bar->Render();
	if ( ternary )
	{
		// Scroll Backdrop
		graphics->DrawTexture( scroll, { 320, 155 }, 0.0f, { }, { }, { 0.45f, 0.4f } );

		// Ability Texts
		for ( int i = 0; i < myTernTargets.num_targets; i++ )
		{
			GameData::GetInstance()->GetFont()->DrawString( myTernTargets.abilities[ i ]->GetAbilityName(), 330.0f, (float)210 + ( i * 30 ), { 0, 0, 255 } );
		}

		if ( myTernTargets.num_targets == 3 )
		{
			GameData::GetInstance()->GetFont()->DrawString( "Press Enter", 330.0f, 300.0f, { 0, 255, 0 } );
		}
	}

	if ( ternary_gauge == MAXTG )
		//ternary_bar = CreateBar( { 64, 16 }, { 250, 390 }, nullptr, { 255, 255, 0 }, { 0, 0 }, false );
	{
		// Here!  Here is the Cheese!
		graphics->DrawTexture( cursor, { 180, 370 } );
		graphics->DrawTexture( cursor, { 580, 370 }, 0.0f, { }, { }, { -1.0f, 1.0f } );

	}

}


Enemy * GamePlayState::CreateCommonEnemy( std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, float _speed, float _progress, Ability* abilityarr[ ], SGD::Point _position, SGD::Size _size, std::string _animfilename )
{
	CommonEnemy * temp = new CommonEnemy();

	OwnedHUD* turnindicator = new OwnedHUD;
	turnindicator->SetActive( true );
	turnindicator->SetImage( TurnIndicator );
	turnindicator->SetOwner( temp );
	turnindicator->SetOffset( SGD::Point( -10, -64 ) );
	temp->SetTurnIndicator( turnindicator );

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
			temp->SetAbility( i, abilityarr[ i ] );
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
CombatPlayer * GamePlayState::CreateCombatPlayer( std::string name, Stats _stats, int _lvl, int _hp, int _maxhp, int _bp, int _maxbp, float _speed, float _progress, Ability* abilityarr[ ], SGD::Point _position, SGD::Size _size, std::string _animfilename )
{
	CombatPlayer * temp = new CombatPlayer();

	OwnedHUD* turnindicator = new OwnedHUD;
	turnindicator->SetActive( true );
	turnindicator->SetImage( TurnIndicator );
	turnindicator->SetOwner( temp );
	turnindicator->SetOffset( SGD::Point( -10, -42 ) );
	temp->SetTurnIndicator( turnindicator );

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
			temp->SetAbility( i, abilityarr[ i ] );
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

void GamePlayState::AddTrap( std::string loot, SGD::Point pos )
// string needs parsed thusly: 000.111
// 000 are individual item ids, 111 is the amount of gold to recieve
{
	int i1, i2, i3;
	i1 = loot[ 0 ] - '0';
	i2 = loot[ 1 ] - '0';
	i3 = loot[ 2 ] - '0';
	std::string temp = "";
	temp += loot[ 4 ];
	temp += loot[ 5 ];
	temp += loot[ 6 ];
	int gold = std::stoi( temp.c_str() );

	RatTrap* toadd = CreateRatTrap( pos, i1, i1, i3, gold );
	traps.push_back( toadd );

}

RatTrap * GamePlayState::CreateRatTrap( SGD::Point pos, int item1, int item2, int item3, int gold )
//enum ItemType { SmallHP, LargeHP, SmallBP, LargeBP, Revive, MaxRevive };
// Traps hold up to 3 items, -1 will translate to nothing + gold
{
	RatTrap* temp = new RatTrap();
	temp->SetPosition( pos );
	temp->items[ 0 ] = item1;
	temp->items[ 1 ] = item2;
	temp->items[ 2 ] = item3;
	temp->gold = gold;
	temp->SetImage( rattrap );
	return temp;
}

void GamePlayState::DialogueUpdate( float dt )
{
	if ( !dialogue->Update( dt ) )
	{
		state = laststate;
		if ( FinalBossFight && laststate == BattleSummary )
		{

			FinalBossFight = false;
			laststate = Town;
			state = Town;
			GameData::GetInstance()->SwapState( GameOverWinState::GetInstance() );
		}
	}
}
void GamePlayState::DialogueRender()
{
	if ( laststate == Combat )
	{
		CombatRender();
	}
	else if ( laststate == Map )
	{
		MapRender();
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
		m_vSelectableItems[ _index ] = _obj;
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

		std::string tempStr = root->FirstChildElement( "Portrait" )->GetText();
		std::string charPortrait = "../Trapped Rat/Assets/Textures/" + tempStr;
		tempStr = root->FirstChildElement( "Timeline" )->GetText();
		//std::string timeLine = "../Trapped Rat/Assets/Textures/" + tempStr;
		std::string timeLine = tempStr;

		if ( type == "Ally" )
		{
			//create a player
			toon = CreateCombatPlayer( name, stats, level, HP, HP, BP, BP, speed, 0, nullptr, { 0.0f, 0.0f }, { 0.0f, 0.0f }, animation );
			toon->SetAlliance( type );
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

		AnimationSystem* tempani = new AnimationSystem();
		tempani->Load( timeLine.c_str() );
		tempani->Play( 0 );
		toon->SetTimelineAnimation( tempani );
		toon->SetPortrait( SGD::GraphicsManager::GetInstance()->LoadTexture( charPortrait.c_str() ) );

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
		std::string tempStr = root->FirstChildElement( "Portrait" )->GetText();
		std::string charPortrait = "../Trapped Rat/Assets/Textures/" + tempStr;
		tempStr = root->FirstChildElement( "Timeline" )->GetText();
		//std::string timeLine = "../Trapped Rat/Assets/Textures/" + tempStr;
		std::string timeLine = tempStr;
		if ( type == "Ally" )
		{

			return nullptr;
		}
		else if ( type == "Enemies" || type == "Boss" || type == "Mini Boss" )
		{
			//create a enemy
			toon = CreateCommonEnemy( name, stats, level, HP, HP, speed, 0, nullptr, { 0.0f, 0.0f }, { 64.0f, 64.0f }, animation );
			toon->SetAlliance( type );
		}
		else if ( type == "Guard" )
		{
			return nullptr;
		}
		else
		{
			return nullptr;
		}
		switch ( SelectedTown )
		{
			case 0:
				toon->SetEtype( ETYPE::WIND );
				break;
			case 1:
				toon->SetEtype( ETYPE::FIRE );
				break;
			case 2:
				toon->SetEtype( ETYPE::EARTH );
				break;
			case 3:
				toon->SetEtype( ETYPE::WATER );
				break;
			case 4:
			{
					  int randomEle = rand() % 4;
					  switch ( randomEle )
					  {
						  case 1:
							  toon->SetEtype( ETYPE::WIND );
							  break;
						  case 2:
							  toon->SetEtype( ETYPE::FIRE );
							  break;
						  case 3:
							  toon->SetEtype( ETYPE::EARTH );
							  break;
						  case 4:
							  toon->SetEtype( ETYPE::WATER );
							  break;
						  default:
							  break;
					  }
			}
				break;
			default:
				break;
		}
		/*if ( element == "Wind" )
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
		}*/

		AnimationSystem* tempani = new AnimationSystem();
		tempani->Load( timeLine.c_str() );
		tempani->Play( 0 );
		toon->SetTimelineAnimation( tempani );
		toon->SetPortrait( SGD::GraphicsManager::GetInstance()->LoadTexture( charPortrait.c_str() ) );
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
	if ( val >= 0 )
		ternary_gauge = val;
	if ( val == 0 )
		triptern = true;
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

void GamePlayState::SetTernary( bool buul )
{
	ternary = buul;
}

void GamePlayState::AddToTB( Ability* abi, Character* target )
{
	// Don't do this so many times
	if ( myTernTargets.num_targets >= 3 ) return;

	myTernTargets.targets.push_back( target );
	myTernTargets.abilities.push_back( abi );
}

void GamePlayState::AddTarget()
{
	myTernTargets.num_targets++;
}

bool GamePlayState::RemoveTarget()
{
	myTernTargets.num_targets--;
	if ( myTernTargets.num_targets < 0 )
	{
		myTernTargets.num_targets = 0;
		return false;
	}
	return true;
}

void GamePlayState::DeselectTernaryTarget()
{
	// No longer need this style of deselect
	//if ( myTernTargets.abilities[myTernTargets.abilities.size()-1]->GetAOE() )	// AOE ability
	//{
	//	for(unsigned int i=0; i < TurnManager::GetInstance()->GetEnemies().size(); i++)
	//	{
	//		if ( TurnManager::GetInstance( )->GetEnemies( )[i]->isAlive())
	//		{
	//			myTernTargets.abilities.pop_back( );
	//			myTernTargets.targets.pop_back( );
	//		}
	//	}
	//}

	//myTernTargets.num_targets--;
	myTernTargets.abilities.pop_back();
	myTernTargets.targets.pop_back();

}

void GamePlayState::ClearTernary()
{
	myTernTargets.abilities.clear();
	myTernTargets.targets.clear();
	myTernTargets.num_targets = 0;
}

bool GamePlayState::AddToGauge( int val )
// Adds value to the gauge
// returns true if gauge has been filled
{
	ternary_gauge += val;
	if ( ternary_gauge >= MAXTG )
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

void GamePlayState::CutsceneUpdate( float dt )
{
	CutsceneManager::GetInstance()->Update( dt );
}

void GamePlayState::CutsceneRender()
{
	CutsceneManager::GetInstance()->Render();
}
void GamePlayState::Loading( std::string _loading )
{
	//here we..
	//clear the screen
	//display _loading
	//
	//SGD::GraphicsManager::GetInstance()->Update();
	SGD::GraphicsManager::GetInstance()->DrawTextureSection( background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f } );
	SGD::GraphicsManager::GetInstance()->DrawTextureSection( scroll, { 520.0f, 200.0f }, { 0, 0, 300, 540 }, SGD::PI / 2, { 112.5f, 27.5f } );
	GameData::GetInstance()->GetFont()->DrawString( _loading, 250.0f, 250.0f, { 0, 0, 0 }, 2.0f );
	SGD::GraphicsManager::GetInstance()->Update();
	Sleep( 250 );

}

void GamePlayState::CheckAbilityUnlocked( bool EOC )
{
	abilityunlocked = false;
	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		for ( unsigned int k = 0; k < 8; k++ )
		{
			if ( !Party[ i ]->GetAbility( k )->GetUnlocked() && Party[ i ]->GetLevel() >= Party[ i ]->GetAbility( k )->GetUnlockLevel() )
			{
				Party[ i ]->GetAbility( k )->SetUnlocked( true );
				if ( EOC )
				{

					//Enter code to notify loot/exp screen that a new ability was unlocked(i.e. level gained...show stat gains and new ability unlocked)
					// Print to Window Screen the details
					abilityunlocked = true;
				}
			}
		}
	}
}

int GamePlayState::GetGuards()
{
	return guards.size();
}

void GamePlayState::MapUpdate( float dt )
{
	TileSystem::GetInstance()->Exit();

	//if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_overAudio))
	//{
	//		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Audio))
	//		SGD::AudioManager::GetInstance()->StopAudio(m_Audio);
	if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Map ) )
	{
		StopAllBackgroundMusic();
		SGD::AudioManager::GetInstance()->PlayAudio( m_Map );
	}
	//}

	SGD::InputManager * input = SGD::InputManager::GetInstance();
	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Six ) )
	{
		if ( unlockedTowns >= 1 )
		{
			unlockedTowns--;
		}
	}
	else if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Seven ) )
	{
		if ( unlockedTowns <= 3 )
		{
			unlockedTowns++;
		}
	}

	if ( WorldMapAnsys != nullptr )
	{
		GameData::GetInstance()->SetCamera( { 0, 0 } );
		WorldMapAnsys->Update( dt );
	}
	if ( unlockedTowns == 4 && SelectedTown != 4 )
	{
		SelectedTown = 4;
		WorldMapAnsys->Play( "Down" );

	}
	else
	{
		if ( ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Right ) || input->IsDPadPressed( 0, SGD::DPad::Right ) || input->GetLeftJoystick( 0 ).x > 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
			if ( unlockedTowns != 4 )
			{
				SelectedTown = 0;
				GameData::GetInstance()->PlaySelectionChange();
				WorldMapAnsys->Play( "Up" );
				GameData::GetInstance()->input_timer = 0.15f;
			}
		}
		else if ( ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Up ) || input->IsDPadPressed( 0, SGD::DPad::Up ) || input->GetLeftJoystick( 0 ).y < 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
			if ( unlockedTowns >= 1 && unlockedTowns != 4 )
			{
				SelectedTown = 1;
				GameData::GetInstance()->PlaySelectionChange();
				WorldMapAnsys->Play( "Left" );

				GameData::GetInstance()->input_timer = 0.15f;
			}
		}
		else if ( ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Left ) || input->IsDPadPressed( 0, SGD::DPad::Left ) || input->GetLeftJoystick( 0 ).x < 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
			if ( unlockedTowns >= 2 && unlockedTowns != 4 )
			{
				SelectedTown = 2;
				GameData::GetInstance()->PlaySelectionChange();
				WorldMapAnsys->Play( "Down" );

				GameData::GetInstance()->input_timer = 0.15f;
			}
		}
		else if ( ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Down ) || input->IsDPadPressed( 0, SGD::DPad::Down ) || input->GetLeftJoystick( 0 ).y > 0 ) && GameData::GetInstance()->input_timer < 0 )
		{
			if ( unlockedTowns >= 3 && unlockedTowns != 4 )
			{
				SelectedTown = 3;
				GameData::GetInstance()->PlaySelectionChange();
				WorldMapAnsys->Play( "Right" );

				GameData::GetInstance()->input_timer = 0.15f;
			}
		}
	}
	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) || input->IsButtonPressed( 0, 1 ) )
	{
		for ( unsigned int i = 0; i < traps.size(); i++ )
		{
			delete traps[ i ];
			traps[ i ] = nullptr;
		}
		traps.clear();
		for ( unsigned int i = 0; i < overworldObjects.size(); ++i )
		{
			if ( overworldObjects[ i ] != nullptr )
			{
				delete overworldObjects[ i ];
			}
		}
		overworldObjects.clear();
		for ( unsigned int i = 0; i < guards.size(); ++i )
		{
			if ( guards[ i ] != nullptr )
			{
				delete guards[ i ];
			}
		}
		guards.clear();

		switch ( SelectedTown )
		{
			PlaySoundEffect( 6 );
			case 0: //Wind
				Loading( "Loading Map" );
				TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\WindTown2.xml" );
				//here we load in the dialogue
				//	dialogue->Load( "Assets/Scripts/windywoods_enter.xml" );
				if ( CutscenesPlayed == 0 )
				{
					CutsceneManager::GetInstance()->Terminate();
					CutsceneManager::GetInstance()->Initialize( 5 );
					state = Cuts;
					CutsceneManager::GetInstance()->Play( 0 );
					CutscenesPlayed++;
				}
				else
				{
					state = Town;
				}

				//Load Audio
				if ( m_overAudio != SGD::INVALID_HANDLE )
				{
					SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
					m_overAudio = SGD::INVALID_HANDLE;
				}
				m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/ZeldaMetal.xwm" );
				break;
			case 1://Fire
				Loading( "Loading Map" );
				TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalFireTown.xml" );
				//here we load in the dialogue
				//dialogue->Load( "Assets/Scripts/magmafalls_enter.xml" );
				if ( CutscenesPlayed == 1 )
				{
					CutsceneManager::GetInstance()->Terminate();
					CutsceneManager::GetInstance()->Initialize( 6 );
					state = Cuts;
					CutsceneManager::GetInstance()->Play( 0 );
					CutscenesPlayed++;
				}
				else
				{
					state = Town;
				}
				if ( m_overAudio != SGD::INVALID_HANDLE )
				{
					SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
					m_overAudio = SGD::INVALID_HANDLE;
				}
				m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Fire.xwm" );
				break;
			case 2://Earth
				Loading( "Loading Map" );
				TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalEarthTown.xml" );
				//here we load in the dialogue
				//dialogue->Load( "Assets/Scripts/earthytown_enter.xml" );
				if ( CutscenesPlayed == 2 )
				{
					CutsceneManager::GetInstance()->Terminate();
					CutsceneManager::GetInstance()->Initialize( 7 );
					state = Cuts;
					CutsceneManager::GetInstance()->Play( 0 );
					CutscenesPlayed++;
				}
				else
				{
					state = Town;
				}
				if ( m_overAudio != SGD::INVALID_HANDLE )
				{
					SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
					m_overAudio = SGD::INVALID_HANDLE;
				}
				m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Earth.xwm" );
				break;
			case 3://Water
				Loading( "Loading Map" );
				TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalWaterTown.xml" );
				//here we load in the dialogue
				//dialogue->Load( "Assets/Scripts/newwatercity_enter.xml" );
				if ( CutscenesPlayed == 3 )
				{
					CutsceneManager::GetInstance()->Terminate();
					CutsceneManager::GetInstance()->Initialize( 8 );
					state = Cuts;
					CutsceneManager::GetInstance()->Play( 0 );
					CutscenesPlayed++;
				}
				else
				{
					state = Town;
				}
				if ( m_overAudio != SGD::INVALID_HANDLE )
				{
					SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
					m_overAudio = SGD::INVALID_HANDLE;
				}
				m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Water.xwm" );
				break;
			case 4://Final Town
				Loading( "Loading Map" );
				TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalLanding.xml" );
				//here we load in the dialogue
				//dialogue->Load( "Assets/Scripts/heroslanding_enter.xml" );
				if ( CutscenesPlayed == 4 )
				{
					CutsceneManager::GetInstance()->Terminate();
					CutsceneManager::GetInstance()->Initialize( 3 );
					state = Cuts;
					CutsceneManager::GetInstance()->Play( 0 );
					CutscenesPlayed++;
				}
				else
				{
					state = Town;
				}
				if ( m_overAudio != SGD::INVALID_HANDLE )
				{
					SGD::AudioManager::GetInstance()->UnloadAudio( m_overAudio );
					m_overAudio = SGD::INVALID_HANDLE;
				}
				m_overAudio = SGD::AudioManager::GetInstance()->LoadAudio( "../Trapped Rat/Assets/Sounds/Hero.xwm" );
				break;
			default:
				//TileSystem::GetInstance()->Initialize("Assets\\TileMaps\\TestTown.xml");
				break;
		}
		StopAllBackgroundMusic();
		SGD::AudioManager::GetInstance()->PlayAudio( m_overAudio );
		laststate = Town;
		GameData::GetInstance()->PlaySelectionChange();

	}
}

void GamePlayState::MapRender()
{
	if ( WorldMap != SGD::INVALID_HANDLE )
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture( WorldMap, { 0, 0 } );
	}
	int i = unlockedTowns + 1;
	for ( ; i < 5; i++ )
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture( PadLock, townpoints[ i ] );
	}
	if ( WorldMapAnsys != nullptr )
	{
		WorldMapAnsys->Render( townpoints[ SelectedTown ].x + 32, townpoints[ SelectedTown ].y + 32 );
	}
	//Render Nameof town
	switch ( SelectedTown )
	{
		case 0:
			GameData::GetInstance()->GetFont()->DrawString( "Windy Woods", 94, 14, { 0, 0, 0 }, 7.0f );
			GameData::GetInstance()->GetFont()->DrawString( "Windy Woods", 90, 10, { 50, 200, 50 }, 7.0f );
			break;
		case 1:
			GameData::GetInstance()->GetFont()->DrawString( "Magma Falls", 94, 14, { 0, 0, 0 }, 7.0f );
			GameData::GetInstance()->GetFont()->DrawString( "Magma Falls", 90, 10, { 200, 50, 50 }, 7.0f );
			break;
		case 2:
			GameData::GetInstance()->GetFont()->DrawString( "Earthy Town", 94, 14, { 0, 0, 0 }, 7.0f );
			GameData::GetInstance()->GetFont()->DrawString( "Earthy Town", 90, 10, { 153, 83, 32 }, 7.0f );
			break;
		case 3:
			GameData::GetInstance()->GetFont()->DrawString( "New Water City", 49, 14, { 0, 0, 0 }, 6.5f );
			GameData::GetInstance()->GetFont()->DrawString( "New Water City", 45, 10, { 50, 50, 200 }, 6.5f );
			break;
		case 4:
			GameData::GetInstance()->GetFont()->DrawString( "Hero's Landing", 94, 14, { 0, 0, 0 }, 6.5f );
			GameData::GetInstance()->GetFont()->DrawString( "Hero's Landing", 90, 10, { 50, 200, 50 }, 6.5f );
			break;
		default:
			break;
	}
}

void GamePlayState::AddItem( ItemType it )
{
	inventory.push_back( shopinv[ it ] );
	loot.push_back( it );

}

void GamePlayState::AddGold( int val )
{
	gold += val;
	loot_gold = val;
}

int GamePlayState::GetGold( void ) const
{
	return gold;
}

void GamePlayState::SummaryUpdate( float dt )
{
	if ( ignore_game_over )
	{
		ignore_game_over = false;
		//dialogue->Load( "Assets/Scripts/tutorialfinish.xml" );
		//state = Dia;
		//laststate = Map
		CutsceneManager::GetInstance()->Terminate();
		CutsceneManager::GetInstance()->Initialize( 2 );
		CutsceneManager::GetInstance()->Play( 0 );
		state = Cuts;
		laststate = Cuts;
		for ( unsigned int i = 0; i < m_vhuditems.size(); i++ )
		{
			delete m_vhuditems[ i ];
			m_vhuditems[ i ] = nullptr;
		}
		m_vhuditems.clear();
		for ( unsigned int i = 0; i < guards.size(); i++ )
		{
			delete guards[ i ];
		}
		guards.clear();
		return;
	}
	GameData::GetInstance()->SetCamera( { 0, 0 } );
	for ( size_t i = 0; i < Party.size(); i++ )
	{
		Party[ i ]->UpdateAnimation( dt );
	}

	//if (loot.size() == 0)
	//{
	//	loot.push_back(ItemType::SmallHP);
	//	loot.push_back(ItemType::SmallBP);
	//	loot_xp = 100;
	//	loot_gold = 5;
	//}
	if ( !SGD::AudioManager::GetInstance()->IsAudioPlaying( m_SummaryAudio ) )
	{
		StopAllBackgroundMusic();
		SGD::AudioManager::GetInstance()->PlayAudio( m_SummaryAudio );
	}

	if ( SGD::InputManager::GetInstance()->IsKeyPressed( SGD::Key::Enter ) || SGD::InputManager::GetInstance()->IsButtonPressed( 0, 1 ) )
	{
		PlaySoundEffect( 6 );
		RemoveTrap( tripped_trap );
		state = Town;
		loot.clear();
		loot_xp = 0;
		loot_gold = 0;
		for ( unsigned int i = 0; i < Party.size(); i++ )
		{
			if ( Party[ i ]->hasLeveled() )
				Party[ i ]->SethasLeveled( false );
		}
		abilityunlocked = false;
		if ( MinibossFight )
		{
			MinibossFight = false;
			if ( unlockedTowns == SelectedTown )
				unlockedTowns++;


			for ( unsigned int i = 0; i < guards.size(); i++ )
			{
				delete guards[ i ];
				guards[ i ] = nullptr;
			}
			guards.clear();


			switch ( SelectedTown )
			{
				case 0: //Wind
					//here we load in the dialogue
					dialogue->Load( "Assets/Scripts/windywoods_exit.xml" );
					break;
				case 1://Fire
					//here we load in the dialogue
					dialogue->Load( "Assets/Scripts/magmafalls_exit.xml" );
					break;
				case 2://Earth				
					//here we load in the dialogue
					dialogue->Load( "Assets/Scripts/earthytown_exit.xml" );
					break;
				case 3://Water
					//here we load in the dialogue
					dialogue->Load( "Assets/Scripts/newwatercity_exit.xml" );
					break;
				default:

					break;
			}


			TileSystem::GetInstance()->Exit();
			//here we load in the correct dialogue for which town we are exiting
			state = Dia;
			laststate = Map;
		}
		if ( FinalBossFight )
		{
			//Trigger Ending Cutscene or Dialouge
			//for now we will have to just do dialogue
			//			dialogue->Load( "../Trapped Rat/Assets/Scripts/cecildefeateddialogue.xml" );
			CutsceneManager::GetInstance()->Terminate();
			CutsceneManager::GetInstance()->Initialize( 4 );
			CutsceneManager::GetInstance()->Play( 0 );
			state = Cuts;
			laststate = Cuts;
			//FinalBossFight = false;
		}

		// Guard Code
		if ( guard_index != -1 && !runAway )
			// We killed a guard, guard_index is the index of the array
		{
			auto iter = guards.begin();
			while ( ( *iter ) != guards[ guard_index ] )
				iter++;

			delete ( *iter );
			guards.erase( iter );

			guard_index = -1;
		}

		for ( size_t i = 0; i < m_vhuditems.size(); i++ )
		{
			delete m_vhuditems[ i ];
		}
		m_vhuditems.clear();
	}

}

void GamePlayState::GoToTown()
{
	for ( unsigned int i = 0; i < guards.size(); i++ )
	{
		delete guards[ i ];
		guards[ i ] = nullptr;
	}
	guards.clear();

	for ( unsigned int i = 0; i < traps.size(); i++ )
	{
		delete traps[ i ];
		traps[ i ] = nullptr;
	}
	traps.clear();
	//TileSystem::GetInstance( )->Exit( );
	//here we load in the correct dialogue for which town we are exiting
	//state = Dia;
	laststate = Town;
	state = Map;
}
void GamePlayState::SummaryRender()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture( background, { 0, 0 } );
	std::vector<CombatPlayer*> tempparty;
	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		tempparty.push_back( Party[ i ] );
		tempparty[ i ]->SetPosition( { 150.0f + ( 100.0f * i ), 200.0f } );
		tempparty[ i ]->Render();
		if ( tempparty[ i ]->hasLeveled() && tempparty[ i ]->isAlive() )
		{
			SGD::Point pos = tempparty[ i ]->GetPosition();
			if ( i % 2 )
			{
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 98, pos.y - 38 - ( 30 * i ), { 0, 0, 0 }, 3.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 102, pos.y - 42 - ( 30 * i ), { 0, 0, 0 }, 3.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 100, pos.y - 40 - ( 30 * i ), { 150, 200, 100 }, 3.0f );
			}
			else
			{
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 98, pos.y - 38, { 0, 0, 0 }, 3.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 102, pos.y - 42, { 0, 0, 0 }, 3.0f );
				GameData::GetInstance()->GetFont()->DrawString( "Level Up", pos.x - 100, pos.y - 40, { 150, 200, 100 }, 3.0f );
			}


		}
		if ( abilityunlocked )
		{
			GameData::GetInstance()->GetFont()->DrawString( "New Abilities Learned", 198, 262, { 0, 0, 0 }, 3.0f );
			GameData::GetInstance()->GetFont()->DrawString( "New Abilities Learned", 202, 258, { 0, 0, 0 }, 3.0f );
			GameData::GetInstance()->GetFont()->DrawString( "New Abilities Learned", 200, 260, { 155, 0, 155 }, 3.0f );
		}
		SGD::Point characterOrderPosition;
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)( Party[ i ]->GetOrderPosition() * 100 + 150 );
		tempparty[ i ]->SetPosition( characterOrderPosition );
	}
	SGD::GraphicsManager::GetInstance()->DrawTexture( scroll, { 800, 310 }, SGD::PI / 2, { }, { }, { 1.0f, 1.4f } );
	std::string str = "Experience Gained:  " + std::to_string( loot_xp ) + "_Gold Gained:  " + std::to_string( loot_gold ) + "_Items Gained:  _  ";
	for ( size_t i = 0; i < loot.size(); i++ )
	{
		str += shopinv[ loot[ i ] ].GetName() + " _  ";
	}

	GameData::GetInstance()->GetFont()->DrawString( str, 250, 350, { 20, 0, 20 }, 2.0f );
	GameData::GetInstance()->GetFont()->DrawString( "Press Enter to Continue", 250, 300, { 50, 50, 50 }, 2.0f );
}

void GamePlayState::AddToParty( CombatPlayer*_player )
{
	Party.push_back( _player );
	state = Dia;
	laststate = Town;
	if ( _player->GetName() == "Biggs" )
	{
		dialogue->Load( "Assets/Scripts/ratsmeetsbiggs.xml" );
	}
	if ( _player->GetName() == "Jeeves" )
	{
		dialogue->Load( "Assets/Scripts/ratsmeetsjeeves.xml" );
	}
	if ( _player->GetName() == "Slippy" )
	{

		dialogue->Load( "Assets/Scripts/ratsmeetsslippy.xml" );
	}
	if ( _player->GetName() == "Checkers" )
	{
		dialogue->Load( "Assets/Scripts/ratsmeetscheckers.xml" );
	}

}

std::map<std::string, Ability*> GamePlayState::GetMasterList()
{
	return MasterAbilityList;
}

void GamePlayState::CreateFinalFight()
{

	//FinalBossFight = true;
	SGD::Point characterOrderPosition;
	std::vector<Enemy*> bosses;
	std::vector<Ability*> partyAbilities;

	Enemy* cecilFinal = nullptr;
	cecilFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Cecil.xml" );
	cecilFinal->SetOrderPosition( 1 );
	characterOrderPosition.x = 600.0f;
	characterOrderPosition.y = (float)( cecilFinal->GetOrderPosition() * 100 + 150 + 16 );
	cecilFinal->SetPosition( characterOrderPosition );
	partyAbilities.push_back( MasterAbilityList[ "Holy Shield" ] );
	partyAbilities.push_back( MasterAbilityList[ "Holy Mace" ] );
	partyAbilities.push_back( MasterAbilityList[ "Holy Flare" ] );
	partyAbilities.push_back( MasterAbilityList[ "Retribution" ] );
	partyAbilities.push_back( MasterAbilityList[ "Cover" ] );
	cecilFinal->InitializeAbilities( partyAbilities );
	partyAbilities.clear();

	Enemy* janeFinal = nullptr;
	janeFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final Jane.xml" );
	janeFinal->SetOrderPosition( 0 );
	janeFinal->SetLiving( false );
	//janeFinal->SetHP( 0 );
	characterOrderPosition.x = 1600.0f;
	characterOrderPosition.y = (float)( janeFinal->GetOrderPosition() * 100 + 150 + 16 );
	janeFinal->SetPosition( characterOrderPosition );
	partyAbilities.push_back( MasterAbilityList[ "Healing Light" ] );
	partyAbilities.push_back( MasterAbilityList[ "Dia" ] );
	partyAbilities.push_back( MasterAbilityList[ "Protect" ] );
	janeFinal->InitializeAbilities( partyAbilities );
	partyAbilities.clear();

	Enemy* johnFinal = nullptr;
	johnFinal = LoadEnemy( "../Trapped Rat/Assets/Scripts/Final John.xml" );
	johnFinal->SetOrderPosition( 2 );
	johnFinal->SetLiving( false );
	//johnFinal->SetHP( 0 );
	characterOrderPosition.x = 1600.0f;
	characterOrderPosition.y = (float)( johnFinal->GetOrderPosition() * 100 + 150 + 16 );
	johnFinal->SetPosition( characterOrderPosition );
	partyAbilities.push_back( MasterAbilityList[ "Sure Shot" ] );
	partyAbilities.push_back( MasterAbilityList[ "Barrage" ] );
	partyAbilities.push_back( MasterAbilityList[ "Haste" ] );
	johnFinal->InitializeAbilities( partyAbilities );
	partyAbilities.clear();

	bosses.push_back( janeFinal );
	bosses.push_back( cecilFinal );
	bosses.push_back( johnFinal );


	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
	}
	TurnManager::GetInstance()->Initialize( Party, bosses );
	for ( size_t i = 0; i < Party.size(); i++ )
	{
		if ( Party[ i ]->GetActive() )
		{
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
		}
	}

	for ( size_t i = 0; i < bosses.size(); i++ )
	{
		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
	}
	GameData::GetInstance()->SetIsInCombat( true );
	state = Combat;
	laststate = Combat;


	state = Dia;
	laststate = Combat;
	dialogue->Load( "Assets/Scripts/bossdialogue.xml" );
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
	BossAnimation();

	switch ( SelectedTown )  //NOTE FOR JOE****Make sure to increment "unlockedTowns" after you win the fight against these Minibosses
	{
		case 0: //Wind <---
		{
					Enemy* BWR1 = nullptr;
					BWR1 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Benevolent Wind Rogue.xml" );
					BWR1->SetOrderPosition( 1 );
					characterOrderPosition.x = 600.0f;
					characterOrderPosition.y = (float)( BWR1->GetOrderPosition() * 100 + 150 + 16 );
					BWR1->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Wind Split" ] );
					BWR1->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					Enemy* BWR2 = nullptr;
					BWR2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Benevolent Wind Rogue.xml" );
					BWR2->SetOrderPosition( 0 );
					BWR2->SetLiving( false );
					//BWR2->SetHP( 0 );
					characterOrderPosition.x = 1600.0f;
					characterOrderPosition.y = (float)( BWR2->GetOrderPosition() * 100 + 150 + 16 );
					BWR2->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Wind Split" ] );
					BWR2->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					bosses.push_back( BWR1 );
					bosses.push_back( BWR2 );

					for ( unsigned int i = 0; i < Party.size(); i++ )
					{
						Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
					}
					TurnManager::GetInstance()->Initialize( Party, bosses );
					for ( size_t i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetActive() )
						{
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
						}
					}

					for ( size_t i = 0; i < bosses.size(); i++ )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
					}
					GameData::GetInstance()->SetIsInCombat( true );
					state = Combat;
					laststate = Combat;

					state = Dia;
					laststate = Combat;
					dialogue->Load( "Assets/Scripts/ratsfightswind.xml" );
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
					partyAbilities.push_back( MasterAbilityList[ "Flame Spout" ] );
					FFW1->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					Enemy* FFW2 = nullptr;
					FFW2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Ferocious Fire Warrior.xml" );
					FFW2->SetOrderPosition( 0 );
					FFW2->SetLiving( false );
					FFW2->SetHP( 0 );
					characterOrderPosition.x = 1600.0f;
					characterOrderPosition.y = (float)( FFW2->GetOrderPosition() * 100 + 150 + 16 );
					FFW2->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Flame Spout" ] );
					FFW2->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					bosses.push_back( FFW1 );
					bosses.push_back( FFW2 );

					for ( unsigned int i = 0; i < Party.size(); i++ )
					{
						Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
					}
					TurnManager::GetInstance()->Initialize( Party, bosses );
					for ( size_t i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetActive() )
						{
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
						}
					}

					for ( size_t i = 0; i < bosses.size(); i++ )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
					}
					GameData::GetInstance()->SetIsInCombat( true );
					state = Combat;
					laststate = Combat;

					state = Dia;
					laststate = Combat;
					dialogue->Load( "Assets/Scripts/ratsfightsfire.xml" );
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
					partyAbilities.push_back( MasterAbilityList[ "Earthen Shield" ] );
					partyAbilities.push_back( MasterAbilityList[ "Earthen Mace" ] );
					SEM1->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					Enemy* SEM2 = nullptr;
					SEM2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Sturdy Earth Monk.xml" );
					SEM2->SetOrderPosition( 0 );
					SEM2->SetLiving( false );
					SEM2->SetHP( 0 );
					characterOrderPosition.x = 1600.0f;
					characterOrderPosition.y = (float)( SEM2->GetOrderPosition() * 100 + 150 + 16 );
					SEM2->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Earthen Shield" ] );
					partyAbilities.push_back( MasterAbilityList[ "Earthen Ward" ] );
					SEM2->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					bosses.push_back( SEM1 );
					bosses.push_back( SEM2 );

					for ( unsigned int i = 0; i < Party.size(); i++ )
					{
						Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
					}
					TurnManager::GetInstance()->Initialize( Party, bosses );
					for ( size_t i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetActive() )
						{
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
						}
					}

					for ( size_t i = 0; i < bosses.size(); i++ )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
					}
					GameData::GetInstance()->SetIsInCombat( true );
					state = Combat;
					laststate = Combat;

					state = Dia;
					laststate = Combat;
					dialogue->Load( "Assets/Scripts/ratsfightsearth.xml" );
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
					partyAbilities.push_back( MasterAbilityList[ "Water Wall" ] );
					WWW1->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					Enemy* WWW2 = nullptr;
					WWW2 = LoadEnemy( "../Trapped Rat/Assets/Scripts/Wise Water Wizard.xml" );
					WWW2->SetOrderPosition( 0 );
					WWW2->SetLiving( false );
					WWW2->SetHP( 0 );
					characterOrderPosition.x = 1600.0f;
					characterOrderPosition.y = (float)( WWW2->GetOrderPosition() * 100 + 150 + 16 );
					WWW2->SetPosition( characterOrderPosition );
					partyAbilities.push_back( MasterAbilityList[ "Water Wall" ] );
					WWW2->InitializeAbilities( partyAbilities );
					partyAbilities.clear();

					bosses.push_back( WWW1 );
					bosses.push_back( WWW2 );

					for ( unsigned int i = 0; i < Party.size(); i++ )
					{
						Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
						Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
					}
					TurnManager::GetInstance()->Initialize( Party, bosses );
					for ( size_t i = 0; i < Party.size(); i++ )
					{
						if ( Party[ i ]->GetActive() )
						{
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
							m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
						}
					}

					for ( size_t i = 0; i < bosses.size(); i++ )
					{
						m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), bosses[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
					}
					GameData::GetInstance()->SetIsInCombat( true );
					state = Combat;
					laststate = Combat;

					state = Dia;
					laststate = Combat;
					dialogue->Load( "Assets/Scripts/ratsfightswater.xml" );
					break;
		}
		case 4://Cecils HomeTown **Should Not Be Used because no miniboss in that town
			break;
	}

}

void GamePlayState::HoldOntoAbility( Ability* used )
{
	AbilityUsed = true;
	CurrentAbilityUsed = used;
	abilityTimer = 2.0f;
	CurrentAbilityUsed->GetAnimate()->ResetAll();
	CurrentAbilityUsed->GetAnimate()->Play( 0 );
}

void GamePlayState::GuardAnimation()
{
	SGD::AudioManager::GetInstance()->PlayAudio( entercombat );
	for ( int i = 0; i < 50; i++ )
	{
		Sleep( 1 );
		TownRender();
		int x = rand() % 100;
		int y = rand() % 100;
		SGD::Rectangle rect;
		rect.left = (float)x * 8;
		rect.top = (float)y * 6;
		rect.right = rect.left + 40;
		rect.bottom = rect.top + 40;
		SGD::GraphicsManager::GetInstance()->DrawRectangle( rect, { 0, 0, 0 } );
		SGD::GraphicsManager::GetInstance()->Update();
	}
}
void GamePlayState::BossAnimation()
{
	SGD::AudioManager::GetInstance()->PlayAudio( entercombat );
	for ( int i = 0; i < 8; i++ )
	{
		Sleep( 100 );
		if ( i % 2 == 0 )
			TownRender();
		else
			SGD::GraphicsManager::GetInstance()->DrawRectangle( { 0, 0, 800, 600 }, { 0, 0, 0 } );

		SGD::GraphicsManager::GetInstance()->Update();
	}
}
void GamePlayState::PlaySoundEffect( int _index )
{
	if ( _index < (int)m_vsoundeffects.size() )
	{
		SGD::AudioManager::GetInstance()->PlayAudio( m_vsoundeffects[ _index ] );
	}
}
void GamePlayState::LoadGame( int index )
{

	//this just takes in the relative path
	saveindex = index;
	TCHAR path[ MAX_PATH ];
	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path ) ) )
	{
		PathAppend( path, TEXT( "basicinfo.txt" ) );
	}
	std::ifstream sfin;
	sfin.open( path );
	if ( sfin.is_open() )
	{
		for ( int i = 0; i < 3; i++ )
		{
			data temp;
			sfin >> temp.playtime >> temp.town >> temp.partysize >> temp.gold;
			files[ i ] = temp;
			sfin.ignore( INT_MAX, '\n' );
		}

	}
	playtime = files[ saveindex ].playtime;
	std::ifstream fin;
	saveindex = index;
	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path ) ) )
	{
		if ( saveindex == 0 )
			PathAppend( path, TEXT( "savegame0.txt" ) );
		else if ( saveindex == 1 )
			PathAppend( path, TEXT( "savegame1.txt" ) );
		else if ( saveindex == 2 )
			PathAppend( path, TEXT( "savegame2.txt" ) );
		else
			PathAppend( path, TEXT( "lolnothing" ) );

	}
	fin.open( path );
	if ( fin.is_open() )
	{
		fin >> SelectedTown;
		fin.ignore( INT_MAX, '\n' );
		fin >> unlockedTowns;
		fin.ignore( INT_MAX, '\n' );
		float x;
		float y;
		fin >> x;
		fin.ignore( INT_MAX, '\n' );
		fin >> y;
		fin.ignore( INT_MAX, '\n' );
		GameData::GetInstance()->GetOverworldPlayer()->SetPosition( { x, y } );
		int count;
		fin >> count;
		fin.ignore( INT_MAX, '\n' );
		inventory.clear();
		for ( int i = 0; i < count; i++ )//this is just for items
		{
			std::string temp;
			char* string = new char[ 32 ];
			fin.getline( string, 32, '\n' );
			temp = string;
			delete[ ] string;
			//fin.ignore( INT_MAX, '\n' );
			for ( unsigned int j = 0; j < shopinv.size(); j++ )
			{
				if ( temp == shopinv[ j ].GetName() )
				{
					inventory.push_back( shopinv[ j ] );
					break;
				}
			}
		}
		fin >> gold;
		fin.ignore( INT_MAX, '\n' );
		fin >> count;
		for ( unsigned int i = 0; i < Party.size(); i++ )
		{
			delete Party[ i ];
		}
		Party.clear();
		for ( int i = 0; i < count; i++ )//party stuff
		{
			std::string temp;
			int lvl;
			int bp;
			int hp;
			fin >> temp;
			fin.ignore( INT_MAX, '\n' );
			fin >> lvl;
			fin.ignore( INT_MAX, '\n' );
			fin >> hp;
			fin.ignore( INT_MAX, '\n' );
			fin >> bp;
			fin.ignore( INT_MAX, '\n' );

			SGD::Point characterOrderPosition;
			std::vector<Ability*> partyAbilities;

			if ( temp == "Ratsputin" )
			{
				CombatPlayer* p1 = nullptr;
				p1 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Ratsputin.xml" ) );
				p1->SetOrderPosition( Party.size() );
				characterOrderPosition.x = 100.0f;
				characterOrderPosition.y = (float)( p1->GetOrderPosition() * 100 + 150 );
				p1->SetPosition( characterOrderPosition );
				p1->SetSize( { 64, 64 } );
				partyAbilities.push_back( MasterAbilityList[ "Burrow" ] );
				partyAbilities.push_back( MasterAbilityList[ "Water Fang" ] );
				partyAbilities.push_back( MasterAbilityList[ "Slow Claw" ] );
				partyAbilities.push_back( MasterAbilityList[ "Earth Fang" ] );
				partyAbilities.push_back( MasterAbilityList[ "Poison Fang" ] );
				partyAbilities.push_back( MasterAbilityList[ "Fire Fang" ] );
				partyAbilities.push_back( MasterAbilityList[ "Counter Claw" ] );
				partyAbilities.push_back( MasterAbilityList[ "Wind Fang" ] );
				p1->SetActive( true );
				p1->InitializeAbilities( partyAbilities );
				p1->SetLevel( lvl );
				p1->SetBP( bp );
				p1->SetHP( hp );
				Party.push_back( p1 );
			}
			if ( temp == "Biggs" )
			{
				GameData::GetInstance()->GetOverworldPlayer()->UnregisterFromEvent( "GainBiggs" );
				CombatPlayer* p4 = nullptr;
				p4 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Biggs.xml" ) );
				p4->SetOrderPosition( Party.size() );
				characterOrderPosition.x = 100.0f;
				characterOrderPosition.y = (float)( p4->GetOrderPosition() * 100 + 150 );
				p4->SetPosition( characterOrderPosition );
				p4->SetSize( { 64, 64 } );
				partyAbilities.push_back( MasterAbilityList[ "Leaf on the Wind" ] );
				partyAbilities.push_back( MasterAbilityList[ "Zephyr" ] );
				partyAbilities.push_back( MasterAbilityList[ "Wind Vale" ] );
				partyAbilities.push_back( MasterAbilityList[ "Second Wind" ] );
				partyAbilities.push_back( MasterAbilityList[ "Tailwind" ] );
				partyAbilities.push_back( MasterAbilityList[ "Tornado" ] );
				partyAbilities.push_back( MasterAbilityList[ "Whispering Wind" ] );
				partyAbilities.push_back( MasterAbilityList[ "Tempest" ] );
				p4->InitializeAbilities( partyAbilities );
				if ( Party.size() >= 3 )
					p4->SetActive( false );
				else
					p4->SetActive( true );
				p4->SetLevel( lvl );
				p4->SetBP( bp );
				p4->SetHP( hp );
				Party.push_back( p4 );
			}
			if ( temp == "Jeeves" )
			{
				GameData::GetInstance()->GetOverworldPlayer()->UnregisterFromEvent( "GainJeeves" );
				CombatPlayer* p5 = nullptr;
				p5 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/testcharacterJeeves.xml" ) );
				p5->SetOrderPosition( Party.size() );
				characterOrderPosition.x = 100.0f;
				characterOrderPosition.y = (float)( p5->GetOrderPosition() * 100 + 150 );
				p5->SetPosition( characterOrderPosition );
				p5->SetSize( { 64, 64 } );
				partyAbilities.push_back( MasterAbilityList[ "Collapse" ] );
				partyAbilities.push_back( MasterAbilityList[ "Ignite" ] );
				partyAbilities.push_back( MasterAbilityList[ "Scorch" ] );
				partyAbilities.push_back( MasterAbilityList[ "Rib-a-Rang" ] );
				partyAbilities.push_back( MasterAbilityList[ "Emblazon" ] );
				partyAbilities.push_back( MasterAbilityList[ "Firefall" ] );
				partyAbilities.push_back( MasterAbilityList[ "Fire Spikes" ] );
				partyAbilities.push_back( MasterAbilityList[ "Incinerate" ] );
				p5->InitializeAbilities( partyAbilities );
				if ( Party.size() >= 3 )
					p5->SetActive( false );
				else
					p5->SetActive( true );
				p5->SetLevel( lvl );
				p5->SetBP( bp );
				p5->SetHP( hp );
				Party.push_back( p5 );
			}
			if ( temp == "Checkers" )
			{
				GameData::GetInstance()->GetOverworldPlayer()->UnregisterFromEvent( "GainCheckers" );
				CombatPlayer* p3 = nullptr;
				p3 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Checkers.xml" ) );
				p3->SetOrderPosition( Party.size() );
				characterOrderPosition.x = 100.0f;
				characterOrderPosition.y = (float)( p3->GetOrderPosition() * 100 + 150 );
				p3->SetPosition( characterOrderPosition );
				p3->SetSize( { 64, 64 } );
				partyAbilities.push_back( MasterAbilityList[ "Hedge Guard" ] );
				partyAbilities.push_back( MasterAbilityList[ "Rock Spike" ] );
				partyAbilities.push_back( MasterAbilityList[ "Rampart" ] );
				partyAbilities.push_back( MasterAbilityList[ "Tremor" ] );
				partyAbilities.push_back( MasterAbilityList[ "Cover" ] );
				partyAbilities.push_back( MasterAbilityList[ "Geo Crush" ] );
				partyAbilities.push_back( MasterAbilityList[ "Pinch" ] );
				partyAbilities.push_back( MasterAbilityList[ "Quake" ] );
				if ( Party.size() >= 3 )
					p3->SetActive( false );
				else
					p3->SetActive( true );
				p3->SetLevel( lvl );
				p3->SetBP( bp );
				p3->SetHP( hp );
				p3->InitializeAbilities( partyAbilities );
				Party.push_back( p3 );
			}
			if ( temp == "Slippy" )
			{
				GameData::GetInstance()->GetOverworldPlayer()->UnregisterFromEvent( "GainSlippy" );
				CombatPlayer* p2 = nullptr;
				p2 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/Slippy.xml" ) );
				p2->SetOrderPosition( Party.size() );
				characterOrderPosition.x = 100.0f;
				characterOrderPosition.y = (float)( p2->GetOrderPosition() * 100 + 150 );
				p2->SetPosition( characterOrderPosition );
				p2->SetSize( { 64, 64 } );
				partyAbilities.push_back( MasterAbilityList[ "Puddle" ] );
				partyAbilities.push_back( MasterAbilityList[ "Squirt" ] );
				partyAbilities.push_back( MasterAbilityList[ "Dissolve" ] );
				partyAbilities.push_back( MasterAbilityList[ "Splash" ] );
				partyAbilities.push_back( MasterAbilityList[ "Acid Rain" ] );
				partyAbilities.push_back( MasterAbilityList[ "Whirlpool" ] );
				partyAbilities.push_back( MasterAbilityList[ "Torrent" ] );
				partyAbilities.push_back( MasterAbilityList[ "Flood" ] );
				if ( Party.size() >= 3 )
					p2->SetActive( false );
				else
					p2->SetActive( true );
				p2->SetLevel( lvl );
				p2->SetBP( bp );
				p2->SetHP( hp );
				p2->InitializeAbilities( partyAbilities );
				Party.push_back( p2 );
			}


		}
		fin >> ternary_gauge;
		fin.ignore(INT_MAX, '\n');
		fin >> CutscenesPlayed;
		CheckAbilityUnlocked( false );
		fin.close();
	}
	else
	{
		is_tutorial = true;
		if ( is_tutorial )
		{
			TileSystem::GetInstance()->Initialize( "Assets\\TileMaps\\FinalLanding.xml" );
			CutsceneManager::GetInstance()->Terminate();
			CutsceneManager::GetInstance()->Initialize( 1 );
			CutsceneManager::GetInstance()->Play( 0 );
			state = Cuts;
			laststate = Cuts;
		}
		TutorialStart();
	}

}
void GamePlayState::SaveGame()
{

	TCHAR path[ MAX_PATH ];
	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path ) ) )
	{
		PathAppend( path, TEXT( "basicinfo.txt" ) );
	}
	std::ifstream fin;
	std::vector<data> datas;

	fin.open( path );
	if ( fin.is_open() )
	{
		for ( int i = 0; i < 3; i++ )
		{
			data temp;
			fin >> temp.playtime >> temp.town >> temp.partysize >> temp.gold;
			files[ i ] = temp;
			fin.ignore( INT_MAX, '\n' );
		}
		fin.close();
	}
	files[ saveindex ].playtime = playtime;
	files[ saveindex ].town = SelectedTown;
	files[ saveindex ].partysize = Party.size();
	files[ saveindex ].gold = gold;


	std::ofstream out;
	out.open( path, std::ios_base::trunc );
	if ( out.is_open() )
	{
		if ( out.is_open() )
		{
			for ( int i = 0; i < 3; i++ )
			{
				out << files[ i ].playtime << ' ' << files[ i ].town << ' ' << files[ i ].partysize << ' ' << files[ i ].gold << "\n";
			}
			out.close();
		}
	}


	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path ) ) )
	{
		if ( saveindex == 0 )
			PathAppend( path, TEXT( "savegame0.txt" ) );
		if ( saveindex == 1 )
			PathAppend( path, TEXT( "savegame1.txt" ) );
		if ( saveindex == 2 )
			PathAppend( path, TEXT( "savegame2.txt" ) );
	}
	std::ofstream fout;
	fout.open( path );
	if ( fout.is_open() )
	{
		//here we save
		//first lets do what town we are in
		fout << SelectedTown << "\n";
		//now we write in what towns we have unlocked
		fout << unlockedTowns << "\n";
		//overworldinfo
		fout << GameData::GetInstance()->GetOverworldPlayer()->GetPosition().x << "\n";
		fout << GameData::GetInstance()->GetOverworldPlayer()->GetPosition().y << "\n";
		fout << inventory.size() << "\n";
		for ( unsigned int i = 0; i < inventory.size(); i++ )
		{
			fout << inventory[ i ].GetName() << "\n";//we only need the name
			//when we load it in we will copy the item with the same name from the shopinv

		}
		fout << gold << "\n";//how much gold we got?
		fout << Party.size() << "\n";
		for ( unsigned int i = 0; i < Party.size(); i++ )
		{
			fout << Party[ i ]->GetName() << "\n";
			fout << Party[ i ]->GetLevel() << "\n";
			fout << Party[ i ]->GetHP() << "\n";
			fout << Party[ i ]->GetBP() << "\n";
		}
		fout << ternary_gauge << "\n";//i think this is all we need for ternary stuff
		fout << CutscenesPlayed << "\n";
		fout.close();
	}
}

void GamePlayState::TutorialStart()
{
	if ( is_tutorial )
	{
		SelectedTown = 0;
		unlockedTowns = 0;
		std::vector<Ability*> partyAbilities;
		SGD::Point characterOrderPosition;
		std::vector<Enemy*> tempEnemy;
		std::vector<CombatPlayer*> tempParty;
		CombatPlayer* p6 = nullptr;
		p6 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/mom.xml" ) );
		p6->SetOrderPosition( 0 );
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)( p6->GetOrderPosition() * 100 + 150 );
		p6->SetPosition( characterOrderPosition );
		p6->SetSize( { 64, 64 } );
		partyAbilities.push_back( MasterAbilityList[ "Burrow" ] );
		partyAbilities.push_back( MasterAbilityList[ "Water Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Slow Claw" ] );
		partyAbilities.push_back( MasterAbilityList[ "Earth Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Poison Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Fire Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Counter Claw" ] );
		partyAbilities.push_back( MasterAbilityList[ "Wind Fang" ] );
		p6->InitializeAbilities( partyAbilities );
		p6->SetActive( true );
		Parents.push_back( p6 );

		partyAbilities.clear();

		CombatPlayer* p7 = nullptr;
		p7 = ( LoadCombatPlayer( "../Trapped Rat/Assets/Scripts/dad.xml" ) );
		p7->SetOrderPosition( 1 );
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)( p7->GetOrderPosition() * 100 + 150 );
		p7->SetPosition( characterOrderPosition );
		p7->SetSize( { 64, 64 } );
		partyAbilities.push_back( MasterAbilityList[ "Burrow" ] );
		partyAbilities.push_back( MasterAbilityList[ "Water Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Slow Claw" ] );
		partyAbilities.push_back( MasterAbilityList[ "Earth Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Poison Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Fire Fang" ] );
		partyAbilities.push_back( MasterAbilityList[ "Counter Claw" ] );
		partyAbilities.push_back( MasterAbilityList[ "Wind Fang" ] );
		p7->InitializeAbilities( partyAbilities );
		p7->SetActive( true );
		Parents.push_back( p7 );

		Enemy* cecil = nullptr;
		cecil = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cecil.xml" );
		cecil->SetOrderPosition( 0 );
		characterOrderPosition.x = 600.0f;
		characterOrderPosition.y = (float)( cecil->GetOrderPosition() * 100 + 150 + 16 );
		cecil->SetPosition( characterOrderPosition );
		tutorialenemy.push_back( cecil );

		for ( unsigned int i = 0; i < Parents.size(); i++ )
		{
			Parents[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Parents[ i ] );
			Parents[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Parents[ i ] );
		}
		TurnManager::GetInstance()->Initialize( Parents, tutorialenemy );
		for ( size_t i = 0; i < Parents.size(); i++ )
		{
			if ( Parents[ i ]->GetActive() )
			{
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Parents[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Parents[ i ]->GetOrderPosition()*50.0f ) ), Parents[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
				m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Parents[ i ]->GetOrderPosition()*50.0f ) ), Parents[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
			}
		}


		for ( size_t i = 0; i < tutorialenemy.size(); i++ )
		{
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), tutorialenemy[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
		}
		state = Combat;
		laststate = Combat;
		GameData::GetInstance()->SetIsInCombat( true );

		inventory.push_back( shopinv[ 0 ] );
	}
}


void GamePlayState::AddToHUDItems( HUDItem* item )
{
	m_vhuditems.push_back( item );
}

void GamePlayState::CreateOverworldObject( std::string object, std::string animation, SGD::Point destination, bool allied )
{
	WorldObject* tempObj = new WorldObject( object, animation, allied );

	tempObj->SetPosition( destination );

	overworldObjects.push_back( tempObj );
}
void GamePlayState::StopAllBackgroundMusic()
{
	if ( m_Audio != SGD::INVALID_HANDLE )
	{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Audio ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_Audio );
	}
	if ( m_Credits != SGD::INVALID_HANDLE )
	{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Credits ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_Credits );
	}
	if ( m_Map != SGD::INVALID_HANDLE )
	{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_Map ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_Map );
	}
	if ( m_overAudio != SGD::INVALID_HANDLE )
	{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_overAudio ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_overAudio );
	}
	if ( m_SummaryAudio != SGD::INVALID_HANDLE )
	{
		if ( SGD::AudioManager::GetInstance()->IsAudioPlaying( m_SummaryAudio ) )
			SGD::AudioManager::GetInstance()->StopAudio( m_SummaryAudio );
	}
	//ADD Any additional background audio here
}

bool GamePlayState::CheckIfExist( std::string _name )
{
	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		if ( Party[ i ]->GetName() == _name )
			return true;
	}
	return false;
}
void GamePlayState::PhaseSwitchDia( int _index )
{
	switch ( _index )
	{
		case 1:
			break;
		case 2:
			dialogue->Load( "Assets/Scripts/phase1-2.xml" );
			state = Dia;
			laststate = Combat;
			break;
		case 3:
			dialogue->Load( "Assets/Scripts/phase2-3.xml" );
			state = Dia;
			laststate = Combat;
			break;

	}
}
void GamePlayState::PopulateUniqueNames()
{
	uniquenames.clear();
	uniquecounts.clear();
	for ( unsigned int i = 0; i < inventory.size(); i++ )
	{
		int count = 0;
		for ( unsigned int j = 0; j < inventory.size(); j++ )
		{
			if ( inventory[ i ] == inventory[ j ] )
				count++;
		}
		bool is = true;
		for ( unsigned int j = 0; j < uniquenames.size(); j++ )
		{

			if ( inventory[ i ].GetName() == uniquenames[ j ] )
				is = false;
		}
		if ( is )
		{
			uniquecounts.push_back( count );
			uniquenames.push_back( inventory[ i ].GetName() );
		}
	}
}
void GamePlayState::CreateEnemyFight( int index )
{
	Enemy* tempRandomEnemy;
	std::vector<Ability*> partyAbilities;
	state = GPStates::Combat;
	std::vector<Enemy*> tempEnemy;
	SGD::Point characterOrderPosition;

	switch ( index )
	{
		case 0:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Dog.xml" );
			tempRandomEnemy->dogTarget = 0;
			partyAbilities.push_back( MasterAbilityList[ "Feral Bite" ] );
			break;
		case 1:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Cat.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Scratch" ] );
			break;
		case 2:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Raven.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Feathered Flurry" ] );
			break;
		case 3:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Chef.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Fresh Meat" ] );
			break;
		case 4:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Blacksmith.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Bellows Blast" ] );
			break;
		case 5:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Shopkeeper.xml" );
			tempRandomEnemy->lastAttacker = -1;
			partyAbilities.push_back( MasterAbilityList[ "Elemental Poison" ] );
			break;
		case 6:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Tailor.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Stitch in Time" ] );
			break;
		case 7:
			tempRandomEnemy = LoadEnemy( "../Trapped Rat/Assets/Scripts/Priestess.xml" );
			partyAbilities.push_back( MasterAbilityList[ "Healing Light" ] );
			break;
		default:
			break;
	}
	int templevel = Party[ 0 ]->GetLevel();
	templevel -= 2;
	if ( templevel <= 0 ) templevel = 1;
	templevel += ( rand() % 5 );
	tempRandomEnemy->SetLevel( templevel );
	tempRandomEnemy->SetXPVal( 50 + ( ( templevel - Party[ 0 ]->GetLevel() ) * 5 ) );
	tempRandomEnemy->SetHP( tempRandomEnemy->GetMaxHP() );

	tempRandomEnemy->SetOrderPosition( 0 );
	characterOrderPosition.x = 600.0f;
	characterOrderPosition.y = (float)( tempRandomEnemy->GetOrderPosition() * 100 + 150 + 16 );
	tempRandomEnemy->SetPosition( characterOrderPosition );
	tempRandomEnemy->InitializeAbilities( partyAbilities );
	partyAbilities.clear();
	tempEnemy.push_back( tempRandomEnemy );

	for ( unsigned int i = 0; i < Party.size(); i++ )
	{
		Party[ i ]->GetAbility( 0 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 1 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 2 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 3 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 4 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 5 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 6 )->CalcluateBpScaledCost( Party[ i ] );
		Party[ i ]->GetAbility( 7 )->CalcluateBpScaledCost( Party[ i ] );
	}

	TurnManager::GetInstance()->Initialize( Party, tempEnemy );
	for ( size_t i = 0; i < Party.size(); i++ )
	{
		if ( Party[ i ]->GetActive() )
		{
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -25 ) ) );
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 440 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( 0, 0 ) ) );
			m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point( 630, 465 + ( Party[ i ]->GetOrderPosition()*50.0f ) ), Party[ i ], SGD::Color( 0, 100, 255 ), SGD::Point( 0, 0 ), false ) );
		}
	}


	for ( size_t i = 0; i < tempEnemy.size(); i++ )
	{
		m_vhuditems.push_back( CreateBar( { 64, 16 }, SGD::Point(), tempEnemy[ i ], SGD::Color( 0, 255, 0 ), SGD::Point( -30, -45 ) ) );
	}
	GameData::GetInstance()->SetIsInCombat( true );
	laststate = Combat;
}