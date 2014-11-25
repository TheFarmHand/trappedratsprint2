#include "TestStateP.h"
#include "../Core/GameData.h"
#include "GameOverLoseState.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "../Core/StatusEffect.h"
#include "../Core/Character.h"


TestStateP* TestStateP::GetInstance()
{
	static TestStateP data;
	return &data;
}

void TestStateP::Enter( )
{
	myParticles = ParticleManager::GetInstance();
	std::string relative = "../Trapped Rat/Assets/Scripts/";
	myParticles->LoadEmitter( relative + "bloodparticle.xml", "Test");

	mySEM = StatusEffectManager::GetInstance();
	mySEM->Initialize();

}

void TestStateP::Update( float dt )
{
	SGD::InputManager *input = SGD::InputManager::GetInstance( );
	//bool loopToggle = true;

	// Right Side of Screen
	if ( input->IsKeyPressed( SGD::Key::E ) )
	{
		myParticles->CreateEmitter("Test");

	}

		if ( input->IsKeyPressed( SGD::Key::Escape) )
	{
		GameData::GetInstance( )->SwapState( GameOverLoseState::GetInstance( ) );
		return;
	}

	myParticles->Update(dt);
	Render();

}

void const TestStateP::Render( )
{
	// Render all Emitters in active
	myParticles->Render();
}

void TestStateP::Exit( )
{
	// Clear up particle manager stuffs
	myParticles->UnloadEmitter("looping");
	myParticles->UnloadEmitter("single");
	myParticles->UnloadEmitter("Test");

	myParticles->ClearAll();
	

	myParticles = nullptr;
	myParticles = NULL;
}