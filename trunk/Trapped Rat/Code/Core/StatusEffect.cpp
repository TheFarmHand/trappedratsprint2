
#include "StatusEffect.h"
#include "Ability.h"
#include "Character.h"


StatusEffect::StatusEffect()
{

}


StatusEffect::~StatusEffect()
{

}

void StatusEffect::Initialize( )
{
	// Lingering status effects that have immediate reprecussions.
	switch ( myType )
	{
		case DOT:

			break;

		case SPECIAL:
			HandleSpecial( );

			break;

		case STAT:
			HandleStat( );
			break;
	}
}

void  StatusEffect::React(Character* owner, ETYPE ele = PHYS)
{
	// Happens when unit is struck by an attack
	
}

void  StatusEffect::Turntick()
{
	// Apply whatever status effect does
	// Finite State time!
	//int deal_damage = dmg_tick;
	curr_tick++;
	
	switch(myType)
	{
		case DOT:

			// Turnmanager Attack/DoDamage to player call instead?
			HandleDOT();
			break;

		case SPECIAL:
			//HandleSpecial();
			
			break;

		case STAT:
			//HandleStat();
			break;
	}

	
}

void StatusEffect::HandleSpecial()
{
	
}

void StatusEffect::HandleStat()
{

}

void StatusEffect::HandleDOT()
{
	owner->TakeDamage( dmg_tick );
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
	element = rhs.element;
	myType = rhs.myType;


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

ETYPE StatusEffect::GetElement( )
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
void StatusEffect::SetElement( ETYPE ele )
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

void StatusEffect::SetType( int type )
{
	myType = (STYPE) type;
}