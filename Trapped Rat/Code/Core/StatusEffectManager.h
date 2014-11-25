/* 
	This class stores StatusEffects to be pointed to by other things
*/

#include <vector>

class StatusEffect;


class StatusEffectManager
{
public:
	// Singleton Access
	static StatusEffectManager* GetInstance();
	void Initialize();
	void Terminate();

private:
	StatusEffectManager() = default;
	~StatusEffectManager( ) = default;
	StatusEffectManager(const StatusEffectManager&) = delete;
	StatusEffectManager& operator=(const StatusEffectManager&) = delete;

	std::vector<StatusEffect*> StatusList;


	void LoadStatusEffect(std::string filename);
};