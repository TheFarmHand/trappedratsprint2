
#include "Character.h"
#include "Numbers.h"
#include "TurnManager.h"
#include <cmath>
#include "OwnedHUD.h"

Character::Character()
{

}


Character::~Character()
{
	for ( auto iter = effects.begin(); iter != effects.end(); iter++ )
	{
		delete ( *iter );
	}
	for (size_t i = 0; i < damagenumbers.size(); i++)
	{
		delete damagenumbers[i];
	}
	damagenumbers.clear();
	if (portrait != SGD::INVALID_HANDLE)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(portrait);
	}
	if (timelineanimation != SGD::INVALID_HANDLE)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(timelineanimation);
	}
	if (TurnIndicator != nullptr)
	{
		delete TurnIndicator;
		TurnIndicator = nullptr;
	}

}
void Character::Update( float dt )
{
	for (size_t i = 0; i < damagenumbers.size(); i++)
	{
		damagenumbers[i]->Update(dt);
	}
}
void Character::UpdateAnimation( float dt )
{

}
void Character::Render()
{
	for (size_t i = 0; i < damagenumbers.size(); i++)
	{
		damagenumbers[i]->Render();
	}
	if (progress >= 100.0f || stepforward || stepbackward)
	{
		if (TurnIndicator != nullptr)
		{
			TurnIndicator->Render();
		}
	}
	int offset = 0;
	for ( auto iter = effects.begin( ); iter != effects.end( ); iter++ )
	{
		SGD::GraphicsManager::GetInstance( )->DrawTexture( ( *iter )->GetIcon( ), { (position.x-size.width/2) + 10*offset, position.y - size.height - 5 } );
		offset++;
	}
}

void Character::Attack( Character* owner, Character * target )
{
	int atk = owner->GetAttack();
	int dmg = rand() % atk + atk;
	dmg -= (int)(0.25f * target->GetDefense());
	if ( dmg <= 0 )
		dmg = 0;
	TurnManager::GetInstance( )->AttackTarget(owner, target, dmg);
}

void Character::UseAbility()
// Nothing?  Animations/Particles?
//lol
{

}

void Character::TakeDamage( int dmg , bool firefall)
{
	SetHP(HP - dmg);
	if ( name == "Cecil" && CecilPhase == 3 )
		{
		damageDealt += dmg;
		}
	if ( name == "Jane" && alive == true )
		++JaneHit;
	else if ( name == "John" && alive == true )
		++JohnHit;
	
	/*if ( HP <= 0 )
	{
		HP = 0;
		alive = false;
	}
	if ( HP > GetMaxHP() )
	{
		HP = GetMaxHP();
	}*/
	SGD::Point tempoffset;
	tempoffset.y = 0;
	if (GetType() == Enemy)
	{
		tempoffset.x = -50;
	}
	else
	{
		tempoffset.x = 50;
	}
	if (dmg > 0 && !firefall)
	{
		//hurt
		Numbers* temp = new Numbers(dmg, SGD::Color(180, 0, 0),this,tempoffset);
		damagenumbers.push_back(temp);
	}
	else if (!firefall )
	{
		//healed

		Numbers* temp = new Numbers(abs(dmg), SGD::Color(0, 180, 0),this,tempoffset);
		damagenumbers.push_back(temp);
	}
	if ( firefall )
		{
		tempoffset.x -= 30;
		tempoffset.y += 10;
		Numbers* temp = new Numbers( dmg, SGD::Color( 180, 0, 0 ), this, tempoffset );
		damagenumbers.push_back( temp );
		}
}

bool Character::HasEffect( std::string effect )
{
	for ( auto iter = GetEffects( ).begin( ); iter != GetEffects( ).end( ); iter++ )
	{
		if( (*iter)->GetName() == effect)
			return true;
	}

	return false;
}

void Character::RemoveEffect( std::string effect )
{
	for ( auto iter = GetEffects( ).begin( ); iter != GetEffects( ).end( ); iter++ )
	{
		if ( ( *iter )->GetName( ) == effect )
		{
			(*iter)->Clear( );
			break;
		}
	}
}

void Character::React()
// Loops through status effects on the unit and calls their React function (for things like dodge and counter)
// Actually this was handled in TurnManager I think
{

}

void Character::UpdateToLevel( )
{
	
}

//accessors
std::string Character::GetName()
{
	return name;
}
Stats& Character::GetStats()
{
	return stats;
}

int Character::GetAttack()
{
	return stats.attack + ( int )( stats.attack_scale * level );
}

int Character::GetDefense( )
{
	return stats.defense + ( int )( stats.defense_scale * level );
}

int Character::GetMagic( )
{
	return stats.magic +(int)( stats.magic_scale * level );
}

int Character::GetAvoision( )
{
	return stats.avoision + ( int )( stats.avoision_scale * level );
}

