#pragma once
class Stats
{
public:

	Stats();
	~Stats();
	int attack;
	float attack_scale;
	int defense;
	float defense_scale;
	int magic;
	float magic_scale;
	int avoision;

	//scaling for stuff within the character itself
	float avoision_scale;
	float hp_scale;
	float bp_scale;
	float speed_scale;
};

