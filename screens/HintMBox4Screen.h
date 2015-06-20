#ifndef __MusicalAdventure__HintMBox4Screen__
#define __MusicalAdventure__HintMBox4Screen__

#include "IScreen.h"
#include "IGUIInterface.h"


#define SOUND_BARABAN       "audio/scenes/L1_3_1_musicBox2/baraban.wav"
#define SOUND_FORTEPIANO    "audio/scenes/L1_3_1_musicBox2/fortepiano.wav"
#define SOUND_GITARA        "audio/scenes/L1_3_1_musicBox2/gitara.wav"
#define SOUND_TRIANGLE      "audio/scenes/L1_3_1_musicBox2/triangle.wav"
#define SOUND_SKRIPKA       "audio/scenes/L1_3_1_musicBox2/skripka.wav"
#define SOUND_TRUBA         "audio/scenes/L1_3_1_musicBox2/truba.wav"
#define SOUND_KLARNET       "audio/scenes/L1_3_1_musicBox2/klarnet.wav"
#define SOUND_SAX           "audio/scenes/L1_3_1_musicBox2/sax.wav"
#define SOUND_VIBRAPHONE    "audio/scenes/L1_3_1_musicBox2/vibraphon.wav"
#define SOUND_AKKORDEON     "audio/scenes/L1_3_1_musicBox2/akkordeon.wav"

//---------------------------------------------------------------------------------------------------------------------------------
class HintMBox4Screen: public IScreen
{
public:
	static HintMBox4Screen*	create();
    
	void                        update(float _dt);
    
protected:
	bool                        init();
    
private:
	HintMBox4Screen();
	~HintMBox4Screen();
    
public:
    void                        callbackClose(cocos2d::Ref* _pSender);
    
private:
    bool                        onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    cocos2d::Sprite*          backHint;
    //Menu for shot, jump and pause buttons
    cocos2d::Menu*             m_pControlsMenu;
    //TEMP: for new camera test
    void                menuCallbackNote1(cocos2d::Ref* _pSender);
    void                menuCallbackNote2(cocos2d::Ref* _pSender);
    void                menuCallbackNote3(cocos2d::Ref* _pSender);
    void                menuCallbackNote4(cocos2d::Ref* _pSender);
    void                menuCallbackNote5(cocos2d::Ref* _pSender);
    void                menuCallbackNote6(cocos2d::Ref* _pSender);
    void                menuCallbackNote7(cocos2d::Ref* _pSender);
    void                menuCallbackNote8(cocos2d::Ref* _pSender);
    void                menuCallbackNote9(cocos2d::Ref* _pSender);
    void                menuCallbackNote10(cocos2d::Ref* _pSender);
    
    IAudioSource*       audioSrc;
};

//---------------------------------------------------------------------------------------------------------------------------------

#endif /* defined(__MusicalAdventure__HintMBox4Screen__) */
