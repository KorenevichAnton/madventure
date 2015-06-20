#ifndef __DIFFICULTY_SCENE__
#define __DIFFICULTY_SCENE__

#include "IScreen.h"
#include "IGUIInterface.h"
#include "LevelManager.h"

//---------------------------------------------------------------------------------------------------------------------------------
class DifficultyScreen: public IScreen, public IGUIInterface
{
public:
	static DifficultyScreen*    create();
    
	virtual void                update( float _dt ) override;
    
    
    void                        show();
    void                        hide();
    
    void                        setLevelForLoading( level_id _nextLevel );
    void                        setLevelForDifficultySelection( level_id _level );
    
private:
	DifficultyScreen();
	~DifficultyScreen();


    virtual bool                init() override;
	virtual void                initBackground() override;
	virtual void                initButtons() override;

	void                        initLabels();
	void                        initDifficultyScroll();
    
    void                        addDifficultyToList(const char* difficulty);
    
    // acceptable index from 0 to (m_difficultyList.size() - 1)
    void                        moveDifficultyTo(int index);
    void                        moveToNextDifficulty();
    void                        moveToPrevDifficulty();
    void                        endOfMoving();
	
    // callbacks
	void                        onMainMenuClick(cocos2d::Ref* sender);
	void                        onNextClick(cocos2d::Ref* sender);
	void                        onLeftArrowClick(cocos2d::Ref* sender);
	void                        onRightArrowClick(cocos2d::Ref* sender);

    
private:
    IAudioSource*                       aSource;  // for play a clicking sound of buttons
    
    level_id                            m_levelForLoading;
    level_id                            m_levelNumForDifficultySelection;
    
	cocos2d::Layer*                     m_pDifficultyListLayer;
	std::vector< cocos2d::Label* >      m_difficultyList;
	int                                 m_currentDifficultyIndex;
	bool                                m_isDifficultyListMoving;

	cocos2d::Sprite*                    m_pBackgroundPanel;
	cocos2d::Sprite*                    m_pStarPanel;
    
    cocos2d::Sprite*                    m_pStar1;
    cocos2d::Sprite*                    m_pStar2;
    cocos2d::Sprite*                    m_pStar3;
    
	cocos2d::Label*                     m_pDifficultyValue;
	cocos2d::Label*                     m_pScoreValue;
	cocos2d::Label*                     m_pTimeValue;
    
	cocos2d::Label*                     m_pLevelRecordValue;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif  //__DIFFICULTY_SCENE__