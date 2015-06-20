#ifndef __GAME_CAMERA__
#define __GAME_CAMERA__

#include "cocos2d.h"
#include "tinyxml2.h"

#include "GameWorld.h"

//-----------------------------------------------------------------------------------------------------
class GameCamera
{
public:
    static GameCamera*      create( tinyxml2::XMLElement* _xmlCamera, GameWorld* _pGameWorld );
    void                    release();
    
    void                    update( float _dt );
    
    
	// XY positioning.
	const cocos2d::Vec2     getPosition() const { return this->position; }
	void                    setPosition( const cocos2d::Vec2& _position );

	const float             getPositionX() const { return this->position.x; }
	void                    setPositionX( float _x );

	const float             getPositionY() const { return this->position.y; }	
	void                    setPositionY( float _y );
    
    
	// Z positioning ("== 1.0" - normal, "< 1.0" - zoom in, "> 1.0" - zoom out).
	const float             getDistance() const { return this->distance; }
	void                    setDistance( float _distance );
    

    // Locking positioning on axes.
    void                    lockAxisX( bool _lock ) { this->lockedAxisX = _lock; }
    void                    lockAxisY( bool _lock ) { this->lockedAxisY = _lock; }
    

	// Rectangle of calculational part of world.
	const cocos2d::Rect     getRect() const { return this->calcRect; }

    // Reload from logic to add unactive objects to part of the world.
    void                    dynamicReload();  // dynamic reload of calculational part of world
    
    // Set smooth movement of the camera to target.
    void                    moveCameraTo( const cocos2d::Vec2& _pos, float _time, bool _lock = false );
    
private:
	GameCamera(){};
	GameCamera( GameWorld* _pGameWorld );
    ~GameCamera();
    
    bool                    initWithXML( tinyxml2::XMLElement* _xmlCamera );

	void                    updateBoundingRect();	// Update the rectangle of bounding the camera move.
    void                    updatePreloadRect();    // Update the rectangle of preloading resources.
	void                    updateCalcRect();		// Update the rectangle of calculational part of world.
    
    void                    smoothMovement( float _dt );  // smooth movement of the camera.
   
    
private:
    GameWorld*          pGameWorld;

	// Positioning.
	cocos2d::Vec2       position;		// Camera position in cells, has anchor point in center of self.
	float				distance;		// Relative distance to the camera.
    
    bool                lockedAxisX;
    bool                lockedAxisY;

    
	// Bounding the camera move.
	cocos2d::Vec2       minBound;		// Minimal bound of the camera move (not changes value in squares).
	cocos2d::Vec2       maxBound;		// Maximal bound of the camera move (not changes value  in squares).
	cocos2d::Rect		boundingRect;	// Rectangle of bounding the camera move (changes due to changing distance to the camera). Measured in cells.

    // Preloading resources.
    cocos2d::Vec2       preloadRange;   // Range of preloading resources (not changes value in squares).
    cocos2d::Rect       preloadRect;    // Rectangle of preloading resources (changes due to changing position of the camera). Measured in cells.
    
	// Calculational part of world.
	cocos2d::Vec2       calcRange;		// Range of calculational part of world (not changes value in squares).
	cocos2d::Rect       calcRect;		// Rectangle of calculational part of world (changes due to changing position of the camera). Measured in cells.
    
    
    // Smooth movement of the camera.
    cocos2d::Vec2       smoothTargetPosition;
    float               smoothChangingTime;
    bool                smoothLockedReuse;
};

//-----------------------------------------------------------------------------------------------------
#endif  // __GAME_CAMERA__