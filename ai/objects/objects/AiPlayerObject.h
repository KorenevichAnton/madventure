//
//  AiPlayerObject.h
//  MusicalAdventure
//
//  Created by pool on 09.07.14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiPlayerObject__
#define __MusicalAdventure__AiPlayerObject__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
    class AiPlayerObject: public IAiObject
    {
    public:
        static AiPlayerObject*                      create( tinyxml2::XMLElement* _xmlAiObject );
        void                                        release();
        
        void                                        update(float _dt);
        
        GameObject*                                 getGameObject() const { return pGameObject; }
        void                                        setGameObject( GameObject* _pGameObject ) { pGameObject = _pGameObject; }
    
    private:
        AiPlayerObject();
        ~AiPlayerObject();
        
        bool                                        initWithXml( tinyxml2::XMLElement* _xmlAiObject );
    
    public:
        void                                        fire();
        void                                        moveLeft();
        void                                        moveRight();
        void                                        jump();
        
        void                                        stop();
        
        void                                        onDamaged( GameObject* _contactObj );

        bool                                        getDamagedState( bool damagedState ) const { return damagedState; };
        
        bool                                        isDead() { return this->deadState; };
        
    private:
        
        bool                deadState;
        
        int                 shotObjectId;
        
        float               damagedTime;
        bool                damagedState;
    };
    
}   //namespace ai

//-----------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__AiPlayerObject__) */