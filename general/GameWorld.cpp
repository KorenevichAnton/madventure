#include "GameWorld.h"
#include "AppController.h"

#include "GameObject.h"
#include "GameCamera.h"

#include "PhysObjectFactory.h"
#include "AiObjectFactory.h"
#include "GraphicObjectFactory.h"

#include "LoadingScreen.h"


#define GAME_WORLD_LOADING_OFFSET   0.3f
#define GAME_WORLD_LOADING_WEIGHT   0.5f


using namespace std;
using namespace cocos2d;
using namespace tinyxml2;
using namespace physic;
using namespace graphic;
using namespace ai;

//-----------------------------------------------------------------------------------------------------
GameWorld* GameWorld::create( const char* _worldXmlPath, const char* _objectsXmlPath )
{
    GameWorld* pWorld = new GameWorld();
    
    if( pWorld->initFromXml( _worldXmlPath, _objectsXmlPath ) )
        return pWorld;
    else
    {
        pWorld->release();
        return nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::release()
{
    this->activeGameObjectList.clear();
    this->trashGameObjectsList.clear();

    // release physical world matrix
    for( int i = 0; i < this->worldWidth; i++ )
    {
        for( int j = 0; j < this->worldHeight; j++ )
        {
            if( this->worldMatrix[ i ][ j ] )
            {
                this->worldMatrix[ i ][ j ]->release();
                this->worldMatrix[ i ][ j ] = nullptr;
            }
        }
    }
    delete[] this->worldMatrix;
    this->worldMatrix = nullptr;

    //release game camera
    this->pGameCamera->release();
    
    delete this;
}

//-----------------------------------------------------------------------------------------------------
GameWorld::GameWorld():
//world matrix
worldMatrix( nullptr ),
worldWidth( 0 ),
worldHeight( 0 ),
dimensionX( 0 ),
dimensionY( 0 ),
//camera
pGameCamera( nullptr ),
//parallax
backgroundTilePath1( "" ),
midgroundTilePath1( "" ),
backgroundTilePath2( "" ),
midgroundTilePath2( "" ),
backgroundTilePath3( "" ),
midgroundTilePath3( "" ),
parallaxRect1( 0, 0, 0, 0 ),
parallaxRect2( 0, 0, 0, 0 ),
parallaxRect3( 0, 0, 0, 0 )
{
}

//-----------------------------------------------------------------------------------------------------
GameWorld::~GameWorld()
{
}

//-----------------------------------------------------------------------------------------------------
bool GameWorld::initFromXml( const char* _worldXmlPath, const char* _objectsXmlPath )
{
    auto pLoadingScreen = dynamic_cast< LoadingScreen* >( GET_SCREEN( GET_RUNNING_SCENE, screen_id::LOADING ) );

    // Open world xml file
    XMLDocument worldFile;
    if( worldFile.LoadFile( GET_FULL_PATH( _worldXmlPath ) ) )
    {
        log( "Error initialize of physical world, because file '%s' not may be opened!", _worldXmlPath );
        return false;
    }
    
    XMLElement* worldRootElement = worldFile.FirstChildElement( "world" );
    if( !worldRootElement )
    {
        log( "Error initialize of physical world, because file '%s' is not valid!", _worldXmlPath );
        return false;
    }
    
    
    // Open objects xml file
    XMLDocument objectsFile;
    if( objectsFile.LoadFile( GET_FULL_PATH( _objectsXmlPath ) ) )
    {
        log( "Error initialize of physical world, because file '%s' not may be opened!", _objectsXmlPath );
        return false;
    }
    
    XMLElement* objectsRootElement = objectsFile.FirstChildElement( "levelObjects" );
    if( !objectsRootElement )
    {
        log( "Error initialize of physical world, because file '%s' is not valid!", _objectsXmlPath );
        return false;
    }
    
    
    // Initialize landscape
    XMLElement* worldLandscapeElement = worldRootElement->FirstChildElement( "landscape" );
    if( worldLandscapeElement )
    {
        this->dimensionX = atoi( worldLandscapeElement->Attribute( "dimensionX" ) );
        this->dimensionY = atoi( worldLandscapeElement->Attribute( "dimensionY" ) );
                
        this->worldWidth = atoi( worldLandscapeElement->Attribute( "swidth" ) );
        this->worldHeight = atoi( worldLandscapeElement->Attribute( "sheight" ) );
                
        //allocate memory for physical world matrix
        this->worldMatrix = new GameSquare**[ this->worldWidth ];
        for( int i = 0; i < this->worldWidth; i++ )
        {
            this->worldMatrix[ i ] = new GameSquare*[ this->worldHeight ];
            for( int j = 0; j < this->worldHeight; j++ )
                this->worldMatrix[ i ][ j ] = nullptr;
        }
                
                
        // Initialize all squares in landscape
        int squareCounter = 0;
                
        XMLElement* worldSquareElement = worldLandscapeElement->FirstChildElement( "square" );
        while( worldSquareElement )
        {
            // set loading progress
            if( pLoadingScreen )
                pLoadingScreen->setProgress( ( float )squareCounter / ( this->worldWidth * this->worldHeight ) * GAME_WORLD_LOADING_WEIGHT + GAME_WORLD_LOADING_OFFSET );
                    
            squareCounter++;
                    
                    
            // add square to world matrix
            auto pSquare = GameSquare::create( this, worldSquareElement );
            this->addSquareToWorld( pSquare );
                    
                    
            // Initialize all objects in current square
            XMLElement* worldObjectElement = worldSquareElement->FirstChildElement( "object" );
            while( worldObjectElement )
            {
                int objectId = atoi( worldObjectElement->Attribute( "id" ) );
                
                XMLElement* objectsObjectElement = objectsRootElement->FirstChildElement( "object" );
                while( objectsObjectElement )
                {
                    if( atoi( objectsObjectElement->Attribute( "id" ) ) == objectId )
                    {
                        // create game object and add him to square
                        auto pGameObject = GameObject::create( pSquare, objectsObjectElement, Vec2( atof( worldObjectElement->Attribute( "x" ) ), atof( worldObjectElement->Attribute( "y" ) ) ) );
                        pSquare->addGameObject( pGameObject );
                        
                        // create and add physic to game object
                        XMLElement* objectsObjectPhysElement = objectsObjectElement->FirstChildElement( "physic" );
                        if( objectsObjectPhysElement )
                        {
                            IPhysObject* physObj = PhysObjectFactory::createPhysObject( objectsObjectPhysElement, this );
                            pGameObject->setPhysic( physObj );
                        }
                        
                        // create and add graphic to game object
                        XMLElement* objectsObjectGraphicElement = objectsObjectElement->FirstChildElement( "graphic" );
                        if( objectsObjectGraphicElement )
                        {
                            IGraphicObject* graphicObj = GraphicObjectFactory::createGraphicObject( objectsObjectGraphicElement );
                            pGameObject->setGraphic( graphicObj );
                        }
                        
                        // create and add ai to game object
                        XMLElement* objectsObjectAiElement = objectsObjectElement->FirstChildElement( "ai" );
                        if( objectsObjectAiElement )
                        {
                            IAiObject* aiObj = AiObjectFactory::createAiObject( objectsObjectAiElement );
                            pGameObject->setAi( aiObj );
                        }
                    }
                    
                    objectsObjectElement = objectsObjectElement->NextSiblingElement( "object" );
                }
        
                worldObjectElement = worldObjectElement->NextSiblingElement( "object" );
            }

            worldSquareElement = worldSquareElement->NextSiblingElement( "square" );
        }
    }

    
    
    // Initialize world camera
    XMLElement* worldCameraElement = worldRootElement->FirstChildElement( "camera" );
    assert( worldCameraElement );
    
    this->pGameCamera = GameCamera::create( worldCameraElement, this );
    
    
    
    // Initialize world parallax
    XMLElement* worldParallaxElement = worldRootElement->FirstChildElement( "parallax" );
    if( worldParallaxElement )
    {
        XMLElement* worldParallaxElementTile1Element = worldParallaxElement->FirstChildElement( "tile1" );
        if( worldParallaxElementTile1Element )
        {
            this->backgroundTilePath1 = worldParallaxElementTile1Element->Attribute( "backgroundTile" );
            this->midgroundTilePath1 = worldParallaxElementTile1Element->Attribute( "midgroundTile" );
        }
        
        XMLElement* worldParallaxElementTile2Element = worldParallaxElement->FirstChildElement("tile2");
        if( worldParallaxElementTile2Element )
        {
            this->backgroundTilePath2 = worldParallaxElementTile2Element->Attribute( "backgroundTile" );
            this->midgroundTilePath2 = worldParallaxElementTile2Element->Attribute( "midgroundTile" );
        }
        
        XMLElement* worldParallaxElementTile3Element = worldParallaxElement->FirstChildElement("tile3");
        if( worldParallaxElementTile3Element )
        {
            this->backgroundTilePath3 = worldParallaxElementTile3Element->Attribute( "backgroundTile" );
            this->midgroundTilePath3 = worldParallaxElementTile3Element->Attribute( "midgroundTile" );
        }
        
        
        XMLElement* worldParallaxElementRect1Element = worldParallaxElement->FirstChildElement("rect1");
        if( worldParallaxElementRect1Element )
        {
            this->parallaxRect1 = Rect( atof( worldParallaxElementRect1Element->Attribute( "x_c" ) ),
                                        atof( worldParallaxElementRect1Element->Attribute( "y_c" ) ),
                                        atof( worldParallaxElementRect1Element->Attribute( "width_c" ) ),
                                        atof( worldParallaxElementRect1Element->Attribute( "height_c" ) ) );
        }
        
        XMLElement* worldParallaxElementRect2Element = worldParallaxElement->FirstChildElement("rect2");
        if( worldParallaxElementRect2Element )
        {
            this->parallaxRect2 = Rect( atof( worldParallaxElementRect2Element->Attribute( "x_c" ) ),
                                        atof( worldParallaxElementRect2Element->Attribute( "y_c" ) ),
                                        atof( worldParallaxElementRect2Element->Attribute( "width_c" ) ),
                                        atof( worldParallaxElementRect2Element->Attribute( "height_c" ) ) );
        }
        
        XMLElement* worldParallaxElementRect3Element = worldParallaxElement->FirstChildElement("rect3");
        if( worldParallaxElementRect3Element )
        {
            this->parallaxRect3 = Rect( atof( worldParallaxElementRect3Element->Attribute( "x_c" ) ),
                                        atof( worldParallaxElementRect3Element->Attribute( "y_c" ) ),
                                        atof( worldParallaxElementRect3Element->Attribute( "width_c" ) ),
                                        atof( worldParallaxElementRect3Element->Attribute( "height_c" ) ) );
        }
    }

    
    
    // set loading progress
    if( pLoadingScreen )
        pLoadingScreen->setProgress( GAME_WORLD_LOADING_WEIGHT + GAME_WORLD_LOADING_OFFSET );
    
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::update( float _dt )
{
    // Quantization of delta time for ensure the stable update of physic.
    float dtRemained = _dt;
    while( dtRemained > 0 )
    {
        float dtQuant = QUANTUM_DT < dtRemained ? QUANTUM_DT : dtRemained;

        // update of active game objects calculation
        for( auto it = this->activeGameObjectList.begin(); it != this->activeGameObjectList.end(); it++ )
            ( *it )->update( dtQuant );
        
        // release game objects from trash
        for( auto it = this->trashGameObjectsList.begin(); it != this->trashGameObjectsList.end(); it++ )
            ( *it )->release();
        this->trashGameObjectsList.clear();
        
        dtRemained -= QUANTUM_DT;
    }

    
    // Update of game camera.
    this->pGameCamera->update( _dt );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
bool GameWorld::addSquareToWorld( GameSquare* _square )
{
	if( _square )
	{
        Vec2 position = _square->getPositionInSquares();
		int sx = position.x;
		int sy = position.y;
        
		if( ( sx >= 0 && sx < this->worldWidth ) && ( sy >= 0 && sy < this->worldHeight ) )
		{
			this->worldMatrix[ sx ][ sy ] = _square;
			return true;
		}
	}
    
	return false;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void GameWorld::addGameObjectToPreload( GameObject* _gameObject )
{
    if( _gameObject )
    {
        // check game object on been added
        for( auto it = this->preloadGameObjectList.begin(); it != this->preloadGameObjectList.end(); it++ )
        {
            if( *it == _gameObject )
                return;
        }
        
        this->preloadGameObjectList.push_back( _gameObject );
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::removeGameObjectFromPreload( GameObject* _gameObject )
{
    if( _gameObject )
    {
        for( auto it = this->preloadGameObjectList.begin(); it != this->preloadGameObjectList.end(); it++ )
        {
            if( *it == _gameObject )
            {
                this->preloadGameObjectList.erase( it );
                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::addGameObjectToActive( GameObject* _gameObject )
{
    if( _gameObject )
    {
        // check game object on been added
        for( auto it = this->activeGameObjectList.begin(); it != this->activeGameObjectList.end(); it++ )
        {
            if( *it == _gameObject )
                return;
        }
        
        this->activeGameObjectList.push_back( _gameObject );
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::removeGameObjectFromActive( GameObject* _gameObject )
{
    if( _gameObject )
    {
        for( auto it = this->activeGameObjectList.begin(); it != this->activeGameObjectList.end(); it++ )
        {
            if( *it == _gameObject )
            {
                this->activeGameObjectList.erase( it );
                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::removeGameObject( GameObject* _gameObject )
{
    if( _gameObject )
    {
        for( auto it = this->trashGameObjectsList.begin(); it != this->trashGameObjectsList.end(); it++ )
        {
            if( *it == _gameObject )
                return;
        }

        this->trashGameObjectsList.push_back( _gameObject );
    }
}

//-----------------------------------------------------------------------------------------------------
void GameWorld::removeGameObjects( const std::vector< GameObject* >& _gameObjects )
{
    for( auto it = _gameObjects.begin(); it != _gameObjects.end(); it++ )
        this->removeGameObject( *it );
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------