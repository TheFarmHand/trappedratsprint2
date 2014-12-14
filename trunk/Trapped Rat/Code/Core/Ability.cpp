
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
	explination = noBPexplination;

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


	//load the sound
	std::string soundpath;
	soundpath = "../Trapped Rat/Assets/Sounds/" + abilityName + ".wav";
	sound = SGD::AudioManager::GetInstance()->LoadAudio(soundpath.c_str());
	}


Ability::~Ability()
	{
	delete animate;
	if (sound != SGD::INVALID_HANDLE)
	{
		SGD::AudioManager::GetInstance()->UnloadAudio(sound);
	}
	}

void Ability::Render()
	{
	RenderAnimation();
	}
void Ability::Update( float dt )
	{
	animate->Update( dt );
	}
void Ability::CastAbility( Character* owner, Character* target, int AoeCounter, bool ternary )
	{
	//Hack Code, Fix Later
	//GamePlayState::GetInstance()->PlaySoundEffect(1);
	//EndHack
	if (sound != SGD::INVALID_HANDLE)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(sound);
	}
	int tempFirefall = 0;
	Abilowner = owner;
	Abiltarget = target;
	CalculateFormula( owner, target );
	if ( target->HasEffect( "WaterWall" ) )
		{
		target->TakeDamage( (int)( formulaTotal ), true );
		target->TakeDamage( (int)( -( formulaTotal / 4 ) ) );
		return;
		}
	if ( abilityName == "Second Wind" )
		{
		target->SetLiving( true );
		dynamic_cast<CombatPlayer*>(target)->GetAnimations()->Play(0);
		}
	if ( offensive && !status )
		{
		if ( abilityName == "Firefall" )
			{
			tempFirefall = (int)( formulaTotal / 2 );
			target->TakeDamage( (int)tempFirefall );
			target->TakeDamage( (int)tempFirefall, true );
			}
		else
			target->TakeDamage( (int)formulaTotal );
		}
	if ( healing )
		{
		
		int dmg = (int)-formulaTotal;
		if ( abilityName == "Whispering Wind" )
			{
			if ( ternary )
				dmg = (int)( dmg * 1.75f );
			}
		else if ( abilityName == "Second Wind" )
			{
			if ( ternary )
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
		TurnManager::GetInstance()->GetEnemies()[1]->SetPosition( SGD::Point( 600.0f, (float)( TurnManager::GetInstance()->GetEnemies()[1]->GetOrderPosition() * 100 + 150 + 16 ) ) );
		owner->SetMaxHP( owner->GetMaxHP() / 2 );
		owner->SetHP( owner->GetMaxHP() );
		}
	if ( status && statusName == "Cover" )
	{
		target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( statusName ), owner, ternary );
		owner->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Guarding" ), target, ternary );
	}

	else if ( status && abilityName == "Acid Rain" )
	{
		if ( ternary )
		{
			float tickdmg = (float)StatusEffectManager::GetInstance()->GetStatus( "Poison" ).GetTickDmg();
			tickdmg += ( 0.35f * owner->GetMagic() );
			tickdmg *= 3;
			tickdmg -= ( 0.25f * target->GetDefense() + 0.25f * target->GetMagic() );
			target->TakeDamage( (int)tickdmg );
		}
		else
			{
			if ( !target->HasEffect( statusName ) )
				target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "Poison" ) );
			
	}
		}

	else if ( status )
		{
		if ( atkMod > 0 || mgcMod > 0 )
			target->TakeDamage( (int)formulaTotal );
		if ( !target->HasEffect( statusName ) )
		target->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( statusName ), nullptr, ternary );
		}

	if ( abilityName == "Rib-a-Rang" )
		if ( !ternary )
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
		formulaTotal = ( owner->GetMaxHP() * hpMod ) + ( mgcMod * owner->GetMagic() );
		return;
		}
	if ( weak )
		formulaTotal *= 0.5f;
	else if ( strong )
		formulaTotal *= 1.5f;
	}
