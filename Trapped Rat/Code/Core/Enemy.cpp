
#include "Enemy.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "TurnManager.h"
#include <sstream>
#include "../Font/Font.h"
#include "GameData.h"


Enemy::Enemy()
	{
	}


Enemy::~Enemy()
	{
	if ( ansys != nullptr )
		delete ansys;
	}
void Enemy::Update( float dt )
	{

	if ( TurnManager::GetInstance()->getTimeStop() == false && alive )
		{
		progress += speed * dt;
		}

	else if ( progress < 100.0f )
		{
		if (stepbackward == true && stepforward == false)
		{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x += stepvelocity*dt;
			SetPosition(pos);
			if (stepTime <= 0.0f)
				stepbackward = false;
		}
		if (stepbackward == false && stepforward == false)
		{
			TurnManager::GetInstance()->setProgressFullReached(false);
			TurnManager::GetInstance()->setTimeStop(false);
		}
		return;
		}

	if (progress >= 100.0f)
	{
		if (!TurnManager::GetInstance()->getProgressFullReached())
		{
			StatusTick();

			if (!alive)
			{
				progress = 0.0f;
				return;
			}
			TurnManager::GetInstance()->setProgressFullReached(true);
			TurnManager::GetInstance()->setTimeStop(true);
			return;
		}

		if (HasEffect("Stun"))
		{
			// Lose your turn if stunned
			progress = 0.0f;
			return;
		}

		if (HasEffect("Confused"))
		{

			// Do Confused Action (attack random target, anyone)
			int targets = TurnManager::GetInstance()->GetAll().size();
			int hit_this_guy = rand() % targets;

			Attack(this, TurnManager::GetInstance()->GetAll()[hit_this_guy]);

			TurnManager::GetInstance()->setProgressFullReached(false);
			TurnManager::GetInstance()->setTimeStop(false);
			TurnManager::GetInstance()->setTurnPause(true);

			progress = 0.0f;
			return;
		}

		if (stepforward == false && stepbackward == false)
		{
			stepforward = true;
			stepTime = 2.0f;
		}
		if (stepforward == true && stepbackward == false)
		{
			stepTime -= dt;
			SGD::Point pos = GetPosition();
			pos.x -= stepvelocity*dt;
			SetPosition(pos);
			if (stepTime <= 0.0f)
				stepforward = false;
		}

		if (stepbackward == false && stepforward == false)
		{
			// Enemy Select Player
			int pool = TurnManager::GetInstance()->GetAllies().size();
			std::vector<int> living;

			for (int i = 0; i < pool; i++)
			{
				if (TurnManager::GetInstance()->GetAllies()[i]->isAlive())
					living.push_back(i);
			}

			if (living.size() > 0)
				pool = rand() % (living.size());
			else
				pool = -1;


			//if ( SGD::InputManager::GetInstance( )->IsKeyPressed( SGD::Key::Enter ) )
			{
				if (pool == -1) // Hack job to prevent enemies from murdering dead players/breaking code
				{
					progress = 0.0f;
					TurnManager::GetInstance()->setTimeStop(false);
					return;
				}

				Attack(this, TurnManager::GetInstance()->GetAllies()[living[pool]]);
				if (stepbackward == false && stepforward == false && progress != 0.0f)
				{
					stepbackward = true;
					stepTime = 2.0f;
					progress = 0.0f;
				}
				//Attack( this, TurnManager::GetInstance()->GetAllies()[living[pool]] );
				progress = 0.0f;
				TurnManager::GetInstance()->setProgressFullReached(false);
				TurnManager::GetInstance()->setTurnPause(true);
			}
		}
	}
	}
void Enemy::UpdateAnimation( float dt )
	{
	if ( ansys != nullptr )
		{
		ansys->Update( dt );
		}
	Character::Update( dt );
	}
void Enemy::Render()
	{
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 51, position.y - 30, { 0, 0, 0 }, 1.6f );
	GameData::GetInstance()->GetFont()->DrawString( name.c_str(), position.x + 50, position.y - 30, { 200, 0, 0 }, 1.6f );
	std::ostringstream sp;
	sp << "Progress: " << progress;
	GameData::GetInstance()->GetFont()->DrawString( sp.str(), position.x + 50, position.y - 10, { 255, 0, 0 } );
	std::ostringstream sa;
	sa << "Attack: " << stats.attack;
	GameData::GetInstance()->GetFont()->DrawString( sa.str(), position.x + 50, position.y + 5, { 0, 0, 0 } );
	std::ostringstream sd;
	sd << "Defense: " << stats.defense;
	GameData::GetInstance()->GetFont()->DrawString( sd.str(), position.x + 50, position.y + 15, { 0, 0, 0 } );
	std::ostringstream sm;
	sm << "Magic: " << stats.magic;
	GameData::GetInstance()->GetFont()->DrawString( sm.str(), position.x + 50, position.y + 25, { 0, 0, 0 } );
	std::ostringstream sav;
	sav << "Avoision: " << stats.avoision;
	GameData::GetInstance()->GetFont()->DrawString( sav.str(), position.x + 50, position.y + 35, { 0, 0, 0 } );
	std::ostringstream ss;
	ss << "Speed: " << speed;
	GameData::GetInstance()->GetFont()->DrawString( ss.str(), position.x + 50, position.y + 45, { 0, 0, 0 } );

	if ( ansys != nullptr )
		{
		ansys->Render( position.x, position.y );
		}


	Character::Render();
	}
void Enemy::BehaviorAI()
	{

	}

void Enemy::SetAnimations( AnimationSystem* _an )
	{
	if ( ansys != nullptr )
		delete ansys;
	ansys = _an;
	}
AnimationSystem* Enemy::GetAnimations()
	{
	return ansys;
	}
int Enemy::GetType()
	{
	return type::Enemy;
	}


int Enemy::GetXPValue( )
{
	return xp_value;
}

int Enemy::GetLevel( )
{
	return level;
}

void Enemy::SetLevel( int lev )
{
	level = lev;
}

void Enemy::SetXPVal( int val )
{
	xp_value = val;
}