
#include "Ability.h"
#include "Character.h"
#include "StatusEffect.h"
#include "StatusEffectManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "GameData.h"
#include "../States/GamePlayState.h"
#include "../tinyxml/tinyxml.h"

Ability::Ability( const char* path )
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

	TiXmlElement* help = root->FirstChildElement( "Help" );
	const char* tempHelp = help->Attribute( "Text" );
	std::string tempH( tempHelp );
	noBPexplination = tempH;

	TiXmlElement* affinity = help->NextSiblingElement( "Affinity" );
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
	else if ( tempA == "Physical" )
		elementType = 0;

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

	TiXmlElement* self = aoe->NextSiblingElement( "Self" );
	const char* tempSelf = self->Attribute( "selfTarget" );
	std::string tempS( tempSelf );
	if ( tempS == "true" )
		selfTarget = true;
	else
		selfTarget = false;

	TiXmlElement* heals = self->NextSiblingElement( "Healing" );
	const char* tempHeal = heals->Attribute( "Heals" );
	std::string tempHealing( tempHeal );
	if ( tempHealing == "true" )
		healing = true;
	else
		healing = false;

	TiXmlElement* unlock = heals->NextSiblingElement( "UnlockLevel" );
	unlock->Attribute( "Level", &unlockLevel );

	TiXmlElement* paths = unlock->NextSiblingElement( "Paths" );
	TiXmlElement* aniPath = paths->FirstChildElement( "AniPath" );
	const char* tempAni = aniPath->Attribute( "Path" );
	std::string tempAnimation( tempAni );
	animationPath = tempAnimation;
	TiXmlElement* wavPath = aniPath->NextSiblingElement( "WavPath" );
	const char* tempWav = wavPath->Attribute( "Path" );
	std::string tempWavS( tempWav );
	wavSoundPath = "../Trapped Rat/Assets/Sounds/" + tempWavS;

	TiXmlElement* damage = paths->NextSiblingElement( "DamageInfo" );
	double tempValue;
	TiXmlElement* attack = damage->FirstChildElement( "AttackMod" );
	attack->Attribute( "atkMod", &tempValue );
	atkMod = (float)tempValue;
	TiXmlElement* magic = attack->NextSiblingElement( "MagicMod" );
	magic->Attribute( "mgcMod", &tempValue );
	mgcMod = (float)tempValue;
	TiXmlElement* bp = magic->NextSiblingElement( "BaseBP" );
	bp->Attribute( "bp", &baseBP );
	TiXmlElement* hp = bp->NextSiblingElement( "HPMod" );
	hp->Attribute( "hp", &tempValue );
	hpMod = (float)( tempValue / 100 );

	access = false;
	unlocked = false;
	animate = new AnimationSystem();
	animate->Load( animationPath );
	}


Ability::~Ability()
	{
	delete animate;
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
	//animate->Render( Abiltarget->GetPosition().x - 100.0f, Abiltarget->GetPosition().y );
	RenderAnimation();
	}
void Ability::Update( float dt )
	{
	animate->Update( dt );
	}
void Ability::CastAbility( Character* owner, Character* target, int AoeCounter, bool ternary )
	{
	int tempFirefall = 0;
	Abilowner = owner;
	Abiltarget = target;
	CalculateFormula( owner, target );
	if ( abilityName == "Second Wind" )
		{
		target->SetLiving( true );
		}
	if ( offensive && !status)
		{
		if ( abilityName == "Firefall" )
			{
			tempFirefall = (int)( formulaTotal / 2 );
			target->TakeDamage( (int)tempFirefall );
			target->TakeDamage( (int)tempFirefall, true);
			}
		else
			target->TakeDamage( (int)formulaTotal );
		}
	if ( healing )
		{
		int dmg = (int)-formulaTotal;
		if(abilityName == "Whispering Wind")
			{
			if ( ternary )
				dmg = (int)( dmg * 1.75f );
			}
		else if ( abilityName == "Second Wind" )
			{
			if (ternary)
				dmg *= 2;
			}
			target->TakeDamage( dmg );
		}
	if ( abilityName == "Wind Split" )
		{
		owner->WRsplit = true;
		TurnManager::GetInstance()->GetEnemies()[1]->SetLiving( true );
		TurnManager::GetInstance()->GetEnemies()[1]->SetMaxHP( owner->GetMaxHP() / 2 );
		TurnManager::GetInstance()->GetEnemies()[1]->SetHP( TurnManager::GetInstance()->GetEnemies()[1]->GetMaxHP() );
		TurnManager::GetInstance()->GetEnemies()[1]->WRsplit = true;
		owner->SetMaxHP( owner->GetMaxHP() / 2 );
		owner->SetHP( owner->GetMaxHP() );
		}
	if ( status && statusName == "Cover" )
	{
		target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( statusName ), owner, ternary );
		owner->AddStatus( &StatusEffectManager::GetInstance( )->GetStatus( "Guarding" ), target, ternary);
	}

	else if (status && abilityName == "Acid Rain" )
	{
		if(ternary)
		{
			float tickdmg = (float)StatusEffectManager::GetInstance()->GetStatus("Poison").GetTickDmg();
			tickdmg += (0.35f * owner->GetMagic());
			tickdmg *= 3;
			tickdmg -= ( 0.25f * target->GetDefense() + 0.25f * target->GetMagic());
			target->TakeDamage((int)tickdmg);
		}
		else
			target->AddStatus(&StatusEffectManager::GetInstance()->GetStatus("Poison"));
	}

	else if ( status )
		{
		if (atkMod > 0 || mgcMod > 0)
			target->TakeDamage( (int)formulaTotal );
		target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( statusName ), nullptr, ternary );
		}

	if ( abilityName == "Rib-a-Rang" )
		if(!ternary)
			owner->SetHP( owner->GetHP() - (int)( ( owner->GetHP() * hpMod ) ) );
	
	if ( AoeCounter == 0 ) 
		{
		if ( GamePlayState::GetInstance()->usingTernary() )
			owner->SetBP( owner->GetBP() - bpCost / 2 );
		else
			owner->SetBP( owner->GetBP() - bpCost );
		}
	}
