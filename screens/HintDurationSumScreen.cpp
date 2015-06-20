#include "HintDurationSumScreen.h"
#include "AppController.h"
#include "HudScreen.h"

#define HINT_MSG_FONT_SIZE      24
#define HINT_TITLE_FONT_SIZE    32

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
HintDurationSumScreen* HintDurationSumScreen::create()
{
    HintDurationSumScreen* pRet = new HintDurationSumScreen();
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
HintDurationSumScreen::HintDurationSumScreen():
backHint(NULL),
functionCallBackOnClose( nullptr )
{
    selfId = screen_id::HINT_DURATION_SUM;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintDurationSumScreen::~HintDurationSumScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintDurationSumScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    
    this->backHint = CCSprite::create("textures/screens/helper/podlojka.png");
    this->backHint->setAnchorPoint(Vec2(0, 0));
    this->backHint->setPosition(Vec2(5, 100));
    this->addChild(this->backHint);
    
    m_pControlsMenu = Menu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0));
    this->backHint->addChild(m_pControlsMenu);
    
    MenuItemImage* btnClose = CCMenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( HintDurationSumScreen::callbackClose, this ) );
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
    Sprite* eightNote = Sprite::create("textures/screens/helper/eightNote.png");
    eightNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    eightNote->setPosition( Vec2( 130, 270 ) );
    this->backHint->addChild( eightNote );
    
    auto lEightNote = Label::createWithTTF( L10N_STRING( 2104 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lEightNote->setColor( FONT_COLOR_TITLE );
    lEightNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lEightNote->setPosition( Vec2( 190, 280 ) );
    this->backHint->addChild( lEightNote );
    
    Sprite* eightPart = Sprite::create("textures/screens/helper/redPart.png");
    eightPart->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    eightPart->setPosition( Vec2( 500, 285 ) );
    eightPart->setScale( 0.7 );
    this->backHint->addChild( eightPart );
    
    //
    
    Sprite* halfNote = Sprite::create("textures/screens/helper/halfNote.png");
    halfNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    halfNote->setPosition( Vec2( 130, 200 ) );
    this->backHint->addChild( halfNote );
    
    auto lHalfNote = Label::createWithTTF( L10N_STRING( 2102 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHalfNote->setColor( FONT_COLOR_TITLE );
    lHalfNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lHalfNote->setPosition( Vec2( 180, 210 ) );
    this->backHint->addChild( lHalfNote );
    
    Sprite* halfPart = Sprite::create("textures/screens/helper/yellowPart.png");
    halfPart->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    halfPart->setPosition( Vec2( 520, 200 ) );
    halfPart->setScale( 0.7 );
    this->backHint->addChild( halfPart );
    
    //
    
    Sprite* quarterNote = Sprite::create("textures/screens/helper/quarterNote.png");
    quarterNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    quarterNote->setPosition( Vec2( 130, 130 ) );
    this->backHint->addChild( quarterNote );
    auto lQuarterNote = Label::createWithTTF( L10N_STRING( 2103 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    
    lQuarterNote->setColor( FONT_COLOR_TITLE );
    lQuarterNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    lQuarterNote->setPosition( Vec2( 180, 140 ) );
    this->backHint->addChild( lQuarterNote );
    
    Sprite* quarterPart = Sprite::create("textures/screens/helper/bluePart.png");
    quarterPart->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    quarterPart->setPosition( Vec2( 500, 130 ) );
    quarterPart->setScale( 0.7 );
    this->backHint->addChild( quarterPart );
    
    //
    
    Sprite* fullNote = Sprite::create("textures/screens/helper/fullNote.png");
    fullNote->setAnchorPoint( Vec2( 0.0, 0.0 ) );
    fullNote->setPosition( Vec2( 650, 130 ) );
    fullNote->setScale( 0.7 );
    this->backHint->addChild( fullNote );
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintDurationSumScreen::update(float _dt)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintDurationSumScreen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void HintDurationSumScreen::callbackClose(cocos2d::Ref* _pSender)
{
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT_DURATION_SUM);
    if( MANAGER_Memory->getTempData()->getLevelState( "hint_first_show_level_1_2_2_mb_3" ) == 1 )
        MANAGER_Memory->getTempData()->setLevelState( "hint_first_show_level_1_2_2_mb_3", 1 );
    else
        dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->showSticker();
    
    if( this->functionCallBackOnClose )
        this->functionCallBackOnClose( -1 );
    
}

//---------------------------------------------------------------------------------------------------------------------------------