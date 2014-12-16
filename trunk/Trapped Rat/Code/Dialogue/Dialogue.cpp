#pragma once
#include "Dialogue.h"
#include "../Core/GameData.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../Font/Font.h"
#include "../tinyxml/tinyxml.h"
Dialogue::Dialogue()
{
	scroll = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/Scroll.png");
	pborder = SGD::GraphicsManager::GetInstance()->LoadTexture("../Trapped Rat/Assets/Textures/portraitborder.png", {255,255,255});
}
Dialogue::~Dialogue()
{
	for (unsigned int i = 0; i < images.size(); i++)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(images[i]);
	}
	SGD::GraphicsManager::GetInstance()->UnloadTexture(scroll);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(pborder);
	
}
void Dialogue::Load(std::string filepath, bool _auto , SGD::Point _pos, float _time)
{
	m_auto = _auto;
	position = _pos;
	//unload the current thing

	timer = _time;


	//load in the stuff from the xml
	//put it into the vector
	//check the map if the imagepath is already in there, if not load in the texture and put it in the map

	std::vector<std::string> paths;
	TiXmlDocument doc;
	if (doc.LoadFile(filepath.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8))
	{
		{
			TiXmlElement * root = doc.RootElement();
			TiXmlElement * _mess = root->FirstChildElement("Message");

			while (_mess != nullptr)
			{
				message temp;

				temp.name = _mess->FirstChildElement("Name")->GetText();
				temp.words = _mess->FirstChildElement("Words")->GetText();
				temp.imagepath = _mess->FirstChildElement("Portrait")->GetText();

				messages.push_back(temp);

				//get the next pointer
				_mess = _mess->NextSiblingElement();
			}
		}


		//now get some images

		for (unsigned int i = 0; i < messages.size(); i++)
		{
			paths.push_back("../Trapped Rat/Assets/Textures/" + messages[i].imagepath);
		}
	}

	//now load in those images
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		SGD::HTexture temp = SGD::GraphicsManager::GetInstance()->LoadTexture(paths[i].c_str());
		images.push_back(temp);
	}
}
bool Dialogue::Update(float dt)
{
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Escape) || SGD::InputManager::GetInstance()->IsButtonPressed(0, 2))
		messages.clear();
	if (messages.size() > 0)
	{
		//here we take in input and make sure they dont just spam through the whole dialogue
		timer -= dt;
		if (timer <= 0 && m_auto)
		{

			GameData::GetInstance()->PlaySelectionChange();
			messages.erase(messages.begin());
			SGD::GraphicsManager::GetInstance()->UnloadTexture(images[0]);
			images.erase(images.begin());
			timer = 3.0f;
		}


		if ((SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) || SGD::InputManager::GetInstance()->IsButtonPressed(0,1)) && !m_auto)
		{
			if (timer <= 0)
			{

				GameData::GetInstance()->PlaySelectionChange();
				messages.erase(messages.begin());
				SGD::GraphicsManager::GetInstance()->UnloadTexture(images[0]);
				images.erase(images.begin());
				timer = 3.0f;
			}
			else
			{
				timer = 0.0f;
			}
		}
		return true;
	}

	return false;



}
void Dialogue::Render()
{
	if (messages.size() > 0)
	{

		//render the reactangle at the bottom of the screen
		//make sure to show the portrait
		//make sure the rectangle is big enough to read easily
		//



		//render the rectangle
		//SGD::GraphicsManager::GetInstance()->DrawRectangle({ 0.0, 400.0, 800.0, 600.0 }, { 155, 155, 155 }, {}, 5);
		//lets render a scroll sideways, its gonna be rad
		if (messages.front().words == ".")
			return;
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(scroll, position, { 0, 0, 300, 540 }, SGD::PI / 2, { 112.5f, 27.5f });


		
		//render the words of the top message in the vector along with the portrait
		std::string temp = messages.front().words;
		GameData::GetInstance()->GetFont()->DrawString(temp, position.x - 270.0f, position.y + 25.0f, { 0, 0, 0 });
		if (images[0] != SGD::INVALID_HANDLE)
		{
			SGD::GraphicsManager::GetInstance()->DrawTexture(images[0], { position.x - 270.0f, position.y - 50.0f  });
			//draw the border for the portrait
			SGD::GraphicsManager::GetInstance()->DrawTexture(pborder, { position.x  - 270.0f, position.y - 50.0f });

		}


		//display at the bottom to continue
		if (timer <= 0 && !m_auto)
		{

			GameData::GetInstance()->GetFont()->DrawString("Hit Enter", position.x - 270.0f, position.y + 150.0f, { 0, 0, 0 });
		}

	}


}


