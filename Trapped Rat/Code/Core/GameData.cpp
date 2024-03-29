
#include "GameData.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include <ctime>
#include "../States/MainMenuState.h"
#include <iostream>
#include <Windows.h>
#include "../Animation/AnimTestState.h"
#include "../Font/FontTestState.h"
#include "../States/GamePlayState.h"
#include "BaseObject.h"
#include "../Font/Font.h"
#include "../Core/CombatPlayer.h"
#include "../States/LoadGameState.h"
#include <fstream>
#include <sstream>
#include "../SGD Wrappers/SGD_EventManager.h"

/*********  Grant's Evil Test State *********/
//#include "../States/AiTestState.h"


bool GameData::Initialize()
{
	srand((unsigned int)time(nullptr));
	rand();

	//getting the wrappers
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * audio = SGD::AudioManager::GetInstance();
	SGD::InputManager * input = SGD::InputManager::GetInstance();


	//initializing the wrappers
	if (!graphics->Initialize())
		return false;
	if (!audio->Initialize())
		return false;
	if (!input->Initialize())
		return false;
	SGD::EventManager::GetInstance()->Initialize();

	//set the screen to fullscreen or not
	SGD::GraphicsManager::GetInstance()->Resize({ screenwidth, screenheight }, is_windowed);

	//allocating the object manager
	//add overworldPlayer to ObjManager when implemented
	overworldPlayer = new Player();
	OManager = new ObjectManager();

	//setting the time for delta time
	lasttime = GetTickCount();


	//have to do this so it does not break
	menu_music = audio->LoadAudio("../Trapped Rat/Assets/Sounds/tower.xwm");
	//mounting an initial state
	CurrentState = MainMenuState::GetInstance();
	//CurrentState = LoadGameState::GetInstance();
	CurrentState->Enter();

	CameraPos = SGD::Point(0.0, 0.0);

	//set up the bitmap font
	font = new Font();
	font->LoadFont("Assets/Textures/testpng.png", "Assets/Scripts/BitMapFont.txt", { 0, 0, 0 });

	//load in some sound
	selection_sound = audio->LoadAudio("../Trapped Rat/Assets/Sounds/selectionchange.wav");
	confirm_sound = audio->LoadAudio("../Trapped Rat/Assets/Sounds/confirm.wav");
	cancel_sound = audio->LoadAudio("../Trapped Rat/Assets/Sounds/cancel.wav");
	
	//load in the options
	//read in the options based on which save file it is
	std::ifstream fin;
	std::ostringstream save;
	save << "../Trapped Rat/Assets/Scripts/Options" << save_file << ".txt";
	fin.open(save.str().c_str());
	if (fin.is_open())
	{
		fin >> music_volume;
		fin.ignore(INT_MAX, '\n');
		fin >> effect_volume;
		fin.ignore(INT_MAX, '\n');
		std::string temp;
		fin >> temp;
		if (temp == "true")
			is_windowed = true;
		else
			is_windowed = false;
		fin.ignore(INT_MAX, '\n');
		fin >> temp;
		if (temp == "true")
			GameData::GetInstance()->GetFont()->SetSpanish(true);
		else
			GameData::GetInstance()->GetFont()->SetSpanish(false);

		SetEffectVolume(effect_volume);
		SetMusicVolume(music_volume);
		fin.close();
	}

	return true;
}

void GameData::Terminate()
{
	//save the options stuff out
	std::ofstream fout;
	std::ostringstream save;
	save << "../Trapped Rat/Assets/Scripts/Options" << save_file << ".txt";
	fout.open(save.str().c_str(), std::ios_base::trunc);
	if (fout.is_open())
	{
		std::ostringstream mout;
		mout.clear();
		mout << music_volume;

		fout.write(mout.str().c_str(), mout.str().size());
		fout.write("\n", 1);
		std::ostringstream eout;
		eout.clear();
		eout << effect_volume;
		fout.write(eout.str().c_str(), eout.str().size());
		fout.write("\n", 1);
		if (is_windowed)
			fout.write("true", 4);
		else if (!is_windowed)
			fout.write("false", 5);
		fout.write("\n", 1);
		if (GameData::GetInstance()->GetFont()->IsSpanish())
			fout.write("true", 4);
		else
			fout.write("false", 5);


		fout.close();
	}


	//exit current state
	CurrentState->Exit();

	//delete objects

	delete font;
	font = nullptr;
	delete overworldPlayer;
	overworldPlayer = nullptr;
	//get rid of the object manager
	OManager->Clean();
	delete OManager;
	//delete Party


	//get rid of the sound
	SGD::AudioManager::GetInstance()->UnloadAudio(selection_sound);
	SGD::AudioManager::GetInstance()->UnloadAudio(confirm_sound);
	SGD::AudioManager::GetInstance()->UnloadAudio(cancel_sound);
	SGD::AudioManager::GetInstance()->UnloadAudio(menu_music);
	//terminate all wrappers
	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::GetInstance()->DeleteInstance();
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::GetInstance()->DeleteInstance();
	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::GetInstance()->DeleteInstance();


	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::GetInstance()->DeleteInstance();
}

