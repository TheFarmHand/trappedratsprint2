#pragma once
#include "Character.h"
#include "Items.h"
#include <vector>
#include <map>
#include<unordered_map>
#include "SelectableObject.h"
#include "Ability.h"

struct ItemSelection
{

	bool item_selected = false;
	int item_cursor = 0;
	int max_index = 0;
	Items chosen;
	std::vector<Items> * inventory = nullptr;
	std::vector<std::string> uniquenames;
	std::vector<int> uniquecounts;

	SGD::HTexture scroll = SGD::INVALID_HANDLE;
	SGD::HTexture cursor = SGD::INVALID_HANDLE;

	ItemSelection();
	~ItemSelection();
	void PopulateUniqueItems();
	bool Update(float dt);
	void Render();
	void CollectItem(int _index);
	std::string GetCurrentExplanation();

};