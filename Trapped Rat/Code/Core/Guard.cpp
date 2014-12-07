
#include "Guard.h"
#include "../Core/GameData.h"



Guard::Guard()
{
	ansys = new AnimationSystem();
	ansys->Load("GuardOverWorld.xml");
}


Guard::~Guard()
{
	delete ansys;
}
void Guard::Update(float dt)
{
	//ansys->ResetAll();
	
	if (velocity.x > 0.0f)
	{
		ansys->Play("Right");
	}
	if (velocity.x < 0.0f)
	{
		ansys->Play("Left");
	}
	if (velocity.y > 0.0f)
	{
		ansys->Play("Down");
	}
	if (velocity.y < 0.0f)
	{
		ansys->Play("Up");
	}
	else
	{
		ansys->Play("Right");
	}
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
