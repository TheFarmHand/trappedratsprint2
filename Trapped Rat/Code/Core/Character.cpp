
#include "Character.h"

Character::Character()
{

}


Character::~Character()
{
	for ( auto iter = effects.begin(); iter != effects.end(); iter++ )
	{
		delete ( *iter );
	}
}
void Character::Update( float dt )
{

}
void Character::UpdateAnimation( float dt )
{

}
void Character::Render()
{

}
void Character::Attack( Character* owner, Character * target )
{
	// This needs to do proper math formula based on attack stats, etc.
	//owner->TakeDamage(20, target);
}
void Character::UseAbility()
{
	// All Status Effects are applied through this function (attached to Abilities)	
}

void Character::TakeDamage( int dmg, Character* attacker )	// pointer is now the person hitting you
{
	HP -= dmg;
	if ( HP <= 0 )
	{
		HP = 0;
		alive = false;
	}
	if ( HP > MaxHP )
	{
		HP = MaxHP;
	}

}
void Character::React()
{

}

//accessors
std::string Character::GetName()
{
	return name;
}
Stats Character::GetStats()
{
	return stats;
}
int Character::GetLevel()
{
	return level;
}
int Character::GetHP()
{
	return HP;
}
int Character::GetMaxHP()
{
	return MaxHP;
}
float Character::GetSpeed()
{
	return speed;
}
float Character::GetProgress()
{
	return progress;
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
	if ( HP == 0 )
	{
		// Set to dead
	}
}
void Character::SetMaxHP( int _max )
{
	MaxHP = _max;
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

void Character::StatusTick()
// Happens at the beginning of a characters turn
// Applies 1 tick of every "ticking" status effect on the player
{
	for ( auto iter = effects.begin(); iter != effects.end(); iter++ )
	{
		( *iter )->Turntick();
	}
}

void Character::AddStatus( StatusEffect *status )
{
	effects.push_back( status );
}