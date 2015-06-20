#ifndef __MusicalAdventure__IAiObject__
#define __MusicalAdventure__IAiObject__

#include "cocos2d.h"
#include "tinyxml2.h"

//-----------------------------------------------------------------------------------------------------
class GameObject;

namespace ai
{
    
enum AiObjectType
{
    AI_DEFAULT_OBJ = 0,
    AI_PLAYER_OBJ,
    AI_MOVABLE_PLATFORM_OBJ,
    AI_ROPE_OBJ,
    AI_UNIT_OBJ,
    AI_BOSS1_OBJ,
    AI_SHOT_OBJ,
    AI_WATERSTREAM_OBJ,
    AI_NPC_BAT_OBJ
};

//-----------------------------------------------------------------------------------------------------
class IAiObject
{
public:
    virtual void                                    release() = 0;
    virtual void                                    update( float _dt ) = 0;
    
    
    AiObjectType                                    getType() const { return this->type; }
    
    virtual GameObject*                             getGameObject() const = 0;
    virtual void                                    setGameObject( GameObject* _pGameObject ) = 0;
    
    bool                                            getActivity() const { return this->activity; }
    void                                            setActivity( bool _activity ) { this->activity = _activity; }
    
protected:
    IAiObject();
    IAiObject( const IAiObject& ){};
    ~IAiObject();
    
    IAiObject& operator=( const IAiObject& ){ return *this; }
    
    
protected:
    AiObjectType            type;
    GameObject*             pGameObject;
    
    bool                    activity;
};
    
}  // namespase ai

//-----------------------------------------------------------------------------------------------------
#endif  // __MusicalAdventure__IAiObject__