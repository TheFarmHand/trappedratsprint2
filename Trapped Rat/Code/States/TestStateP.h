#pragma once

#include "GameState.h"
#include "../Particle/ParticleManager.h"
#include "../Core/StatusEffectManager.h"


class TestStateP : public GameState
{
public:

	static TestStateP* GetInstance( );
	void virtual Enter( );
	void virtual Update( float dt );
	void virtual const Render( );
	void virtual Exit( );

private:
	TestStateP& operator=( TestStateP& rhs ) = delete;
	TestStateP( TestStateP& rhs ) = delete;
	TestStateP( ) = default;
	virtual ~TestStateP( ) = default;

	ParticleManager* myParticles;
	StatusEffectManager* mySEM;
	
};

