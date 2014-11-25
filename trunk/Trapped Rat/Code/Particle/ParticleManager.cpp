
#include "ParticleManager.h"
#include "../tinyxml/tinyxml.h"



//ParticleManager::ParticleManager()
//{
//
//}
//
//
//ParticleManager::~ParticleManager()
//{
//
//}

ParticleManager * ParticleManager::GetInstance()
{
	static ParticleManager data;
	return &data;
}


bool ParticleManager::LoadEmitter(std::string filename, std::string particlename)
// Adds Emitter(s) found in filename to the LoadedEmitters List
// DURING TESTING
{
	// Real Function to be written here
	TiXmlDocument doc; doc.LoadFile(filename.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8);
	TiXmlElement fly( *doc.FirstChildElement( "PEffect" )->FirstChildElement("FlyWeight"));
	TiXmlElement emitter( *doc.FirstChildElement( "PEffect" )->FirstChildElement( "Emitter" ) );
	
	
	Emitter temp;

	float x,y;
	// Figured out how to pull individual elements out after these first assignments
	x = std::stof(doc.FirstChildElement("PEffect")->FirstChildElement("FlyWeight")->FirstChildElement( "MinLife" )->GetText());
	y = std::stof( doc.FirstChildElement( "PEffect" )->FirstChildElement( "FlyWeight" )->FirstChildElement( "MaxLife" )->GetText( ) );
	temp.GetFly( ).SetLife(x,y);
	x = std::stof( doc.FirstChildElement( "PEffect" )->FirstChildElement( "FlyWeight" )->FirstChildElement( "ScaleStart" )->GetText( ) );
	y = std::stof( doc.FirstChildElement( "PEffect" )->FirstChildElement( "FlyWeight" )->FirstChildElement( "ScaleEnd" )->GetText( ) );
	temp.GetFly().SetScale(x,y);
	x = std::stof( doc.FirstChildElement( "PEffect" )->FirstChildElement( "FlyWeight" )->FirstChildElement( "VelocityStart" )->GetText( ) );
	y = std::stof( doc.FirstChildElement( "PEffect" )->FirstChildElement( "FlyWeight" )->FirstChildElement( "VelocityEnd" )->GetText( ) );
	temp.GetFly().SetVelocity(x,y);
	x = std::stof(fly.FirstChildElement("RotationStart")->GetText());
	y = std::stof( fly.FirstChildElement( "RotationEnd" )->GetText( ) );
	temp.GetFly().SetRoation(x,y);
	x = std::stof( fly.FirstChildElement( "ROffsetX" )->GetText( ) );
	y = std::stof( fly.FirstChildElement( "ROffsetY" )->GetText( ) );
	temp.GetFly().SetOffset(x,y);

	unsigned char a, r, g, b;
	a = (unsigned char)std::stoul(fly.FirstChildElement("ColorSA")->GetText());
	r = (unsigned char)std::stoul( fly.FirstChildElement( "ColorSR" )->GetText( ) );
	g = (unsigned char)std::stoul( fly.FirstChildElement( "ColorSG" )->GetText( ) );
	b = (unsigned char)std::stoul( fly.FirstChildElement( "ColorSB" )->GetText( ) );
	temp.GetFly().SetColorStart(a,r,g,b);
	a = (unsigned char)std::stoul( fly.FirstChildElement( "ColorEA" )->GetText( ) );
	r = (unsigned char)std::stoul( fly.FirstChildElement( "ColorER" )->GetText( ) );
	g = (unsigned char)std::stoul( fly.FirstChildElement( "ColorEG" )->GetText( ) );
	b = (unsigned char)std::stoul( fly.FirstChildElement( "ColorEB" )->GetText( ) );
	temp.GetFly( ).SetColorEnd( a, r, g, b );

	// Load in the image
	std::string path = "../Trapped Rat/Assets/Textures/";
	path += fly.FirstChildElement( "FileName" )->GetText( );
	SGD::HTexture img = SGD::GraphicsManager::GetInstance()->LoadTexture(path.c_str( ) );
	temp.GetFly().SetImage(img);

	temp.SetShape(emitter.FirstChildElement("Shape")->GetText());
	temp.SetSize(std::stof(emitter.FirstChildElement("Size")->GetText()));
	temp.SetSpawnRate(std::stof(emitter.FirstChildElement("SpawnRate")->GetText()));

	x = std::stof(emitter.FirstChildElement("MinParticles")->GetText());
	y = std::stof( emitter.FirstChildElement( "MaxParticles" )->GetText( ) );
	temp.SetMinParticles((int)x);
	temp.SetMaxParticles((int)y);

	std::string buul = emitter.FirstChildElement( "Looping" )->GetText( );
	
	if(buul == "True")
		temp.SetLooping(true);
	else
		temp.SetLooping(false);


	x = std::stof( emitter.FirstChildElement( "PositionX" )->GetText( ) );
	y = std::stof( emitter.FirstChildElement( "PositionY" )->GetText( ) );
	temp.SetPosition(x,y);
	x = std::stof( emitter.FirstChildElement( "DirectionX" )->GetText( ) );
	y = std::stof( emitter.FirstChildElement( "DirectionY" )->GetText( ) );
	temp.SetDirection(x,y);

	bool bx, by;
	buul = emitter.FirstChildElement( "RangeX" )->GetText( );
	if ( buul == "True" )
		bx = true;
	else bx = false;
	buul = emitter.FirstChildElement( "RangeY" )->GetText( );
	if ( buul == "True" )
		by = true;
	else by = false;

	temp.SetDir(bx,by);	
	
	LoadedEmitters[particlename] = temp;
	return true;
}

