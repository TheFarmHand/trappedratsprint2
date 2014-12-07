#include "Layer.h"
#include <vector>
#include <iostream>
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "..\Core\GameData.h"
#include "..\tinyxml\tinyxml.h"
#include "../Core/Guard.h"
#include "../States/GamePlayState.h"

void Layer::Initialize( const char* path)
	{
	TiXmlDocument layerDoc;
	int layerCounter = 0;

	if ( !layerDoc.LoadFile( path ) )
		return;

	TiXmlElement* root = layerDoc.RootElement();
	if ( root == nullptr )
		return;

	//Loads the tileset into memory
	const char* tempTileset = root->Attribute( "tileSetFile" );
	std::string filename(tempTileset);
	std::string filepath;
	filepath = "Assets\\TileSets\\" + filename;
	tileSet = SGD::GraphicsManager::GetInstance()->LoadTexture( filepath.c_str() );


	TiXmlElement* layer = root->FirstChildElement( "layer" );
	layer->Attribute( "height", &layerHeight );
	layer->Attribute( "width", &layerWidth );
	
	TiXmlElement* tileset = layer->NextSiblingElement( "tileset" );
	tileset->Attribute( "height", &tileSetHeight );
	tileset->Attribute( "width", &tileSetWidth );

	TiXmlElement* tileSize = tileset->NextSiblingElement( "tilesize" );
	tileSize->Attribute( "height", &tileHeight );
	tileSize->Attribute( "width", &tileWidth );
	TiXmlElement* player = tileSize->NextSiblingElement( "player" );
	int x, y;
	SGD::Point position;
	player->Attribute( "startX", &x );
	player->Attribute( "startY", &y );
	position.x = (float)x + GameData::GetInstance()->GetOverworldPlayer()->GetSize().width / 2;
	position.y = (float)y + GameData::GetInstance()->GetOverworldPlayer()->GetSize().height / 2;
	GameData::GetInstance()->GetOverworldPlayer()->SetPosition( position );

	TiXmlElement* guard = player->NextSiblingElement( "guard" );
	while ( guard != nullptr )
		{
		guard->Attribute( "startX", &x );
		guard->Attribute( "startY", &y );
		position.x = (float)x;
		position.y = (float)y;

		//Change to guard objects
		Guard * temp = new Guard();
		temp->SetPosition(position);
		//Read in waypoints for each guard
		TiXmlElement * waypoint = guard->FirstChildElement("waypoint");
		while(waypoint != nullptr)
		{
			int way = 0;
			waypoint->Attribute("tilePoint", &way);
			temp->AddWayPoint(way);
			waypoint = waypoint->NextSiblingElement();
		}
		GamePlayState::GetInstance()->AddGuard(temp);
		guard = guard->NextSiblingElement( "guard" );
		}

	TiXmlElement* tiles = tileSize->NextSiblingElement( "tiles" );
	int layerDim = layerHeight * layerWidth;
	tileMap = new Tile[layerDim];
	while ( tiles != nullptr )
		{
		tiles->Attribute( "id", &tileID );
		tiles->Attribute( "collision", &tileCollision );
		const char* tempEvent = tiles->Attribute( "event" );
		const char* tempFire = tiles->Attribute( "onfire" );
		std::string tF( tempFire );
		std::string tE( tempEvent );


		tiles = tiles->NextSiblingElement( "tiles" );
		
		tileMap[layerCounter].SetTileID( tileID );
		tileMap[layerCounter].SetCollisionType( tileCollision );
		tileMap[layerCounter].SetEventID( tE );
		tileMap[layerCounter].setOnFireID( tF );

		eventIDs[tE] = layerCounter;


		++layerCounter;
		}
	}

void Layer::Render()
	{
	//Array counter
	int n = -1, w = 0, h = 0;
	//Variables used for tile locations within the tileset
	int pixX, pixY, x, y;
	//Simulates a 2D array using the 1D array
	for ( int i = 0; i < layerHeight; ++i )
		{
		w = 0;
		//Culls rows that are off the screen
		if ( i * tileHeight + tileHeight < GameData::GetInstance()->GetCamera().y)
			{
			n += layerWidth;
			continue;
			}
		if ( i * tileHeight > GameData::GetInstance()->GetCamera().y + GameData::GetInstance()->GetScreenHeight() )
			break;
		for ( int j = 0; j < layerWidth; ++j )
			{
			++n;
			//Calculates the location of the tile in the tileset to draw to the screen using TileID
			x = tileMap[n].GetTileID() % tileSetWidth;
			y = tileMap[n].GetTileID() / tileSetWidth;
			pixX = x * tileWidth;
			pixY = y * tileHeight;
			//Cull columns that are off the screen
			if ( j * tileWidth + tileWidth < GameData::GetInstance()->GetCamera().x )
				continue;
			if ( j * tileWidth > GameData::GetInstance()->GetCamera().x + GameData::GetInstance()->GetScreenWidth() )
				{
				n += ( layerWidth - 1 - j );
				break;
				}
			//Renders the selected tile to the screen in the appropriate location in the world
			SGD::GraphicsManager::GetInstance()->DrawTextureSection( tileSet, SGD::Point( (float)( j * tileWidth - GameData::GetInstance()->GetCamera().x ), (float)( i * tileHeight - GameData::GetInstance()->GetCamera().y ) ), SGD::Rectangle( (float)pixX, (float)pixY, (float)( pixX + tileWidth ), (float)( pixY + tileHeight ) ) );
			}
		}
	}

void Layer::Exit()
	{
	if (tileMap !=  nullptr )
		delete[] tileMap;
	tileMap = nullptr;
	//Change to guard
	//Here
	SGD::GraphicsManager::GetInstance()->UnloadTexture( tileSet );
	}

int Layer::GetLayerHeight()
	{
	return layerHeight;
	}

int Layer::GetLayerWidth()
	{
	return layerWidth;
	}

int Layer::GetTileHeight()
	{
	return tileHeight;
	}

int Layer::GetTileWidth()
	{
	return tileWidth;
	}

int Layer::GetTileSetWidth()
	{
	return tileSetWidth;
	}

int Layer::GetTileSetHeight()
	{
	return tileSetHeight;
	}

int Layer::GetTileCollision()
	{
	return tileCollision;
	}

int Layer::GetTileID()
	{
	return tileID;
	}

Tile* Layer::GetTileMap()
	{
	return tileMap;
	}

SGD::HTexture Layer::GetTileSet()
	{
	return tileSet;
	}

std::map<std::string, int> Layer::GetEventMap()
	{
	return eventIDs;
	}

void Layer::SetLayerHeight( int LH )
	{
	layerHeight = LH;
	}

void Layer::SetLayerWidth( int LW )
	{
	layerWidth = LW;
	}

void Layer::SetTileHeight( int TH )
	{
	tileHeight = TH;
	}

void Layer::SetTileWidth( int TW )
	{
	tileWidth = TW;
	}

void Layer::SetTileSetHeight( int TSH )
	{
	tileSetHeight = TSH;
	}

void Layer::SetTileSetWidth( int TSW )
	{
	tileSetWidth = TSW;
	}

void Layer::SetTileCollision( int TC )
	{
	tileCollision = TC;
	}

void Layer::SetTileID( int TID )
	{
	tileID = TID;
	}

void Layer::SetTileMap( Tile* TM )
	{
	tileMap = TM;
	}

void Layer::SetTileSet( SGD::HTexture TS )
	{
	tileSet = TS;
	}