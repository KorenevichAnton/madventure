//
//  AiFlyingMobObject.h
//  MusicalAdventure
//
//  Created by madmin on 06/17/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiFlyingMobObject__
#define __MusicalAdventure__AiFlyingMobObject__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
    class AiFlyingMobObject: public IAiObject
    {
    public:
        static AiFlyingMobObject*                   create( tinyxml2::XMLElement* _xmlAiObject );
        void                                        release();
        
        void                                        update(float _dt);
        
        GameObject*                                 getGameObject() const { return pGameObject; }
        void                                        setGameObject(GameObject* _pGameObject) { pGameObject = _pGameObject; }
        
    private:
        AiFlyingMobObject();
        ~AiFlyingMobObject();
        
        bool                                        initWithXml( tinyxml2::XMLElement* _xmlAiObject );
        
        void                                        changePosition(const cocos2d::Vec2 _position) const;
        
    public:
        void                                        onDamaged( GameObject* _contactObj );
        
        int                                         getHp() const { return hp; }
        void                                        setHp(int _hp) { hp = _hp; }
        
    private:
        int                             hp;
        
        bool                            cyclic;
        
        std::vector< cocos2d::Vec2 >    wayPoints;
        float                           speed;
        
        int                             passedPoints;
        cocos2d::Vec2                   beginPos;
    };
    
}   //namespase ai

//-----------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__AiFlyingMobObject__) */
