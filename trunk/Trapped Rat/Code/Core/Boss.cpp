
#include "Boss.h"
#include "OwnedHUD.h"
#include "../tinyxml/tinyxml.h"
#include "../States/GamePlayState.h"

Boss::Boss( const char* _name )
	{
	if ( _name == "Cecil" )
		{
		CreateCecil( "../Trapped Rat/Assets/Scripts/Final Cecil.xml" );
		}
	else if ( _name == "Jane" )
		{
		CreateJane( "../Trapped Rat/Assets/Scripts/Final Jane.xml" );
		}
	else if ( _name == "John" )
		{
		CreateJohn( "../Trapped Rat/Assets/Scripts/Final John.xml" );
		}
	}


Boss::~Boss()
	{
	}
void Boss::Update( float dt )
	{
	Enemy::Update( dt );
	}
void Boss::Render()
	{
	Enemy::Render();
	}
void Boss::BehaviorAI()
	{

	}
void Boss::Cecil()
	{

	}
void Boss::Jane()
	{

	}
void Boss::John()
	{

	}

void Boss::CreateCecil( const char* path )
	{
	TiXmlDocument doc;
	if ( doc.LoadFile( path ) )
		{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string loadname = root->FirstChildElement( "Name" )->GetText();
		std::string loadelement = root->FirstChildElement( "Element" )->GetText();
		std::string loadtype = root->FirstChildElement( "Type" )->GetText();
		std::string loadrandom = root->FirstChildElement( "IsRandom" )->GetText();
		//load in the Stats
		Stats loadstats;
		int loadHP;
		int loadBP;
		int loadlevel;
		float loadspeed;
		TiXmlElement * Stat = root->FirstChildElement( "Stats" );
		loadHP = std::stoi( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.hp_scale = std::stof( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Scale" )->GetText() );

		loadBP = std::stoi( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.bp_scale = std::stof( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.attack = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.attack_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.magic = std::stoi( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Base" )->GetText() );
		loadstats.magic_scale = std::stof( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.defense = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.defense_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadspeed = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Base" )->GetText() );
		loadstats.speed_scale = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.avoision = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.avoision_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadlevel = std::stoi( root->FirstChildElement( "Level" )->GetText() );

		std::string loadhurt = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();
		std::string loadattack = root->FirstChildElement( "Sound" )->FirstChildElement( "Attack" )->GetText();
		std::string loaddeath = root->FirstChildElement( "Sound" )->FirstChildElement( "Death" )->GetText();

		std::string loadanimation = root->FirstChildElement( "Animation" )->GetText();
		if ( loadelement == "Wind" )
			{
			SetEtype( ETYPE::WIND );
			}
		else if ( loadelement == "Fire" )
			{
			SetEtype( ETYPE::FIRE );
			}
		else if ( loadelement == "Earth" )
			{
			SetEtype( ETYPE::EARTH );
			}
		else if ( loadelement == "Water" )
			{
			SetEtype( ETYPE::WATER );
			}
		else
			{
			SetEtype( ETYPE::MULTI );
			}

		
		SetName( loadname );
		GetStats().attack = loadstats.attack;
		GetStats().avoision = loadstats.avoision;
		GetStats().defense = loadstats.defense;
		GetStats().magic = loadstats.magic;
		SetLevel( loadlevel );
		SetHP( loadHP );
		SetMaxHP( loadHP );
		SetSpeed( loadspeed );
		SetProgress( 0.0f );
		//ability addition here
		SetOrderPosition( 1 );
		SGD::Point characterOrderPosition;
		characterOrderPosition.x = 600.0f;
		characterOrderPosition.y = (float)( GetOrderPosition() * 100 + 150 + 16 );
		SetPosition( characterOrderPosition );
		SGD::Size toonSize;
		toonSize.width = 64.0f;
		toonSize.height = 64.0f;
		SetSize( toonSize );
		OwnedHUD* turnindicator = new OwnedHUD;
		turnindicator->SetActive( true );
		turnindicator->SetImage( GamePlayState::GetInstance()->getTurnInd() );
		turnindicator->SetOwner( this );
		turnindicator->SetOffset( SGD::Point( -10, -64 ) );
		SetTurnIndicator( turnindicator );
		AnimationSystem* tempanimsys = new AnimationSystem;
		if ( loadanimation.size() )
			{
			tempanimsys->Load( loadanimation );
			tempanimsys->Play( 0 );
			}
		else
			tempanimsys = nullptr;

		SetAnimations( tempanimsys );

		if ( GetName() == "Cecil" )
			{
			SetTimelineAnimation( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/DogTimeline.png" ) );
			}
		}
	}
void Boss::CreateJohn( const char* path )
	{
	TiXmlDocument doc;
	if ( doc.LoadFile( path ) )
		{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string loadname = root->FirstChildElement( "Name" )->GetText();
		std::string loadelement = root->FirstChildElement( "Element" )->GetText();
		std::string loadtype = root->FirstChildElement( "Type" )->GetText();
		std::string loadrandom = root->FirstChildElement( "IsRandom" )->GetText();
		//load in the Stats
		Stats loadstats;
		int loadHP;
		int loadBP;
		int loadlevel;
		float loadspeed;
		TiXmlElement * Stat = root->FirstChildElement( "Stats" );
		loadHP = std::stoi( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.hp_scale = std::stof( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Scale" )->GetText() );

		loadBP = std::stoi( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.bp_scale = std::stof( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.attack = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.attack_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.magic = std::stoi( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Base" )->GetText() );
		loadstats.magic_scale = std::stof( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.defense = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.defense_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadspeed = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Base" )->GetText() );
		loadstats.speed_scale = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.avoision = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.avoision_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadlevel = std::stoi( root->FirstChildElement( "Level" )->GetText() );

		std::string loadhurt = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();
		std::string loadattack = root->FirstChildElement( "Sound" )->FirstChildElement( "Attack" )->GetText();
		std::string loaddeath = root->FirstChildElement( "Sound" )->FirstChildElement( "Death" )->GetText();

		std::string loadanimation = root->FirstChildElement( "Animation" )->GetText();
		if ( loadelement == "Wind" )
			{
			SetEtype( ETYPE::WIND );
			}
		else if ( loadelement == "Fire" )
			{
			SetEtype( ETYPE::FIRE );
			}
		else if ( loadelement == "Earth" )
			{
			SetEtype( ETYPE::EARTH );
			}
		else if ( loadelement == "Water" )
			{
			SetEtype( ETYPE::WATER );
			}
		else
			{
			SetEtype( ETYPE::MULTI );
			}


		SetName( loadname );
		GetStats().attack = loadstats.attack;
		GetStats().avoision = loadstats.avoision;
		GetStats().defense = loadstats.defense;
		GetStats().magic = loadstats.magic;
		SetLevel( loadlevel );
		SetHP( loadHP );
		SetMaxHP( loadHP );
		SetSpeed( loadspeed );
		SetProgress( 0.0f );
		//ability addition here
		SetOrderPosition( 2 );
		SGD::Point characterOrderPosition;
		characterOrderPosition.x = 600.0f;
		characterOrderPosition.y = (float)( GetOrderPosition() * 100 + 150 + 16 );
		SetPosition( characterOrderPosition );
		SGD::Size toonSize;
		toonSize.width = 64.0f;
		toonSize.height = 64.0f;
		SetSize( toonSize );
		OwnedHUD* turnindicator = new OwnedHUD;
		turnindicator->SetActive( true );
		turnindicator->SetImage( GamePlayState::GetInstance()->getTurnInd() );
		turnindicator->SetOwner( this );
		turnindicator->SetOffset( SGD::Point( -10, -64 ) );
		SetTurnIndicator( turnindicator );
		AnimationSystem* tempanimsys = new AnimationSystem;
		if ( loadanimation.size() )
			{
			tempanimsys->Load( loadanimation );
			tempanimsys->Play( 0 );
			}
		else
			tempanimsys = nullptr;

		SetAnimations( tempanimsys );

		if ( GetName() == "John" )
			{
			SetTimelineAnimation( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/DogTimeline.png" ) );
			}
		}
	}
void Boss::CreateJane( const char* path )
	{
	TiXmlDocument doc;
	if ( doc.LoadFile( path ) )
		{
		TiXmlElement * root = doc.RootElement();
		//basic info
		std::string loadname = root->FirstChildElement( "Name" )->GetText();
		std::string loadelement = root->FirstChildElement( "Element" )->GetText();
		std::string loadtype = root->FirstChildElement( "Type" )->GetText();
		std::string loadrandom = root->FirstChildElement( "IsRandom" )->GetText();
		//load in the Stats
		Stats loadstats;
		int loadHP;
		int loadBP;
		int loadlevel;
		float loadspeed;
		TiXmlElement * Stat = root->FirstChildElement( "Stats" );
		loadHP = std::stoi( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.hp_scale = std::stof( Stat->FirstChildElement( "HitPoints" )->FirstChildElement( "Scale" )->GetText() );

		loadBP = std::stoi( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Base" )->GetText() );
		loadstats.bp_scale = std::stof( Stat->FirstChildElement( "BattlePoints" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.attack = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.attack_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.magic = std::stoi( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Base" )->GetText() );
		loadstats.magic_scale = std::stof( Stat->FirstChildElement( "Magic" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.defense = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.defense_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadspeed = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Base" )->GetText() );
		loadstats.speed_scale = std::stof( Stat->FirstChildElement( "Speed" )->FirstChildElement( "Scale" )->GetText() );

		loadstats.avoision = std::stoi( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Base" )->GetText() );
		loadstats.avoision_scale = std::stof( Stat->FirstChildElement( "Attack" )->FirstChildElement( "Scale" )->GetText() );

		loadlevel = std::stoi( root->FirstChildElement( "Level" )->GetText() );

		std::string loadhurt = root->FirstChildElement( "Sound" )->FirstChildElement( "Hurt" )->GetText();
		std::string loadattack = root->FirstChildElement( "Sound" )->FirstChildElement( "Attack" )->GetText();
		std::string loaddeath = root->FirstChildElement( "Sound" )->FirstChildElement( "Death" )->GetText();

		std::string loadanimation = root->FirstChildElement( "Animation" )->GetText();
		if ( loadelement == "Wind" )
			{
			SetEtype( ETYPE::WIND );
			}
		else if ( loadelement == "Fire" )
			{
			SetEtype( ETYPE::FIRE );
			}
		else if ( loadelement == "Earth" )
			{
			SetEtype( ETYPE::EARTH );
			}
		else if ( loadelement == "Water" )
			{
			SetEtype( ETYPE::WATER );
			}
		else
			{
			SetEtype( ETYPE::MULTI );
			}


		SetName( loadname );
		GetStats().attack = loadstats.attack;
		GetStats().avoision = loadstats.avoision;
		GetStats().defense = loadstats.defense;
		GetStats().magic = loadstats.magic;
		SetLevel( loadlevel );
		SetHP( loadHP );
		SetMaxHP( loadHP );
		SetSpeed( loadspeed );
		SetProgress( 0.0f );
		//ability addition here
		SetOrderPosition( 0 );
		SGD::Point characterOrderPosition;
		characterOrderPosition.x = 600.0f;
		characterOrderPosition.y = (float)( GetOrderPosition() * 100 + 150 + 16 );
		SetPosition( characterOrderPosition );
		SGD::Size toonSize;
		toonSize.width = 64.0f;
		toonSize.height = 64.0f;
		SetSize( toonSize );
		OwnedHUD* turnindicator = new OwnedHUD;
		turnindicator->SetActive( true );
		turnindicator->SetImage( GamePlayState::GetInstance()->getTurnInd() );
		turnindicator->SetOwner( this );
		turnindicator->SetOffset( SGD::Point( -10, -64 ) );
		SetTurnIndicator( turnindicator );
		AnimationSystem* tempanimsys = new AnimationSystem;
		if ( loadanimation.size() )
			{
			tempanimsys->Load( loadanimation );
			tempanimsys->Play( 0 );
			}
		else
			tempanimsys = nullptr;

		SetAnimations( tempanimsys );

		if ( GetName() == "Jane" )
			{
			SetTimelineAnimation( SGD::GraphicsManager::GetInstance()->LoadTexture( "../Trapped Rat/Assets/Textures/DogTimeline.png" ) );
			}
		}
	}
