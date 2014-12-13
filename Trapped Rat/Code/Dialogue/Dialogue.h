#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include <string>
#include <vector>
#include <map>
struct message
{
	std::string words;//the words in the message
	std::string imagepath;//the path for the image for message
	std::string name;//the name of the message or the name of the person talking
	
};
class Dialogue
{
private:
	std::vector<message> messages; //will hold the messages
	std::vector<SGD::HTexture> images;//will hold the filepath and the image that has been loaded in already
	SGD::HTexture scroll = SGD::INVALID_HANDLE;
	SGD::HTexture pborder = SGD::INVALID_HANDLE;
	float timer = 5.0f;
	bool m_auto = false;
	SGD::Point position;
public:
	void Load(std::string filepath, bool _auto = false, SGD::Point _pos = { 520.0f, 200.0f });//load in the xml into the vector
	bool Update(float dt);
	void Render();
	
	Dialogue();
	~Dialogue();
};