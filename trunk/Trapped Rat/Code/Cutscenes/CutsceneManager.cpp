#include "CutsceneManager.h"


CutsceneManager * CutsceneManager::GetInstance()
{
	static CutsceneManager data;
	return &data;
}
void CutsceneManager::Initialize()
{
	Cutscene* cuts = new Cutscene();
	cuts->LoadCutscene(0);
	m_vCutscenes.push_back(cuts);
}
void CutsceneManager::Update(float dt)
{
	if (is_playing)
	{
		m_vCutscenes[currentCutscene]->Update(dt);
	}
}
void CutsceneManager::Render()
{
	if (is_playing)
	{
		m_vCutscenes[currentCutscene]->Render();
	}
}
void CutsceneManager::Play(int _index)
{
	currentCutscene = _index;
	is_playing = true;
}
void CutsceneManager::Stop()
{
	is_playing = false;
}
void CutsceneManager::Terminate()
{
	for (size_t i = 0; i < m_vCutscenes.size(); i++)
	{
		delete m_vCutscenes[i];
	}
	m_vCutscenes.clear();
}