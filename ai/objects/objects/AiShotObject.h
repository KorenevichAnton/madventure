//
//  AiShotObject.h
//  MusicalAdventure
//
//  Created by korenevichanton@gmail.com on 7/2/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiShotObject__
#define __MusicalAdventure__AiShotObject__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
    class AiShotObject: public IAiObject
    {
    public:
        static AiShotObject*                        create( tinyxml2::XMLElement* _xmlAiObject );
        void                                        release();
        
        void                                        update(float _dt);
        
        GameObject*                                 getGameObject() const { return pGameObject; }
        void                                        setGameObject(GameObject* _pGameObject) { pGameObject = _pGameObject; }
        
        void                                        setPlayersShot(bool _state) { playersShot = _state; }
        bool                                        getPlayersShot() { return playersShot; }
        
    private:
        AiShotObject();
        ~AiShotObject();
        
        bool                                        initWithXml( tinyxml2::XMLElement* _xmlAiObject );
        
        void                                        changePosition(const cocos2d::Vec2 _position) const;
        
    public:
        void                                        onHitStaticObject( GameObject* _contactObj );
        void                                        onHitDynamicObject( GameObject* _contactObj );
        
    private:
        bool                            cyclic;
        
        bool                            playersShot;
        
        std::vector<cocos2d::Vec2>      wayPoints;
        float                           speed;
        
        int                             passedPoints;
        cocos2d::Vec2                   beginPos;
    };
    
}   //namespase ai

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__AiShotObject__) */
