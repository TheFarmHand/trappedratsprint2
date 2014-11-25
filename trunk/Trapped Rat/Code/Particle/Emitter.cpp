#include <math.h>

#include "Emitter.h"
#include "Particle.h"
#include "ParticleFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"



Emitter::Emitter()
{
	

}


Emitter::~Emitter()
{

}

void Emitter::Update( float dt )
{
	// Make Particles to add based on spawnrate (or MinParticles if list is empty)
	float addParts = mySpawnRate;
	if ( myLiveList.size() == 0 && myDeadList.size() == 0 ) addParts = (float)myMinParticles;

	if ( !myLooping )	// Not looping
	{
		if ( !( myTotalParticles >= addParts + myMaxParticles ) )
		{
			for ( int i = 0; i < addParts; i++, myTotalParticles++ )
			{
				Particle* temp = new Particle();
				SetParticle( temp );

				myLiveList.push_back( temp );
			}
		}

		else
		{
			if(myLiveList.size() == 0)
				active = false;
		}
	}

	else if ( myLooping )
		// Have spawned all the necessary particles in a looping emitter
	{
		// Reset particles from dead list instead of making new ones
		std::list<Particle*>::iterator tempit;
		for ( int i = 0; i < mySpawnRate; i++ )
		{
			if ( !myDeadList.size() == 0 )
			{
				tempit = myDeadList.begin();
				SetParticle( ( *tempit ) );

				myLiveList.splice( myLiveList.begin(), myDeadList, tempit );
			}

			else
			{
				// Debug else statement for breakpoints; nothing real happens down here
				tempit;
				int x;
				x = 5;
			}
		}
	}


	else
		// Have spawned all the necessary particles in a burst emitter
	{
	}




	// Update Live list, moving dead particles to Dead list
	int i = 0;
	for ( std::list<Particle*>::iterator iter = myLiveList.begin(); iter != myLiveList.end(); iter++, i++ ) // i++
	{
		// This updates position and lifespan only
		( *iter )->Update( dt );

		// Checks to see if the Particle has outlived it's lifespan
		if ( ( *iter )->GetLifeCurrent() >= ( *iter )->GetLifeEnd() )
			// Kill Particle
		{
			// Iterators are nasty buggers; temp bookmarks the previous node for after the splice
			// After the splice, the list is shrunk by 1 item, at iter's location (list is joined back together, but iter is now garbage)
			// assigning iter to the the bookmarked temp afterward puts us back to where we need to be when iter++ happens
			std::list<Particle*>::iterator temp;
			if ( iter == myLiveList.begin() )
			{
				// If we splice out the 0 index, there is no previous to bookmark, reset to beginning and skip update
				myDeadList.splice( myDeadList.begin(), myLiveList, iter );
				iter = myLiveList.begin();
				if ( myLiveList.size() == 0 )
				{
					SetActive( false );
					break;
				}
				continue;
			}

			else
			{
				temp = std::prev( iter );

				// Swaps this particle to the dead list (splicing is cool) // Cool, but frustrating
				myDeadList.splice( myDeadList.begin(), myLiveList, iter );
				iter = temp;
				i--;  // this should help us keep accurate count (all i is doing right now, debugging)
			}

		}

		// Particle still good
		else
		{
			// Update color and velocity (and scale eventually)
			float endLife = ( *iter )->GetLifeEnd();				// lifespan value is used a lot, storing it in temp
			float percent = ( *iter )->GetLifeCurrent() / endLife;	// % of lifespan that has passed
			float color;											// value of color that will be added to StartColor value to get current color

			SGD::Color tempColor;									// Assigning temp color to start values for adding color to later
			tempColor.alpha = myFly.GetColorStartA();
			tempColor.blue = myFly.GetColorStartB();
			tempColor.green = myFly.GetColorStartG();
			tempColor.red = myFly.GetColorStartR();

			// Reusing color variable for each individual color
			color = (float)myFly.GetColorEndA() - (float)myFly.GetColorStartA();	color *= percent;	tempColor.alpha += (unsigned char)color;
			color = (float)myFly.GetColorEndR() - (float)myFly.GetColorStartR();	color *= percent;	tempColor.red += (unsigned char)color;
			color = (float)myFly.GetColorEndG() - (float)myFly.GetColorStartG();	color *= percent;	tempColor.green += (unsigned char)color;
			color = (float)myFly.GetColorEndB() - (float)myFly.GetColorStartB();	color *= percent;	tempColor.blue += (unsigned char)color;

			( *iter )->SetColor( tempColor );

			// Set Velocity, Scale, and Rotation based on time elapsed vs. total change
			// Velocity update
			/*float newvelocity = (float)nVelEnd.Value - (float)nVelStart.Value;
			newvelocity = myParticles[ i ].Velocity + ( newvelocity / myParticles[ i ].LifeEnd * (float)dt.TotalSeconds );
			myParticles[ i ].Velocity = newvelocity;*/

			float newvelocity = myFly.GetVelocityEnd() - myFly.GetVelocityStart();
			newvelocity = ( *iter )->GetVelocity() + ( newvelocity / endLife * dt );
			( *iter )->SetVelocity( newvelocity );
			//( *iter )->SetVelocity( (*iter)->GetVelocity() + ( ( ( myFly.GetVelocityEnd() - myFly.GetVelocityStart() ) / endLife ) * dt) );
			( *iter )->SetScale( ( *iter )->GetScale() + ( ( ( myFly.GetScaleEnd() - myFly.GetScaleStart() ) / endLife ) * dt ) );
			( *iter )->SetRotation( ( *iter )->GetRotation() + ( ( myFly.GetRotationEnd() - myFly.GetRotationStart() ) / endLife * dt ) );
		}

	}
}

