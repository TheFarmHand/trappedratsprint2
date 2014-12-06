#pragma once
#include "Enemy.h"
class Boss :
	public Enemy
	{
	public:
		Boss( const char* _name );
		virtual ~Boss();

		void virtual Update( float dt );
		void virtual Render();
		void virtual BehaviorAI();
		void Cecil();
		void Jane();
		void John();

	private:

		void CreateCecil( const char* path );
		void CreateJohn( const char* path );
		void CreateJane( const char* path );
	};

