#pragma once
#include "Dialogue.h"
#include "../Core/GameData.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../Font/Font.h"
#include "../tinyxml/tinyxml.h"
Dialogue::Dialogue()
{

}
Dialogue::~Dialogue()
{
	for (unsigned int i = 0; i < images.size(); i++)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(images[i]);
	}
}
void Dialogue::Load(std::string filepath)
{
	//unload the current thing




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
				temp.imagepath =  _mess->FirstChildElement("Portrait")->GetText();

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
		if (messages.size() > 0)
		{
			//here we take in input and make sure they dont just spam through the whole dialogue
			timer -= dt;



			if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) && timer <= 0)
			{
				messages.erase(messages.begin());
				SGD::GraphicsManager::GetInstance()->UnloadTexture(images[0]);
				images.erase(images.begin());
				timer = 2.0f;
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
			SGD::GraphicsManager::GetInstance()->DrawRectangle({ 0.0, 400.0, 800.0, 600.0 }, { 155, 155, 155 }, {}, 5);



			//render the words of the top message in the vector along with the portrait
			std::string temp = messages.front().words;
			GameData::GetInstance()->GetFont()->DrawString(temp, 50.0f, 450.0f, { 0, 0, 0 });
			if (images[0] != SGD::INVALID_HANDLE)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(images[0], { 700.0f, 425.0f });
			}


			//display at the bottom to continue
			if (timer <= 0)
			{

				GameData::GetInstance()->GetFont()->DrawString("Hit Enter", 50.0f, 550.0f, { 0, 0, 0 });
			}

		}
		

	}

	
