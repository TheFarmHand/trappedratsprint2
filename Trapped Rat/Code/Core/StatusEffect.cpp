
#include "StatusEffect.h"
#include "Ability.h"
#include "Character.h"
#include "StatusEffectManager.h"


StatusEffect::StatusEffect()
{

}


StatusEffect::~StatusEffect()
{

}

void StatusEffect::Initialize()
{
	// Lingering status effects that have immediate reprecussions.
	switch ( myType )
	{
		case DOT:

			break;

		case SPECIAL:
			HandleSpecial();

			break;

		case STAT:
			HandleStat();
			break;
	}
}

void  StatusEffect::React( Character* owner, ETYPE ele = PHYS )
// Dilapidated function I think
{
	// Happens when unit is struck by an attack

}

void  StatusEffect::Turntick()
{
	// Apply whatever status effect does
	// Finite State time!
	//int deal_damage = dmg_tick;
	curr_tick++;

	switch ( myType )
	{
		case DOT:

			// Turnmanager Attack/DoDamage to player call instead?
			HandleDOT();
			break;

		case SPECIAL:
			if ( name == "Cover" )
				curr_tick--;

			break;

		case STAT:

			break;
	}

}

int StatusEffect::ElementalMod()
{
	// Modifies damage by elemental types
	int damage = dmg_tick;
	if ( element == FIRE )
	{
		if ( owner->GetEType() == WIND )	// Resist Damage
		{
			return damage / 2;
		}

		else if ( owner->GetEType() == EARTH )
		{
			return (int)( damage * 1.5f );
		}
	}

	else if ( element == WATER )
	{
		if ( owner->GetEType() == WIND )
		{
			return (int)( damage * 1.5f );
		}

		if ( owner->GetEType() == EARTH )
		{
			return damage / 2;
		}
	}

	else if ( element == WIND )
	{
		if ( owner->GetEType() == FIRE )	// Extra Damage
		{
			return (int)( damage * 1.5f );
		}

		if ( owner->GetEType() == WATER )
		{
			return damage / 2;
		}
	}

	else if ( element == EARTH )
	{
		if ( owner->GetEType() == FIRE )
		{
			return damage / 2;
		}

		if ( owner->GetEType() == WATER )
		{
			return (int)( damage * 1.5f );
		}
	}

	else if ( element == MULTI )		// is this still a thing?
	{
		// What do I do with this? phys + ele I think
		return damage;
	}

	// Physical
	return damage;
}

void StatusEffect::HandleSpecial()
// Does things as soon as status is applied
{
	StatusEffectManager* SEM = StatusEffectManager::GetInstance();
	// Delerium
	if ( name == "Delerium" )
	{
		// Start a timer, but how to save it?
	}

	if ( name == "Confused" )
	{
		if ( ternary_effect )
			num_ticks += 2;
	}

	// Advanced Cover

	// Counter

	// Enfire

	// Hedge
	//if ( name == "Hedge" )
	//{
	//	owner->AddStatus( &SEM->GetStatus( "Regen" ) );
	//	owner->AddStatus( &SEM->GetStatus( "DefenseUp" ) );
	//}

}