void Ability::CalculateFormula( Character* owner, Character* target )
	{
	bool weak = false;
	bool strong = false;

	switch ( elementType )
		{
		case 0:
			weak = false;
			strong = false;
			break;
		case 1:
			if ( target->GetEType() == WATER )
				weak = true;
			else if ( target->GetEType() == FIRE )
				strong = true;
			break;
		case 2:
			if ( target->GetEType() == WIND )
				weak = true;
			else if ( target->GetEType() == EARTH )
				strong = true;
			break;
		case 3:
			if ( target->GetEType() == FIRE )
				weak = true;
			else if ( target->GetEType() == WATER )
				strong = true;
			break;
		case 4:
			if ( target->GetEType() == EARTH )
				weak = true;
			else if ( target->GetEType() == WIND )
				strong = true;
			break;
		default:
			weak = false;
			strong = false;
			break;
		}

	if ( offensive )
		{
		formulaTotal = ( atkMod * owner->GetAttack() + mgcMod * owner->GetMagic() ) - ( 0.25f * target->GetDefense() + 0.25f * target->GetMagic() );
		if ( formulaTotal <= 0.0f )
			formulaTotal = 0.0f;
		}
	else if ( healing )
		{
		formulaTotal = ( owner->GetMaxHP() * hpMod ) + ( mgcMod * owner->GetMagic());
		return;
		}
	if ( weak )
		formulaTotal *= 0.5f;
	else if ( strong )
		formulaTotal *= 1.5f;
	}
void Ability::CalcluateBpScaledCost( Character* owner )
	{
	bpCost = baseBP + (int)( owner->GetLevel() * 0.5f );
	std::string temp = std::to_string( bpCost );
	temp += " BP cost" + noBPexplination;
	//explination = temp;
	explination = noBPexplination;
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
bool Ability::GetUnlocked()
	{
	return unlocked;
	}
int Ability::GetUnlockLevel()
	{
	return unlockLevel;
	}
bool Ability::GetAccess()
	{
	return access;
	}
bool Ability::GetSelfTarget()
	{
	return selfTarget;
	}
AnimationSystem* Ability::GetAnimate()
	{
	return animate;
	}
Character* Ability::GetOwner()
	{
	return Abilowner;
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
void Ability::SetUnlocked( bool unlock )
	{
	unlocked = unlock;
	}
void Ability::SetAccess( bool combatUse )
	{
	access = combatUse;
	}
void Ability::RenderAnimation()
{
	//This Function is called in Render

	//On Top Of Target
	if (abilityName == "Burrow" || abilityName == "Earth Fang" || abilityName == "Water Fang" || abilityName == "Fire Fang" || abilityName == "Wind Fang" || abilityName == "Poison Fang")
	{
		animate->Render(Abiltarget->GetPosition().x, Abiltarget->GetPosition().y);
	}
	//Adjusted Up and Left
	else if (abilityName == "Slow Claw")
	{
		animate->Render(Abiltarget->GetPosition().x - 32, Abiltarget->GetPosition().y - 32);
	}
	//At Left of Target
	else if (abilityName == "Earth Fang" || abilityName == "Water Fang")
	{
		animate->Render(Abiltarget->GetPosition().x - 100, Abiltarget->GetPosition().y);
	}
	//Right of Target
	else if (abilityName == "Counter Claw")
	{
		animate->Render(Abiltarget->GetPosition().x + 100, Abiltarget->GetPosition().y);
	}
	//Top of Enemy Side
	else if (abilityName == "Acid Rain")
	{
		animate->Render(Abiltarget->GetPosition().x - 50.0f, 100.0f);
	}
	else
	{
		animate->Render(Abiltarget->GetPosition().x - 100.0f, Abiltarget->GetPosition().y);

	}
}