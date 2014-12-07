#pragma once
#include "Enemy.h"
#include <vector>
class Guard :
	public Enemy
{
private:
	int startpos = 0;//the position they start at
	std::vector<int> waypoints;//points to follow after start pos
	AnimationSystem* ansys;
public:
	Guard();
	virtual ~Guard();


	void virtual Update(float dt);
	void virtual Render();
	void virtual BehaviorAI();

	void AddWayPoint(int _waypoint);
	void SetStartPos(int _pos);
};

