#pragma once
#include "GameState.h"
#include "..\Tile\TileSystem.h"
#include <vector>
#include "..\Tile\Layer.h"
#include "..\Core\Player.h"
#include "..\SGD Wrappers\SGD_GraphicsManager.h"
#include "..\SGD Wrappers\SGD_Geometry.h"

class TileDebugState :
	public GameState
	{
	private:

		TileDebugState() = default;
		virtual ~TileDebugState() = default;
		TileDebugState& operator=( TileDebugState& rhs ) = delete;
		TileDebugState( TileDebugState& rhs ) = delete;

		Player * playa = nullptr;
		SGD::HTexture ratsputin = SGD::INVALID_HANDLE;

		Layer temp;
		TileSystem tiler;
		//std::vector<TileSystem> layers;
		int index = 0;
	public:
		static TileDebugState * GetInstance();
		void virtual Enter();
		void virtual Update( float dt );
		void virtual const Render();
		void virtual Exit();
	};

