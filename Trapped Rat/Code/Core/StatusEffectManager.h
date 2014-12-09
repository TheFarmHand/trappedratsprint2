#pragma once

/* 
	This class stores StatusEffects to be pointed to by other things
*/

#include <map>
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class StatusEffect;


class StatusEffectManager
{
public:
	// Singleton Access
	static StatusEffectManager* GetInstance();
	void Initialize();
	void Terminate();

	// Accessors
	StatusEffect& GetStatus(std::string status);

private:
	StatusEffectManager() = default;
	~StatusEffectManager( ) = default;
	StatusEffectManager(const StatusEffectManager&) = delete;
	StatusEffectManager& operator=(const StatusEffectManager&) = delete;

	std::map<std::string, StatusEffect*> StatusList;

	void LoadStatusEffect(std::string filename);
};