void StatusEffect::HandleStat()
{
	if ( name == "SpeedDown" )
	{
		stat_value = owner->GetSpeed() * .333f;

		owner->SetSpeed( owner->GetSpeed() - stat_value );
	}

	else if ( name == "SpeedUp" )
	{
		stat_value = owner->GetSpeed() * .333f;
		if ( ternary_effect ) stat_value *= 2;
		owner->SetSpeed( owner->GetSpeed() + stat_value );
	}

	else if ( name == "AttackUp" )
	{
		stat_value = owner->GetAttack() * .333f;
		owner->GetStats().attack += (int)stat_value;
	}

	else if ( name == "AttackDown" )
	{
		stat_value = owner->GetAttack() * .333f;
		owner->GetStats().attack -= (int)stat_value;
	}

	else if ( name == "DefenseUp" )
	{
		stat_value = owner->GetDefense() * .333f;
		owner->GetStats().defense += (int)stat_value;
	}

	else if ( name == "DefenseDown" )
	{
		stat_value = owner->GetDefense() * .333f;
		owner->GetStats().defense -= (int)stat_value;
	}

	else if ( name == "MagicUp" )
	{
		stat_value = owner->GetMagic() * .333f;
		owner->GetStats().magic += (int)stat_value;
	}

	else if ( name == "MagicDown" )
	{
		stat_value = owner->GetMagic() * .333f;
		owner->GetStats().magic -= (int)stat_value;
	}

	else if ( name == "AvoisionUp" )
	{
		stat_value = owner->GetAvoision() * .333f;
		owner->GetStats().avoision += (int)stat_value;
		if ( ternary_effect ) owner->AddStatus( &StatusEffectManager::GetInstance()->GetStatus( "DefenseUp" ) );
	}

	else if ( name == "AvoisionDown" )
	{
		stat_value = owner->GetAvoision() * .333f;
		owner->GetStats().avoision -= (int)stat_value;
	}
}

void StatusEffect::HandleDOT()
{
	if ( name == "Regen" )
		owner->TakeDamage( -dmg_tick );
	else
		owner->TakeDamage( ElementalMod(),false,true );	// Tweaks numbers according to Elemental affinities

}



void StatusEffect::Clear()
// Removes this effect from the player
{
	if ( owner == NULL )
	{
		// What are you doing?
		return;
	}

	std::list<StatusEffect*>::iterator iter = owner->GetEffects().begin();
	while ( ( *iter ) != this )
	{
		iter++;
	}

	
	Recover();	// Stats
	owner->GetEffects().erase( iter );
	
	delete this;
}

void StatusEffect::Recover()
{
	if ( name == "SpeedDown" )
	{
		owner->SetSpeed( owner->GetSpeed() + stat_value );
	}

	if ( name == "SpeedUp" )
	{
		owner->SetSpeed( owner->GetSpeed() - stat_value );
	}

	else if ( name == "AttackUp" )
	{
		owner->GetStats().attack -= (int)stat_value;
	}

	else if ( name == "AttackDown" )
	{
		owner->GetStats().attack += (int)stat_value;
	}

	else if ( name == "DefenseUp" )
	{
		owner->GetStats().defense -= (int)stat_value;
	}

	else if ( name == "DefenseDown" )
	{
		owner->GetStats().defense += (int)stat_value;
	}

	else if ( name == "MagicUp" )
	{
		owner->GetStats().magic -= (int)stat_value;
	}

	else if ( name == "MagicDown" )
	{
		owner->GetStats().magic += (int)stat_value;
	}

	else if ( name == "AvoisionUp" )
	{
		owner->GetStats().avoision -= (int)stat_value;
	}

	else if ( name == "AvoisionDown" )
	{
		owner->GetStats().avoision += (int)stat_value;
	}

	else if ( name == "Guarding" )
	{
		if(guard_caster->HasEffect("Cover") )
			guard_caster->RemoveEffect( "Cover" );
	}
}

StatusEffect& StatusEffect::operator=( const StatusEffect& rhs )
{
	owner = rhs.owner;
	name = rhs.name;
	element = rhs.element;
	icon = rhs.icon;
	num_ticks = rhs.num_ticks;
	curr_tick = rhs.curr_tick;
	dmg_tick = rhs.dmg_tick;
	element = rhs.element;
	myType = rhs.myType;
	guard_caster = rhs.guard_caster;
	stat_value = rhs.stat_value;


	return *this;
}


// Access Granted
Character* StatusEffect::GetOwner()
{
	return owner;
}

std::string StatusEffect::GetName()
{
	return name;
}

ETYPE StatusEffect::GetElement()
{
	return element;
}

SGD::HTexture StatusEffect::GetIcon()
{
	return icon;
}

int StatusEffect::GetCurrTick()
{
	return curr_tick;
}

int StatusEffect::GetNumTicks()
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
	myType = (STYPE)type;
}