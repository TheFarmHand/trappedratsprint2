
#include "Player.h"
#include "Items.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../States/GamePlayState.h"
#include "GameData.h"
#include "..\Tile\TileSystem.h"
#include "../SGD Wrappers/SGD_Event.h"


Player::Player() : Listener(this)
{
	//start the player on a tile
	size = SGD::Size(32, 32);
	image = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/rat.png");
	ansys = new AnimationSystem();
	ansys->Load("RatAnimOverworld.xml");
	ansys->Play(0);
	velocity = SGD::Vector(0, 0);
	Listener::RegisterForEvent( "World1" );
	Listener::RegisterForEvent( "House1" );
	Listener::RegisterForEvent( "World2" );
	Listener::RegisterForEvent( "House2" );
	Listener::RegisterForEvent( "World3" );
	Listener::RegisterForEvent( "House3" );
}


Player::~Player()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
	image = SGD::INVALID_HANDLE;
	delete ansys;
}
void Player::Update(float dt)
{
	//udpate based on movement
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	char dir = 'a';

	//if they are holding down the button
		//move at a constant rate
	if (input->IsKeyDown(SGD::Key::Up) && (int)(position.x - size.width / 2) % 8 == 0)
	{
		velocity.y = -4.0f;
		velocity.x = 0.0f;
		dir = 'u';
	}
	else if (input->IsKeyDown(SGD::Key::Down) && (int)(position.x - size.width / 2) % 8 == 0)
	{
		velocity.y = 4.0f;
		velocity.x = 0.0;
		dir = 'd';
	}
	else if (input->IsKeyDown(SGD::Key::Left)&& (int)(position.y - size.height / 2) % 8 == 0)
	{
		velocity.y = 0.0f;
		velocity.x = -4.0f;
		dir = 'l';
	}
	else if (input->IsKeyDown(SGD::Key::Right) && (int)(position.y - size.height / 2) % 8 == 0)
	{
		velocity.y = 0.0f;
		velocity.x = 4.0f;
		dir = 'r';
	}
	else
	{
		//if they have moved the right amount..stop

		/*if ((int)position.x % 32 == 0 && velocity.x != 0)
			velocity.x = 0.0f;
		if ((int)position.y % 32 == 0 && dvelocity.y != 0)
			velocity.y = 0.0f;*/
		if ((int)(position.x - size.width / 2) % 8 == 0 && (int)(position.y - size.height / 2) % 8 == 0)
		{
 			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}
		
	}

	//update the position based on velocity
	position += velocity;
	TileSystem::GetInstance()->TileCollision( position.x, position.y, GetRect(), dir );
	TileSystem::GetInstance()->TileEvent( position );
	//swap to proper animation if player changed direction
	static char prevdir = 'a';
	if (prevdir != dir)
	{
		prevdir = dir;
		switch (dir)
		{	
		case 'u':
			ansys->ResetAll();
			ansys->Play("Up");
			break;
		case 'd':
			ansys->ResetAll();
			ansys->Play("Down");
			break;
		case 'r':
			ansys->ResetAll();
			ansys->Play("Right");
			break;
		case 'l':
			ansys->ResetAll();
			ansys->Play("Left");
			break;
		default:
			break;
		}
	}
	
	//update the animation
	ansys->Update(dt);
	//if Not Moving, Pause
	if (!IsMoving())
	{
		prevdir = 'a';
		ansys->Pause(true);
	}
	
}
void Player::Render()
{
	SGD::Rectangle rect = { position, size };
	SGD::Point camera = GameData::GetInstance()->GetCamera();
	SGD::Point center = { (position.x-size.width/2) - camera.x, (position.y- size.height/2) - camera.y };
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(image, center, { 132, 1, 164, 33 });
	ansys->Render(position.x,position.y);

}
bool Player::IsMoving()
{
	return velocity.y != 0.0 || velocity.x != 0.0;
}

void Player::HandleEvent( const SGD::Event* pEvent )
	{
	if ( pEvent->GetEventID() == "World1" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House1" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "World2" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + ( size.width / 2 );
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House2" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "World3" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + ( size.width / 2 );
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House3" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	}