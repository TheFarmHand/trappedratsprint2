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
	myParticles = new ParticleManager;
	std::string relative = "../Trapped Rat/Assets/Scripts/";
	myParticles->LoadEmitter( relative + "bloodparticle.xml", "Test");
}

void TestStateP::Update( float dt )
{
	SGD::InputManager *input = SGD::InputManager::GetInstance( );
	bool loopToggle = true;
	
	// Test Particle Cases

	// Left Side of Screen
	//if ( input->IsKeyPressed( SGD::Key::Q ) )
	//{
	//	// Add looping
	//	if(loopToggle)	
	//	{
	//		myParticles->CreateEmitter("Test");
	//		loopToggle = false;
	//	}
	//}

	// Right Side of Screen
	if ( input->IsKeyPressed( SGD::Key::E ) )
	{
		myParticles->CreateEmitter("Test");

	}

	if(input->IsKeyPressed(SGD::Key::U))
	{
		Character rats;
		StatusEffect test;

		rats.AddStatus(&test);
		
		(*rats.GetEffects().begin())->Clear();
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
	
	delete myParticles;
	myParticles = NULL;
}