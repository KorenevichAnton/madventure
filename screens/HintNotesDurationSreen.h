//
//  HintNotesDurationSreen.h
//  MusicalAdventure
//
//  Created by ampaUser on 3/19/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#ifndef __MusicalAdventure__HintNotesDurationSreen__
#define __MusicalAdventure__HintNotesDurationSreen__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class HintNotesDurationSreen: public IScreen
{
public:
    static HintNotesDurationSreen*	create();
    
    void                        update(float _dt);
    
protected:
    bool                        init();
    
private:
    HintNotesDurationSreen();
    ~HintNotesDurationSreen();
    
public:
    void                        callbackClose(cocos2d::Ref* _pSender);
    
    void                        setCallBackClose(const std::function<void(int)> &_functionCallBack) { this->functionCallBackOnClose = _functionCallBack; }
private:
    bool                        onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    cocos2d::Sprite*            backHint;
    
    cocos2d::Menu*              m_pControlsMenu;
    
    std::function<void(int)>    functionCallBackOnClose;
    
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__HintNotesDurationSreen__) */
