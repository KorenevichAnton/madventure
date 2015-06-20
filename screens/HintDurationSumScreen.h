#ifndef __MusicalAdventure__HintDurationSumScreen__
#define __MusicalAdventure__HintDurationSumScreen__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class HintDurationSumScreen: public IScreen
{
public:
    static HintDurationSumScreen*	create();
    
    void                        update(float _dt);
    
protected:
    bool                        init();
    
private:
    HintDurationSumScreen();
    ~HintDurationSumScreen();
    
public:
    void                        callbackClose(cocos2d::Ref* _pSender);
    
    void                        setCallBackClose(const std::function<void(int)> &_functionCallBack) { this->functionCallBackOnClose = _functionCallBack; }
private:
    virtual bool                onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    cocos2d::Sprite*            backHint;
    
    cocos2d::Menu*              m_pControlsMenu;
    
    std::function<void(int)>    functionCallBackOnClose;

};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__HintDurationSum__) */
