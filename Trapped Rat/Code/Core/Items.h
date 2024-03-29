#pragma once
#include <string>
class Items
{
private:
	std::string explination = "You have no Available Items";
	std::string name = "nothing";
	int price = 0;
public:
	Items();
	~Items();

	void SetExplination(std::string st){ explination = st; }
	std::string GetExplination(){ return explination; }

	void SetName(std::string _name){ name = _name; }
	std::string GetName(){ return name; }

	void SetPrice(int _price){ price = _price; }
	int GetPrice(){ return price; }

	bool operator==(Items &rhs);
	bool operator<(const Items &rhs);
	//bool operator>(Items)
};

