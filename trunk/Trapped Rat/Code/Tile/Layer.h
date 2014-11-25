#pragma once
#include "Tile.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include <vector>
#include <map>
#include "..\Core\Player.h"

class Layer
{
public:
	Layer() = default;
	~Layer() = default;

	void Initialize( const char* path );
	void Render();
	void Exit();

	int GetLayerHeight();
	int GetLayerWidth();
	int GetTileHeight();
	int GetTileWidth();
	int GetTileSetWidth();
	int GetTileSetHeight();
	int GetTileCollision();
	int GetTileID();
	Tile* GetTileMap();
	SGD::HTexture GetTileSet();
	std::map<std::string, int> GetEventMap();

	void SetLayerHeight( int LH );
	void SetLayerWidth( int LW );
	void SetTileHeight( int TH );
	void SetTileWidth( int TW );
	void SetTileSetHeight( int TSH );
	void SetTileSetWidth( int TSW );
	void SetTileCollision( int TC );
	void SetTileID( int TID );
	void SetTileMap( Tile* TM );
	void SetTileSet( SGD::HTexture TS );


private:
	int layerHeight;
	int layerWidth;
	int tileHeight;
	int tileWidth;
	int tileSetWidth;
	int tileSetHeight;
	int tileCollision;
	int tileID;
	std::map<std::string, int> eventIDs;
	
	Tile* tileMap;

	SGD::HTexture tileSet;
};

