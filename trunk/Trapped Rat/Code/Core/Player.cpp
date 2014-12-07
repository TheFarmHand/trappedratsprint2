
#include "Player.h"
#include "Items.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../States/GamePlayState.h"
#include "GameData.h"
#include "..\Tile\TileSystem.h"
#include "../SGD Wrappers/SGD_Event.h"


Player::Player() : Listener(this)
{
	//start the player on a tile
	size = SGD::Size(32, 32);
	image = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/Textures/rat.png");
	ansys = new AnimationSystem();
	ansys->Load("RatAnimOverworld.xml");
	ansys->Play(0);
	velocity = SGD::Vector(0, 0);
	Listener::RegisterForEvent( "World1" );
	Listener::RegisterForEvent( "House1" );
	Listener::RegisterForEvent( "World2" );
	Listener::RegisterForEvent( "House2" );
	Listener::RegisterForEvent( "World3" );
	Listener::RegisterForEvent( "House3" );
	Listener::RegisterForEvent("World4");
	Listener::RegisterForEvent("House4");
	Listener::RegisterForEvent("World5");
	Listener::RegisterForEvent("House5");
	Listener::RegisterForEvent("firehouse1");
	Listener::RegisterForEvent("fireworld1");
	Listener::RegisterForEvent("firehouse2");
	Listener::RegisterForEvent("fireworld2");
	Listener::RegisterForEvent("firehouse3");
	Listener::RegisterForEvent("fireworld3");
	Listener::RegisterForEvent("firekeep");
	Listener::RegisterForEvent("firekeepworld");
	Listener::RegisterForEvent("herohouse1");
	Listener::RegisterForEvent("heroworld1");
	Listener::RegisterForEvent("herohouse2");
	Listener::RegisterForEvent("heroworld2");
	Listener::RegisterForEvent("herohouse3");
	Listener::RegisterForEvent("heroworld3");
	Listener::RegisterForEvent("herokeep");
	Listener::RegisterForEvent("herokeepworld");
	Listener::RegisterForEvent("GainJeeves");

}


Player::~Player()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
	image = SGD::INVALID_HANDLE;
	delete ansys;
}
void Player::Update(float dt)
{
	//udpate based on movement
	SGD::InputManager * input = SGD::InputManager::GetInstance();
	char dir = 'a';

	//if they are holding down the button
		//move at a constant rate
	if (input->IsKeyDown(SGD::Key::Up) && (int)(position.x - size.width / 2) % 8 == 0)
	{
		velocity.y = -4.0f;
		velocity.x = 0.0f;
		dir = 'u';
	}
	else if (input->IsKeyDown(SGD::Key::Down) && (int)(position.x - size.width / 2) % 8 == 0)
	{
		velocity.y = 4.0f;
		velocity.x = 0.0;
		dir = 'd';
	}
	else if (input->IsKeyDown(SGD::Key::Left)&& (int)(position.y - size.height / 2) % 8 == 0)
	{
		velocity.y = 0.0f;
		velocity.x = -4.0f;
		dir = 'l';
	}
	else if (input->IsKeyDown(SGD::Key::Right) && (int)(position.y - size.height / 2) % 8 == 0)
	{
		velocity.y = 0.0f;
		velocity.x = 4.0f;
		dir = 'r';
	}
	else
	{
		//if they have moved the right amount..stop

		/*if ((int)position.x % 32 == 0 && velocity.x != 0)
			velocity.x = 0.0f;
		if ((int)position.y % 32 == 0 && dvelocity.y != 0)
			velocity.y = 0.0f;*/
		if ((int)(position.x - size.width / 2) % 8 == 0 && (int)(position.y - size.height / 2) % 8 == 0)
		{
 			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}
		
	}

	//update the position based on velocity
	position += velocity;
	TileSystem::GetInstance()->TileCollision( position.x, position.y, GetRect(), dir );
	TileSystem::GetInstance()->TileEvent( position );
	//swap to proper animation if player changed direction
	static char prevdir = 'a';
	if (prevdir != dir)
	{
		prevdir = dir;
		switch (dir)
		{	
		case 'u':
			ansys->ResetAll();
			ansys->Play("Up");
			break;
		case 'd':
			ansys->ResetAll();
			ansys->Play("Down");
			break;
		case 'r':
			ansys->ResetAll();
			ansys->Play("Right");
			break;
		case 'l':
			ansys->ResetAll();
			ansys->Play("Left");
			break;
		default:
			break;
		}
	}
	
	//update the animation
	ansys->Update(dt);
	//if Not Moving, Pause
	if (!IsMoving())
	{
		prevdir = 'a';
		ansys->Pause(true);
	}
	
}
void Player::Render()
{
	SGD::Rectangle rect = { position, size };
	SGD::Point camera = GameData::GetInstance()->GetCamera();
	SGD::Point center = { (position.x-size.width/2) - camera.x, (position.y- size.height/2) - camera.y };
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(image, center, { 132, 1, 164, 33 });
	ansys->Render(position.x,position.y);

}
bool Player::IsMoving()
{
	return velocity.y != 0.0 || velocity.x != 0.0;
}

