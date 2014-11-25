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

	bool GetSelected(){ return is_selected; }
	void SetSelected(bool _is){ is_selected = _is; }

	Ability* GetAbility(){ return m_ability; }
	void SetAbility(Ability* abl){ m_ability = abl; }

	Items* GetItem(){ return m_item; }
	void  SetItem(Items*it){ m_item = it; }

	std::string GetExplination();
	void SetExplination(std::string _str);
};

