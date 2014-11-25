
#include "Ability.h"
#include "Character.h"
#include "StatusEffect.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "GameData.h"

Ability::Ability()
{
	//TODO: Add xml loading
	atkDam = 0;
	mgcDam = 2.5;
}


Ability::~Ability()
{
}

void Ability::Render()
{

	if ( offensive )
	{
		//SGD::GraphicsManager::GetInstance()->DrawString( L"Offensive", SGD::Point( GameData::GetInstance()->GetScreenWidth() / 2, GameData::GetInstance()->GetScreenHeight() / 2 ), SGD::Color( 255, 255, 255, 255 ) );
		//abilityName = "Drown";
		//SGD::GraphicsManager::GetInstance()->DrawString( abilityName.c_str(), SGD::Point( GameData::GetInstance()->GetScreenWidth() / 2 - 20, GameData::GetInstance()->GetScreenHeight() / 2 + 20 ), SGD::Color( 255, 255, 255, 255 ) );
	}
	if ( healing )
	{
		//SGD::GraphicsManager::GetInstance()->DrawString( L"Defensive", SGD::Point( GameData::GetInstance()->GetScreenWidth() / 2, GameData::GetInstance()->GetScreenHeight() / 2 ), SGD::Color( 255, 255, 255, 255 ) );
		//abilityName = "Healing Light";
		//SGD::GraphicsManager::GetInstance()->DrawString( abilityName.c_str(), SGD::Point( GameData::GetInstance()->GetScreenWidth() / 2 - 20, GameData::GetInstance()->GetScreenHeight() / 2 + 20 ), SGD::Color( 255, 255, 255, 255 ) );
	}
}
void Ability::CastAbility( Character* owner, Character* target )
{
	if ( offensive )
	{
		target->TakeDamage( (int)formulaTotal, owner );
	}
	if ( healing )
		target->TakeDamage( (int)-formulaTotal, owner );
	//if (status)
	//	target->AddStatus()
}
void Ability::CalculateFormula( Character* owner, Character* target )
{
	if ( offensive )
	{
		formulaTotal = ( atkDam * owner->GetStats().attack + mgcDam * owner->GetStats().magic ) - ( 0.25f * target->GetStats().defense + 0.25f * target->GetStats().magic );
	}
	else if ( healing )
	{
		formulaTotal = ( owner->GetMaxHP() * 0.1f ) + ( mgcDam * owner->GetStats().magic );
	}
}
std::string Ability::GetAbilityName()
{
	return abilityName;
}
bool Ability::GetOffensive()
{
	return offensive;
}
bool Ability::GetHealing()
{
	return healing;
}
float Ability::GetFormulaTotal()
{
	return formulaTotal;
}
int Ability::GetStatusID()
{
	return statusID;
}
float Ability::GetAtkDam()
{
	return atkDam;
}
float Ability::GetMgcDam()
{
	return mgcDam;
}
int Ability::GetBPCost()
	{
	return bpCost;
	}
void Ability::SetAbilityName( std::string name )
{
	abilityName = name;
}
void Ability::SetOffensive( bool offense )
{
	offensive = offense;
}
void Ability::SetHealing( bool heal )
{
	healing = heal;
}
void Ability::SetFormulaTotal( float total )
{
	formulaTotal = total;
}
void Ability::SetStatusID( int id )
{
	statusID = id;
}
void Ability::SetAtkDam( float atk )
{
	atkDam = atk;
}
void Ability::SetMgcDam( float mgc )
{
	mgcDam = mgc;
}