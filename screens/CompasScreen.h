#ifndef __MusicalAdventure__CompasScreen__
#define __MusicalAdventure__CompasScreen__

#include "IScreen.h"
#include "IGUIInterface.h"

//---------------------------------------------------------------------------------------------------------------------------------
class CompasScreen: public IScreen, public IGUIInterface
{
public:
	static CompasScreen*	create();
    
	virtual void            update( float _dt ) override;
    
    
    void                    setDirection( float _angle );
    float                   getDirection() { return arrow->getRotation(); };
    
    void                    showTask();
    void                    hideTask();
    
    bool                    isActive() { return active; };
    
private:
	CompasScreen();
	~CompasScreen();
    
    
    virtual bool            init() override;
    
    virtual void            initBackground() override;
    virtual void            initButtons() override;
    
    virtual bool            onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    
    // callbacks
    void                    onHint(cocos2d::Ref* pSender);
    
    
private:
    cocos2d::Menu*                 m_ButtonMenu;
    cocos2d::MenuItemImage*        m_ButtonHint;
    cocos2d::Sprite*               arrow;
    
    bool                           active;
    
    cocos2d::Label*         currentTask;
    cocos2d::Sprite*        textFeild;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /* defined(__MusicalAdventure__CompasScreen__) */
