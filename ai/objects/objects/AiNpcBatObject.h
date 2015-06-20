//
//  AiNpcBatObject.h
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiNpcBatObject__
#define __MusicalAdventure__AiNpcBatObject__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
    class AiNpcBatObject: public IAiObject
    {
    public:
        static AiNpcBatObject*          create( tinyxml2::XMLElement* _xmlAiObject );
        void                            release();
        
        void                            update(float _dt);
        
        GameObject*                     getGameObject() const { return pGameObject; }
        void                            setGameObject(GameObject* _pGameObject) { pGameObject = _pGameObject; }
        
        void                            moveTo( cocos2d::Vec2 _targetPosition, float _speed, const std::function<void()> &_functionCallback );
    private:
        AiNpcBatObject();
        ~AiNpcBatObject();
        
        bool                            initWithXml( tinyxml2::XMLElement* _xmlAiObject );
        
    private:
        bool                            isStart;
        
        float                           speed;
        
        std::function<void()>           functionCallBackOnMoveTo;

        
        //move
        cocos2d::Vec2                targetPos;
        
        cocos2d::Vec2                beginPosition;
        cocos2d::Vec2                targetPosition;
        
    };
    
}   //namespase ai

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__AiNpcBatObject__) */
