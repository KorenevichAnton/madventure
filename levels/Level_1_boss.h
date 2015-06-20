//
//  Level_1_boss.h
//  MusicalAdventure
//
//  Created by madmin on 6/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_boss__
#define __MusicalAdventure__Level_1_boss__

#include "ILevel.h"
#include "AppController.h"

#include "Boss1_MGScreen.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_boss: public ILevel , public cocos2d::Ref
{
public:
    static Level_1_boss*                create();
    
    void                                onEnter();
    void                                onExit();
    
    void                                update(float _dt);
    
    void                                release();

    bool                                onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind);
    void                                onPortal(GameObject* _portal);
    void                                onMinigameEnd(bool _isWin, screen_id _screen);
    void                                onSwitchArea(GameObject* _switcher);
    void                                onDialogueEnd( int _id );
    void                                onCheckPoint( GameObject* _checkPoint );
    void                                saveLevelStatData();
    
    void                                unShadow();
    
    void                                setStartBossMG(bool _state) { startBossMG = true; };
    
    void                                setGameWorld( GameWorld* _pGameWorld );
    
    void                                onVictory();
    void                                onDefeat();
    
    bool                                init();
    
    void                                determinePlayerPosition();
    
private:
    Level_1_boss();
    ~Level_1_boss();
    
private:
    cocos2d::Layer*                     shadow;
    cocos2d::Sprite*                    preShadow;
    cocos2d::Sprite*                    afterShadow;
    
    bool                                cameraFollowPlayer;
    bool                                cameraFollowPlayerY;
    bool                                cameraAutoReplace;
    bool                                cameraAutoReplaceLeft;
    bool                                cameraAutoReplaceRight;
    
    Boss1_MGScreen*                     pBossMinigameScreen;
    
    float                               shadowValue;
    
    int                                 earthquaker;
    float                               earthquakeTime;
    float                               earthquakeLimit;
    
    GameObject*                         bossObject;
    bool                                startBossMG;
    
    bool                                afterES;
    
private:
    void                                followPlayerCameraY();
    
    void                                singleMinigameLogic();
    //
    
    void                                onBatPositionSoldier();
    void                                onBatPositionBlind();
    void                                onBatPositionLeaveBoss();
    
    IAudioSource*                       aSource;
    
    int                                 oldHpBoss;
    
};

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__Level_1_boss__) */
