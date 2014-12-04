#pragma once
#include "Cutscene.h"
#include <vector>
class CutsceneManager
{
private:
	std::vector<Cutscene*> m_vCutscenes;
	int currentCutscene;
	bool is_playing = false;
public:
	CutsceneManager() = default;
	~CutsceneManager() = default;

	static CutsceneManager * GetInstance();

	void Initialize();
	void Update(float dt);
	void Render();
	void Play(int _index);
	void Stop();
	void Terminate();

};