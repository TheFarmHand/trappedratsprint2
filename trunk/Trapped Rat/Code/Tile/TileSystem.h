#pragma once
#include "Layer.h"
#include <vector>


class BaseObject;

class TileSystem
{
public:
	TileSystem() = default;
	~TileSystem() = default;

	static TileSystem * GetInstance();
	void Initialize( const char* path );
	void Render();
	void Exit();
	SGD::Size GetTileSize();

	void TileCollision( float &x, float &y, SGD::Rectangle &rect, char dir );
	void TileEvent( SGD::Point pos );
	int GetLayerWidth();
	int GetLayerHeight();
	SGD::Point GetTilePosition( int TiD );
	int GetTileIndex( float x, float y );

private:
	Layer layers;
	SGD::Point destination;
	SGD::Size tileSize;
	
};

