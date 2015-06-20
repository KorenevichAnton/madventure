#ifndef __COMPLETE_SCREEN__
#define __COMPLETE_SCREEN__

#include "IScreen.h"
#include "IGUIInterface.h"
#include "LevelManager.h"

//---------------------------------------------------------------------------------------------------------------------------------
class CompleteScreen: public IScreen, public IGUIInterface
{
public:
	static CompleteScreen*      create();

    void                        show();
    void                        hide();
    
    void                        setCompletedLevel( int _completedLevel );
    
private:
    CompleteScreen();
    ~CompleteScreen();
    
	virtual bool            init() override;
    
    virtual void            initBackground() override;
	virtual void            initButtons() override;
    
	void                    initLabels();
    
    
	virtual void            update( float _dt ) override;
    
    
    cocos2d::Label*         createLabelWithTitle( const char* title, const char* labelText = "" );

    
    void                    onBtnMenuClick(cocos2d::Ref* _sender);
    void                    onBtnMenuClickRecall();
    
	void                    onBtnNextClick(cocos2d::Ref* sender);
    void                    onBtnNextClickRecall();

    
private:
    int                     m_pCompletedLevel;
    
    cocos2d::Vec2           m_startLabelPos;
    float                   m_labelOffsetY;
    int                     m_labelCounter;
	cocos2d::Label*         m_pDifficulty;
	cocos2d::Label*         m_pScore;
	cocos2d::Label*         m_pMechanismsBroken;
	cocos2d::Label*         m_pMusicTasks;
	cocos2d::Label*         m_pTimeBonus;
	cocos2d::Label*         m_pSleepedCharacters;
	cocos2d::Label*         m_pSecretPlaces;
	cocos2d::Label*         m_pAdditionalTask;
	cocos2d::Label*         m_pTotalScore;

    cocos2d::Sprite*        m_pStar1;
    cocos2d::Sprite*        m_pStar2;
    cocos2d::Sprite*        m_pStar3;
    
    cocos2d::Sprite*        m_pStarPanel;
	cocos2d::Sprite*        m_pBackgroundPanel;
    
    IAudioSource*           aSource;  // for play a clicking sound of buttons
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif  // __COMPLETE_SCREEN__