
#include "Character.h"
#include "Numbers.h"
#include <cmath>
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
}
void Character::Attack( Character* owner, Character * target )
{

}

void Character::UseAbility()
// 
{

}

void Character::TakeDamage( int dmg )
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
	if (dmg > 0)
	{
		//hurt
		Numbers* temp = new Numbers(dmg, SGD::Color(180, 0, 0),this,tempoffset);
		damagenumbers.push_back(temp);
	}
	else
	{
		//healled

		Numbers* temp = new Numbers(abs(dmg), SGD::Color(0, 180, 0),this,tempoffset);
		damagenumbers.push_back(temp);
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

void Character::React()
// Loops through status effects on the unit and calls their React function (for things like dodge and counter)
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
int Character::GetBP()
	{
	return BP;
	}
int Character::GetMaxBP()
	{
	return MaxBP;
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
	if ( HP == 0 )
	{
		// Set to dead
	}
}
void Character::SetMaxHP( int _max )
{
	MaxHP = _max;
}
void Character::SetBP( int _bp )
	{
	BP = _bp;
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

void Character::AddStatus( StatusEffect *status )
// Needs access to attacker for damage calculations
// Probably done in TM
{
	StatusEffect* temp = new StatusEffect();
	// Setup damage values
	*temp = *status;
	temp->SetOwner( this );
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