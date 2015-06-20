//
//  SleepingGuards.h
//  MusicalAdventure
//
//  Created by madmin on 5/15/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__L1_3_Quest_1_MGScreen__
#define __MusicalAdventure__L1_3_Quest_1_MGScreen__

#include "IMiniGameScreen.h"
#include "GraphicAnimationsSprite.h"

#include "IGUIInterface.h"

class L1_3_Quest_1_MGScreen: public IMiniGameScreen, public IGUIInterface
{
public:
    static L1_3_Quest_1_MGScreen*                   create();
    
    virtual bool                                    init() override;
    
    virtual void                                    initBackground() override {}
    virtual void                                    initButtons() override {}
    
    virtual void                                    update( float _dt ) override;
    
    virtual void                                    onVictory() override;
    virtual void                                    onDefeat() override;
    
    virtual void                                    showMinigameInterface() override;
    virtual void                                    hideMinigameInterface() override;

    virtual void                                    onStartGame() override;

    //touch events
    virtual bool                     onTouchBegan( cocos2d::Touch *_touch, cocos2d::Event *_event ) override;
    virtual void                     onTouchEnded( cocos2d::Touch *_touch, cocos2d::Event *_event ) override;
    virtual void                     onTouchMoved( cocos2d::Touch *_touch, cocos2d::Event *_event ) override;
    virtual void                     onTouchCancelled( cocos2d::Touch *_touch, cocos2d::Event *_event) override;
    //

private:
    L1_3_Quest_1_MGScreen();
    ~L1_3_Quest_1_MGScreen();
    

public:
    void                                            setWinState(bool _isWin);
    
private:
    //
    void                                            onBtnPlay1(cocos2d::Ref* pSender);
    void                                            onBtnPlay2(cocos2d::Ref* pSender);
    void                                            onBtnPlay3(cocos2d::Ref* pSender);
    void                                            onBtnPlay4(cocos2d::Ref* pSender);
    void                                            onBtnPlay5(cocos2d::Ref* pSender);
    void                                            onBtnPlay6(cocos2d::Ref* pSender);
    //
    void                                            playCurSound();
    
    void                                            initGuards();
    
    std::string                                     curSound;
    cocos2d::Menu                                  *buttonMenu;
    
    //
    std::vector<cocos2d::Label*>                    numbers;
    std::vector<cocos2d::MenuItemSprite*>           playBtns;
    std::vector<cocos2d::Sprite*>                   activeDefs;
    std::vector<graphic::GraphicAnimationObject*>   vAnimGuards;
    //
    
    std::string                                     currentSoung;
    
    int                                             def1;
    int                                             def2;
    int                                             def3;
    
    cocos2d::Vec2                                   oldPoint;
    cocos2d::Label                                 *tempNum;
    
    cocos2d::Label                                 *typeMusicGuard[3];
    
    // Number of incorrect attempts
    int                                             countFail;
    int                                             maxCountFail;
    //
    
    IAudioSource                                   *aSourceCurSound;
    
    cocos2d::Label                                 *One;
    cocos2d::Label                                 *Two;
    cocos2d::Label                                 *Three;
    cocos2d::Label                                 *Four;
    cocos2d::Label                                 *Five;
    cocos2d::Label                                 *Six;

    bool                                            isWin;
    
    GameObject                                     *objGuard1;
    GameObject                                     *objGuard2;
    GameObject                                     *objGuard3;
    
    //
    GameObject                                     *objWokeUpGuard1;
    GameObject                                     *objWokeUpGuard2;
    GameObject                                     *objWokeUpGuard3;
    
    cocos2d::Touch                                 *curTouch;
    
    // Call when anim hand near note
    void                                            onHandTakeNote( cocos2d::Sprite* _sprite );
    
    void                                            functionCallEndHintHand();
    //
    
public:
    //flying hand - hint
    void                                            showHintHand();
    
    void                                            restart();
};


#endif /* defined(__MusicalAdventure__L1_3_Quest_1_MGScreen__) */