void Player::HandleEvent( const SGD::Event* pEvent )
	{
	if ( pEvent->GetEventID() == "World1" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House1" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "World2" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + ( size.width / 2 );
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House2" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "World3" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + ( size.width / 2 );
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if ( pEvent->GetEventID() == "House3" )
		{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>( pEvent->GetData() );
		position.x = dest->x + size.width / 2;
		position.y = dest->y + ( size.height / 2 ) - (int)TileSystem::GetInstance()->GetTileSize().height;
		}
	else if (pEvent->GetEventID() == "World4")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "House4")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "World5")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "House5")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "fireworld1")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "firehouse1")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "fireworld2")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "firehouse2")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "fireworld3")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "firehouse3")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "firekeepworld")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "firekeep")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "heroworld1")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "herohouse1")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "heroworld2")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "herohouse2")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "heroworld3")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2);
		position.y = dest->y + size.height / 2 + (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "herohouse3")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + size.width / 2;
		position.y = dest->y + (size.height / 2) - (int)TileSystem::GetInstance()->GetTileSize().height;
	}
	else if (pEvent->GetEventID() == "herokeepworld")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2) -(int)TileSystem::GetInstance()->GetTileSize().width;
		position.y = dest->y + size.height / 2;
	}
	else if (pEvent->GetEventID() == "herokeep")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2) + (int)TileSystem::GetInstance()->GetTileSize().width;
		position.y = dest->y + size.height / 2;
	}
	else if (pEvent->GetEventID() == "herokeep")
	{
		SGD::Point* dest = reinterpret_cast<SGD::Point*>(pEvent->GetData());
		position.x = dest->x + (size.width / 2) + (int)TileSystem::GetInstance()->GetTileSize().width;
		position.y = dest->y + size.height / 2;
	}
	else if (pEvent->GetEventID() == "GainJeeves")
	{
		//here we create jeeves and push him into the the vector
		SGD::Point characterOrderPosition;
		std::vector<Ability*> partyAbilities;
		CombatPlayer* jeeves = nullptr;
		jeeves = (GamePlayState::GetInstance()->LoadCombatPlayer("../Trapped Rat/Assets/Scripts/testcharacterJeeves.xml"));
		jeeves->SetOrderPosition(4);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(jeeves->GetOrderPosition() * 100 + 150);
		jeeves->SetPosition(characterOrderPosition);
		jeeves->SetSize({ 64, 64 });
		std::map<std::string, Ability*> MasterAbilityList;
		MasterAbilityList = GamePlayState::GetInstance()->GetMasterList();
		partyAbilities.push_back(MasterAbilityList["Collapse"]);
		partyAbilities.push_back(MasterAbilityList["Emblazon"]);
		partyAbilities.push_back(MasterAbilityList["Firefall"]);
		partyAbilities.push_back(MasterAbilityList["Fire Spikes"]);
		partyAbilities.push_back(MasterAbilityList["Ignite"]);
		partyAbilities.push_back(MasterAbilityList["Scorch"]);
		partyAbilities.push_back(MasterAbilityList["Rib-a-Rang"]);

		partyAbilities.push_back(MasterAbilityList["Incinerate"]);
		jeeves->InitializeAbilities(partyAbilities);
		jeeves->SetActive(false);
		GamePlayState::GetInstance()->AddToParty(jeeves);
	}
	else if (pEvent->GetEventID() == "GainSlippy")
	{
		//here we create jeeves and push him into the the vector
		SGD::Point characterOrderPosition;
		std::vector<Ability*> partyAbilities;
		CombatPlayer* slippy = nullptr;
		slippy = (GamePlayState::GetInstance()->LoadCombatPlayer("../Trapped Rat/Assets/Scripts/Slippy.xml"));
		slippy->SetOrderPosition(1);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(slippy->GetOrderPosition() * 100 + 150);
		slippy->SetPosition(characterOrderPosition);
		slippy->SetSize({ 64, 64 });
		std::map<std::string, Ability*> MasterAbilityList;
		MasterAbilityList = GamePlayState::GetInstance()->GetMasterList();
		partyAbilities.push_back(MasterAbilityList["Puddle"]);
		partyAbilities.push_back(MasterAbilityList["Whirlpool"]);
		partyAbilities.push_back(MasterAbilityList["Acid Rain"]);
		partyAbilities.push_back(MasterAbilityList["Torrent"]);
		partyAbilities.push_back(MasterAbilityList["Flood"]);
		partyAbilities.push_back(MasterAbilityList["Squirt"]);
		partyAbilities.push_back(MasterAbilityList["Dissolve"]);
		partyAbilities.push_back(MasterAbilityList["Splash"]);

		slippy->SetActive(false);
		slippy->InitializeAbilities(partyAbilities);
		GamePlayState::GetInstance()->AddToParty(slippy);
	}
	else if (pEvent->GetEventID() == "GainCheckers")
	{
		SGD::Point characterOrderPosition;
		std::vector<Ability*> partyAbilities;
		CombatPlayer* checkers = nullptr;
		checkers = (GamePlayState::GetInstance()->LoadCombatPlayer("../Trapped Rat/Assets/Scripts/Checkers.xml"));
		checkers->SetOrderPosition(2);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(checkers->GetOrderPosition() * 100 + 150);
		checkers->SetPosition(characterOrderPosition);
		checkers->SetSize({ 64, 64 });
		std::map<std::string, Ability*> MasterAbilityList;
		MasterAbilityList = GamePlayState::GetInstance()->GetMasterList();
		partyAbilities.push_back(MasterAbilityList["Cover"]);
		partyAbilities.push_back(MasterAbilityList["Geo Crush"]);
		partyAbilities.push_back(MasterAbilityList["Pinch"]);
		partyAbilities.push_back(MasterAbilityList["Quake"]);
		partyAbilities.push_back(MasterAbilityList["Hedge Guard"]);
		partyAbilities.push_back(MasterAbilityList["Rock Spike"]);
		partyAbilities.push_back(MasterAbilityList["Rampart"]);
		partyAbilities.push_back(MasterAbilityList["Tremor"]);

		checkers->SetActive(false);
		checkers->InitializeAbilities(partyAbilities);
		
		GamePlayState::GetInstance()->AddToParty(checkers);
	}
	else if (pEvent->GetEventID() == "GainBiggs")
	{
		SGD::Point characterOrderPosition;
		std::vector<Ability*> partyAbilities;
		CombatPlayer* biggs = nullptr;
		biggs = (GamePlayState::GetInstance()->LoadCombatPlayer("../Trapped Rat/Assets/Scripts/Biggs.xml"));
		biggs->SetOrderPosition(3);
		characterOrderPosition.x = 100.0f;
		characterOrderPosition.y = (float)(biggs->GetOrderPosition() * 100 + 150);
		biggs->SetPosition(characterOrderPosition);
		biggs->SetSize({ 64, 64 });
		std::map<std::string, Ability*> MasterAbilityList;
		MasterAbilityList = GamePlayState::GetInstance()->GetMasterList();
		partyAbilities.push_back(MasterAbilityList["Zephyr"]);
		partyAbilities.push_back(MasterAbilityList["Leaf on the Wind"]);
		partyAbilities.push_back(MasterAbilityList["Second Wind"]);
		partyAbilities.push_back(MasterAbilityList["Whispering Wind"]);
		partyAbilities.push_back(MasterAbilityList["Tailwind"]);
		partyAbilities.push_back(MasterAbilityList["Tempest"]);
		partyAbilities.push_back(MasterAbilityList["Tornado"]);
		partyAbilities.push_back(MasterAbilityList["Wind Vale"]);
		biggs->InitializeAbilities(partyAbilities);
		biggs->SetActive(false);
		GamePlayState::GetInstance()->AddToParty(biggs);
	}
	}
