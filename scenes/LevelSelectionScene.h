#ifndef __LEVEL_SELECTION__
#define __LEVEL_SELECTION__

#include "IScene.h"
#include "AppController.h"

#include "IGUIInterface.h"
#include "LevelManager.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
class LevelSelectionScene: public IScene, public IGUIInterface
{
public:
    static LevelSelectionScene* create();
    
    virtual void                clean() override;
    
    virtual void                update( float _dt ) override;
    
    cocos2d::Menu*              getLevelsMenu() { return m_pLevelsMenu; };    // disable menu items when showing difficultyScreen
    cocos2d::Menu*              getStaticLevelsMenu() { return staticButtonsMenu; };
    
protected:
    virtual bool                init() override;
    
    virtual void                initBackground() override;
    virtual void                initButtons() override;
    
    virtual void                onEnter() override;
    virtual void                onExit() override;
    
private:
    LevelSelectionScene();
    ~LevelSelectionScene();
    
    void                        initFirstStage();
    void                        initSecondStage();
    void                        initThirdStage();
    
    void                        onLevelClick(cocos2d::Ref* sender);
    
    void                        onBtnMenuClick( cocos2d::Ref* _sender );
    void                        onBtnMenuClickRecall();
    
    void                        onBtnMenuSwypeLeft( cocos2d::Ref* _sender );
    void                        onBtnMenuSwypeRight( cocos2d::Ref* _sender );
    
    void                        onDonateBtn( cocos2d::Ref* _sender );
    
    struct GateButtonInfo
    {
        int             index;
        level_state     state;
        level_id        level;
        int             levelNum;
        int             cost;
    };
    
    cocos2d::Sprite*          createDoorSprite(const char* doorPath, int stars, const char* name);
    cocos2d::MenuItem*        createGateButton(level_state state, const char* name, int stars, level_id level, int levelNum, int levelCost);
    
    //stages count on levels layer
    const int                               stages_count;
    cocos2d::Layer*                         levelsLayer;
    
    //player's money amount
    cocos2d::Label*                         playerMoneyLabel;
    
    //donate lock
    cocos2d::Sprite*                        donateLockNor[16];
    cocos2d::Sprite*                        donateLockSel[16];
    
    //button-arrows for swype stages
    cocos2d::MenuItemSprite*                arrowLeft;
    cocos2d::MenuItemSprite*                arrowRight;
    
    void                                    enableSwypeButtons();
    
    cocos2d::Menu*                    m_pLevelsMenu;
    cocos2d::Menu*                    staticButtonsMenu;
    
    vector<cocos2d::MenuItem*>        m_buttonsVec;
    
private:
    IAudioSource*       aSource;  // for play a clicking sound of buttons
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif /*__LEVEL_SELECTION__*/