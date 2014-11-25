#pragma once
#include "BaseObject.h"
#include <string>
class HUDItem :
	public BaseObject
{
protected:
	bool is_active;
	std::string string;
public:
	HUDItem();
	virtual ~HUDItem();

	void virtual Update(float dt);
	void virtual Render();

	void SetActive(bool _is);
	bool GetActive();

	void SetString(std::string _string);
	std::string GetString();
};

