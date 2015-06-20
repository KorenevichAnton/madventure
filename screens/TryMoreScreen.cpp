#include "TryMoreScreen.h"
#include "MainMenuScene.h"
#include "LevelManager.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
TryMoreScreen* TryMoreScreen::create()
{
    TryMoreScreen* pRet = new TryMoreScreen();
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
TryMoreScreen::TryMoreScreen()
	: m_pBackground(NULL)
{
    selfId = screen_id::TRY_MORE;
}

//---------------------------------------------------------------------------------------------------------------------------------
TryMoreScreen::~TryMoreScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool TryMoreScreen::init()
{
	if ( !IScreen::init() )
		return false;

    
	initBackground();
	initButtons();
    
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    this->hide();

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::update(float _dt)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool TryMoreScreen::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if( this->isVisible() )
        return true;
    else
        return false;
}


//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::initBackground()
{
	createBackgroundFadeWithOpacity(this);

    Size winSize = Director::getInstance()->getWinSize();

	m_pBackground = CCSprite::create("pause_try_more/plashka.png");
	m_pBackground->setPosition( winSize / 2 );
	addChild( m_pBackground );

	Sprite* labelSprite = Sprite::create("pause_try_more/trymore_d.png");
	labelSprite->setPosition( Vec2(m_pBackground->getContentSize().width / 2, m_pBackground->getContentSize().height - 50) );
	m_pBackground->addChild( labelSprite );

	auto label = Label::createWithTTF( L10N_STRING( 1001 ).c_str(), FONT_PATH, FONT_SIZE_VALUE );
    label->setColor( FONT_COLOR_TITLE );
	label->setPosition( Vec2(labelSprite->getContentSize().width / 2, labelSprite->getContentSize().height / 1.4 ) );
	labelSprite->addChild( label );
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::initButtons()
{
	MenuItemSprite* mainMenuButton = MenuItemSprite::create(createViewStateOfButton("btn/menu.png"),
																createViewStateOfButton("btn/menu_down.png"),
																CC_CALLBACK_1( TryMoreScreen::onBtnMenuClick, this ) );
	mainMenuButton->setPosition( Vec2(m_pBackground->getContentSize().width / 4, m_pBackground->getContentSize().height / 3) );

	MenuItemSprite* restartButton = MenuItemSprite::create(createViewStateOfButton("btn/restart.png"),
																createViewStateOfButton("btn/restart_down.png"),
																CC_CALLBACK_1( TryMoreScreen::onBtnRestartClick, this ) );
	restartButton->setPosition( Vec2(m_pBackground->getContentSize().width - m_pBackground->getContentSize().width / 4,
									m_pBackground->getContentSize().height / 3) );

	Menu* menu = Menu::create(mainMenuButton, restartButton, NULL);
	menu->setPosition( Vec2(0, 0) );
	m_pBackground->addChild( menu );
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::onBtnMenuClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    MANAGER_Level->getCurrentLevel()->getEmbiantSource()->setGain(0.0f);
    MANAGER_Level->getCurrentLevel()->getBackMusicSource()->setGain(0.0f);
    
    Sequence* action = Sequence::create( DelayTime::create( 0.70f ), CallFunc::create( CC_CALLBACK_0( TryMoreScreen::onBtnMenuClickRecall, this ) ), nullptr );
    this->runAction( action );
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::onBtnMenuClickRecall()
{
    MANAGER_Scene->replaceScene( scene_id::MAIN_MENU );
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::onBtnRestartClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    MANAGER_Level->replaceLevel(MANAGER_Level->getCurrentLevel()->getLevelName());
}
	 
//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::show()
{
    //stop swallowing touches
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );
    
    //level timer
    MANAGER_Level->stopLevelTimer();
    
    this->setVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------------------
void TryMoreScreen::hide()
{
    //resume swallowing touches
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( false );
    
    this->setVisible(false);
}

//---------------------------------------------------------------------------------------------------------------------------------