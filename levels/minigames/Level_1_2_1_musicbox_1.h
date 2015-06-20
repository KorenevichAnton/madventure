//
//  Level_1_2_1_musicbox_1.h
//  MusicalAdventure
//
//  Created by ampa_games on 8/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_2_1_musicbox_1__
#define __MusicalAdventure__Level_1_2_1_musicbox_1__

#include "ILevel.h"
#include "AppController.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_2_1_musicbox_1:  public cocos2d::Ref, public ILevel
{
public:
    static Level_1_2_1_musicbox_1*                  create();
    void                                            onEnter();
    void                                            onExit();
    
    void                                            update(float _dt);
    
    void                                            release();
    
    bool                                            onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind);
    void                                            onPortal(GameObject* _portal);
    void                                            onMinigameEnd(bool _isWin, screen_id _screen);
    void                                            onSwitchArea(GameObject* _switcher) {};
    void                                            onDialogueEnd( int _id );
    void                                            onCheckPoint( GameObject* _checkPoint ) {};
    void                                            saveLevelStatData();
    
    void                                            onVictory();
    void                                            onDefeat();

    bool                                            init();
    
    void                                            determinePlayerPosition();
    
private:
    Level_1_2_1_musicbox_1();
    ~Level_1_2_1_musicbox_1();
    
private:
    IAudioSource*                       aSource;
    
    cocos2d::Sequence*                  defeatAction;
    cocos2d::Sequence*                  winAction;
    
};

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__Level_1_2_1_musicbox_1__) */
