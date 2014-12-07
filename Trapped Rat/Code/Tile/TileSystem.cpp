#include "TileSystem.h"
#include "..\Core\BaseObject.h"
#include "..\SGD Wrappers\SGD_Geometry.h"
#include "..\SGD Wrappers\SGD_Event.h"
#include "..\SGD Wrappers\SGD_EventManager.h"
#include "..\Core\GameData.h"

TileSystem * TileSystem::GetInstance()
	{
	static TileSystem data;
	return &data;
	}

void TileSystem::Initialize( const char* path )
	{
	layers.Initialize( path );
	tileSize.width = (float)layers.GetTileWidth();
	tileSize.height = (float)layers.GetTileHeight();
	}

void TileSystem::Render()
	{
	layers.Render();
	}

void TileSystem::Exit()
	{
	layers.Exit();
	}

void TileSystem::TileCollision( float &x, float &y, SGD::Rectangle &rect, char dir )
	{
	bool collided = false;
	int tileIndex;
	SGD::Point pos;
	pos.x = x;
	pos.y = y;

	tileIndex = GetTileIndex( rect.left + 1, rect.top );
	if ( layers.GetTileMap()[tileIndex].GetCollisionType() == 1 )
		collided = true;
	else
		{
		tileIndex = GetTileIndex( rect.right - 1, rect.top );
		if ( layers.GetTileMap()[tileIndex].GetCollisionType() == 1 )
			collided = true;
		else
			{
			tileIndex = GetTileIndex( rect.left + 1, rect.bottom - 1 );
			if ( layers.GetTileMap()[tileIndex].GetCollisionType() == 1 )
				collided = true;
			else
				{
				tileIndex = GetTileIndex( rect.right - 1, rect.bottom - 1 );
				if ( layers.GetTileMap()[tileIndex].GetCollisionType() == 1 )
					collided = true;
				}
			}
		}

	if ( collided )
		{
		switch ( dir )
			{
			case 'u':
				pos.y = (float)( tileIndex / layers.GetLayerWidth() ) * layers.GetTileHeight() + layers.GetTileHeight();
				pos.y += ( rect.bottom - rect.top ) / 2;
				break;
			case 'd':
				pos.y = (float)( tileIndex / layers.GetLayerWidth() ) * layers.GetTileHeight();
				pos.y -= ( rect.bottom - rect.top ) / 2;
				break;
			case 'r':
				pos.x = (float)( tileIndex % layers.GetLayerWidth() ) * layers.GetTileWidth();
				pos.x -= ( rect.right - rect.left ) / 2;
				break;
			case 'l':
				pos.x = (float)( tileIndex % layers.GetLayerWidth() ) * layers.GetTileWidth() + layers.GetTileWidth();
				pos.x += ( rect.right - rect.left ) / 2;
				break;
			default:
				break;
			}
		x = pos.x;
		y = pos.y;
		}
	}

void TileSystem::TileEvent( SGD::Point pos )
	{
	int tileIndex;
	int foundTileEvent;
	int x, y, pixX, pixY;
	SGD::Point* dest;

	tileIndex = GetTileIndex( pos.x, pos.y );
	if ( layers.GetTileMap()[tileIndex].GetEventID() != "none" )
		{
		foundTileEvent = layers.GetEventMap()[layers.GetTileMap()[tileIndex].GetOnFireID()];
		x = foundTileEvent % layers.GetLayerWidth();
		y = foundTileEvent / layers.GetLayerWidth();
		pixX = x * layers.GetTileWidth();
		pixY = y * layers.GetTileHeight();
		destination.x = (float)pixX;
		destination.y = (float)pixY;
		dest = &destination;
		SGD::Event* tEvent = new SGD::Event{ layers.GetTileMap()[tileIndex].GetOnFireID().c_str(), dest };
		tEvent->QueueEvent( nullptr );
		}
	}

SGD::Point TileSystem::GetTilePosition( int TiD )
	{
	int x, y, pixX, pixY;
	SGD::Point destination;
	x = TiD % layers.GetLayerWidth();
	y = TiD / layers.GetLayerWidth();
	pixX = x * layers.GetTileWidth();
	pixY = y * layers.GetTileHeight();
	destination.x = (float)pixX;
	destination.y = (float)pixY;

	return destination;
	}
int TileSystem::GetTileIndex( float x, float y )
	{
	int tileX, tileY, tileIndex;
	tileX = (int)( x / layers.GetTileWidth() );
	tileY = (int)( y / layers.GetTileHeight() );
	tileIndex = ( tileY * layers.GetLayerWidth() ) + tileX;
	return tileIndex;
	}

SGD::Size TileSystem::GetTileSize()
	{
	return tileSize;
	}

int TileSystem::GetLayerWidth()
	{
	return layers.GetLayerWidth();
	}
int TileSystem::GetLayerHeight()
	{
	return layers.GetLayerHeight();
	}