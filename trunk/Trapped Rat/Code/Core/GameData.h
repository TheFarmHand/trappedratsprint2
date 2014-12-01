#pragma once
#include "../States/GameState.h"
#include "ObjectManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "..\Core\Player.h"
#include <vector>

class Font;
class CombatPlayer;
class GameData
{
private:

	int save_file = 0;
	GameState * CurrentState;
	ObjectManager * OManager;
	unsigned long lasttime;
	bool is_running = true;
	float screenwidth = 800;
	float screenheight = 600;
	
	SGD::Point CameraPos;//camera

	//Combat Party Members
	//font
	Font * font = nullptr;
	Player * overworldPlayer = nullptr;
	
	GameData() = default;
	~GameData() = default;
	GameData& operator=(GameData& rhs) = delete;
	GameData(GameData& rhs) = delete;

	//function for randomly activating combat
	//bool Fight();
	
	//bool if we are in a fight 
	bool is_inCombat =false;

	//sound volumes
	int effect_volume = 100;
	int music_volume = 100;
	bool is_windowed = true;
public:
	static GameData * GetInstance();
	void SetRunning(bool _run);
	bool Initialize();
	void Terminate();
	bool Update();
	void SwapState(GameState * _state);
	void LoadSave();
	void AltTab();

	//accessors
	float GetScreenWidth(){ return screenwidth; }
	float GetScreenHeight(){ return screenheight; }
	Font * GetFont(){ return font; }
	SGD::Point GetCamera();
	Player* GetOverworldPlayer() { return overworldPlayer; }
	
	bool GetIsInCombat(){ return is_inCombat; }
	int GetEffectVolume(){ return effect_volume; }
	int GetMusicVolume(){ return music_volume; }
	bool GetWindowed(){ return is_windowed; }
	int GetSaveFile(){ return save_file; }
	//mutators
	void SetCamera(SGD::Point point){ CameraPos = point; }
	void UpdateCamera(BaseObject* obj);
	void SetIsInCombat(bool inCombat){ is_inCombat = inCombat; }
	void SetEffectVolume(int _effect);
	void SetMusicVolume(int _music);
	void SetWindowed(bool _window){ is_windowed = _window; }
	void SetSaveFile(int _save){ save_file = _save; }
};

