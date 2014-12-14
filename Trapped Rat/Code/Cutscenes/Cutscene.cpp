#include "Cutscene.h"
#include "../Core/GameData.h"
#include "../Tile/TileSystem.h"
#include "../States/GamePlayState.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../States/GameOverWinState.h"
#include "CutsceneManager.h"
#include "../Font/Font.h"
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
		tempobj->ansys->Load("CombatAnimations/DogAnimBattle.xml");
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
		dlg->Load("../Trapped Rat/Assets/Scripts/testdialogue.xml", true, {200,1000});
	}
	else if (_index == 1)
	{
		//Tutorial Cutscene
		Scene* temp = new Scene();
		SceneObject* mom = new SceneObject();
		SceneObject* dad = new SceneObject();
		SceneObject* cecil = new SceneObject();
		SceneObject* rat = new SceneObject();

		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(2000, 950));
		camVelocity.y = 0;
		camVelocity.x = 0;

		temp->index = _index;
		temp->eventTimer = 10.0f;
		//who moves first
		mom->is_waiting = true;
		dad->is_waiting = true;
		cecil->is_waiting = true;
		rat->is_waiting = false;

		//Load the Animation and start it
		mom->ansys = new AnimationSystem();
		mom->ansys->Load("RatAnimOverworld.xml");
		mom->ansys->Play("Left");

		dad->ansys = new AnimationSystem();
		dad->ansys->Load("RatAnimOverworld.xml");
		dad->ansys->Play("Left");

		cecil->ansys = new AnimationSystem();
		cecil->ansys->Load("OverworldCecil.xml");
		cecil->ansys->Play(0);

		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimOverworld.xml");
		rat->ansys->Play("Up");


		//Setup Objects Initial Waypoint  and Velocities
		mom->currentWaypoint = 0;
		dad->currentWaypoint = 0;
		cecil->currentWaypoint = 0;
		rat->currentWaypoint = 0;

		mom->velocity.x = 3;
		mom->velocity.y = 3;

		mom->position.x = 2000;
		mom->position.y = 1000;

		mom->currentWait = 2.0f;

		dad->velocity.x = 3;
		dad->velocity.y = 3;

		dad->position.x = 2040;
		dad->position.y = 1000;

		dad->currentWait = 2.0f;


		cecil->velocity.x = 3;
		cecil->velocity.y = 3;

		cecil->position.x = 2000;
		cecil->position.y = 700;

		cecil->currentWait = 5.0f;

		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 2000;
		rat->position.y = 800;

		rat->currentWait = 2.0f;

		//Setup Waypoints
		SGD::Point point = SGD::Point(2000, 1000);
		
		point.x += 200;
		mom->m_vWaypoints.push_back(point);
		point.x += -200;
		mom->m_vWaypoints.push_back(point);
		point.x += 200;
		mom->m_vWaypoints.push_back(point);
		point.x += -200;
		mom->m_vWaypoints.push_back(point);
		point.x += 200;
		mom->m_vWaypoints.push_back(point);
		point.x += -200;
		mom->m_vWaypoints.push_back(point);
		point.x += 200;
		mom->m_vWaypoints.push_back(point);
		point.x += -200;
		mom->m_vWaypoints.push_back(point);
		point.x += 200;
		mom->m_vWaypoints.push_back(point);
		point.x += -200;
		mom->m_vWaypoints.push_back(point);

		point = SGD::Point(2040, 1000);

		point.x += 200;
		dad->m_vWaypoints.push_back(point);
		point.x += -200;
		dad->m_vWaypoints.push_back(point);
		point.x += 200;
		dad->m_vWaypoints.push_back(point);
		point.x += -200;
		dad->m_vWaypoints.push_back(point);
		point.x += 200;
		dad->m_vWaypoints.push_back(point);
		point.x += -200;
		dad->m_vWaypoints.push_back(point);
		point.x += 200;
		dad->m_vWaypoints.push_back(point);
		point.x += -200;
		dad->m_vWaypoints.push_back(point);
		point.x += 200;
		dad->m_vWaypoints.push_back(point);
		point.x += -200;
		dad->m_vWaypoints.push_back(point);

		point = SGD::Point(2000, 700);

		point.y += 50;
		cecil->m_vWaypoints.push_back(point);
		point.y += 50;
		cecil->m_vWaypoints.push_back(point);
		point.y += 200;
		cecil->m_vWaypoints.push_back(point);

		point = SGD::Point(2000, 800);

		rat->m_vWaypoints.push_back(point);
		rat->m_vWaypoints.push_back(point);
		rat->m_vWaypoints.push_back(point);
		rat->m_vWaypoints.push_back(point);
		rat->m_vWaypoints.push_back(point);
		rat->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(mom);
		temp->m_vObjects.push_back(dad);
		temp->m_vObjects.push_back(cecil);
		temp->m_vObjects.push_back(rat);

		//ADD WAIT TIMES
		for (size_t i = 0; i < 20; i++)
		{
			mom->m_vWaitTimes.push_back(2.0f);
			dad->m_vWaitTimes.push_back(2.0f);
			rat->m_vWaitTimes.push_back(2.0f);
			cecil->m_vWaitTimes.push_back(3.0f);
		}
		mom->m_vWaitTimes.push_back(10.0f);
		mom->m_vWaitTimes.push_back(40.0f);
		mom->m_vWaitTimes.push_back(20.0f);

		dad->m_vWaitTimes.push_back(10.0f);
		dad->m_vWaitTimes.push_back(40.0f);
		dad->m_vWaitTimes.push_back(20.0f);

		cecil->m_vWaitTimes.push_back(10.0f);
		cecil->m_vWaitTimes.push_back(40.0f);
		cecil->m_vWaitTimes.push_back(20.0f);

		rat->m_vWaitTimes.push_back(10.0f);
		rat->m_vWaitTimes.push_back(40.0f);
		rat->m_vWaitTimes.push_back(20.0f);

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/TutCutsceneIntro.xml", true, {500,500});
	}
	else if (_index == 2)
	{
		//Leaving Tutorial Cutscene
		Scene* temp = new Scene();
		SceneObject* mom = new SceneObject();
		SceneObject* dad = new SceneObject();
		SceneObject* cecil = new SceneObject();
		SceneObject* rat = new SceneObject();
		SceneObject* Ratsptuin = new SceneObject();


		temp->index = _index;
		temp->eventTimer = 11.0f;


		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(1950, 950));
		camVelocity.y = 0;
		camVelocity.x = 0;

		//who moves first
		mom->is_waiting = false;
		dad->is_waiting = false;
		cecil->is_waiting = true;
		rat->is_waiting = false;
		Ratsptuin->is_waiting = true;

		//Load the Animation and start it
		mom->ansys = new AnimationSystem();
		mom->ansys->Load("RatAnimBattle.xml");
		mom->ansys->Play("dead");

		dad->ansys = new AnimationSystem();
		dad->ansys->Load("RatAnimBattle.xml");
		dad->ansys->Play("dead");

		cecil->ansys = new AnimationSystem();
		cecil->ansys->Load("OverworldCecil.xml");
		cecil->ansys->Play("Left");

		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimBattle.xml");
		rat->ansys->Play("dead");

		Ratsptuin->ansys = new AnimationSystem();
		Ratsptuin->ansys->Load("RatAnimOverworld.xml");
		Ratsptuin->ansys->Play("Left");

		//Setup Objects Initial Waypoint  and Velocities
		mom->currentWaypoint = 0;
		dad->currentWaypoint = 0;
		cecil->currentWaypoint = 0;
		rat->currentWaypoint = 0;
		Ratsptuin->currentWaypoint = 0;

		mom->velocity.x = 3;
		mom->velocity.y = 3;

		mom->position.x = 2000;
		mom->position.y = 1000;

		mom->currentWait = 500.0f;

		dad->velocity.x = 3;
		dad->velocity.y = 3;

		dad->position.x = 2040;
		dad->position.y = 1000;

		dad->currentWait = 500.0f;


		cecil->velocity.x = 3;
		cecil->velocity.y = 3;

		cecil->position.x = 2000;
		cecil->position.y = 1000;

		cecil->currentWait = 5.0f;

		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 2000;
		rat->position.y = 800;

		rat->currentWait = 500.0f;

		Ratsptuin->velocity.x = 6;
		Ratsptuin->velocity.y = 3;

		Ratsptuin->position.x = 2200;
		Ratsptuin->position.y = 1040;
		Ratsptuin->currentWait = 8.0f;


		//Setup Waypoints
		SGD::Point point = SGD::Point(2000, 1000);

		mom->m_vWaypoints.push_back(point);
		point.x += 40;
		dad->m_vWaypoints.push_back(point);
		point.x = 2000;
		point.y = 800;
		rat->m_vWaypoints.push_back(point);

		point = SGD::Point(2000, 1000);

		point.x += 200;
		cecil->m_vWaypoints.push_back(point);
		point.x += 500;
		cecil->m_vWaypoints.push_back(point);

		point = SGD::Point(2200, 1040);
		point.x -= 200;
		Ratsptuin->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(mom);
		temp->m_vObjects.push_back(dad);
		temp->m_vObjects.push_back(cecil);
		temp->m_vObjects.push_back(rat);
		temp->m_vObjects.push_back(Ratsptuin);

		//Add Wait Times

		for (size_t i = 0; i < 20; i++)
		{
			mom->m_vWaitTimes.push_back(20.0f);
			dad->m_vWaitTimes.push_back(20.0f);
			rat->m_vWaitTimes.push_back(20.0f);
			cecil->m_vWaitTimes.push_back(3.0f);
			Ratsptuin->m_vWaitTimes.push_back(3.0f);
		}

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/TutCutsceneOutro.xml", true, { 500, 500 });

	}
	else if (_index == 3)
	{
		//Leaving Tutorial Cutscene
		Scene* temp = new Scene();
		SceneObject* Ratsptuin = new SceneObject();
		SceneObject* Jeeves = new SceneObject();
		SceneObject* Slippy = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		SceneObject* Checkers = new SceneObject();

		temp->index = _index;
		temp->eventTimer = 17.0f;


		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(472, 592));
		camVelocity.y = 0;
		camVelocity.x = 0;

		//who moves first
		Jeeves->is_waiting = true;
		Slippy->is_waiting = true;
		Biggs->is_waiting = true;
		Checkers->is_waiting = true;
		Ratsptuin->is_waiting = true;

		//Load the Animation and start it
		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		Slippy->ansys = new AnimationSystem();
		Slippy->ansys->Load("OverworldSlippy.xml");
		Slippy->ansys->Play(0);

		Checkers->ansys = new AnimationSystem();
		Checkers->ansys->Load("OverworldCheckers.xml");
		Checkers->ansys->Play(0);

		Jeeves->ansys = new AnimationSystem();
		Jeeves->ansys->Load("OverworldJeeves.xml");
		Jeeves->ansys->Play(0);

		Ratsptuin->ansys = new AnimationSystem();
		Ratsptuin->ansys->Load("RatAnimOverworld.xml");
		Ratsptuin->ansys->Play("Right");

		//Setup Objects Initial Waypoint  and Velocities
		Biggs->currentWaypoint = 0;
		Slippy->currentWaypoint = 0;
		Checkers->currentWaypoint = 0;
		Jeeves->currentWaypoint = 0;
		Ratsptuin->currentWaypoint = 0;

		Ratsptuin->velocity.x = 2;
		Ratsptuin->velocity.y = 3;

		Ratsptuin->position.x = 112;
		Ratsptuin->position.y = 624;
		Ratsptuin->currentWait = 1.0f;

		Slippy->velocity.x = 3;
		Slippy->velocity.y = 3;

		Slippy->position.x = 140;
		Slippy->position.y = 600;
		Slippy->currentWait = 5.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 180;
		Biggs->position.y = 590;
		Biggs->currentWait = 5.0f;

		Jeeves->velocity.x = 3;
		Jeeves->velocity.y = 3;

		Jeeves->position.x = 220;
		Jeeves->position.y = 590;
		Jeeves->currentWait = 5.0f;

		Checkers->velocity.x = 3;
		Checkers->velocity.y = 3;

		Checkers->position.x = 260;
		Checkers->position.y = 590;
		Checkers->currentWait = 5.0f;
		//Setup Waypoints
		
		SGD::Point point = SGD::Point(112, 624);

		point.x += 100;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 50;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 50;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;


		point = SGD::Point(140,600);

		Slippy->m_vWaypoints.push_back(point);
		point.y -= 10;
		point.x += 40;
		Biggs->m_vWaypoints.push_back(point);

		point.x += 40;
		Jeeves->m_vWaypoints.push_back(point);

		point.x += 40;
		Checkers->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(Ratsptuin);
		temp->m_vObjects.push_back(Slippy);
		temp->m_vObjects.push_back(Biggs);
		temp->m_vObjects.push_back(Jeeves);
		temp->m_vObjects.push_back(Checkers);

		//Add Wait Times

		for (size_t i = 0; i < 20; i++)
		{
			Jeeves->m_vWaitTimes.push_back(40.0f);
			Checkers->m_vWaitTimes.push_back(40.0f);
			Slippy->m_vWaitTimes.push_back(40.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
			Ratsptuin->m_vWaitTimes.push_back(10.0f);
		}

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/HeroLandingIntro.xml", true, { 500, 500 });
	}
	else if (_index == 31)
	{
		//Leaving Tutorial Cutscene
		Scene* temp = new Scene();
		SceneObject* Ratsptuin = new SceneObject();
		SceneObject* Jeeves = new SceneObject();
		SceneObject* Slippy = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		SceneObject* Checkers = new SceneObject();

		temp->index = _index;
		temp->eventTimer = 20.0f;


		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(1232, 1360));
		camVelocity.y = -92;
		camVelocity.x = -140;

		//who moves first
		Jeeves->is_waiting = true;
		Slippy->is_waiting = true;
		Biggs->is_waiting = true;
		Checkers->is_waiting = true;
		Ratsptuin->is_waiting = true;

		//Load the Animation and start it
		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		Slippy->ansys = new AnimationSystem();
		Slippy->ansys->Load("OverworldSlippy.xml");
		Slippy->ansys->Play(0);

		Checkers->ansys = new AnimationSystem();
		Checkers->ansys->Load("OverworldCheckers.xml");
		Checkers->ansys->Play(0);

		Jeeves->ansys = new AnimationSystem();
		Jeeves->ansys->Load("OverworldJeeves.xml");
		Jeeves->ansys->Play(0);

		Ratsptuin->ansys = new AnimationSystem();
		Ratsptuin->ansys->Load("RatAnimOverworld.xml");
		Ratsptuin->ansys->Play("Right");

		//Setup Objects Initial Waypoint  and Velocities
		Biggs->currentWaypoint = 0;
		Slippy->currentWaypoint = 0;
		Checkers->currentWaypoint = 0;
		Jeeves->currentWaypoint = 0;
		Ratsptuin->currentWaypoint = 0;

		Ratsptuin->velocity.x = 2;
		Ratsptuin->velocity.y = 3;

		Ratsptuin->position.x = 112;
		Ratsptuin->position.y = 624;
		Ratsptuin->currentWait = 1.0f;

		Slippy->velocity.x = 3;
		Slippy->velocity.y = 3;

		Slippy->position.x = 140;
		Slippy->position.y = 600;
		Slippy->currentWait = 5.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 180;
		Biggs->position.y = 590;
		Biggs->currentWait = 5.0f;

		Jeeves->velocity.x = 3;
		Jeeves->velocity.y = 3;

		Jeeves->position.x = 220;
		Jeeves->position.y = 590;
		Jeeves->currentWait = 5.0f;

		Checkers->velocity.x = 3;
		Checkers->velocity.y = 3;

		Checkers->position.x = 260;
		Checkers->position.y = 590;
		Checkers->currentWait = 5.0f;
		//Setup Waypoints

		SGD::Point point = SGD::Point(112, 624);

		point.x += 100;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 50;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 50;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;


		point = SGD::Point(140, 600);

		Slippy->m_vWaypoints.push_back(point);
		point.y -= 10;
		point.x += 40;
		Biggs->m_vWaypoints.push_back(point);

		point.x += 40;
		Jeeves->m_vWaypoints.push_back(point);

		point.x += 40;
		Checkers->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(Ratsptuin);
		temp->m_vObjects.push_back(Slippy);
		temp->m_vObjects.push_back(Biggs);
		temp->m_vObjects.push_back(Jeeves);
		temp->m_vObjects.push_back(Checkers);

		//Add Wait Times

		for (size_t i = 0; i < 20; i++)
		{
			Jeeves->m_vWaitTimes.push_back(40.0f);
			Checkers->m_vWaitTimes.push_back(40.0f);
			Slippy->m_vWaitTimes.push_back(40.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
			Ratsptuin->m_vWaitTimes.push_back(10.0f);
		}

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/8secsofNoDialog.xml", true, { 500, 500 });
	}
	else if (_index == 4)
	{
		//Leaving Tutorial Cutscene
		Scene* temp = new Scene();
		SceneObject* Ratsptuin = new SceneObject();
		SceneObject* Jeeves = new SceneObject();
		SceneObject* Slippy = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		SceneObject* Checkers = new SceneObject();
		SceneObject* Cecil = new SceneObject();

		temp->index = _index;
		temp->eventTimer = 8.0f;


		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(1656, 2588));
		camVelocity.y = 0;
		camVelocity.x = 0;

		//who moves first
		Jeeves->is_waiting = true;
		Slippy->is_waiting = true;
		Biggs->is_waiting = true;
		Checkers->is_waiting = true;
		Ratsptuin->is_waiting = true;
		Cecil->is_waiting = true;
		//Load the Animation and start it
		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		Slippy->ansys = new AnimationSystem();
		Slippy->ansys->Load("OverworldSlippy.xml");
		Slippy->ansys->Play(0);

		Checkers->ansys = new AnimationSystem();
		Checkers->ansys->Load("OverworldCheckers.xml");
		Checkers->ansys->Play(0);

		Jeeves->ansys = new AnimationSystem();
		Jeeves->ansys->Load("OverworldJeeves.xml");
		Jeeves->ansys->Play(0);

		Ratsptuin->ansys = new AnimationSystem();
		Ratsptuin->ansys->Load("RatAnimOverworld.xml");
		Ratsptuin->ansys->Play("Down");

		Cecil->ansys = new AnimationSystem();
		Cecil->ansys->Load("CecilBattle.xml");
		Cecil->ansys->Play("dead");
		//Setup Objects Initial Waypoint  and Velocities
		Biggs->currentWaypoint = 0;
		Slippy->currentWaypoint = 0;
		Checkers->currentWaypoint = 0;
		Jeeves->currentWaypoint = 0;
		Ratsptuin->currentWaypoint = 0;
		Cecil->currentWaypoint = 0;

		Ratsptuin->velocity.x = 2;
		Ratsptuin->velocity.y = 3;

		Ratsptuin->position.x = 1656;
		Ratsptuin->position.y = 2500;
		Ratsptuin->currentWait = 8.0f;

		Slippy->velocity.x = 3;
		Slippy->velocity.y = 3;

		Slippy->position.x = 1616;
		Slippy->position.y = 2610;
		Slippy->currentWait = 15.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 1696;
		Biggs->position.y = 2600;
		Biggs->currentWait = 15.0f;

		Jeeves->velocity.x = 3;
		Jeeves->velocity.y = 3;

		Jeeves->position.x = 1736;
		Jeeves->position.y = 2600;
		Jeeves->currentWait = 15.0f;

		Checkers->velocity.x = 3;
		Checkers->velocity.y = 3;

		Checkers->position.x = 1586;
		Checkers->position.y = 2600;
		Checkers->currentWait = 15.0f;

		Cecil->velocity.x = 3;
		Cecil->velocity.y = 3;

		Cecil->position.x = 1656;
		Cecil->position.y = 2380;
		Cecil->currentWait = 50.0f;
		//Setup Waypoints

		SGD::Point point = SGD::Point(1656, 2700);

		point.y += 100;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 500;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 50;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.y += 20;
		Ratsptuin->m_vWaypoints.push_back(point);
		point.x += 20;


		point = SGD::Point(1616, 2610);

		Slippy->m_vWaypoints.push_back(point);
		point.y -= 10;
		point.x += 80;
		Biggs->m_vWaypoints.push_back(point);

		point.x += 40;
		Jeeves->m_vWaypoints.push_back(point);

		point.x -= 160;
		Checkers->m_vWaypoints.push_back(point);

		point = SGD::Point(1656, 2380);
		Cecil->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(Ratsptuin);
		temp->m_vObjects.push_back(Slippy);
		temp->m_vObjects.push_back(Biggs);
		temp->m_vObjects.push_back(Jeeves);
		temp->m_vObjects.push_back(Checkers);
		temp->m_vObjects.push_back(Cecil);
		//Add Wait Times

		for (size_t i = 0; i < 20; i++)
		{
			Jeeves->m_vWaitTimes.push_back(40.0f);
			Checkers->m_vWaitTimes.push_back(40.0f);
			Slippy->m_vWaitTimes.push_back(40.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
			Ratsptuin->m_vWaitTimes.push_back(2.0f);
			Cecil->m_vWaitTimes.push_back(50.0f);
		}

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/CecilCutsceneOutro.xml", true, { 500, 500 });
	}
	else if (_index == 5)
	{
		//Wind Intro
		Scene* temp = new Scene();
		SceneObject* rat = new SceneObject();

		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(300, 496));
		camVelocity.y = 50;
		camVelocity.x = 50;

		temp->index = _index;
		temp->eventTimer = 9.0f;
		//who moves first
		rat->is_waiting = true;

		//Load the Animation and start it
		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimOverworld.xml");
		rat->ansys->Play("Right");


		//Setup Objects Initial Waypoint  and Velocities
		rat->currentWaypoint = 0;

		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 48;
		rat->position.y = 496;

		rat->currentWait = 2.0f;

		//Setup Waypoints
		SGD::Point point = SGD::Point(48, 496);

		rat->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(rat);

		//ADD WAIT TIMES
		for (size_t i = 0; i < 20; i++)
		{
			rat->m_vWaitTimes.push_back(2.0f);
		}

		rat->m_vWaitTimes.push_back(10.0f);
		rat->m_vWaitTimes.push_back(40.0f);
		rat->m_vWaitTimes.push_back(20.0f);

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/WindyWoodsCutscene.xml", true, { 500, 500 });
	}
	else if (_index == 6)
	{
		//Fire Intro
		Scene* temp = new Scene();
		SceneObject* rat = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(300, 496));
		camVelocity.y = 20;
		camVelocity.x = 50;

		temp->index = _index;
		temp->eventTimer = 9.0f;
		//who moves first
		rat->is_waiting = true;
		Biggs->is_waiting = true;
		//Load the Animation and start it
		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimOverworld.xml");
		rat->ansys->Play("Up");

		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		//Setup Objects Initial Waypoint  and Velocities
		rat->currentWaypoint = 0;
		Biggs->currentWaypoint = 0;
		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 48;
		rat->position.y = 496;

		rat->currentWait = 2.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 48;
		Biggs->position.y = 456;

		Biggs->currentWait = 2.0f;

		//Setup Waypoints
		SGD::Point point = SGD::Point(48, 496);

		rat->m_vWaypoints.push_back(point);

		point.y = 456;
		Biggs->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(rat);
		temp->m_vObjects.push_back(Biggs);
		//ADD WAIT TIMES
		for (size_t i = 0; i < 20; i++)
		{
			rat->m_vWaitTimes.push_back(40.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
		}


		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/MagmaFallsCutscene.xml", true, { 500, 500 });
	}
	else if (_index == 7)
	{
		//Earth Intro
		Scene* temp = new Scene();
		SceneObject* rat = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		SceneObject* Jeeves = new SceneObject();
		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(720, 256));
		camVelocity.y = 20;
		camVelocity.x = 50;

		temp->index = _index;
		temp->eventTimer = 9.0f;
		//who moves first
		rat->is_waiting = true;
		Biggs->is_waiting = true;
		Jeeves->is_waiting = true;
		//Load the Animation and start it
		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimOverworld.xml");
		rat->ansys->Play("Down");

		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		Jeeves->ansys = new AnimationSystem();
		Jeeves->ansys->Load("OverworldJeeves.xml");
		Jeeves->ansys->Play(0);

		//Setup Objects Initial Waypoint  and Velocities
		rat->currentWaypoint = 0;
		Biggs->currentWaypoint = 0;
		Jeeves->currentWaypoint = 0;

		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 720;
		rat->position.y = 56;

		rat->currentWait = 2.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 760;
		Biggs->position.y = 56;

		Biggs->currentWait = 2.0f;

		Jeeves->velocity.x = 3;
		Jeeves->velocity.y = 3;

		Jeeves->position.x = 800;
		Jeeves->position.y = 56;

		Jeeves->currentWait = 2.0f;
		//Setup Waypoints
		SGD::Point point = SGD::Point(720, 56);

		rat->m_vWaypoints.push_back(point);

		point.x = 760;
		Biggs->m_vWaypoints.push_back(point);
		point.x = 800;
		Jeeves->m_vWaypoints.push_back(point);

		//Add Objects to Scene
		temp->m_vObjects.push_back(rat);
		temp->m_vObjects.push_back(Biggs);
		temp->m_vObjects.push_back(Jeeves);

		//ADD WAIT TIMES
		for (size_t i = 0; i < 20; i++)
		{
			rat->m_vWaitTimes.push_back(40.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
			Jeeves->m_vWaitTimes.push_back(40.0f);
		}


		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/EarthCutscene.xml", true, { 500, 500 });
	}
	else if (_index == 8)
	{
		//Water Intro
		Scene* temp = new Scene();
		SceneObject* rat = new SceneObject();
		SceneObject* Jeeves = new SceneObject();
		SceneObject* Biggs = new SceneObject();
		SceneObject* Checkers = new SceneObject();

		//Setup Camera
		CameraObject = new BaseObject();
		CameraObject->SetPosition(SGD::Point(300, 272));
		camVelocity.y = 50;
		camVelocity.x = 50;

		temp->index = _index;
		temp->eventTimer = 9.0f;
		//who moves first
		rat->is_waiting = true;
		Jeeves->is_waiting = true;
		Biggs->is_waiting = true;
		Checkers->is_waiting = true;
		
		//Load the Animation and start it
		rat->ansys = new AnimationSystem();
		rat->ansys->Load("RatAnimOverworld.xml");
		rat->ansys->Play("Right");

		Biggs->ansys = new AnimationSystem();
		Biggs->ansys->Load("OverworldBiggs.xml");
		Biggs->ansys->Play(0);

		Jeeves->ansys = new AnimationSystem();
		Jeeves->ansys->Load("OverworldJeeves.xml");
		Jeeves->ansys->Play(0);

		Checkers->ansys = new AnimationSystem();
		Checkers->ansys->Load("OverworldCheckers.xml");
		Checkers->ansys->Play(0);

		//Setup Objects Initial Waypoint  and Velocities
		rat->currentWaypoint = 0;
		Biggs->currentWaypoint = 0;
		Jeeves->currentWaypoint = 0;
		Checkers->currentWaypoint = 0;


		rat->velocity.x = 3;
		rat->velocity.y = 3;

		rat->position.x = 88;
		rat->position.y = 252;

		rat->currentWait = 2.0f;

		Biggs->velocity.x = 3;
		Biggs->velocity.y = 3;

		Biggs->position.x = 88;
		Biggs->position.y = 292;

		Biggs->currentWait = 2.0f;

		Jeeves->velocity.x = 3;
		Jeeves->velocity.y = 3;

		Jeeves->position.x = 88;
		Jeeves->position.y = 212;

		Jeeves->currentWait = 2.0f;

		Checkers->velocity.x = 3;
		Checkers->velocity.y = 3;

		Checkers->position.x = 88;
		Checkers->position.y = 332;

		Checkers->currentWait = 2.0f;

		//Setup Waypoints
		SGD::Point point = SGD::Point(88, 252);

		rat->m_vWaypoints.push_back(point);

		point.y = 292;

		Biggs->m_vWaypoints.push_back(point);

		point.y = 212;

		Jeeves->m_vWaypoints.push_back(point);

		point.y = 332;

		Checkers->m_vWaypoints.push_back(point);
		//Add Objects to Scene
		temp->m_vObjects.push_back(rat);
		temp->m_vObjects.push_back(Biggs);
		temp->m_vObjects.push_back(Jeeves);
		temp->m_vObjects.push_back(Checkers);
		//ADD WAIT TIMES
		for (size_t i = 0; i < 20; i++)
		{
			rat->m_vWaitTimes.push_back(20.0f);
			Biggs->m_vWaitTimes.push_back(40.0f);
			Jeeves->m_vWaitTimes.push_back(40.0f);
			Checkers->m_vWaitTimes.push_back(40.0f);
		}

		//Add Scenes to Vector of Scenes
		m_vScenes.push_back(temp);

		//load dialogue
		dlg = new Dialogue();
		dlg->Load("../Trapped Rat/Assets/Scripts/WaterCutscene.xml", true, { 500, 500 });
	}
}
void Cutscene::Update(float dt)
{	
	
	if (dlg != nullptr)
	{
		if (!dlg->Update(dt))
		{
			if (m_vScenes[currScene]->index == 1)//Go into tutorial fight
			{
				GamePlayState::GetInstance()->TutorialStart();

			}
			else if (m_vScenes[currScene]->index == 2)//Go into Map Selection
			{
				//Unload the Map
				TileSystem::GetInstance()->Exit();
				GamePlayState::GetInstance()->SetState(GPStates::Map);
				GamePlayState::GetInstance()->SetLastState(GPStates::Map);
			}
			else if (m_vScenes[currScene]->index == 4)//End the game?
			{
				GameData::GetInstance()->SwapState(GameOverWinState::GetInstance());
			}
			else
			{
				GamePlayState::GetInstance()->SetState(GPStates::Town);
			}
			return;
		}
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape))
	{
		if (m_vScenes[currScene]->index == 1)//Go into tutorial fight
		{
			GamePlayState::GetInstance()->TutorialStart();
		}
		else if (m_vScenes[currScene]->index == 2)//Go into Map Selection
		{
			//Unload the Map
			TileSystem::GetInstance()->Exit();
			GamePlayState::GetInstance()->SetState(GPStates::Map);
			GamePlayState::GetInstance()->SetLastState(GPStates::Map);
		}
		else if (m_vScenes[currScene]->index == 4)//End the game?
		{
			GameData::GetInstance()->SwapState(GameOverWinState::GetInstance());
		}
		else
		{
			GamePlayState::GetInstance()->SetState(GPStates::Town);
		}
		return;

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
	m_vScenes[currScene]->eventTimer -= dt;
	if (m_vScenes[currScene]->eventTimer < 0.0f)
	{
		m_vScenes[currScene]->eventTimer = 50.0f;
		//Do an event
		if (m_vScenes[currScene]->index == 1)
		{
			//kill the rat
			m_vScenes[currScene]->m_vObjects[3]->ansys->UnloadAll();
			m_vScenes[currScene]->m_vObjects[3]->ansys->Load("RatAnimBattle.xml");
			m_vScenes[currScene]->m_vObjects[3]->ansys->ResetAll();
			m_vScenes[currScene]->m_vObjects[3]->ansys->Play("dead");
			//play sound effect
			GamePlayState::GetInstance()->PlaySoundEffect(8);
		}
		else if (m_vScenes[currScene]->index == 2)
		{
			GamePlayState::GetInstance()->PlaySoundEffect(9);
		}
		else if (m_vScenes[currScene]->index == 3)
		{
			CutsceneManager::GetInstance()->Terminate();
			CutsceneManager::GetInstance()->Initialize(31);
			CutsceneManager::GetInstance()->Play(0);
			return;
		}
		else if (m_vScenes[currScene]->index == 4)
		{
			camVelocity.x = 5;
			camVelocity.y = -30;
		}
		else if (m_vScenes[currScene]->index == 5)
		{
			camVelocity.x = -50;
			camVelocity.y = -50;
		}
		else if (m_vScenes[currScene]->index == 6)
		{
			camVelocity.x = 40;
			camVelocity.y = 30;
		}
	}
	for (size_t i = 0; i < m_vScenes[currScene]->m_vObjects.size(); i++)
	{
		SceneObject * obj = m_vScenes[currScene]->m_vObjects[i];
		if (obj->is_waiting)
		{
			obj->currentWait -= dt;
			if (obj->currentWait <= 0.0f)
			{
				if (obj->m_vWaitTimes.size() > obj->currentWaypoint)
				{
					obj->currentWait = obj->m_vWaitTimes[obj->currentWaypoint];
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
					if ((i == 0 || i == 1) && m_vScenes[currScene]->index == 1 && !(obj->currentWaypoint % 2))
					{
						obj->ansys->Play("Right");
					}
					else if ((i == 0 || i == 1) && m_vScenes[currScene]->index == 1 && (obj->currentWaypoint % 2))
					{
						obj->ansys->Play("Left");
					}
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
	GameData::GetInstance()->GetFont()->DrawString("Press Escape to Skip Cutscene", 78, 8, { 0, 0, 0 }, 3.0f);
	GameData::GetInstance()->GetFont()->DrawString("Press Escape to Skip Cutscene", 82, 12, { 0, 0, 0 }, 3.0f);
	GameData::GetInstance()->GetFont()->DrawString("Press Escape to Skip Cutscene", 80, 10, { 255, 255, 255 }, 3.0f);
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
