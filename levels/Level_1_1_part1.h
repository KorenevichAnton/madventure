//
//  Level_1_1_part1.h
//  MusicalAdventure
//
//  Created by madmin on 6/2/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_1_part1__
#define __MusicalAdventure__Level_1_1_part1__

#include "ILevel.h"
#include "AppController.h"
#include "NotesMinigameScreen.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_1_part1: public ILevel , public cocos2d::Ref
{
public:
    static Level_1_1_part1*             create();
    
    void                                onEnter();
    void                                onExit();
    
    void                                update(float _dt);
    
    void                                release();
    
    bool                                onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind);
    void                                onPortal(GameObject* _portal);
    void                                onMinigameEnd(bool _isWin, screen_id _screen);
    void                                onDialogueEnd( int _id );
    void                                onSwitchArea(GameObject* _switcher) {};
    void                                onCheckPoint( GameObject* _checkPoint );
    void                                saveLevelStatData();
    
    void                                onVictory();
    void                                onDefeat();
    
    bool                                init();

    void                                determinePlayerPosition();
    
private:
    Level_1_1_part1();
    ~Level_1_1_part1();

private:
    void                                addNotesMinigame();
    void                                singleMinigameLogic();
    void                                callPostAnimation();

private:
    //minigame with notes screen
    NotesMinigameScreen*                pNotesMinigameScreen;
    
    //notes counter for minigame on the bridge
    int                                 mgSevenCoinsCounter;
    
    IAudioSource*                       aSource;

};

//-----------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__Level_1_1_part1__) */
