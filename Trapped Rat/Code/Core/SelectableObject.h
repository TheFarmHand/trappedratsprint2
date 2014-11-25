#pragma once
#include "HUDItem.h"
#include <string>
class Ability;
class Items;
class SelectableObject :
	public HUDItem
{
private:
	std::string name;
	bool is_selected;
	SGD::HTexture selectedImage = SGD::INVALID_HANDLE;
	int m_objecttype = 0;//0 Home Selection Items   1 Ability Items    2 Item Items
	Ability* m_ability = nullptr;
	Items*    m_item = nullptr;
	std::string explination;
public:
	SelectableObject();
	virtual ~SelectableObject();

	void virtual Update(float dt);
	void virtual Render();

	bool GetSelected();
	void SetSelected(bool _is);

	Ability* GetAbility();
	void SetAbility(Ability* abl);

	Items* GetItem();
	void  SetItem(Items*it);

	int GetObjectType();
	void SetObjectType(int type);

	std::string GetExplination();
	void SetExplination(std::string _str);


};

