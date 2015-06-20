#include "CompasScreen.h"
#include "AppController.h"

#include "HudScreen.h"
#include "TipHand.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
CompasScreen* CompasScreen::create()
{
    CompasScreen* pRet = new CompasScreen();
    if( pRet && pRet->init() )
        return pRet;
    else
    {
        delete pRet;
        pRet = NULL;
    }
    
	return NULL;
}

//---------------------------------------------------------------------------------------------------------------------------------
CompasScreen::CompasScreen():
arrow( nullptr ),
m_ButtonMenu( nullptr ),
currentTask( nullptr ),
textFeild( nullptr ),
active(true),
m_ButtonHint( nullptr )
{
    selfId = screen_id::COMPAS;
}

//---------------------------------------------------------------------------------------------------------------------------------
CompasScreen::~CompasScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool CompasScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    this->textFeild = CCSprite::create("quests/plate.png");
    this->textFeild->setAnchorPoint(Vec2(0.5, 0.5));
    this->textFeild->setScaleX(1.3);
    this->textFeild->setPosition(Vec2(FRAME_WIDTH / 1.8, 680));
    this->addChild(this->textFeild);
    
    this->currentTask = Label::createWithTTF( "", FONT_PATH, 48 );
    this->currentTask->setColor( FONT_COLOR_TITLE );
    this->currentTask->setAnchorPoint(Vec2(0.5, 0.5));
    this->currentTask->setPosition(Vec2(FRAME_WIDTH / 2, 700));
    this->addChild(this->currentTask);
    
    this->m_ButtonMenu = CCMenu::create();
    this->m_ButtonMenu->setPosition(Vec2::ZERO);
    this->addChild(this->m_ButtonMenu);
    
    m_ButtonHint = MenuItemImage::create("hud/hint.png", "", CC_CALLBACK_1( CompasScreen::onHint, this ) );
    m_ButtonHint->setPosition(Vec2(812, 700));
    this->m_ButtonMenu->addChild(m_ButtonHint);
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::initBackground()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::initButtons()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::update( float _dt )
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool CompasScreen::onTouchBegan( Touch *pTouch, Event *pEvent )
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::setDirection( float _angle )
{//PRAVO - 90 NIZ - 180 LEVO - 270 VERH - 0
    if( _angle >= 0 )
    {
        if(_angle == 0)
            this->currentTask->setString( L10N_STRING( 1901 ).c_str() );
        else if (_angle == 90)
            this->currentTask->setString( L10N_STRING( 1902 ).c_str() );
        else if (_angle == 180)
            this->currentTask->setString( L10N_STRING( 1903 ).c_str() );
        else if (_angle == 270)
            this->currentTask->setString( L10N_STRING( 1904 ).c_str() );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::onHint(cocos2d::Ref *pSender)
{
    /*
    auto player = LEVEL_MANAGER->getCurrentLevel()->getPlayer();
    if( player && !player->isDead() )
    {
        if( MANAGER_Memory->getTempData()->getLevelState( "1_2_1_second_hint" ) == 1 )
        {
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::SCREEN_HINT_NOTES_CALC );
        }
        else
        {
            auto hudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( CURRENT_SCENE, SCREEN_HUD ) );
            if( hudScreen && hudScreen->getTipHand() )
                hudScreen->getTipHand()->stop();
            
            MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT );
        }
    }
    */
    if( MANAGER_Memory->getTempData()->getLevelState( "1_2_1_second_hint" ) == 1 )
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT_DURATION_SUM );
    else
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::HINT);
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::showTask()
{
    if( !this->textFeild->isVisible() )
        dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD ) )->hideTimer();
    //
    this->textFeild->setVisible(true);
    this->currentTask->setVisible(true);
    this->m_ButtonHint->setVisible(true);
    this->active = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompasScreen::hideTask()
{
    if( this->textFeild->isVisible() )
        dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD ) )->showTimer();
    //
    
    this->textFeild->setVisible(false);
    this->currentTask->setVisible(false);
    this->m_ButtonHint->setVisible(false);
    this->active = false;
}

//---------------------------------------------------------------------------------------------------------------------------------