void Emitter::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render Live

	for ( std::list<Particle*>::iterator iter = myLiveList.begin(); iter != myLiveList.end(); iter++ )
	{
		pGraphics->DrawTexture( myFly.GetImage(),
		{ ( *iter )->GetPos().x, ( *iter )->GetPos().y },
		( *iter )->GetRotation(),
		{ myFly.GetOffSetX(), myFly.GetOffSetY() },
		//	{255,255,255,255},
		( *iter )->GetColor(),
		{ ( *iter )->GetScale(), ( *iter )->GetScale() } );
	}
}


void Emitter::SetShape( std::string shape )
{
	if ( shape == "Point" )
		myShape = 0;
	else if ( shape == "Rectangle" )
		myShape = 1;
	else if ( shape == "Circle" )
		myShape = 2;
	else if ( shape == "Line" )
		myShape = 3;
}


void Emitter::SetParticle( Particle* &temp )
{
	//temp->SetLife( rand() % (int)( myFly.GetMaxLife() - myFly.GetMinLife() ) + myFly.GetMinLife() );
	// Set Variable life spans based on min/max FlyWeight values
	float tempMin, tempMax, tempLife;
	tempMin = myFly.GetMinLife() * 100;
	tempMax = myFly.GetMaxLife() * 101;
	tempLife = rand() % (int)( tempMax - tempMin ) + tempMin;
	temp->SetLife( tempLife / 100.0f );


	temp->SetColor( myFly.GetColorStartA(), myFly.GetColorStartR(), myFly.GetColorStartG(), myFly.GetColorStartB() );
	temp->SetPosition( myPosition );
	//temp->SetPosition({0,0});
	temp->SetVelocity( myFly.GetVelocityStart() );
	temp->SetScale( myFly.GetScaleStart() );
	temp->SetRotation( myFly.GetRotationStart() );

	// Setting direction a little squirrely, gotta do math for the range.
	SGD::Vector range;
	if ( myDirX )
	{
		range.x = (float)( rand() % ( (int)myDirection.x * 2 ) );
		range.x -= myDirection.x;
	}

	else
	{
		range.x = (float)( rand() % ( (int)myDirection.y ) );
		if ( myDirection.x < 0 ) range.x *= -1;
	}

	if ( myDirY )
	{
		range.y = (float)( rand() % ( (int)myDirection.y * 2 ) );
		range.y -= myDirection.y;
	}
	else
	{
		range.y = (float)( rand() % ( (int)myDirection.y ) );
		if ( myDirection.y < 0 ) range.y *= -1;
	}


	//range.x *= (rand( ) % (myFly.GetRangeX( )) + 1);
	//range.y *= (rand( ) % (myFly.GetRangeY( )) + 1);
	temp->SetDirection( range );

	// Position Modding by shape
	SGD::Vector tempPos = myPosition;
	switch ( myShape )
	{
		// Point
		case 0:
			// Actually, do nothing different here
			break;

			// Rectangle (square)
		case 1:
			tempPos.x -= mySize.width;
			tempPos.y -= mySize.height;
			tempPos.x += rand() % (int)( mySize.width * 2 );
			tempPos.y += rand() % (int)( mySize.height * 2 );
			break;

			// Circle
		case 2:
			//float theta = 2 * (float)Math.PI * (float)rand.Next(0, size + 1);
		{
				  float a = (float)( rand() % 1000 ); a /= 1000;
				  float b = (float)( rand() % 1000 ); b /= 1000;
				  if ( b < a )
				  {
					  float temp = b;
					  b = a;
					  a = temp;
				  }


				  tempPos.x += ( b * mySize.width * (float)cos( 2 * 3.14159f * a / b ) );
				  tempPos.y += ( b * mySize.height * (float)sin( 2 * 3.14159f * a / b ) );
		}
			break;

			// Line
		case 3:
			//tempPos.x -= mySize.width;
			tempPos.x += rand() % (int)( mySize.width );
			break;

	}
	temp->SetPosition( tempPos );
}

