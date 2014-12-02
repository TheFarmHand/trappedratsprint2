
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
		progress += speed * dt;
	else if ( progress < 100.0f )
	{
		return;
	}

	if ( progress >= 100.0f )
	{
		if ( !TurnManager::GetInstance()->getProgressFullReached() )
		{
			StatusTick();
			if ( !alive )
				{
				progress = 0.0f;
				return;
				}
			TurnManager::GetInstance()->setProgressFullReached( true );
			TurnManager::GetInstance()->setTimeStop( true );
			return;
		}

		// Enemy Select Player
		int pool = TurnManager::GetInstance()->GetAllies().size();
		std::vector<int> living;

		for ( int i = 0; i < pool; i++ )
		{
			if ( TurnManager::GetInstance()->GetAllies()[ i ]->isAlive() )
				living.push_back( i );
		}

		if ( living.size() > 0 )
			pool = rand() % ( living.size() );
		else
			pool = -1;


		//if ( SGD::InputManager::GetInstance( )->IsKeyPressed( SGD::Key::Enter ) )
		{
			if ( pool == -1 ) // Hack job to prevent enemies from murdering dead players/breaking code
			{
				progress = 0.0f;
				TurnManager::GetInstance()->setTimeStop( false );
				return;
			}

			TurnManager::GetInstance()->AttackTarget( this, TurnManager::GetInstance()->GetAllies()[ living[ pool ] ], 8 );
			//Attack( this, TurnManager::GetInstance()->GetAllies()[living[pool]] );
			progress = 0.0f;
			TurnManager::GetInstance()->setProgressFullReached( false );
			TurnManager::GetInstance()->setTimeStop( false );
			TurnManager::GetInstance()->setTurnPause( true );
		}
	}
}
void Enemy::UpdateAnimation( float dt )
{
	if ( ansys != nullptr )
	{
		ansys->Update( dt );
	}
	Character::Update(dt);
}
void Enemy::Render()
{
	std::ostringstream ss;
	ss << "Speed: " << speed;
	SGD::GraphicsManager::GetInstance()->DrawString( name.c_str(), position, SGD::Color( 255, 255, 0, 0 ) );
	GameData::GetInstance()->GetFont()->DrawString( ss.str(), position.x, position.y + 30, { 0, 255, 0 } );
	std::ostringstream sp;
	sp << "Progress: " << progress;
	GameData::GetInstance()->GetFont()->DrawString( sp.str(), position.x, position.y + 50, { 0, 255, 255 } );

	if ( ansys != nullptr )
	{
		ansys->Render( position.x, position.y );
	}

	for ( auto iter = effects.begin( ); iter != effects.end( ); iter++ )
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture((*iter)->GetIcon(), {position.x, position.y-size.height-5});
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