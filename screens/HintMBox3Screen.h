#ifndef __MusicalAdventure__HintMBox3Screen__
#define __MusicalAdventure__HintMBox3Screen__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class HintMBox3Screen: public IScreen
{
public:
	static HintMBox3Screen*	create();
    
	void                        update(float _dt);
    
protected:
	bool                        init();
    
private:
	HintMBox3Screen();
	~HintMBox3Screen();
    
public:
    void                        callbackClose(cocos2d::Ref* _pSender);
    
private:
    bool                        onTouchBegan
    (cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    cocos2d::Sprite*          backHint;

    cocos2d::Menu*            m_pControlsMenu;
    
    IAudioSource*               audioSrc;

    void                menuCallbackNote1(cocos2d::Ref* _pSender);
    void                menuCallbackNote2(cocos2d::Ref* _pSender);
    void                menuCallbackNote3(cocos2d::Ref* _pSender);
    void                menuCallbackNote4(cocos2d::Ref* _pSender);
    void                menuCallbackNote5(cocos2d::Ref* _pSender);
    void                menuCallbackNote6(cocos2d::Ref* _pSender);
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__HintMBox3Screen__) */
