//
//  Level_1_1_part3.h
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_1_part3__
#define __MusicalAdventure__Level_1_1_part3__

#include "ILevel.h"
#include "AppController.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_1_part3: public ILevel , public cocos2d::Ref
{
public:
    static Level_1_1_part3*             create();
    
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
    
    void                                onVictory();
    void                                onDefeat();
    
    bool                                init();
    
    void                                determinePlayerPosition();
    
private:
    Level_1_1_part3();
    ~Level_1_1_part3();

public:
    void                                flipXObjects(int _idObj);
private:
    bool                                cameraFollowPlayer;
    bool                                cameraFollowPlayerY;
    bool                                cameraAutoReplace;
    bool                                cameraAutoReplaceLeft;
    bool                                cameraAutoReplaceRight;
    
    IAudioSource*                       aSource;
    
private:
    void                                removeTestVoiceScreen( cocos2d::Ref* _sender );
    
    void                                singleMinigameLogic();
    
    void                                followPlayerCameraY();

};

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__Level_1_1_part3__) */
