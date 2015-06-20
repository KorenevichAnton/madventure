//
//  L1_1_Quest_2_4_MGScreen.h
//  MusicalAdventure
//
//  Created by madmin on 4/22/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__L1_1_Quest_2_4_MGScreen__
#define __MusicalAdventure__L1_1_Quest_2_4_MGScreen__

#include "IMiniGameScreen.h"
#include "Node.h"

class L1_1_Quest_2_4_MGScreen: public IMiniGameScreen
{
public:
    static L1_1_Quest_2_4_MGScreen*    create();
    
    virtual bool                       init() override;
    
    virtual void                       update( float _dt ) override;
    
    virtual void                       onVictory() override;
    virtual void                       onDefeat() override;
    
    //touch event
    virtual bool                       onTouchBegan( cocos2d::Touch *_touch, cocos2d::Event *_event ) override;
    virtual void                       onTouchEnded( cocos2d::Touch *_touch, cocos2d::Event *_event ) override;
    //
private:
    
public:
    bool                               showQuastion( int _id );
    bool                               initWithXML( int _id );
private:
    L1_1_Quest_2_4_MGScreen();
    ~L1_1_Quest_2_4_MGScreen();
    
    cocos2d::Label                    *lQuastion;
    cocos2d::Label                    *lQuastion1;
    std::vector< cocos2d::Sprite* >    answersButtons;
    std::vector< cocos2d::Label* >     answersLables;
    int                                curQuastion;
    int                                qCount;
    std::vector<QNode*>                qNodes;
    QNode                             *curQNode;
    cocos2d::Sprite                   *testWindow;
    
    //TIMER
    cocos2d::Label                    *lTimer;
    float                              timer;
    
    IAudioSource                      *aSourceError;
};

#endif /* defined(__MusicalAdventure__L1_1_Quest_2_4_MGScreen__) */
