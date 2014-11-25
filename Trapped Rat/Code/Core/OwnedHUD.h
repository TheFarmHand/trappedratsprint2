#pragma once
#include "HUDItem.h"
#include "Character.h"
class OwnedHUD :
	public HUDItem
{
protected:
	Character* owner;
	bool is_offset;
	SGD::Point m_offset;
public:
	OwnedHUD();
	virtual ~OwnedHUD();

	void virtual Update(float dt);
	void virtual Render();
	Character* GetOwner();
	void SetOwner(Character * _owner);

	void SetOffset(SGD::Point _offset);

};

