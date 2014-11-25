#pragma once
#include <string>
class Items
{
private:
	std::string explination;
public:
	Items();
	~Items();

	void SetExplination(std::string st){ explination = st; }
	std::string GetExplination(){ return explination; }
};

