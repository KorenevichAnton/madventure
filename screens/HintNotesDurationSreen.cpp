//
//  HintNotesDurationSreen.cpp
//  MusicalAdventure
//
//  Created by ampaUser on 3/19/15.
//  Copyright (c) 2015 ampagames. All rights reserved.
//

#include "HintNotesDurationSreen.h"
#include "AppController.h"
#include "HudScreen.h"

#define HINT_MSG_FONT_SIZE      24
#define HINT_TITLE_FONT_SIZE    32

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
HintNotesDurationSreen* HintNotesDurationSreen::create()
{
    HintNotesDurationSreen* pRet = new HintNotesDurationSreen();
    if(pRet && pRet->init())
        return pRet;
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintNotesDurationSreen::HintNotesDurationSreen():
backHint(NULL),
functionCallBackOnClose( nullptr )
{
    selfId = screen_id::HINT_NOTES_DURATION;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintNotesDurationSreen::~HintNotesDurationSreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintNotesDurationSreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    
    this->backHint = CCSprite::create("textures/screens/helper/podlojka.png");
    this->backHint->setAnchorPoint(Vec2(0, 0));
    this->backHint->setPosition(Vec2(5, 100));
    this->addChild(this->backHint);
    
    m_pControlsMenu = CCMenu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0));
    this->backHint->addChild(m_pControlsMenu);
    
    MenuItemImage* btnClose = MenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( HintNotesDurationSreen::callbackClose, this ) );
    btnClose->setPosition(Vec2( 950, 420 ) );
    this->m_pControlsMenu->addChild(btnClose);
    //
    
    //
    Sprite* header = Sprite::create("textures/screens/helper/header.png");
    header->setAnchorPoint( Vec2( 0, 0 ) );
    header->setPosition( Vec2( 340, 350 ) );
    this->backHint->addChild( header );
    auto lHeader = Label::createWithTTF( L10N_STRING( 2101 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHeader->setColor( FONT_COLOR_TITLE );
    lHeader->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lHeader->setPosition( Vec2( 180, 105 ) );
    header->addChild( lHeader );
    //
    //CONTENT
    Sprite* eightNotePic = CCSprite::create("textures/scenes/minigame/L1_2_Quest_8_MGScreen/buttonPressEighth.png");
    eightNotePic->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    eightNotePic->setPosition( Vec2( 830, 270 ) );
    this->backHint->addChild( eightNotePic );
    
    Sprite* eightNote = CCSprite::create("textures/scenes/minigame/PianoPlay/eighth.png");
    eightNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    eightNote->setPosition( Vec2( 530, 270 ) );
    this->backHint->addChild( eightNote );
    
    auto lEightNote = Label::createWithTTF( L10N_STRING( 2104 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lEightNote->setColor( FONT_COLOR_TITLE );
    lEightNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lEightNote->setPosition( Vec2( 590, 280 ) );
    this->backHint->addChild( lEightNote );
    
    ///Users/ampauser/Work/madventure/MusicalAdventure/data/textures/scenes/minigame/PianoPlay/complete.png
    Sprite* halfNotePic = Sprite::create("textures/scenes/minigame/L1_2_Quest_8_MGScreen/buttonPressHalf.png");
    halfNotePic->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    halfNotePic->setPosition( Vec2( 830, 150 ) );
    this->backHint->addChild( halfNotePic );
    
    Sprite* halfNote = Sprite::create("textures/scenes/minigame/PianoPlay/half.png");
    halfNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    halfNote->setPosition( Vec2( 530, 150 ) );
    this->backHint->addChild( halfNote );
    
    auto lHalfNote = Label::createWithTTF( L10N_STRING( 2102 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHalfNote->setColor( FONT_COLOR_TITLE );
    lHalfNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lHalfNote->setPosition( Vec2( 580, 160 ) );
    this->backHint->addChild( lHalfNote );
    
    //
    
    Sprite* fullNotePic = Sprite::create("textures/scenes/minigame/L1_2_Quest_8_MGScreen/whole_D.png");
    fullNotePic->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    fullNotePic->setPosition( Vec2( 360, 270 ) );
    this->backHint->addChild( fullNotePic );
    
    Sprite* fullNote = Sprite::create("textures/scenes/minigame/PianoPlay/complete.png");
    fullNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    fullNote->setPosition( Vec2( 130, 270 ) );
    this->backHint->addChild( fullNote );
    auto lFullNote = Label::createWithTTF( L10N_STRING( 2105 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    
    lFullNote->setColor( FONT_COLOR_TITLE );
    lFullNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lFullNote->setPosition( Vec2( 180, 280 ) );
    this->backHint->addChild( lFullNote );
    
    //
    Sprite* quarterNotePic = Sprite::create("textures/scenes/minigame/L1_2_Quest_8_MGScreen/buttonPressQuarter.png");
    quarterNotePic->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    quarterNotePic->setPosition( Vec2( 360, 150 ) );
    this->backHint->addChild( quarterNotePic );
    
    Sprite* quarterNote = Sprite::create("textures/scenes/minigame/PianoPlay/quarter.png");
    quarterNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    quarterNote->setPosition( Vec2( 130, 150 ) );
    this->backHint->addChild( quarterNote );
    auto lQuarterNote = Label::createWithTTF( L10N_STRING( 2103 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    
    lQuarterNote->setColor( FONT_COLOR_TITLE );
    lQuarterNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lQuarterNote->setPosition( Vec2( 180, 160 ) );
    this->backHint->addChild( lQuarterNote );
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintNotesDurationSreen::update(float _dt)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintNotesDurationSreen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if(this->backHint && this->backHint->boundingBox().containsPoint( pTouch->getLocation() ))
    {
        //callbackClose(this);
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintNotesDurationSreen::callbackClose(cocos2d::Ref* _pSender)
{
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT_NOTES_DURATION);
//    if( MANAGER_Memory->getTempData()->getLevelState( "hint_first_show_level_1_2_2_mb_3" ) == 1 )
//        MANAGER_Memory->getTempData()->setLevelState( "hint_first_show_level_1_2_2_mb_3", 1 );
//    else
//        dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->showSticker();
    
    if( this->functionCallBackOnClose )
        this->functionCallBackOnClose( -1 );
    
}

//---------------------------------------------------------------------------------------------------------------------------------