Emitter& Emitter::operator=( const Emitter& rhs )
{
	// Super paranoid check
	if ( this == &rhs ) return *this;

	myDirection = rhs.myDirection;
	myPosition = rhs.myPosition;
	mySize = rhs.mySize;
	myLooping = rhs.myLooping;
	mySpawnRate = rhs.mySpawnRate;
	myMaxParticles = rhs.myMaxParticles;
	myMinParticles = rhs.myMinParticles;
	myTotalParticles = rhs.myTotalParticles;
	myShape = rhs.myShape;


	active = rhs.active;
	myFly = rhs.myFly;
	myDirX = rhs.myDirX;
	myDirY = rhs.myDirY;

	return *this;
}

bool Emitter::isActive()
{
	return active;
}

void Emitter::SetActive( bool set )
{
	active = set;
}

void Emitter::Cleanup()
{
	for ( auto iter = myLiveList.begin(); iter != myLiveList.end(); iter++ )
	{
		delete ( *iter );
		( *iter ) = nullptr;
	}
	myLiveList.clear();

	for ( auto iter = myDeadList.begin(); iter != myDeadList.end(); iter++ )
	{
		delete ( *iter );
		( *iter ) = nullptr;
	}
	myDeadList.clear();

	//myFly.ClearImage();
}

void Emitter::EmptyImage()
{
	myFly.ClearImage();
}

void Emitter::TestCase( bool loop )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	if ( loop )
	{
		myFly.SetImage( pGraphics->LoadTexture( "Assets/Textures/TestParticleRed.png" ) );
		myFly.SetLife( 0.2f, 2 );
		myFly.SetColorStart( 255, 0, 0, 255 );
		myFly.SetColorEnd( 255, 0, 255, 0 );
		myFly.SetScale( 1.0f, 2.0f );
		myFly.SetRoation( 0.0f, 1.0f );
		myFly.SetVelocity( 20, 70 );
		myFly.SetRange( 5, 5 );
		myFly.SetOffset( 8, 8 );

		myPosition.x = 200;
		myPosition.y = 300;

		myDirection.x = 1;
		myDirection.y = 1;

		mySize.width = 1;
		mySize.height = 1;

		myMinParticles = 10;
		myMaxParticles = 100;
		myTotalParticles = 0;

		myShape = 3; // point
		mySpawnRate = 5.0f;
		myLooping = true;
	}

	else
	{
		myFly.SetImage( pGraphics->LoadTexture( "Assets/Textures/TestParticleBlue.png" ) );
		myFly.SetLife( 0.2f, 2.0f );
		myFly.SetColorStart( 255, 255, 0, 0 );
		myFly.SetColorEnd( 255, 0, 255, 0 );
		myFly.SetScale( 1.0f, 2.5f );
		myFly.SetRoation( 1.0f, 5.0f );
		myFly.SetVelocity( 10, 25 );
		myFly.SetRange( 10, 10 );
		myFly.SetOffset( 8, 8 );

		myPosition.x = 650;
		myPosition.y = 100;

		myDirection.x = -1;
		myDirection.y = 1;

		mySize.width = 1;
		mySize.height = 1;

		myMinParticles = 10;
		myMaxParticles = 200;
		myTotalParticles = 0;

		myShape = 3; // point
		mySpawnRate = 5.0f;
		myLooping = false;
	}
}