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
	void ApplyTernary( std::string abil, Character* target );
	

	bool GetActive();
	void SetActive( bool a );
	void Reset();
	bool GetTernConfirm() {return tern_confirm;}

	void SetAnimations(AnimationSystem*);
	AnimationSystem* GetAnimations();
	

	//void SetMaxBP(int _bp){ maxBP = _bp; }
	//void SetCurrBP(int _bp){ curBP = _bp; }

	// XP functions
	// Increases XP by the value, handling level up and carry over XP
	// Changes the amount of xp needed to the next level

	int GetXP( );
	int GetLevel( );
	int GetNextlvl( );
	bool hasLeveled() {return leveledup;}
	void AddXP( int xp );
	void AddAbility( );
	void LevelUp( );

	// End XP Functions

private:
	
	bool active;
	int myTarget = 0;
	int states;	// 0-Home 1-Attack 2-Items 3-Ability 4-Run 5-AllySelect 6-EnemySelect 7-TernaryBlast
	int hudSelection;
	ItemSelection * item_choose = nullptr;
	std::vector<SelectableObject*> menu;
	//Character* myTarget;
	AnimationSystem* ansys = nullptr;
	TARGET_SELECT mySelection = none;
	void SetSelection( int selected );
	Items chosen;
	Items * tempitem = nullptr;
	SelectableObject* helpobject = nullptr;
	// Leveling Stuff

	int XP = 0;
	int nextlvl = 250;
	bool leveledup = false;		// Leveled up last time we got XP 
	bool runaway = false;		// Wait what is this about?
	int tern_index = 0;
	bool tern_confirm = false;
	
	// End Leveling Stuff

	void TargetUnit(std::vector<Character*> &targets);

	void TernarySelection();

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



