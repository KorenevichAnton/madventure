#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "cocos2d.h"
#include "tinyxml2.h"

#include "IPhysObject.h"
#include "IGraphicObject.h"
#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
enum class game_object_location
{
    NONE = 0,
    SPACE,
    GROUND,
    ROPE,
    STREAM_WATER
};

enum class game_object_direction
{
    NONE = 0,
    LEFT,
    RIGHT
};

//-----------------------------------------------------------------------------------------------------
class GameObject
{
public:
    static GameObject*          create( GameSquare* _parentSquare, tinyxml2::XMLElement* _objectXML, cocos2d::Vec2 _position );
    void                        release();
    
    void                        update( float _dt );
    
    
    void                        setId( int _id ) { this->id = _id; }
    const int                   getId() const { return this->id; }
    
    
    physic::IPhysObject*        getPhysic() const { return this->pPhysic; }
    void                        setPhysic( physic::IPhysObject* _pPhysic );
    
    graphic::IGraphicObject*    getGraphic() const { return this->pGraphic; }
    void                        setGraphic( graphic::IGraphicObject* _pGraphic );
    
    ai::IAiObject*              getAi() const { return this->pAi; }
    void                        setAi( ai::IAiObject* _pAi );
    

    void                        preloadResources();
    void                        releaseResources();
    
    void                        attachResources();
    void                        detachResources();
    
    
    bool                        getActivity() const { return this->activity; }
    void                        setActivity( bool _activity ) { this->activity = _activity; }
    
    
    cocos2d::Vec2               getPosition() const { return this->position; }
    void                        setPosition( const cocos2d::Vec2& _position );

    void                        onPhysicChangePosition( const cocos2d::Vec2& _position );
    
    
    game_object_location        getLocation() const { return this->location; }
    void                        setLocation( game_object_location _location ) { this->location = _location; }
    
    game_object_direction       getDirection() const { return this->direction; }
    void                        setDirection( game_object_direction _direction ) { this->direction = _direction; }

private:
    GameObject();
    GameObject( const GameObject& ){};
    ~GameObject();
    
    GameObject& operator=( const GameObject& ){ return *this; }
    
    
    bool                        initWithXML( tinyxml2::XMLElement* _objectXML );
    
    void                        dynamicReload();
    
    
private:
    int                         id;
    
    physic::IPhysObject*        pPhysic;
    graphic::IGraphicObject*    pGraphic;
    ai::IAiObject*              pAi;
    
    GameSquare*                 pParentSquare;
    
    bool                        activity;
    
    cocos2d::Vec2               position;

    game_object_location        location;
    game_object_direction       direction;
};

//-----------------------------------------------------------------------------------------------------
#endif  // __GAME_OBJECT__