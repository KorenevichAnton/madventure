//
//  AiBoss1Object.h
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__AiBoss1Object__
#define __MusicalAdventure__AiBoss1Object__

#include "IAiObject.h"

//-----------------------------------------------------------------------------------------------------
namespace ai
{
    
    class AiBoss1Object: public IAiObject
    {
    public:
        static AiBoss1Object*                       create( tinyxml2::XMLElement* _xmlAiObject );
        void                                        release();
        
        void                                        update(float _dt);
        
        GameObject*                                 getGameObject() const { return pGameObject; }
        void                                        setGameObject(GameObject* _pGameObject) { pGameObject = _pGameObject; }
        
        void                                        setStartBoss(bool _state) { startBoss = _state; };
        
    private:
        AiBoss1Object();
        ~AiBoss1Object();
        
        bool                                        initWithXml( tinyxml2::XMLElement* _xmlAiObject );
        
        void                                        changePosition(const cocos2d::Vec2 _position) const;
        
        
        void                                        fire(float _dt);
        void                                        createShoot();
        
    public: 
        //HP percent
        const int                                   getHpRate() const { return hp * 100 / fullHp; }
        //HP
        int                                         getHp() const { return hp; }
        void                                        setHp(int _hp) { hp = _hp; }
        
        const int                                   getFullHp() const { return fullHp; }
        void                                        setFullHp(int _fullHp) { fullHp = _fullHp; }
        
        void                                        onDamaged( GameObject* _contactObj );
        
        int                                         getCountShoot() { return shotCount; };
        
        bool                                        isTouchable() { return this->afterTouchState; };
        
    private:
        int                             hp;
        int                             fullHp;
        //
        //shot config
        bool                            canShot;
        float                           shotTimer;
        float                           shotDelay;
        int                             shotCount;
        
        bool                            startBoss;
        //move
        cocos2d::Vec2                   beginPosition;
        cocos2d::Vec2                   targetPosition;
        int                             pathCounter;
    
        void                            setTargetBossPath();
        void                            getTargetBossPath();
        
        //Disable damage for some time after getting damage
        bool                            afterTouchState;
        float                           disableDmgTimeLimit;
        float                           disableDmgTimer;
    };
    
}   //namespase ai

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__AiBoss1Object__) */
