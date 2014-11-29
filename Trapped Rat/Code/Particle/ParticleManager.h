#pragma once

#include <string>
#include <vector>
#include <map>

#include "Emitter.h"


class ParticleManager
{
public:
	static ParticleManager * GetInstance();
	void Terminate();

	bool LoadEmitter(std::string filename, std::string particlename);
	bool UnloadEmitter(std::string emitter);
	int CreateEmitter(std::string emitter);
	bool FreeEmitter(int EmitterID);
	

	void Update(float dt);
	void Render();

	void ClearAll();
	void UnloadAll();

	// Access
	int GetNumActive();
	int GetNumLoaded();
	Emitter* GetEmitter(std::string emitter_name);

private:
	std::map<std::string, Emitter> LoadedEmitters;
	std::vector<Emitter*> ActiveEmitters;

	// Quadilogy of Quandry
	ParticleManager( ) = default;
	~ParticleManager( ) = default;
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

	// Helpers
	void RenderEmitter( int EmitterID );
	void UpdateEmitter( int EmitterID, float dt );

};

