
#include "WorldObject.h"


WorldObject::WorldObject(std::string imagePath, std::string aniPath, bool allied)
{
std::string path = "Assets/Textures/";
size = SGD::Size( 32, 32 );
path += imagePath + ".png";
image = SGD::GraphicsManager::GetInstance()->LoadTexture( path.c_str() );
ansys = new AnimationSystem();
path = aniPath + ".xml";
ansys->Load( path );
ansys->Play( 0 );

if ( allied )
	{
	AllyCode = type::Ally;
	}
else
	{
	AllyCode = type::Enemy;
	}
}


WorldObject::~WorldObject()
{
SGD::GraphicsManager::GetInstance()->UnloadTexture( image );
image = SGD::INVALID_HANDLE;
delete ansys;
}
void  WorldObject::Update(float dt)
{
ansys->Update( dt );
}
void  WorldObject::Render()
{
ansys->Render( position.x, position.y );
}
