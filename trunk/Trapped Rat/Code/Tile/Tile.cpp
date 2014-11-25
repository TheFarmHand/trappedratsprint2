#include "Tile.h"

int Tile::GetTileID() const
	{
	return tileID;
	}
void Tile::SetTileID( int id )
	{
	tileID = id;
	}
std::string Tile::GetEventID() const
	{
	return eventOnCollision;
	}
void Tile::SetEventID( std::string eID)
	{
	eventOnCollision = eID;
	}
std::string Tile::GetOnFireID() const
	{
	return onFireEvent;
	}
void Tile::setOnFireID( std::string fID )
	{
	onFireEvent = fID;
	}
int Tile::GetCollisionType() const
	{
	return collisionType;
	}
void Tile::SetCollisionType( int collision )
	{
	collisionType = collision;
	}