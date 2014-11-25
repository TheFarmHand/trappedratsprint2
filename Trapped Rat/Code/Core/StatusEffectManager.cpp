#include "StatusEffectManager.h"
#include "StatusEffect.h"

#include <fstream>

StatusEffectManager* StatusEffectManager::GetInstance( )
{
	static StatusEffectManager data;
	return &data;
}

void StatusEffectManager::Initialize( )
{
	// True Functionality will load in a full list from XML
	std::string file = "../Trapped Rat/Assets/Scripts/TestStatus.txt";

	LoadStatusEffect(file);
}

void StatusEffectManager::Terminate( )
{

}

void StatusEffectManager::LoadStatusEffect(std::string filename)
{
	std::fstream infile; 
	infile.open(filename, std::ios_base::in);
	StatusEffect temp;

	int length = 0;
	infile >> length;
	
	std::string name;
	for( ; length>0; length--)
	{
		char a;
		infile >> a;
		name += a;
	}
	int in = 0;



	temp.SetName(name.c_str());
	



	infile.close();
}

//std::string name;
//int element;
//SGD::HTexture icon;
//int num_ticks;
//int dmg_tick;

//int curr_tick = 0;
//Character* owner;