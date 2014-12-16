
#include "Guard.h"
#include "../Core/GameData.h"
#include "../Tile/TileSystem.h"


Guard::Guard()
{
	ansys = new AnimationSystem();
	size = SGD::Size( 32, 32 );
	ansys->Load("GuardOverWorld.xml");
	ansys->Play(0);
}


Guard::~Guard()
{
	delete ansys;
}
void Guard::Update(float dt)
{
	//ansys->ResetAll();
	ansys->Update(dt);
	char dir = 'p';
	AquireTarget();
	SGD::Rectangle tempRect;
	tempRect.left = (position.x + velocity.x * dt) - size.width / 2;
	tempRect.top = position.y - size.height / 2;
	tempRect.right = (position.x + velocity.x * dt) + size.width / 2;
	tempRect.bottom = position.y + size.height / 2;
	
	if ( !TileSystem::GetInstance()->IsColliding( position.x + velocity.x * dt, position.y, tempRect ) )
		position.x += velocity.x * dt;
	
	tempRect.left = position.x - size.width / 2;
	tempRect.top = ( position.y + velocity.y * dt ) - size.height / 2;
	tempRect.right = position.x + size.width / 2;
	tempRect.bottom = ( position.y + velocity.y * dt ) + size.height / 2;
	
	if ( !TileSystem::GetInstance()->IsColliding( position.x, position.y + velocity.y * dt, tempRect ) )
		position.y += velocity.y * dt;
	
	changedir -= dt;
	if (velocity.x > 0.0f && changedir < 0.0f)
	{
		ansys->ResetAll();
		ansys->Play("Right");
		dir = 'r';
	}
	if (velocity.x < 0.0f && changedir < 0.0f)
	{
		ansys->ResetAll();
		ansys->Play("Left");
		dir = 'l';
	}
	if (velocity.y > 0.0f && changedir < 0.0f)
	{
		ansys->ResetAll();
		ansys->Play("Down");
		dir = 'd';
	}
	if (velocity.y < 0.0f && changedir < 0.0f)
	{
		ansys->ResetAll();
		ansys->Play("Up");
		dir = 'u';
	}
	//TileSystem::GetInstance()->TileCollision( position.x, position.y, GetRect(), dir );
	//else
	//{
	//	ansys->ResetAll();
	//	ansys->Play("Right");
	//}
}
void Guard::Render()
{
	SGD::Rectangle rect = { position, size };
	SGD::Point camera = GameData::GetInstance()->GetCamera();
	SGD::Point center = { (position.x - size.width / 2) - camera.x, (position.y - size.height / 2) - camera.y };
	//SGD::GraphicsManager::GetInstance()->DrawTextureSection(image, center, { 132, 1, 164, 33 });
	ansys->Render(position.x, position.y);
}
void Guard::BehaviorAI()
{

}
void Guard::AddWayPoint(int _waypoint)
{
	waypoints.push_back(_waypoint);
}
void Guard::SetStartPos(int _pos)
{
	startpos = _pos;
}
void Guard::AquireTarget()
{
	//Check within Range
	float xdiff = abs(position.x - GameData::GetInstance()->GetOverworldPlayer()->GetPosition().x);
	float ydiff = abs(position.y - GameData::GetInstance()->GetOverworldPlayer()->GetPosition().y);
	if (changedir <= 0.0f)
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		if (xdiff < 100.0f && ydiff < 100.0f)
		{
			//Within Range,  Follow Target
			if (xdiff > ydiff)
			{
				if (position.x < GameData::GetInstance()->GetOverworldPlayer()->GetPosition().x)
				{
					velocity.x = 16.0f;
				}
				else
				{
					velocity.x = -16.0f;
				}
			}
			else
			{
				if (position.y < GameData::GetInstance()->GetOverworldPlayer()->GetPosition().y)
				{
					velocity.y = 16.0f;
				}
				else
				{
					velocity.y = -16.0f;
				}
			}
		}
		else
		{
			if (waypoints.size() <= 0)
				return;
			xdiff = abs(position.x - (TileSystem::GetInstance()->GetTilePosition(waypoints[currentWaypoint]).x + 17.0f));
			ydiff = abs(position.y - (TileSystem::GetInstance()->GetTilePosition(waypoints[currentWaypoint]).y + 17.0f));
			//Go to Waypoint
			if (xdiff < 1.0f && ydiff < 1.0f)
			{
				if (currentWaypoint + 1 < (int)waypoints.size())
				{
					currentWaypoint++;
				}
				else
					currentWaypoint = 0;
			}

			if (xdiff > ydiff)
			{
				if (position.x < TileSystem::GetInstance()->GetTilePosition(waypoints[currentWaypoint]).x + 17.0f)
				{
					velocity.x = 32.0f;
				}
				else
				{
					velocity.x = -32.0f;
				}
			}
			else
			{
				if (position.y < TileSystem::GetInstance()->GetTilePosition(waypoints[currentWaypoint]).y + 17.0f)
				{
					velocity.y = 32.0f;
				}
				else
				{
					velocity.y = -32.0f;
				}
			}
		}
		changedir = 0.5f;
	}
}