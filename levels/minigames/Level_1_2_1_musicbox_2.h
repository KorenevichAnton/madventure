//
//  Level_1_2_1_musicbox_2.h
//  MusicalAdventure
//
//  Created by ampa_games on 8/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_2_1_musicbox_2__
#define __MusicalAdventure__Level_1_2_1_musicbox_2__

#include "ILevel.h"
#include "AppController.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_2_1_musicbox_2: public ILevel , public cocos2d::Ref
{
public:
    static Level_1_2_1_musicbox_2*                  create();
    void                                            onEnter();
    void                                            onExit();
    
    void                                            update(float _dt);
    
    void                                            release();
    
    bool                                            onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind);
    void                                            onPortal(GameObject* _portal);
    void                                            onMinigameEnd(bool _isWin, screen_id _screen);
    void                                            onSwitchArea(GameObject* _switcher);
    void                                            onDialogueEnd( int _id );
    void                                            onCheckPoint( GameObject* _checkPoint ) {};
    void                                            saveLevelStatData();
    
    void                                            onVictory();
    void                                            onDefeat();
    
    bool                                            init();
    
    void                                            determinePlayerPosition();
    
private:
    Level_1_2_1_musicbox_2();
    ~Level_1_2_1_musicbox_2();

public:
    void                                onDialogHint();
    
private:
    IAudioSource*                       aSource;
    
};

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__Level_1_2_1_musicbox_2__) */
