#include "StatusEffectManager.h"
#include "StatusEffect.h"
#include "Ability.h"


#include <fstream>

StatusEffectManager* StatusEffectManager::GetInstance()
{
	static StatusEffectManager data;
	return &data;
}

void StatusEffectManager::Initialize()
{
	// True Functionality will load in a full list from XML
	std::string file = "Assets/Scripts/StatusEffects.txt";

	LoadStatusEffect( file );
}

void StatusEffectManager::Terminate()
{
	// Remove all from Map
	for(auto iter = StatusList.begin(); iter != StatusList.end(); iter++)
	{
		StatusEffect* temp;
		temp = (*iter).second;
		delete temp;
	}
	SGD::GraphicsManager::GetInstance( )->UnloadTexture(bp);
}

void StatusEffectManager::LoadStatusEffect( std::string filename )
{
	std::fstream infile;
	std::string path = "../Trapped Rat/Assets/Textures";
	infile.open( filename, std::ios_base::in );
	StatusEffect *temp;
	char a = ' ';
	std::string name;
	std::string image;
	bp = SGD::GraphicsManager::GetInstance( )->LoadTexture( "../Trapped Rat/Assets/Textures/TestParticleBlue.png" );
	while ( !infile.eof() )
	{
		
		name = "";
		infile >> a;
		if(a == '*')
		{
			break;
		}

		temp = new StatusEffect( );
		do
		{
			name += a;
			infile.get( a );

		} while ( a != '\n' );

		int in = 0;

		// Element, Ticks, Dmg, Type( DOT, STAT, SPECIAL)
		temp->SetName( name.c_str() );
		infile >> in;
		temp->SetElement( (ETYPE)in );
		infile >> in;
		temp->SetTick( in );
		infile >> in;
		temp->SetTickDmg( in );
		infile >> in;
		temp->SetType( in );
		temp->SetOwner( NULL );

		image = "";
		infile >> a;	// Skips white space, grabs first letter it finds
		while ( a != '\n' )
		{
			image += a;
			infile.get(a);
		}

		temp->SetIcon( bp );	// Load image here later
		StatusList[ name.c_str() ] = temp;
	}
	
	infile.close();
}

StatusEffect& StatusEffectManager::GetStatus( std::string status )
{
	if(status == "Burrow" || status == "Puddle" || status == "Collapse" || status == "Leaf on the Wind")
		return *StatusList[ "Dodging" ];

	return *StatusList[ status ];
}

//std::string name;
//int element;
//SGD::HTexture icon;
//int num_ticks;
//int dmg_tick;

//int curr_tick = 0;
//Character* owner;