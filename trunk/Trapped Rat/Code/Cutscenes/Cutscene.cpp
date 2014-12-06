#include "Cutscene.h"
#include "../Core/GameData.h"
#include "../Tile/TileSystem.h"
#include "../States/GamePlayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
Cutscene::Cutscene()
{

}
Cutscene::~Cutscene()
{
	for (size_t i = 0; i < m_vScenes.size(); i++)
	{
		for (size_t j = 0; j < m_vScenes[i]->m_vObjects.size(); j++)
		{
			delete m_vScenes[i]->m_vObjects[j]->ansys;
			delete m_vScenes[i]->m_vObjects[j];
		}
		m_vScenes[i]->m_vObjects.clear();
		delete m_vScenes[i];
	}
	m_vScenes.clear();

	delete CameraObject;
	CameraObject = nullptr;

	delete dlg;
	dlg = nullptr;
}
void Cutscene::LoadCutscene(int _index)
{
	if (_index == 0)
	{
		
		Scene* temp = new Scene();
		SceneObject* tempobj = new SceneObject();

		//Cause it to not be paused
		tempobj->is_waiting = false;

		//Load the Animation and start it
		tempobj->ansys = new AnimationSystem();
		tempobj->ansys->Load("DogAnimBattle.xml");
		tempobj->ansys->Play(0);

		//Setup Objects Initial Waypoint  and Velocities
		tempobj->currentWaypoint = 0;
		tempobj->velocity.x = 3;
		tempobj->velocity.y = 3;

		tempobj->position.x = 100;
		tempobj->position.y = 200;

		tempobj->currentWait = 10.0f;

		//Setup Waypoints
		SGD::Point point = SGD::Point(100, 200);
		//tempobj->m_vWaypoints.push_back(point);
		point.x += 200;
		tempobj->m_vWaypoints.push_back(point);
		point.y += 400;
		tempobj->m_vWaypoints.push_back(point);
		point.x += 400;
		tempobj->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(tempobj);

		//ADD WAIT TIMES
		tempobj->m_vWaitTimes.push_back(10.0f);
		tempobj->m_vWaitTimes.push_back(40.0f);
		tempobj->m_vWaitTimes.push_back(20.0f);


		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(400, 400));
		camVelocity.x = 10;

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/testdialogue.xml");
	}
}
void Cutscene::Update(float dt)
{	
	if (dlg != nullptr)
	{
		if (!dlg->Update(dt))
			GamePlayState::GetInstance()->SetState(GPStates::Town);
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		GamePlayState::GetInstance()->SetState(GPStates::Town);

	}
	nextScene -= dt;
	if (nextScene <= 0.0f)
	{
		nextScene = 5.0f;
		if (currScene + 1 < (int)m_vScenes.size())
			currScene++;
	}

	SGD::Point campos = CameraObject->GetPosition();
	campos.x += camVelocity.x * dt;
	campos.y += camVelocity.y * dt;
	CameraObject->SetPosition(campos);
	GameData::GetInstance()->UpdateCamera(CameraObject);

	for (size_t i = 0; i < m_vScenes[currScene]->m_vObjects.size(); i++)
	{
		SceneObject * obj = m_vScenes[currScene]->m_vObjects[i];

		if (obj->is_waiting)
		{
			obj->currentWait -= dt;
			if (obj->currentWait <= 0.0f || SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
			{
				if (obj->m_vWaitTimes.size() > obj->currentWaypoint)
				{
					obj->currentWait = obj->m_vWaitTimes[obj->currentWaypoint - 1];
				}
				else
				{
					obj->currentWait = 0.0f;
				}
				obj->is_waiting = false;
			}
		}
		else
		{
			obj->position.x += obj->velocity.x;
			obj->position.y += obj->velocity.y;
			obj->velocity.x = 0;
			obj->velocity.y = 0;
			if (abs(obj->position.x - obj->m_vWaypoints[obj->currentWaypoint].x) > 4.0f)
			{
				if (obj->position.x < obj->m_vWaypoints[obj->currentWaypoint].x)
				{
					obj->velocity.x = 3;
				}
				else if (obj->position.x != obj->m_vWaypoints[obj->currentWaypoint].x)
				{
					obj->velocity.x = -3;
				}
			}
			if (abs(obj->position.y - obj->m_vWaypoints[obj->currentWaypoint].y) > 4.0f)
			{
				if (obj->position.y < obj->m_vWaypoints[obj->currentWaypoint].y)
				{
					obj->velocity.y = 3;
				}
				else if (obj->position.y != obj->m_vWaypoints[obj->currentWaypoint].y)
				{
					obj->velocity.y = -3;
				}
			}
			if (abs(obj->m_vWaypoints[obj->currentWaypoint].x - obj->position.x) < 5.0f && abs(obj->m_vWaypoints[obj->currentWaypoint].y - obj->position.y) < 5.0f)
			{
				obj->velocity.x = 0;
				obj->velocity.y = 0;
				//go to next waypoint
				if (obj->currentWaypoint + 1 < obj->m_vWaypoints.size())
				{
					obj->currentWaypoint++;
					obj->is_waiting = true;

					/*if (obj->position.x < obj->m_vWaypoints[obj->currentWaypoint].x)
					{
						obj->velocity.x = 3;
					}
					else if (obj->position.x != obj->m_vWaypoints[obj->currentWaypoint].x)
					{
						obj->velocity.x = -3;
					}
					if (obj->position.y < obj->m_vWaypoints[obj->currentWaypoint].y)
					{
						obj->velocity.y = 3;
					}
					else if (obj->position.y != obj->m_vWaypoints[obj->currentWaypoint].y)
					{
						obj->velocity.y = -3;
					}*/
				}
			}
		}
		obj->ansys->Update(dt);
	}	
}
void Cutscene::Render()
{
	TileSystem::GetInstance()->Render();

	for (size_t i = 0; i < m_vScenes[currScene]->m_vObjects.size(); i++)
	{
		SceneObject * obj = m_vScenes[currScene]->m_vObjects[i];
		obj->ansys->Render(obj->position.x, obj->position.y);
	}
	if (dlg!=nullptr)
	{
		dlg->Render();
	}
}
