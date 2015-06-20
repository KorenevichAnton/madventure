//
//  AiMovablePlatformObject.h
//  MusicalAdventure
//
//  Created by madmin on 5/18/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiMovablePlatformObject__
#define __MusicalAdventure__AiMovablePlatformObject__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
class AiMovablePlatformObject: public IAiObject
{
public:
    static AiMovablePlatformObject*             create( tinyxml2::XMLElement* _xmlAiObject );
    void                                        release();
    
    void                                        update(float _dt);
    
    GameObject*                                 getGameObject() const { return pGameObject; }
    void                                        setGameObject(GameObject* _pGameObject) { pGameObject = _pGameObject; }
    
    void                                        setMovable(bool _state) { movable = _state; }
    bool const                                  getMovable() const { return movable; }
    
    void                                        setWayPoints(std::vector<cocos2d::Vec2> _wayPoints) { wayPoints.clear(); passedPoints = 0; wayPoints =  _wayPoints; };
    std::vector<cocos2d::Vec2>                  getWayPoints() { return wayPoints; };
    
private:
    AiMovablePlatformObject();
    ~AiMovablePlatformObject();
    
    bool                                        initWithXml( tinyxml2::XMLElement* _xmlAiObject );
    
    void                                        changePosition(const cocos2d::Vec2 _position) const;
    
private:
    bool                            cyclic;
    
    std::vector<cocos2d::Vec2>      wayPoints;
    float                           speed;
    
    int                             passedPoints;
    cocos2d::Vec2                   beginPos;
    //
    bool                            movable;
};

}   //namespase ai

//-----------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__AiMovablePlatformObject__) */
