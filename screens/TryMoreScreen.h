#ifndef __TRY_MORE_SCREEN__
#define __TRY_MORE_SCREEN__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class TryMoreScreen: public IScreen, public IGUIInterface
{
public:
	static TryMoreScreen*   create();
    
	virtual void            update( float _dt ) override;
    
    
    void                    show();
    void                    hide();
    
private:
	TryMoreScreen();
	~TryMoreScreen();

    
    virtual bool            init() override;
    
    virtual void            initBackground() override;
	virtual void            initButtons() override;
    
    
    virtual bool            onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    // callbacks
    void                    onBtnMenuClick(cocos2d::Ref* _sender);
    void                    onBtnMenuClickRecall();
    
	void                    onBtnRestartClick(cocos2d::Ref* sender);


private:
    IAudioSource*           aSource;  // for play a clicking sound of buttons
    
	cocos2d::Sprite*        m_pBackground;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /*__TRY_MORE_SCREEN__*/