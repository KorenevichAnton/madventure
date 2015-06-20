//
//  Level_1_2_2_musicbox_3.h
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__Level_1_2_2_musicbox_3__
#define __MusicalAdventure__Level_1_2_2_musicbox_3__

#include "ILevel.h"
#include "AppController.h"

#include "L1_2_MusicBox3_MGScreen.h"

//-----------------------------------------------------------------------------------------------------
class Level_1_2_2_musicbox_3: public ILevel , public cocos2d::Ref
{
public:
    static Level_1_2_2_musicbox_3*                  create();
    void                                            onEnter();
    void                                            onExit();
    
    void                                            update(float _dt);
    
    void                                            release();
    
    bool                                            onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind);
    void                                            onPortal(GameObject* _portal);
    void                                            onMinigameEnd(bool _isWin, screen_id _screen);
    void                                            onSwitchArea(GameObject* _switcher){};
    void                                            onDialogueEnd( int _id );
    void                                            onCheckPoint( GameObject* _checkPoint ) {};
    void                                            saveLevelStatData();
    
    
    void                                            setMusicBox3SumForWin(float _value) { musicBox3SumForWin = _value; }
    float                                           getMusicBox3SumForWin() { return musicBox3SumForWin; }
    
    void                                            onVictory();
    void                                            onDefeat();
    
    bool                                            init();
    
    void                                            determinePlayerPosition();
    
private:
    Level_1_2_2_musicbox_3();
    ~Level_1_2_2_musicbox_3();
    
private:
    bool                                            musicBoxState;
    float                                           musicBox3SumForWin;
    
    int                                             musicBox3True;
    int                                             musicBox3False;
    
    L1_2_MusicBox3_MGScreen*                        pMusicBox3MinigameScreen;
    
    void                                            sortNotes();
private:
    MiniGameControls*                   m_minigameControl;
    IAudioSource*                       aSource;

    cocos2d::Sequence*                  defeatAction;
    cocos2d::Sequence*                  winAction;
    
};

//-----------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__Level_1_2_2_musicbox_3__) */