bool ParticleManager::UnloadEmitter(std::string emitter)
// Removes emitter from the LoadedEmitter Map
{
	if(LoadedEmitters.size() == 0) return true;

	// Gotta get in there and make sure lists are deleted
	LoadedEmitters[ emitter ].EmptyImage( );
	LoadedEmitters[ emitter ].Cleanup( );
	LoadedEmitters.erase(emitter);
	return true;
}

int ParticleManager::CreateEmitter(std::string EmitterID)
// Creates a new Emitter, copied from LoadedEmitters at index EmitterID
// Adds it to ActiveEmitters List
// Returns the new size of ActiveEmitters
{
	Emitter* temp = new Emitter();
	*temp = LoadedEmitters[EmitterID];
	temp->SetActive(true);
	ActiveEmitters.push_back(temp);

	//delete temp;

	
	return -1;
}

bool ParticleManager::FreeEmitter(int EmitterID)
// Removes an Emitter from the ActiveEmitters List
{
	// Empty particles & Emitters
	ActiveEmitters[EmitterID]->Cleanup();
	ActiveEmitters[EmitterID]->SetActive(false);
	delete ActiveEmitters[EmitterID];
	ActiveEmitters.erase(ActiveEmitters.begin() + EmitterID);
	return true;
}

void ParticleManager::RenderEmitter(int EmitterID)
// Permits the Emitter at ActiveEmitters index of EmitterID
//   to spew particles to the screen
{
	ActiveEmitters[ EmitterID ]->Render();
}

void ParticleManager::UpdateEmitter(int EmitterID, float dt)
// Updates the Emitter at EmitterID in ActiveEmitters
{
	if(ActiveEmitters[EmitterID]->isActive())
		// isActive returns true if the Emitter is still running and needs updating
	{
		ActiveEmitters[ EmitterID ]->Update( dt );
	}

	else
		// isActive returns false if the Emitter has run its course and needs deleting
		FreeEmitter(EmitterID);
}

void ParticleManager::Update(float dt)
{
	for(unsigned int i=0; i < ActiveEmitters.size(); i++)
	{
		UpdateEmitter(i, dt);
	}
}

void ParticleManager::Render()
{
	for (unsigned int i = 0; i < ActiveEmitters.size( ); i++ )
	{
		RenderEmitter( i );
	}
}

// These are the wrong ideas
int ParticleManager::GetNumActive()
// Returns the number of Active Emitters
{
	return ActiveEmitters.size();
}

int ParticleManager::GetNumLoaded()
// Returns the number of Loaded Emitters
{
	return LoadedEmitters.size();
}

void ParticleManager::ClearAll()
{
	
	for(unsigned int i=0; i<ActiveEmitters.size(); i++)
	{
		// Cleans up all dynamic memory associated with an emitter
		FreeEmitter( i );
	}

	ActiveEmitters.clear();
}

Emitter* ParticleManager::GetEmitter( std::string emitter_name )
{
	return &LoadedEmitters[emitter_name];
}


void ParticleManager::Terminate()
{
	ClearAll();
}