#include "Font.h"
#include <fstream>
#include <iostream>
Font::Font()
{
	//is_spanish = true;
	//this is were we load in the vectors
	std::ifstream in;

	in.open("../Trapped Rat/Assets/Scripts/Phrases.txt");
	if (in.is_open())
	{
		int index = 0;
		while (!in.eof())
		{
			std::string temp;

			std::getline(in, temp);
			english[temp] = index;

			std::getline(in, temp);
			spanish.push_back(temp);
			index++;
		}
	}
	int blerg = 77;
}


Font::~Font()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
}

void Font::LoadFont(std::string imagefile,std::string scriptfile ,SGD::Color colorkey)
{

	//load in the image
	if (image != SGD::INVALID_HANDLE)
	{
		image= SGD::GraphicsManager::GetInstance()->LoadTexture(imagefile.c_str(), colorkey);
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
		image= SGD::GraphicsManager::GetInstance()->LoadTexture(imagefile.c_str(), colorkey);
	}
	//load in the letter information
	std::ifstream in;

	in.open(scriptfile.c_str());

	if (in.is_open())
	{
		while (!in.eof())
		{
			Letter temp;
			in >> temp.character >> temp.Width >> temp.Height >> temp.X >> temp.Y
				>> temp.xoffset >> temp.yoffset >> temp.xadvance >> temp.yadvance;
			Characters[temp.character] = temp;
			in.ignore(INT_MAX, '\n');
		}
	}
	int x = 0;

}
void Font::DrawString(std::string text, float x, float y, SGD::Color color, float scale)
{
	//for each letter in the text
		//find the corresponding letter in the vector
		//position correctly and draw the image of the letter and advance based on the width of the letter
		//if the carriage return is hit, go to the next line where and on the x where the string started
		//

	//check if spanish
	//switch the phrase to spanish
	if (is_spanish)
	{
		bool is_number = false;
		for (char i = 48; i < 58; i++)
		{
			if (text[0] == i)
				is_number = true;
		}
		if (!is_number)
		{
			int index = english[text];
			text = spanish[index];
		}

		
	}
	SGD::GraphicsManager * graphics = SGD::GraphicsManager::GetInstance();
	float xstart = x;
	float ystart = y;
	float currx = x;
	float curry = y;

	for (int i = 0; text[i]; i++)
	{
		if (text[i] == ' ')
		{
			currx += xspace * scale;
		}
		else if (text[i] == '\n' || text[i] == '_')
		{
			curry += (yspace + Characters['a'].yadvance) * scale;
			currx = xstart;
		}
		else if (text[i] == '\t')
		{
			float fromstart = currx - xstart;
			int tab = (int)fromstart / (int)(xspace * scale);
			currx += ((8 - (tab % 8)) * (xspace * scale));

		}
		else
		{
			//draw this out
			//add to the x based on letter width
			Letter temp = Characters[text[i]];
			
			graphics->DrawTextureSection(image, { currx + temp.xoffset * scale, curry + temp.yoffset * scale }, { temp.X, temp.Y, temp.X + temp.Width, temp.Y + temp.Height }, 0.0f, {}, color, { scale,scale });
			currx += (temp.Width + temp.xadvance) * scale;
		}
	}
}

std::string Font::StringIfSpanish(std::string _english)
{
	int index = english[_english];
	return spanish[index];
}
