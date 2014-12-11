
#include "LoadGameState.h"
#include "../Core/GameData.h"
#include "../Font/Font.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "MainMenuState.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include "GamePlayState.h"
LoadGameState::LoadGameState()
{

	
}


LoadGameState::~LoadGameState()
{
}
LoadGameState* LoadGameState::GetInstance()
{
	static LoadGameState data;

	return &data;
}
void  LoadGameState::Enter()
{
	maxindex = 2;
	background = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/MenuBackground.png");
	button = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/button.png", { 255, 255, 255 });
	cursor = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/cheese.png", { 255, 255, 255 });
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/Scroll.png");


	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
	{

		PathAppend(path, TEXT("basicinfo.txt"));
		std::ifstream fin;
		fin.open(path);
		if (fin.is_open())
		{
			for (int i = 0; i < 3;i++)
			{
				
				data temp;
				fin >> temp.playtime >> temp.town;
				files[i] = temp;
				fin.ignore(INT_MAX, '\n');

			}
			
		}
	}
}
void const LoadGameState::Render()
{
	Font * font = GameData::GetInstance()->GetFont();
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	
	graphics->DrawTextureSection(background, { 0.0, 0.0 }, { 0.0f, 0.0f, 800.0f, 600.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 95.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 175.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(button, { 45.0f, 255.0f }, { 15.0f, 5.0f, 240.0f, 70.0f });
	
	graphics->DrawTextureSection(cursor, { 10.0f, 95.0f + (index * 80) }, { 0, 0, 238, 73 });
	GameData::GetInstance()->GetFont()->DrawString("File 1", 100.0f, 120.0f, { 0, 0, 0 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("File 2", 100.0f, 200.0f, { 0, 0, 0 }, 2.0f);
	GameData::GetInstance()->GetFont()->DrawString("File 3", 100.0f, 280.0f, { 0, 0, 0 }, 2.0f);
	

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, { 440.0f, 50.0f }, { 0, 0, 300, 540 });
	//have a file that has quick info to load in
	//here we display a scroll to show how well the game is going
	//we want
			//time of last save
			//who was in the main party
			//the town they are currently in
	if (files[index].playtime != 0)
	{
		std::ostringstream playtime;
		playtime << "Playtime: " << files[index].playtime;
		GameData::GetInstance()->GetFont()->DrawString(playtime.str(), 475.0f, 225.0f, { 0, 0, 0 });
		std::ostringstream town;
		town << "Town: " << files[index].town;
		GameData::GetInstance()->GetFont()->DrawString(town.str(), 475.0f, 275.0f, { 0, 0, 0 });
	}
	else
	{
		GameData::GetInstance()->GetFont()->DrawString("EMPTY", 475.0f, 175.0f, { 0, 0, 0 });
	}
	
	
}
void LoadGameState::Update(float dt)
{
	if (dt == 0.0f)
		return;
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Up) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Up))
	{
		index--;
		if (index < 0)
			index = maxindex;
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Down) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Down))
	{
		index++;
		if (index > maxindex)
			index = 0;
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) || SGD::InputManager::GetInstance()->IsButtonPressed(0,1))
	{
		/*GameData::GetInstance()->SetSaveFile(index);
		GameData::GetInstance()->LoadSave();
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());*/
		int saveindex = index;
			GameData::GetInstance()->SwapState(GamePlayState::GetInstance());
			if (files[saveindex].playtime == 0)
				GamePlayState::GetInstance()->LoadGame(99);
			else
				GamePlayState::GetInstance()->LoadGame(index);
			
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape) || SGD::InputManager::GetInstance()->IsButtonPressed(0, 2))
	{
		GameData::GetInstance()->SwapState(MainMenuState::GetInstance());
	}
}
void LoadGameState::Exit()
{
	/*TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
	{
		PathAppend(path, TEXT("\\savedata.txt"));
		std::ofstream fout;
		fout.open(path,std::ios_base::trunc);
		if (fout.is_open())
		{
			fout << 1.33 << 4.5 << 1;
		}
	}*/
	SGD::GraphicsManager::GetInstance()->UnloadTexture(background);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(button);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(cursor);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(scroll);
	index = 0;
}