int Character::GetLevel()
{
	return level;
}
int Character::GetHP()
{
return HP ;
}
int Character::GetMaxHP()
{
	return MaxHP + (int)(stats.hp_scale * level);
}
int Character::GetBP()
	{
	return BP ;
	}
int Character::GetMaxBP()
	{
	return MaxBP + (int)(stats.bp_scale * level);
	}
float Character::GetSpeed()
{
	return speed + (stats.speed_scale * level);
}
float Character::GetProgress()
{
	return progress;
}
SGD::HTexture Character::GetPortrait()
{
	return portrait;
}
SGD::HTexture Character::GetTimelineAnimation()
{
	return timelineanimation;
}
Ability* Character::GetAbility( int index )
{
	return abilityList[ index ];
}
int Character::GetOrderPosition()
{
	return order;
}

ETYPE Character::GetEType()
{
	return element;
}
void Character::SetEtype(ETYPE _type)
{
	element = _type;
}

std::list<StatusEffect*>& Character::GetEffects()
{
	return effects;
}


//mutators
void Character::SetName( std::string _name )
{
	name = _name;
}
void Character::SetStats( Stats _stats )
{
	stats = _stats;
}
void Character::SetLevel( int _level )
{
	level = _level;
}
void Character::SetHP( int _hp )
{
	HP = _hp;
	if ( HP / (float)(GetMaxHP()) < 0.1f && name == "Cecil")
	{
		if ( CecilPhase == 1 || CecilPhase == 2 )
			{
			HP = (int)(GetMaxHP() * 0.1f);
			progress = 100.0f;
			}
		}
	else if ( name == "Cecil" && CecilPhase == 3 )
		{
		HP = (int)( GetMaxHP() * 0.1f );
		}
	else if ( HP < 0 )
	{
		HP = 0;
		alive = false;
		if ( name == "Jane" )
			{
			TurnManager::GetInstance()->GetEnemies()[1]->SetProgress( 100.0f );
			JaneDead = true;
			}
		else if ( name == "John" )
			{
			TurnManager::GetInstance()->GetEnemies()[1]->SetProgress( 100.0f );
			JohnDead = true;
			}
	}

	if(HP > GetMaxHP())
		HP = GetMaxHP();
}
void Character::SetMaxHP( int _max )
{
	MaxHP = _max;
}
void Character::SetBP( int _bp )
	{
	BP = _bp;
	if(BP<0) BP = 0;
	if(BP>GetMaxBP()) BP = GetMaxBP();
	
	}
void Character::SetMaxBP( int _max )
	{
	MaxBP = _max;
	}
void Character::SetSpeed( float _speed )
{
	speed = _speed;
}
void Character::SetProgress( float _progress )
{
	progress = _progress;
}
void Character::SetAbility( int index, Ability* _ability )
{
	abilityList[ index ] = _ability;
}
void Character::SetOrderPosition( int index )
{
	order = index;
}

bool Character::isAlive()
{
	return alive;
}

void Character::SetLiving( bool buul )
{
	alive = buul;
}
void Character::SetPortrait(SGD::HTexture _port)
{
	portrait = _port;
}
void Character::SetTimelineAnimation(SGD::HTexture _timeline)
{
	timelineanimation = _timeline;
}
void Character::StatusTick()
// Happens at the beginning of a characters turn
// Applies 1 tick of every "ticking" status effect on the player
{
	std::vector<std::list<StatusEffect*>::iterator> todel;	// Used to clear out unused
	for ( auto iter = effects.begin(); iter != effects.end(); iter++ )
	{
		( *iter )->Turntick();
		if ( ( *iter )->GetCurrTick() == ( *iter )->GetNumTicks() )
		{

			//(*iter)->Clear();
			std::list<StatusEffect*>::iterator temp = iter;
			todel.push_back( ( temp ) );
		}
	}

	for ( unsigned int i = 0; i < todel.size(); i++ )
	{
		( *todel[ i ] )->Clear();
	}
}

void Character::AddStatus( StatusEffect *status , Character* theGuard, bool ternary)
// Needs access to attacker for damage calculations
{
	StatusEffect* temp = new StatusEffect();
	// Setup damage values
	*temp = *status;
	temp->SetOwner( this );
	temp->SetGuard( theGuard );
	temp->SetTernEffect(ternary);
	effects.push_back( temp );
	temp->Initialize();
}
void Character::InitializeAbilities( std::vector<Ability*> toSet )
{
	for ( unsigned int i = 0; i < toSet.size(); ++i )
	{
		abilityList.push_back( toSet[ i ] );
	}
}

void Character::SetTurnIndicator(OwnedHUD* _indicator)
{
	TurnIndicator = _indicator;
}
int Character::GetAbilitiesSize()
{
	return abilityList.size();
}