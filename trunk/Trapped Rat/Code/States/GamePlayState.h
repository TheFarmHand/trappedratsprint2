#pragma once
#include "GameState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../Tile/Layer.h"
#include "../Tile/TileSystem.h"
#include "../Core/Definitions.h"
#include "../Core/HUDItem.h"
#include "../Core/TurnManager.h"
#include "../Dialogue/Dialogue.h"
#include "../Core/SelectableObject.h"
#include "../Core/HelpText.h"
#include "../Core/Ability.h"
#include "../Core/Bars.h"
#include "../Core/StatusEffectManager.h"
#include "../Core/Guard.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../Core/RatTrap.h"

#define MAXTG 250
#define MAXITEM 5
#define MAXREV 1

class Player;
enum GPStates { Map, Town, Combat, Menu,Dia,Cuts, BattleSummary };
enum MenuSubStates{ None, Options, HowTo, Party, Shop };
enum ItemType { SmallHP, LargeHP, SmallBP, LargeBP, Revive, MaxRevive } ;

class HUDItem;
class GamePlayState :
	public GameState
	
{
	GamePlayState() = default;
	virtual ~GamePlayState() = default;
	GamePlayState(GamePlayState& rhs) = delete;
	GamePlayState& operator=(GamePlayState& rhs) = delete;

	HelpText* helptextbox = nullptr;
	SGD::HTexture helpback = SGD::INVALID_HANDLE;
	SGD::HTexture combatback = SGD::INVALID_HANDLE;
	SGD::HTexture background = SGD::INVALID_HANDLE;
	SGD::HTexture button = SGD::INVALID_HANDLE;
	SGD::HTexture cursor = SGD::INVALID_HANDLE;
	//Layer temp;
	std::vector<CombatPlayer*> Party;
	Enemy e1, e2, e3;
	Enemy *enemy1;
	Enemy *enemy2;
	Enemy *enemy3;
	int menuindex = 0;
	int maxindex = 0;
	int stepcounter = 0;
	GPStates state = GPStates::Town;
	GPStates laststate = GPStates::Town;
	MenuSubStates substate = MenuSubStates::None;
	std::vector<HUDItem*> m_vhuditems;
	SGD::HTexture combathud = SGD::INVALID_HANDLE;
	Dialogue * dialogue = nullptr;
	std::vector<Guard*> guards;
	SGD::HAudio m_Audio;
	SGD::HAudio m_overAudio;
	SGD::HAudio entercombat = SGD::INVALID_HANDLE;

	SGD::HTexture rattrap = SGD::INVALID_HANDLE;
	SGD::HTexture enemytargeting = SGD::INVALID_HANDLE;
	SGD::HTexture allytargeting = SGD::INVALID_HANDLE;
	SGD::HTexture buttonimg = SGD::INVALID_HANDLE;
	SGD::HTexture scroll = SGD::INVALID_HANDLE;
	SGD::HTexture trapcursor = SGD::INVALID_HANDLE;
	SGD::HTexture cheesecursor = SGD::INVALID_HANDLE;
	SGD::HTexture TurnIndicator = SGD::INVALID_HANDLE;
	std::vector<SelectableObject*> m_vSelectableItems; //For Combat Use   0Top 1Left 2Right 3Bottom
	SGD::HTexture WorldMap = SGD::INVALID_HANDLE;
	SGD::HTexture PadLock = SGD::INVALID_HANDLE;
	int SelectedTown = 0; //0-4
	AnimationSystem* WorldMapAnsys = nullptr;
	int unlockedTowns = 0;//0-4 based on how many towns are cleared
	SGD::Point townpoints[5];


	std::map<std::string, Ability*> MasterAbilityList;
	StatusEffectManager* pStatManager;

	//items
	
	std::vector<Items> shopinv;
	std::vector<Items> inventory;
	std::vector<ItemType> loot;
	std::vector<RatTrap*> traps;

	int guard_index = -1;
public:
	int gold = 50;
	int loot_gold = 0;			// Updates whenever gold is added to your stack; (at end of combat primarily); use this for End of Combat window
	int loot_xp = 0;			// This is also the XP value earned after combat 
	int tripped_trap = -1;		// Set this to -1 (false) otherwise it is the index of the trap we will be tripping
	int ternary_gauge = 0;
	bool ternary = false;		// used to trip appropriate handling in AbilitySelection when using Ternary blast
	bool trap_combat = false;
	HUDItem* ternary_bar;
	void RemoveTrap(int);
	std::vector<SGD::HAudio> m_vsoundeffects;
private:

	//party & ability selection
	bool selecting_ability = false;
	int character_index = 0;
	bool select_new = false;
	bool selecting_party = false;
	int oldindex = -1;
	bool select_first = false;


	//tutorial stuff
	bool is_tutorial = false;
	std::vector<Enemy*> tutorialenemy;
	std::vector<CombatPlayer*> Parents;



public:
	bool run_succeed = false;
	bool MinibossFight = false;
	bool FinalBossFight = false;
	bool ignore_game_over = false;
	bool AbilityUsed;
	float abilityTimer;
	Ability* CurrentAbilityUsed;
	bool testFinalFight;
	bool is_test_message = false;
	static void Test();
	//function for randomly activating combat
	void Fight();
	void TownUpdate(float dt);
	void TownRender();
	void MenuUpdate(float dt);
	void MenuRender();
	void CombatUpdate(float dt);
	void CombatRender();
	void DialogueUpdate(float dt);
	void DialogueRender();
	void CutsceneUpdate(float dt);
	void CutsceneRender();
	void SummaryUpdate(float dt);
	void SummaryRender();
	void MapUpdate(float dt);
	void MapRender();
	SGD::HTexture getTurnInd() {return TurnIndicator;};
	void AddTrap(std::string loot, SGD::Point pos);
	//factory methods
	Enemy * CreateCommonEnemy(std::string, Stats, int, int, int, float, float, Ability*[],  SGD::Point , SGD::Size, std::string = "" );
	CombatPlayer * CreateCombatPlayer(std::string, Stats, int, int, int, int, int, float, float, Ability*[],  SGD::Point , SGD::Size, std::string = "");
	CombatPlayer * LoadCombatPlayer(std::string _path); //will only return a combat player if you send in the right path
	Enemy* LoadEnemy(std::string _path);//will only return a combat player if you send in the right path

	RatTrap * CreateRatTrap( SGD::Point, int item1, int item2 =-1, int item3 = -1, int gold = 0);

	HUDItem * CreateHudItem(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string string);
	SelectableObject * CreateSelectableObject(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string string);
	HUDItem* CreateBar(SGD::Size _size,SGD::Point _pos,Character * _owner, SGD::Color _color,SGD::Point _offset, bool isHP = true);

	void RandomAnimation();
	void GuardAnimation();
	void BossAnimation();
	void Loading(std::string _loading);

	//basic functions
	static GamePlayState* GetInstance();
	void virtual Enter();
	void virtual Update(float dt);
	void virtual const Render();
	void virtual Exit();
	static void MessageProc(const SGD::Message * mess);

	//accessors
	//This Function returns the selectable objects in the order of Top Left Right Bottom
	std::vector<SelectableObject*> GetSelectableObjects();
	HelpText* GetHelpText();
	int GetGauge();
	int GetMaxGauge();
	bool usingTernary();
	void ClearTernary();
	void AddTarget( );
	void CheckAbilityUnlocked( bool EOC = false );
	std::map<std::string, Ability*> GetMasterList();
	// Items
	void AddItem( ItemType it );
	void AddGold( int val );
	int	GetGold(void) const;
	int GetPartySize(){ return Party.size(); }
	const std::vector<RatTrap*>& GetTraps() const { return traps; }
	
	struct TernaryTargets { std::vector<Character*> targets; std::vector<Ability*> abilities; int num_targets; };
	void AddToTB( Ability* abi, Character* target );
	TernaryTargets myTernTargets;
	std::vector<Items> * GetInventory();

	//Mutators
	void SetSelectableObject(std::vector<SelectableObject*>_vec);
	void SetSelectableObject(SelectableObject* _obj, int _index);
	void SetHelpText(HelpText* help);
	void SetState(GPStates _state){ state = _state; }
	void SetLastState(GPStates _state){ laststate = _state; }
	void PauseGame();
	bool AddToGauge(int val);
	void SetGauge(int val);
	void SetTernary(bool buul);
	void SetLootXP(int val);
	void AddGuard(Guard* _guard){ guards.push_back(_guard); }
	void AddToParty(CombatPlayer*_player);
	int GetTownSelected() { return SelectedTown; }
	void CreateFinalFight();
	void HoldOntoAbility( Ability* used );


	void CreateMinibossFight();

	void PlaySoundEffect(int _index);
};

