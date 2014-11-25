#pragma once
#include "Character.h"
#include "Items.h"
#include <vector>
#include "SelectableObject.h"
#include "Ability.h"

class AnimationSystem;
class CombatPlayer :
	public Character
{
public:
	CombatPlayer();
	~CombatPlayer();
	enum TARGET_SELECT { none, enemy, player, self,allAlly,allEnemy };

	void virtual Update(float dt);
	void virtual UpdateAnimation( float dt );
	void virtual Render();
	void virtual Attack( Character* owner, Character * target );
	void UseItem(Character * target, Items item);

	

	bool GetActive();
	void SetActive( bool a );
	void Reset();

	void SetAnimations(AnimationSystem*);
	AnimationSystem* GetAnimations();
	bool TestAbility;


private:
	bool active;
	int myTarget = 0;
	int states;	// 0-Home 1-Attack 2-Items 3-Ability 4-Run 5-AllySelect 6-EnemySelect
	int hudSelection;

	std::vector<SelectableObject*> menu;
	//Character* myTarget;
	AnimationSystem* ansys = nullptr;
	TARGET_SELECT mySelection = none;
	void SetSelection( int selected );

	void TargetUnit(std::vector<Character*> &targets);

	void HomeUpdate(float dt);
	void AttackUpdate(float dt);
	void ItemsUpdate(float dt);
	void AbilityUpdate(float dt);
	void RunUpdate(float dt);
	void AllySelectUpdate(float dt);
	void EnemySelectUpdate(float dt);

	int maxBP;
	int curBP;
};

