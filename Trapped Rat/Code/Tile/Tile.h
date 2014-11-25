#pragma once
#include <string>

class Tile
{
public:
	Tile() = default;
	~Tile() = default;

	int GetTileID() const;
	void SetTileID( int id);
	std::string GetEventID() const;
	void SetEventID( std::string eID);
	std::string GetOnFireID() const;
	void setOnFireID( std::string fID );
	int GetCollisionType() const;
	void SetCollisionType( int collision );

private:
	int tileID;
	int collisionType;
	std::string eventOnCollision;
	std::string onFireEvent;

};