bool GameData::Update()
{

	//update the wrappers
	if (SGD::AudioManager::GetInstance()->Update() == false
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false)
		return false;


	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt) && SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Enter))
	{
		is_windowed = !is_windowed;
		SGD::GraphicsManager::GetInstance()->Resize({ GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed());
		return true;
	}


	//set deltatime
	unsigned long time = GetTickCount();
	float deltatime = (time - lasttime) / 1000.0f;
	lasttime = time;
	if (deltatime > 0.15f)
		deltatime = 0.15f;

	//update the current state
	CurrentState->Update(deltatime);
	CurrentState->Render();
	input_timer -= deltatime;
	return is_running;
}
void GameData::SwapState(GameState * _state)
{
	//exit the curent state
	CurrentState->Exit();
	//mount the new state
	CurrentState = _state;
	//enter the mounted state
	CurrentState->Enter();
	CurrentState->Update(0.0);
}
GameData* GameData::GetInstance()
{
	static GameData data;

	return &data;
}
void GameData::SetRunning(bool _run)
{
	is_running = _run;
}
void GameData::UpdateCamera(BaseObject* obj)
{
	SGD::Point pos = obj->GetPosition();
	float xpos = pos.x - screenwidth / 2;
	float ypos = pos.y - screenheight / 2;
	if (xpos < 0)
		xpos = 0;
	else if (xpos + screenwidth > TileSystem::GetInstance()->GetLayerWidth() * TileSystem::GetInstance()->GetTileSize().width)
	{
		xpos = ((float)TileSystem::GetInstance()->GetLayerWidth() * TileSystem::GetInstance()->GetTileSize().width) - screenwidth;
	}


	if (ypos < 0)
		ypos = 0;
	else if (ypos + screenheight > TileSystem::GetInstance()->GetLayerHeight() * TileSystem::GetInstance()->GetTileSize().height)
	{
		ypos = ((float)TileSystem::GetInstance()->GetLayerHeight() * TileSystem::GetInstance()->GetTileSize().height) - screenheight;
	}

	//CameraPos = SGD::Point(pos.x - screenwidth / 2, pos.y - screenheight / 2);
	CameraPos = SGD::Point(xpos, ypos);
}


SGD::Point GameData::GetCamera()
{
	if (is_inCombat)
	{
		return SGD::Point(0, 0);
	}
	else
		return CameraPos;
}

void GameData::SetEffectVolume(int _effect)
{
	effect_volume = _effect;
	if (effect_volume > 100)
		effect_volume = 100;
	if (effect_volume < 0)
		effect_volume = 0;

	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, effect_volume);
}
void GameData::SetMusicVolume(int _music)
{
	music_volume = _music;
	if (music_volume > 100)
		music_volume = 100;
	if (music_volume < 0)
		music_volume = 0;

	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, music_volume);


}

void GameData::LoadSave()
{
	std::ifstream fin;
	std::ostringstream save;
	save << "../Trapped Rat/Assets/Scripts/Options" << save_file << ".txt";
	fin.open(save.str().c_str());
	if (fin.is_open())
	{
		fin >> music_volume;
		fin.ignore(INT_MAX, '\n');
		fin >> effect_volume;
		fin.ignore(INT_MAX, '\n');
		std::string temp;
		fin >> temp;
		if (temp == "true")
			is_windowed = true;
		else
			is_windowed = false;
		fin.ignore(INT_MAX, '\n');
		fin >> temp;
		if (temp == "true")
			GameData::GetInstance()->GetFont()->SetSpanish(true);
		else
			GameData::GetInstance()->GetFont()->SetSpanish(false);

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, effect_volume);
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, music_volume);

		fin.close();
	}

	SGD::GraphicsManager::GetInstance()->Resize({ GameData::GetInstance()->GetScreenWidth(), GameData::GetInstance()->GetScreenHeight() }, GameData::GetInstance()->GetWindowed());
}
void GameData::AltTab()
{
	if (CurrentState == GamePlayState::GetInstance())
	{
		GamePlayState::GetInstance()->SetState(GPStates::Menu);
	}
}
void GameData::PlaySelectionChange()
{
	if (selection_sound != SGD::INVALID_HANDLE)
		SGD::AudioManager::GetInstance()->PlayAudio(selection_sound);
}
void GameData::ResetPlayer()
{
	delete overworldPlayer;
	overworldPlayer = nullptr;
	overworldPlayer = new Player();
}
void GameData::PlayConfirm()
{
	SGD::AudioManager::GetInstance()->PlayAudio(confirm_sound);
}
void GameData::PlayCancel()
{
	SGD::AudioManager::GetInstance()->PlayAudio(cancel_sound);
}