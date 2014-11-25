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
	float timer = 2.0f;
public:
	void Load(std::string filepath);//load in the xml into the vector
	bool Update(float dt);
	void Render();
	
	Dialogue();
	~Dialogue();
};