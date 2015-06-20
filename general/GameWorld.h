#ifndef __GAME_WORLD__
#define __GAME_WORLD__

#include "cocos2d.h"
#include "tinyxml2.h"

#include "PhysConfig.h"
#include "GameSquare.h"

//-----------------------------------------------------------------------------------------------------
class GameCamera;

typedef GameSquare***   landscape;

//-----------------------------------------------------------------------------------------------------
class GameWorld
{
public:
    static GameWorld*                   create( const char* _worldXmlPath, const char* _objectsXmlPath );
    void                                release();
        
    void                                update( float _dt );
    
    
    // world matrix
    const landscape                     getWorldMatrix() const { return this->worldMatrix; }
        
    int                                 getWorldWidth() const { return this->worldWidth; }
    int                                 getWorldHeight() const { return this->worldHeight; }
        
    int                                 getDimensionX() const { return this->dimensionX; };
    int                                 getDimensionY() const { return this->dimensionY; };
        
    bool                                addSquareToWorld( GameSquare* _square );


    // camera
    GameCamera*                         getGameCamera() const { return this->pGameCamera; }
        
    
    // Preloading of game objects.
    const std::vector< GameObject* >&   getPreloadGameObjects() const { return this->preloadGameObjectList; }
    
    void                                addGameObjectToPreload( GameObject* _gameObject );
    void                                removeGameObjectFromPreload( GameObject* _gameObject );
    
    // Activity of game objects.
    const std::vector< GameObject* >&   getActiveGameObjects() const { return this->activeGameObjectList; }
    
    void                                addGameObjectToActive( GameObject* _gameObject );
    void                                removeGameObjectFromActive( GameObject* _gameObject );
    
    // Removing of game objects.
    void                                removeGameObject( GameObject* _gameObject );
    void                                removeGameObjects( const std::vector< GameObject* >& _gameObjects );

        
    // parallax
    const char*                         getBackgroundTilePath1() const { return this->backgroundTilePath1.c_str(); }
    const char*                         getMidgroundTilePath1() const { return this->midgroundTilePath1.c_str(); }
        
    const char*                         getBackgroundTilePath2() const { return this->backgroundTilePath2.c_str(); }
    const char*                         getMidgroundTilePath2() const { return this->midgroundTilePath2.c_str(); }
        
    const char*                         getBackgroundTilePath3() const { return this->backgroundTilePath3.c_str(); }
    const char*                         getMidgroundTilePath3() const { return this->midgroundTilePath3.c_str(); }
        
    cocos2d::Rect                       getParallaxRect1() const { return this->parallaxRect1; }
    cocos2d::Rect                       getParallaxRect2() const { return this->parallaxRect2; }
    cocos2d::Rect                       getParallaxRect3() const { return this->parallaxRect3; }
        
private:
    GameWorld();
    GameWorld( const GameWorld& ){};
    ~GameWorld();
    
    GameWorld& operator=( const GameWorld& ){ return *this; }
    
        
    bool                                initFromXml( const char* _worldXmlPath, const char* _objectsXmlPath );
	
    
private:
    // world matrix
    landscape                   worldMatrix;        // world matrix
    int                         worldWidth;         // width of world matrix in square's
    int                         worldHeight;        // height of world matrix in square's
    int                         dimensionX;         // square width in cell's
    int                         dimensionY;         // square height in cell's
        
        
    // camera is manager of dynamic reload of calculational part of world
    GameCamera*					pGameCamera;

    
    // game objects on preloading
    std::vector< GameObject* >  preloadGameObjectList;
    
    // active game objects
    std::vector< GameObject* >  activeGameObjectList;
        
    // game objects to remove
    std::vector< GameObject* >  trashGameObjectsList;
        
    // parallax
    std::string                 backgroundTilePath1;
    std::string                 midgroundTilePath1;
        
    std::string                 backgroundTilePath2;
    std::string                 midgroundTilePath2;
        
    std::string                 backgroundTilePath3;
    std::string                 midgroundTilePath3;
        
    cocos2d::Rect               parallaxRect1;
    cocos2d::Rect               parallaxRect2;
    cocos2d::Rect               parallaxRect3;
};

//-----------------------------------------------------------------------------------------------------
#endif  // __GAME_WORLD__