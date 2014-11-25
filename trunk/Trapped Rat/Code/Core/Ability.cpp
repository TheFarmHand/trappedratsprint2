
#include "Ability.h"
#include "Character.h"
#include "StatusEffect.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "GameData.h"
#include "../tinyxml/tinyxml.h"

Ability::Ability(const char* path)
	{
	TiXmlDocument abilityDoc;
	
	if ( !abilityDoc.LoadFile( path ) )
		return;

	TiXmlElement* root = abilityDoc.RootElement();
	if ( root == nullptr )
		return;

	const char* tempName = root->Attribute( "Name" );
	std::string tempAN( tempName );
	abilityName = tempAN;
	const char* tempOffense = root->Attribute( "Offensive" );
	std::string tempOff( tempOffense );
	if ( tempOff == "true" )
		offensive = true;
	else
		offensive = false;

	TiXmlElement* affinity = root->FirstChildElement( "Affinity" );
	const char* tempAff = affinity->Attribute( "ElementalType" );
	std::string tempA( tempAff );
	if ( tempA == "Wind" )
		elementType = 1;
	else if ( tempA == "Fire" )
		elementType = 2;
	else if ( tempA == "Earth" )
		elementType = 3;
	else if ( tempA == "Water" )
		elementType = 4;

	TiXmlElement* statusEffect = affinity->NextSiblingElement( "StatusEffect" );
	const char* tempStatus = statusEffect->Attribute( "Effect" );
	std::string tempStat( tempStatus );
	statusName = tempStat;
	if ( statusName == "None" )
		status = false;
	else
		status = true;

	TiXmlElement* aoe = statusEffect->NextSiblingElement( "AreaOfEffect" );
	const char* tempAoe = aoe->Attribute( "AOE" );
	std::string tempArea( tempAoe );
	if ( tempArea == "true" )
		areaOfEffect = true;
	else
		areaOfEffect = false;

	TiXmlElement* heals = aoe->NextSiblingElement( "Healing" );
	const char* tempHeal = heals->Attribute( "Heals" );
	std::string tempHealing( tempHeal );
	if ( tempHeal == "true" )
		healing = true;
	else
		healing = false;

	TiXmlElement* paths = heals->NextSiblingElement( "Paths" );
	TiXmlElement* aniPath = paths->FirstChildElement( "AniPath" );
	TiXmlElement* wavPath = aniPath->NextSiblingElement( "WavPath" );

	TiXmlElement* damage = paths->NextSiblingElement( "DamageInfo" );
	double tempValue;
	TiXmlElement* attack = damage->FirstChildElement( "AttackMod");
	attack->Attribute( "atkMod", &tempValue );
	atkMod = (float)tempValue;
	TiXmlElement* magic = attack->NextSiblingElement( "MagicMod" );
	magic->Attribute( "mgcMod", &tempValue );
	mgcMod = (float)tempValue;
	TiXmlElement* bp = magic->NextSiblingElement( "BaseBP" );
	bp->Attribute( "bp", &bpCost );
	TiXmlElement* hp = bp->NextSiblingElement( "HPMod" );
	hp->Attribute( "hp", &tempValue );
	hpMod = (float)tempValue;

	access = false;
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
		formulaTotal = ( atkMod * owner->GetStats().attack + mgcMod * owner->GetStats().magic ) - ( 0.25f * target->GetStats().defense + 0.25f * target->GetStats().magic );
		}
	else if ( healing )
		{
		formulaTotal = ( owner->GetMaxHP() * 0.1f ) + ( mgcMod * owner->GetStats().magic );
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
float Ability::GetAtkMod()
	{
	return atkMod;
	}
float Ability::GetMgcMod()
	{
	return mgcMod;
	}
int Ability::GetBPCost()
	{
	return bpCost;
	}
float Ability::GetHPMod()
	{
	return hpMod;
	}
bool Ability::GetAOE()
	{
	return areaOfEffect;
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
	atkMod = atk;
	}
void Ability::SetMgcDam( float mgc )
	{
	mgcMod = mgc;
	}