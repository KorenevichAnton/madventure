#ifndef __MusicalAdventure__HintScreen__
#define __MusicalAdventure__HintScreen__

#include "IScreen.h"
#include "IGUIInterface.h"


#define SOUND_SIXTEENTH                 "audio/minigame/platforms1_2_1/sixteenth.wav"
#define SOUND_FULL                      "audio/minigame/platforms1_2_1/full.wav"
#define SOUND_FOURTH                    "audio/minigame/platforms1_2_1/fourth.wav"
#define SOUND_HALFTH                    "audio/minigame/platforms1_2_1/halfth.wav"
#define SOUND_EIGHTH                    "audio/minigame/platforms1_2_1/eighth.wav"

//---------------------------------------------------------------------------------------------------------------------------------
class HintScreen: public IScreen
{
public:
	static HintScreen*	create();
    
	void                        update(float _dt);
    
protected:
	bool                        init();
    
private:
	HintScreen();
	~HintScreen();
    
public:
    void                        callbackClose(cocos2d::Ref* _pSender);
    
private:
    void                        onEnter();
    
    bool                        onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    cocos2d::Sprite*            backHint;
    
    cocos2d::Menu*              m_pControlsMenu;

    IAudioSource                *aSource;
    
    cocos2d::Sprite*            backLight[4];
    int                         backLightIndex;
    
    void                        showBackLight();
    void                        hideBackLight();
    
    void                        menuCallbackNote1(cocos2d::Ref* _pSender);
    void                        menuCallbackNote2(cocos2d::Ref* _pSender);
    void                        menuCallbackNote3(cocos2d::Ref* _pSender);
    void                        menuCallbackNote4(cocos2d::Ref* _pSender);
    void                        menuCallbackNote5(cocos2d::Ref* _pSender);
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__HintScreen__) */
