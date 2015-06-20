#include "GameCamera.h"
#include "AppController.h"
#include "GameSquare.h"
#include "GameObject.h"

using namespace std;
using namespace cocos2d;
using namespace tinyxml2;

//-----------------------------------------------------------------------------------------------------
GameCamera* GameCamera::create( XMLElement* _xmlCamera, GameWorld* _pGameWorld )
{
    GameCamera* pRet = new GameCamera( _pGameWorld );
    if( pRet && pRet->initWithXML( _xmlCamera ) )
        return pRet;
    else
    {
        if( pRet )
			pRet->release();
        
        return nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::release()
{   
    delete this;
}

//-----------------------------------------------------------------------------------------------------
GameCamera::GameCamera( GameWorld* _pGameWorld ):
pGameWorld( _pGameWorld ),
// positioning
position( -1, -1 ), // fake value for set first camera position with dynamic load of calculational part of world
distance( CAMERA_DISTANCE ),   // normal value
// lock movement on axes
lockedAxisX( false ),
lockedAxisY( false ),
// bounding the camera move
minBound( Vec2::ZERO ),
maxBound( Vec2::ZERO ),
boundingRect( Rect::ZERO ),
// preloading resources
preloadRange( Vec2::ZERO ),
preloadRect( Rect::ZERO ),
// calculational part of world
calcRange( Vec2::ZERO ),
calcRect( Rect::ZERO ),
// smooth movement of the camera
smoothChangingTime( 0.0f ),
smoothTargetPosition( Vec2::ZERO ),
smoothLockedReuse( false )
{
}

//-----------------------------------------------------------------------------------------------------
GameCamera::~GameCamera()
{
}

//-----------------------------------------------------------------------------------------------------
bool GameCamera::initWithXML( XMLElement* _xmlCamera )
{
    if( _xmlCamera )
    {
		// Set bounding of the camera move.
		XMLElement* boundingElement = _xmlCamera->FirstChildElement( "bounding" );
		if( boundingElement )
		{
			this->minBound = Vec2( atof( boundingElement->Attribute( "minX_s" ) ),
								   atof( boundingElement->Attribute( "minY_s" ) ) );

			this->maxBound = Vec2( atof( boundingElement->Attribute( "maxX_s" ) ),
								   atof( boundingElement->Attribute( "maxY_s" ) ) );
		}
		else
		{
            this->minBound = Vec2::ZERO;
			this->maxBound = Vec2( this->pGameWorld->getWorldWidth() - 1,
								   this->pGameWorld->getWorldHeight() - 1 );
		}

		this->updateBoundingRect();

        
        // Set the range of preloading resources.
		XMLElement* preloadElement = _xmlCamera->FirstChildElement( "preloading" );
		if( preloadElement )
		{
			this->preloadRange = Vec2( atof( preloadElement->Attribute( "hor_s" ) ),
                                       atof( preloadElement->Attribute( "ver_s" ) ) );
		}
        
        
        // Set the range of calculational part of world.
		XMLElement* calcElement = _xmlCamera->FirstChildElement( "calculating" );
		if( calcElement )
		{
			this->calcRange = Vec2( atof( calcElement->Attribute( "hor_s" ) ),
                                    atof( calcElement->Attribute( "ver_s" ) ) );
		}
        
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::update( float _dt )
{
    this->smoothMovement( _dt );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameCamera::setPosition( const Vec2& _position )
{     
	Vec2 squareSize = Vec2( this->pGameWorld->getDimensionX(),
							this->pGameWorld->getDimensionY() );


	Vec2 newPosition = _position;
    
    // Check on locking axes
    if( this->lockedAxisX )
        newPosition.x = this->position.x;
    if( this->lockedAxisY )
        newPosition.y = this->position.y;

    // Check on the camera move bound.
    if( newPosition.x < this->boundingRect.origin.x )
        newPosition.x = this->boundingRect.origin.x;
    
    if( newPosition.y < this->boundingRect.origin.y )
        newPosition.y = this->boundingRect.origin.y;
    
    if( newPosition.x > this->boundingRect.getMaxX() )
        newPosition.x = this->boundingRect.getMaxX();
    
    if( newPosition.y > this->boundingRect.getMaxY() )
        newPosition.y = this->boundingRect.getMaxY();

    
    // Set a new position point of the camera.
    if( !this->position.equals( newPosition ) )
    {
		Vec2 oldPos = Vec2( floor( this->position.x / squareSize.x ),
							floor( this->position.y / squareSize.y ) );

		Vec2 newPos = Vec2( floor( newPosition.x / squareSize.x ),
							floor( newPosition.y / squareSize.y ) );

        
        this->position = newPosition;
		
        // Update a rects for preloading and calculating.
        this->updatePreloadRect();
		this->updateCalcRect();

		// Dynamic reload of game objects.
		if( !oldPos.equals( newPos ) )
			this->dynamicReload();
    }

	//log( "GAME_CAMERA: position { %f, %f }", this->position.x, this->position.y );
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::setPositionX( float _x )
{
	this->setPosition( Vec2( _x, this->position.y ) );
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::setPositionY( float _y )
{
	this->setPosition( Vec2( this->position.x, _y ) );
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::setDistance( float _distance )
{
	if( _distance > 0 )
	{
		this->distance = _distance;

		this->updateBoundingRect();
		this->setPosition( this->position );
	}
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameCamera::updateBoundingRect()
{
	Vec2 squareSize = Vec2( this->pGameWorld->getDimensionX(),
							this->pGameWorld->getDimensionY() );

	Vec2 screenCenter = ( FRAME_CENTER / CELL_SIZE ) * this->distance;

	this->boundingRect.origin.x = this->minBound.x * squareSize.x + screenCenter.x;
	this->boundingRect.origin.y = this->minBound.y * squareSize.y + screenCenter.y;

	this->boundingRect.size.width = ( this->maxBound.x + 1 ) * squareSize.x - this->boundingRect.origin.x - screenCenter.x;
	this->boundingRect.size.height = ( this->maxBound.y + 1 ) * squareSize.y - this->boundingRect.origin.y - screenCenter.y;
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::updatePreloadRect()
{
    Vec2 worldSize = Vec2( this->pGameWorld->getWorldWidth(),
                           this->pGameWorld->getWorldHeight() );
    
	Vec2 squareSize = Vec2( this->pGameWorld->getDimensionX(),
                            this->pGameWorld->getDimensionY() );
	
	// Set rect relatively camera position and her range.
	this->preloadRect.setRect( ( floor( this->position.x / squareSize.x ) - this->preloadRange.x ) * squareSize.x,
                               ( floor( this->position.y / squareSize.y ) - this->preloadRange.y ) * squareSize.y,
                               ( this->preloadRange.x * 2 + 1 ) * squareSize.x,
                               ( this->preloadRange.y * 2 + 1 ) * squareSize.y );
    
	// Check on bounds of world.
    if( this->preloadRect.origin.x < 0 )
        this->preloadRect.origin.x = 0;
    
    if( this->preloadRect.origin.y < 0 )
        this->preloadRect.origin.y = 0;
    
    if( this->preloadRect.getMaxX() >= worldSize.x * squareSize.x )
        this->preloadRect.origin.x = worldSize.x * squareSize.x - this->preloadRect.size.width;
    
    if( this->preloadRect.getMaxY() >= worldSize.y * squareSize.y )
        this->preloadRect.origin.y = worldSize.y * squareSize.y - this->preloadRect.size.height;
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::updateCalcRect()
{
	Vec2 worldSize = Vec2( this->pGameWorld->getWorldWidth(),
						   this->pGameWorld->getWorldHeight() );

	Vec2 squareSize = Vec2( this->pGameWorld->getDimensionX(),
						    this->pGameWorld->getDimensionY() );
	
	// Set rect relatively camera position and her range.
	this->calcRect.setRect( ( floor( this->position.x / squareSize.x ) - this->calcRange.x ) * squareSize.x,
							( floor( this->position.y / squareSize.y ) - this->calcRange.y ) * squareSize.y,
							( this->calcRange.x * 2 + 1 ) * squareSize.x,
							( this->calcRange.y * 2 + 1 ) * squareSize.y );

	// Check on bounds of world.
    if( this->calcRect.origin.x < 0 )
        this->calcRect.origin.x = 0;

    if( this->calcRect.origin.y < 0 )
        this->calcRect.origin.y = 0;
     
    if( this->calcRect.getMaxX() >= worldSize.x * squareSize.x )
        this->calcRect.origin.x = worldSize.x * squareSize.x - this->calcRect.size.width;
            
    if( this->calcRect.getMaxY() >= worldSize.y * squareSize.y )
        this->calcRect.origin.y = worldSize.y * squareSize.y - this->calcRect.size.height;
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::dynamicReload()
{
	landscape worldMatrix = this->pGameWorld->getWorldMatrix();
    for( int i = 0; i < this->pGameWorld->getWorldWidth(); i++ )
    {
        for( int j = 0; j < this->pGameWorld->getWorldHeight(); j++ )
        {
            const vector< GameObject* > squareGameObjectList = worldMatrix[ i ][ j ]->getGameObjects();
            for( auto it = squareGameObjectList.begin(); it < squareGameObjectList.end(); it++ )
            {
                Vec2 objPos = ( *it )->getPosition();
                
                // Preload a game objects, if they are inside of the preloading rect of the camera.
                if( objPos.x >= this->preloadRect.origin.x &&
                    objPos.x < this->preloadRect.getMaxX() &&
                    objPos.y >= this->preloadRect.origin.y &&
                    objPos.y < this->preloadRect.getMaxY() )
                {
                    this->pGameWorld->addGameObjectToPreload( *it );
                    ( *it )->preloadResources();
                }

                // Attach a game objects, if they are inside of the calculating rect of the camera.
                if( objPos.x >= this->calcRect.origin.x &&
                    objPos.x < this->calcRect.getMaxX() &&
                    objPos.y >= this->calcRect.origin.y &&
                    objPos.y < this->calcRect.getMaxY() )
                {
                    this->pGameWorld->addGameObjectToActive( *it );
                    ( *it )->attachResources();
                }
            }
        }
    }

	
    // Detach a game objects, if they are outside of the calculating rect of the camera.
    const vector< GameObject* > activeObjectList = this->pGameWorld->getActiveGameObjects();
    for( auto it = activeObjectList.begin(); it < activeObjectList.end(); it++ )
    {
        Vec2 objPos = ( *it )->getPosition();
        
        if( objPos.x < this->calcRect.origin.x ||
            objPos.x >= this->calcRect.getMaxX() ||
            objPos.y < this->calcRect.origin.y ||
            objPos.y >= this->calcRect.getMaxY() )
        {
            this->pGameWorld->removeGameObjectFromActive( *it );
            ( *it )->detachResources();
        }
    }
    
    // Unload (and detach if need) a game objects, if they are outside of the preloading rect of the camera.
	const vector< GameObject* > preloadObjectList = this->pGameWorld->getPreloadGameObjects();
    for( auto it = preloadObjectList.begin(); it < preloadObjectList.end(); it++ )
    {
        Vec2 objPos = ( *it )->getPosition();

        if( objPos.x < this->preloadRect.origin.x ||
            objPos.x >= this->preloadRect.getMaxX() ||
            objPos.y < this->preloadRect.origin.y ||
            objPos.y >= this->preloadRect.getMaxY() )
        {
            this->pGameWorld->removeGameObjectFromActive( *it );
            this->pGameWorld->removeGameObjectFromPreload( *it );
            
            //( *it )->releaseResources();  // CHANGES: fixed FPS problem.
        }
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameCamera::moveCameraTo( const Vec2& _pos, float _time, bool _lock )
{
    if( !this->smoothLockedReuse && _time > 0 )
    {
        this->smoothChangingTime = _time;
        this->smoothTargetPosition = _pos;
        this->smoothLockedReuse = _lock;
    }
}

//-----------------------------------------------------------------------------------------------------
void GameCamera::smoothMovement( float _dt )
{
    if( this->smoothChangingTime > 0 )
    {
        if( this->smoothChangingTime - _dt > 0 )
        {
            Vec2 changingStep = ( this->smoothTargetPosition - this->position ) / this->smoothChangingTime * _dt;

            this->setPosition( this->position + changingStep );
            this->smoothChangingTime -= _dt;
        }
        else
        {
            this->setPosition( this->smoothTargetPosition );
            this->smoothChangingTime = 0;
            this->smoothLockedReuse = false;
        }
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------