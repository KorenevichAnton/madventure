#ifndef __PAUSE_SCREEN__
#define __PAUSE_SCREEN__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class PauseScreen: public IScreen, public IGUIInterface
{
public:
	static PauseScreen*     create();
    
	virtual void            update( float _dt ) override;
    
    
    void                    show();
    void                    hide();
    
private:
    bool                    onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
private:
	PauseScreen();
	~PauseScreen();
 
    
    virtual bool            init() override;
    
    virtual void            initBackground() override;
	virtual void            initButtons() override;
    
    // callbacks
    void                    onBtnMenuClick(cocos2d::Ref* _sender);
    void                    onBtnMenuClickRecall(cocos2d::Ref* _sender);
    
	void                    onBtnRestartClick(cocos2d::Ref* sender);
    
	void                    onNextClick(cocos2d::Ref* sender);

    
private:
    IAudioSource*               aSource;  // for play a clicking sound of buttons
    
	cocos2d::Sprite*            m_pBackground;
    
    std::vector<IAudioSource*>  vPlayedSrc;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif  // __PAUSE_SCREEN__