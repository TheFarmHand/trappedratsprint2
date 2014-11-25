
#include "StatusEffect.h"
#include "Character.h"


StatusEffect::StatusEffect()
{

}


StatusEffect::~StatusEffect()
{

}

void  StatusEffect::React(Character* attacker)
{
	// Happens when unit is struck by an attack
	
}
void  StatusEffect::Turntick()
{
	// Apply whatever status effect does
	owner->TakeDamage(dmg_tick, NULL);
}

void StatusEffect::Clear( )
// Removes this effect from the player
{
	if(owner == NULL)
	{
		// What are you doing?
		return;
	}

	std::list<StatusEffect*>::iterator iter = owner->GetEffects().begin();
	while((*iter) != this)
	{
		iter++;
	}

	owner->GetEffects().erase(iter);
	delete this;
}


StatusEffect& StatusEffect::operator=( const StatusEffect& rhs )
{
	owner = rhs.owner;
	name = rhs.name;
	element = rhs.element;
	icon = rhs.icon;
	num_ticks = rhs.num_ticks;
	curr_tick = rhs.curr_tick ;
	dmg_tick = rhs.dmg_tick;


	return *this;
	
}


// Access Granted
Character* StatusEffect::GetOwner( )
{
	return owner;
}

std::string StatusEffect::GetName( )
{
	return name;
}

Elements StatusEffect::GetElement( )
{
	return element;
}

SGD::HTexture StatusEffect::GetIcon( )
{
	return icon;
}

int StatusEffect::GetCurrTick( )
{
	return curr_tick;
}

int StatusEffect::GetNumTicks( )
{
	return num_ticks;
}


// Mutates
void StatusEffect::SetOwner( Character* own )
{
	owner = own;
}
void StatusEffect::SetName( std::string nam )
{
	this->name = nam;
}
void StatusEffect::SetElement( Elements ele )
{
	element = ele;
}
void StatusEffect::SetIcon( SGD::HTexture ico )
{
	icon = ico;
}
void StatusEffect::SetTick( int tic )
{
	num_ticks = tic;
}