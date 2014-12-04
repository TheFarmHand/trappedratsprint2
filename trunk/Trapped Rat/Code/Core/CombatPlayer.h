#pragma once
#include "Character.h"
#include "Items.h"
#include <vector>
#include <map>
#include<unordered_map>
#include "SelectableObject.h"
#include "Ability.h"
#include "ItemSelection.h"

struct ItemSelection;
class AnimationSystem;
class CombatPlayer :
	public Character
{
public:
	CombatPlayer();
	~CombatPlayer();
	enum TARGET_SELECT { none, enemy, player, self, allAlly, allEnemy, deadAlly };

	void virtual Update(float dt);
	void virtual UpdateAnimation( float dt );
	void virtual Render();
	void virtual Attack( Character* owner, Character * target );
	void UseItem(Character * target, Items item);
	void TernaryBlast(float dt);
	

	bool GetActive();
	void SetActive( bool a );
	void Reset();

	void SetAnimations(AnimationSystem*);
	AnimationSystem* GetAnimations();
	

	//void SetMaxBP(int _bp){ maxBP = _bp; }
	//void SetCurrBP(int _bp){ curBP = _bp; }


private:
	
	bool active;
	int myTarget = 0;
	int states;	// 0-Home 1-Attack 2-Items 3-Ability 4-Run 5-AllySelect 6-EnemySelect
	int hudSelection;
	ItemSelection * item_choose = nullptr;
	std::vector<SelectableObject*> menu;
	//Character* myTarget;
	AnimationSystem* ansys = nullptr;
	TARGET_SELECT mySelection = none;
	void SetSelection( int selected );
	Items chosen;

	void TargetUnit(std::vector<Character*> &targets);

	void HomeUpdate(float dt);
	void AttackUpdate(float dt);
	void ItemsUpdate(float dt);
	void SelectingItems(float dt);
	void AbilityUpdate(float dt);
	void RunUpdate(float dt);
	void AllySelectUpdate(float dt);
	void EnemySelectUpdate(float dt);
	void SetHomeButtons();
	//int maxBP;
	//int curBP;

	
};