void Ability::CalcluateBpScaledCost( Character* owner )
	{
	bpCost = baseBP + (int)( owner->GetLevel() * 0.2f );
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
	if (abilityName == "Burrow" || abilityName == "Earth Fang" || abilityName == "Water Fang" || abilityName == "Fire Fang" || abilityName == "Wind Fang" || abilityName == "Poison Fang" 
		|| abilityName == "Puddle"|| abilityName == "Whirlpool"
		|| abilityName == "Collapse" || abilityName == "Ignite" || abilityName == "Scorch" || abilityName == "Firefall" ||abilityName == "Fire Spikes"
		|| abilityName == "Leaf on the Wind" || abilityName == "Wind Vale" || abilityName == "Zephyr"
		|| abilityName == "Tornado" 
		|| abilityName == "Hedge Guard" 
		|| abilityName == "Feral Bite")
	{
		animate->Render(Abiltarget->GetPosition().x, Abiltarget->GetPosition().y);
	}
	else if ( abilityName == "Cover" )
		{
		if ( Abiltarget->GetAlliance() == "Ally")
			animate->Render( Abiltarget->GetPosition().x + 70.0f, Abiltarget->GetPosition().y + 20.0f);
		else
			animate->Render( Abiltarget->GetPosition().x - 70.0f, Abiltarget->GetPosition().y );

		}
	//Adjusted Up and Left
	else if ( abilityName == "Second Wind" || abilityName == "Healing Light")
		{
		animate->Render( Abiltarget->GetPosition().x, Abiltarget->GetPosition().y - 20.0f );
		}
	else if ( abilityName == "Geo Crush" )
		{
		animate->Render( Abiltarget->GetPosition().x, Abiltarget->GetPosition().y - 200.0f);
		}
	else if ( abilityName == "Tailwind" )
		{
		animate->Render( Abiltarget->GetPosition().x + 12.0f, Abiltarget->GetPosition().y + 4.0f );
		}
	else if ( abilityName == "Slow Claw" || abilityName == "Scratch")
	{
	if ( Abiltarget->GetAlliance() == "Ally" )
		animate->Render( Abiltarget->GetPosition().x + 32.0f, Abiltarget->GetPosition().y + 32.0f );
	else
		animate->Render( Abiltarget->GetPosition().x - 32.0f, Abiltarget->GetPosition().y - 32.0f);
	}
	//At Left of Target
	else if (abilityName == "Pinch" || abilityName == "Rock Spike")
	{
		animate->Render(Abiltarget->GetPosition().x - 50.0f, Abiltarget->GetPosition().y);
	}
	//Right of Target
	else if (abilityName == "Emblazon" || abilityName == "Counter Claw" || abilityName == "Stitch in Time" || abilityName == "Fresh Meat" || abilityName == "Bellows Blast")
	{
		animate->Render( Abiltarget->GetPosition().x + 100.0f, Abiltarget->GetPosition().y );
	}
	//Top of Enemy Side
	else if (abilityName == "Acid Rain")
	{
		animate->Render(Abiltarget->GetPosition().x - 50.0f, 180.0f);
	}
	//Projectile Rendering from ally to enemy
	else if (abilityName == "Dissolve" || abilityName == "Squirt" || abilityName == "Torrent" || abilityName == "Rib-a-Rang" || abilityName == "Incinerate")
	{
		//This Code Only Works for allies 
		float xDistance = abs(Abiltarget->GetPosition().x - Abilowner->GetPosition().x);
		float yDistance = Abiltarget->GetPosition().y - Abilowner->GetPosition().y;
		float percentage = (2.0f -GamePlayState::GetInstance()->abilityTimer) / 1.0f;
		if (percentage > 1.0f)
		{
			percentage = 1.0f;
		}
		xDistance *= percentage;
		yDistance *= percentage;
		float xpos = Abilowner->GetPosition().x + xDistance;
		float ypos = Abilowner->GetPosition().y + yDistance;
		animate->Render(xpos, ypos);
	}
	//Projectile Rendering from enemy to ally
	else if ( abilityName == "Feathered Flurry" || abilityName == "Elemental Poison")
		{
		//This Code Only Works for enemies 
		float xDistance = abs(Abiltarget->GetPosition().x - Abilowner->GetPosition().x);
		float yDistance = Abiltarget->GetPosition().y - Abilowner->GetPosition().y;
		float percentage = ( 2.0f - GamePlayState::GetInstance()->abilityTimer ) / 1.0f;
		if ( percentage > 1.0f )
			{
			percentage = 1.0f;
			}
		xDistance *= percentage;
		yDistance *= percentage;
		float xpos = Abilowner->GetPosition().x - xDistance;
		float ypos = Abilowner->GetPosition().y + yDistance;
		animate->Render( xpos, ypos );
		}
	//Projectile Rendering for AOE's that Go Across Whole Screen
	else if (abilityName == "Flood")
	{
		//float xDistance = Abiltarget->GetPosition().x - Abilowner->GetPosition().x;
		float percentage = (2.0f - GamePlayState::GetInstance()->abilityTimer);
		//xDistance *= percentage;
		float xpos = Abilowner->GetPosition().x * percentage;
		animate->Render(xpos, 200);
	}
	//Render on Top of all Targets
	else if (abilityName == "Splash" || abilityName == "Whispering Wind" || abilityName == "Tremor")
	{
	if ( Abiltarget->GetAlliance() == "Ally" )
		{
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
			{
			if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
				animate->Render( TurnManager::GetInstance()->GetAllies()[i]->GetPosition().x, (float)(TurnManager::GetInstance()->GetAllies()[i]->GetOrderPosition() * 100 + 150 ));
			}
		}
	else
		{
		for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
			{
				animate->Render( TurnManager::GetInstance()->GetEnemies()[i]->GetPosition().x, (float)(TurnManager::GetInstance()->GetEnemies()[i]->GetOrderPosition() * 100 + 150 + 16 ));
			}
		}
	}
	//AOE RENDERING NEEDS TO BE HARDCODDEDDDD
	else if (abilityName == "Tempest")
	{
	animate->Render( Abiltarget->GetPosition().x - 40.0f, (float)(TurnManager::GetInstance()->GetEnemies()[0]->GetOrderPosition() * 100 + 150 + 16) );
	}
	//Render on Top of in front of Targets
	else if ( abilityName == "Rampart" || abilityName == "Quake" )
		{
		if ( Abiltarget->GetAlliance() == "Ally" )
			{
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetAllies().size(); i++ )
				{
				if ( TurnManager::GetInstance()->GetAllies()[i]->isAlive() )
					animate->Render( TurnManager::GetInstance()->GetAllies()[i]->GetPosition().x + 65.0f, (float)( TurnManager::GetInstance()->GetAllies()[i]->GetOrderPosition() * 100 + 150 ) );
				}
			}
		else
			{
			for ( unsigned int i = 0; i < TurnManager::GetInstance()->GetEnemies().size(); i++ )
				{
					animate->Render( TurnManager::GetInstance()->GetEnemies()[i]->GetPosition().x - 60.0f, (float)( TurnManager::GetInstance()->GetEnemies()[i]->GetOrderPosition() * 100 + 140 ) );
				}
			}
		}
	//Original Default
	else
	{
		//animate->Render( Abiltarget->GetPosition().x - 100.0f, Abiltarget->GetPosition().y );

	}
	}