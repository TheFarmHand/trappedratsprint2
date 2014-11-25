#pragma once
class Ability;
class CombatSelection
{
public:
	CombatSelection();
	virtual ~CombatSelection();

	void SwitchState(int newstate);
	void Update(float dt);
	void QueueAbility(Ability* ablility);
	void Execute();
};

