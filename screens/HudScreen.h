#ifndef __HUD_SCREEN__
#define __HUD_SCREEN__

#include "IScreen.h"

#include "TipHand.h"
#include "Clipping.h"

//---------------------------------------------------------------------------------------------------------------------------------
class HudScreen: public IScreen
{
public:
	static HudScreen*   create();

    void                setStickerState(bool _isActive);
    bool                getStickerState() const;
    
    void                hideSticker();
    void                showSticker();
    void                resetSticker();
    
    void                disableSticker( bool _state );
    
    void                showTimer();
    void                hideTimer();
    
    void                showHandHint( cocos2d::Vec2 _position, float _angle, float _duration, float _speed );
    
    //progress bar control for minigames
    void                showProgressBar();
    void                hideProgressBar();
    //progress bar fill setter
    void                setProgressBarValue( float _value );
    
    
private:
    HudScreen();
    ~HudScreen();
    
    virtual bool        init() override;
    
    void                initControls();
    
    
	virtual void        update( float _dt ) override;
    
    
    void                onTouchesBegan( const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* _event );
    void                onTouchesEnded( const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* _event );
    void                onTouchesMoved( const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* _event );
    void                onTouchesCancelled( const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* _event );


    //progress bar control
    void                initMinigamesProgressBar();
    
    void                onJumpClick(cocos2d::Ref* _pSender);
    void                onShotClick(cocos2d::Ref* _pSender);
    void                onPauseClick(cocos2d::Ref* _pSender);
    
    
    void                movePlayerTo(cocos2d::Vec2 _dir); /**TEST: Player model for camera test */
    

    //TEMP: for new camera test    
    void                menuCallbackZoomInc(cocos2d::Ref* _pSender);
    void                menuCallbackZoomDec(cocos2d::Ref* _pSender);
    void                menuCallbackGMod(cocos2d::Ref* _pSender);
    void                menuCallbackDebugDraw(cocos2d::Ref* _pSender);

    cocos2d::Label*     zoomLabel;
    //
    
    
private:
    TipHand*                    handTip;

    cocos2d::Sprite*            tableTimer;

    //progressbar control clipping
    Clipping*                   progressClipping;
    float                       maxProgress;
    
    //progress bar control for long minigames
    cocos2d::Sprite*            progressBarControl;
    
    
    cocos2d::Label*             m_labelCountShoot;
    cocos2d::Label*             m_labelCountNotes;
    cocos2d::Label*             m_labelCountHP;
    
    //TEST : LEVEL TIMER
    cocos2d::Label*             m_labelLevelTimer;
    cocos2d::Label*             m_labelCoinsToPointes;
    
    bool m_isStickActive;
    
    //
    cocos2d::Touch*             stickerTouch;
    
    //sticker contol
    cocos2d::Node*              m_pStickCenterNode;
    cocos2d::Sprite*            m_pStick;
    cocos2d::Sprite*            m_pStickerBg;
    //
    
    //button controls
    cocos2d::Sprite*            jumpItemUnselected;
    cocos2d::Sprite*            jumpItemSelected;
    cocos2d::MenuItemImage*     shotItem;
    //
    cocos2d::Sprite*            pauseSpriteBtn;
    
    bool                         m_isOnStickClick;
    bool                         m_isStickMoveX;
    bool                         m_isStickMoveY;
    bool                         m_stickerDisabler;
    
    
    //Menu for shot, jump and pause buttons
    cocos2d::Menu*              m_pControlsMenu;
    
    //
    cocos2d::Label*             textLabelGMod;
    cocos2d::Label*             textLabelDebugDraw;
    
    
    bool                        isStickerHide;
};

//---------------------------------------------------------------------------------------------------------------------------------
#endif //__HUD_SCREEN__