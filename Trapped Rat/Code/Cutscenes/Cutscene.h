#pragma once
#include "../Core/Character.h"
#include "../Dialogue/Dialogue.h"
#include "../Animation/AnimationSystem.h"

struct SceneObject
{
	bool is_waiting;
	AnimationSystem* ansys;
	unsigned int currentWaypoint;
	SGD::Point velocity;
	std::vector<SGD::Point> m_vWaypoints;
	SGD::Point position;
	std::vector<float> m_vWaitTimes;
	float currentWait = 0.0f;
};
struct Scene
{
	std::vector<SceneObject*> m_vObjects;
};

class Cutscene
{
private: 
	std::vector<Scene*> m_vScenes;
	float nextScene = 5.0f;
	BaseObject* CameraObject = nullptr;
	SGD::Point camVelocity;
	std::vector<SGD::Point> m_vCamWaypoints;
	int currScene = 0;

	Dialogue * dlg = nullptr;
public:
	Cutscene();
	~Cutscene();

	void LoadCutscene(int _index);
	void Update(float dt);
	void Render();
};