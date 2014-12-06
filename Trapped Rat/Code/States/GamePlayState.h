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
#include "../Core/StatusEffectManager.h"

#define MAXTG 250

class Player;
enum GPStates { Map, Town, Combat, Menu,Dia,Cuts };
enum MenuSubStates{ None, Options, HowTo, Party, Shop };

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
	Layer temp;
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

	SGD::HAudio m_Audio;
	SGD::HAudio m_overAudio;
	SGD::HAudio entercombat = SGD::INVALID_HANDLE;

	SGD::HTexture enemytargeting = SGD::INVALID_HANDLE;
	SGD::HTexture allytargeting = SGD::INVALID_HANDLE;
	SGD::HTexture buttonimg = SGD::INVALID_HANDLE;
	SGD::HTexture scroll = SGD::INVALID_HANDLE;
	SGD::HTexture trapcursor = SGD::INVALID_HANDLE;
	SGD::HTexture cheesecursor = SGD::INVALID_HANDLE;
	SGD::HTexture TurnIndicator = SGD::INVALID_HANDLE;
	std::vector<SelectableObject*> m_vSelectableItems; //For Combat Use   0Top 1Left 2Right 3Bottom

	std::map<std::string, Ability*> MasterAbilityList;
	StatusEffectManager* pStatManager;

	//items
	std::vector<Items> shopinv;
	std::vector<Items> inventory;
	int gold = 50;
	int ternary_gauge = 0;
	bool ternary = false;		// used to trip appropriate handling in AbilitySelection when using Ternary blast
	

	//party & ability selection
	bool selecting_ability = false;
	int character_index = 0;
	bool select_new = false;


	//tutorial stuff
	bool is_tutorial = false;
	
	std::vector<CombatPlayer*> Parents;
	int oldindex = 0;
public:
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
	SGD::HTexture getTurnInd() {return TurnIndicator;};

	//factory methods
	Enemy * CreateCommonEnemy(std::string, Stats, int, int, int, float, float, Ability*[],  SGD::Point , SGD::Size, std::string = "" );
	CombatPlayer * CreateCombatPlayer(std::string, Stats, int, int, int, int, int, float, float, Ability*[],  SGD::Point , SGD::Size, std::string = "");
	CombatPlayer * LoadCombatPlayer(std::string _path); //will only return a combat player if you send in the right path
	Enemy* LoadEnemy(std::string _path);//will only return a combat player if you send in the right path


	HUDItem * CreateHudItem(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string string);
	SelectableObject * CreateSelectableObject(SGD::HTexture _image, SGD::Point _position, SGD::Size _size, std::string string);
	HUDItem* CreateBar(SGD::Size _size,SGD::Point _pos,Character * _owner, SGD::Color _color,SGD::Point _offset, bool isHP = true);

	void RandomAnimation();
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
	void AddToGauge(int val);
	void SetGauge(int val);
	void SetTernary(bool buul);
};

