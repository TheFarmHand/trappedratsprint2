#pragma once
#include <string>
#include <vector>
#include <map>

#include "..\SGD Wrappers\SGD_GraphicsManager.h"
struct Letter
{
	char character;//what character is this
	float Height;//how mush to draw
	float Width;//how much to draw
	float xoffset;
	float yoffset;
	float X;//holds where to start drawing on the image
	float Y;//holds where to start drawing on the image
	float xadvance;//space between letters
	float yadvance;//space between lines

};

class Font
{
	//stuff for drawing
	SGD::HTexture image = SGD::INVALID_HANDLE;
	std::map<char, Letter> Characters;
	int yspace = 15;
	int xspace = 6;
	//need to support spanish
	bool is_spanish = false;
	std::map<std::string,int> english;
	std::vector<std::string> spanish;
public:
	Font();
	~Font();

	void LoadFont(std::string imagefile,std::string scriptfile,SGD::Color colorkey);
	void DrawString(std::string text, float x, float y, SGD::Color = {0,0,0}, float scale = 1.0f);
	bool IsSpanish(){ return is_spanish; }
	void SetSpanish(bool is){ is_spanish = is; }
	std::string StringIfSpanish(std::string);
};

