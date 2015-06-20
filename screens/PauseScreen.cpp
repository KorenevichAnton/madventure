#include "PauseScreen.h"
#include "AppController.h"
#include "LevelManager.h"
#include "IMiniGameScreen.h"
#include "AudioDefinitions.h"

#include "HudScreen.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
PauseScreen* PauseScreen::create()
{
    PauseScreen* pRet = new PauseScreen();
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
PauseScreen::PauseScreen()
{
    selfId = screen_id::PAUSE;
    vPlayedSrc.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
PauseScreen::~PauseScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool PauseScreen::init()
{
	if (!CCLayer::init())
		return false;

    
	initBackground();
	initButtons();
    
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    // Initialize a listener of touches.
    //auto listener = EventListenerTouchOneByOne::create();
    //listener->onTouchBegan = CC_CALLBACK_2( PauseScreen::onTouchBegan, this );
    //this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
    //
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    /* PORT
    this->setTouchEnabled( true );
    this->setTouchMode( kCCTouchesOneByOne );
    this->setTouchPriority( -5 );
    */
    this->setVisible(false);
    
    
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::update(float _dt)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool PauseScreen::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if( this->isVisible() )
        return true;
    else
        return false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::initBackground()
{
	createBackgroundFadeWithOpacity( this );

    Size winSize = Director::getInstance()->getWinSize();

	m_pBackground = CCSprite::create("pause_try_more/plashka.png");
	m_pBackground->setPosition( winSize / 2 );
	addChild( m_pBackground );

	Sprite* labelSprite = Sprite::create("pause_try_more/pause_d.png");
	labelSprite->setPosition( Vec2(m_pBackground->getContentSize().width / 2 + 50, m_pBackground->getContentSize().height - 50) );
	m_pBackground->addChild( labelSprite );

	auto label = Label::createWithTTF( L10N_STRING( 1101 ).c_str(), FONT_PATH, FONT_SIZE_TITLE );
    label->setColor( FONT_COLOR_TITLE );
	label->setPosition( Vec2(labelSprite->getContentSize().width / 2.5, labelSprite->getContentSize().height / 1.5) );
	labelSprite->addChild( label );
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::initButtons()
{
	MenuItemSprite* mainMenuButton = MenuItemSprite::create(createViewStateOfButton("btn/menu.png"),
																createViewStateOfButton("btn/menu_down.png"),
																CC_CALLBACK_1(PauseScreen::onBtnMenuClick, this));
	mainMenuButton->setPosition( Vec2(m_pBackground->getContentSize().width / 5, m_pBackground->getContentSize().height / 3) );

	MenuItemSprite* restartButton = MenuItemSprite::create(createViewStateOfButton("btn/restart.png"),
																createViewStateOfButton("btn/restart_down.png"),
																CC_CALLBACK_1(PauseScreen::onBtnRestartClick, this));
	restartButton->setPosition( Vec2(m_pBackground->getContentSize().width / 2,
									m_pBackground->getContentSize().height / 3) );

	MenuItemSprite* nextButton = MenuItemSprite::create(createViewStateOfButton("btn/next.png"),
																createViewStateOfButton("btn/next_down.png"),
																CC_CALLBACK_1(PauseScreen::onNextClick, this));
	nextButton->setPosition( Vec2(m_pBackground->getContentSize().width - m_pBackground->getContentSize().width / 5,
									m_pBackground->getContentSize().height / 3) );

	Menu* menu = Menu::create(mainMenuButton, restartButton, nextButton, NULL);
	menu->setPosition( Vec2(0, 0) );
	m_pBackground->addChild( menu );
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::onBtnMenuClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    ILevel* curLvl = MANAGER_Level->getCurrentLevel();
    if( curLvl )
    {
        curLvl->getEmbiantSource()->setGain(0.0f);
        curLvl->getBackMusicSource()->setGain(0.0f);
    }
    
    //MEMORY
    MANAGER_Memory->getTempData()->vGetCoinIds.clear();
    MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
    //
    MANAGER_Level->getCurrentLevel()->resetAllLevelStates();
    MANAGER_Level->resetLevelTimer();
    MANAGER_Level->releaseLevel();
    
    MANAGER_Memory->getTempData()->currentLevelTimer = 0.0f;
    
    MANAGER_Scene->replaceScene( scene_id::MAIN_MENU, true );
    //CCSequence* action = CCSequence::create( CCDelayTime::create( 0.70f ), CCCallFunc::create( this, callfunc_selector( PauseScreen::onBtnMenuClickRecall ) ), nullptr );
    //this->runAction( action );
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::onBtnMenuClickRecall(cocos2d::Ref* sender)
{
    MANAGER_Scene->replaceScene( scene_id::MAIN_MENU );
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::onBtnRestartClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME" ) == 1 )
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Level->getCurrentLevel()->getEmbiantSource()->setGain(1.0f);
    MANAGER_Level->getCurrentLevel()->getBackMusicSource()->setGain(1.0f);
    
    //reset game-progress states
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) != 1 &&
        MANAGER_Memory->getTempData()->getLevelState( "MUSIC_BOX_RESTART" ) != 1 )
    {
        ///MEMORY
        MANAGER_Memory->getTempData()->vGetCoinIds.clear();
        MANAGER_Memory->getTempData()->vKilledMobsIds.clear();
        //
        MANAGER_Level->getCurrentLevel()->resetAllLevelStates();
        MANAGER_Level->resetLevelTimer();
        
        MANAGER_Memory->getTempData()->currentLevelTimer = 0.0f;
        
        //
        MANAGER_Memory->getTempData()->resetPlayerData();
        MANAGER_Memory->getTempData()->playerCoinsIntoPoints = 0;
    }
    
    MANAGER_Level->replaceLevel( MANAGER_Level->getCurrentLevel()->getRestartLevel() );
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::onNextClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();

    this->hide();
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::show()
{
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );
    
    IScreen* pHudScreen = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD );
    if( pHudScreen )
    {
        HudScreen* hudScr = dynamic_cast< HudScreen* >( pHudScreen );
        if( hudScr )
            hudScr->disableSticker( true ); //hudScr->hideSticker();
    }
    
    if(!this->isVisible())
    {
        if(MANAGER_Memory->getTempData()->playerHP > 0 &&
           !MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::TRY_MORE)->isVisible() &&
           !MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::COMPLETE)->isVisible())
        {
            std::vector<IScreen*> vScr = MANAGER_Screen->getScreenArray(GET_RUNNING_SCENE);
            for(auto i = vScr.begin() ; i < vScr.end() ; i++)
            {
                if(dynamic_cast<IMiniGameScreen*>((*i)))
                {
                    IMiniGameScreen* scr = dynamic_cast<IMiniGameScreen*>((*i));
                    scr->onPause();
                }
            }
            
            this->vPlayedSrc.clear();
            
            std::vector<IAudioSource*> srcList = MANAGER_Audio->getSourcesList();
            for(auto i = srcList.begin() ; i < srcList.end() ; i++)
            {
                if((*i)->getState() == AudioSourceState::PLAYING)
                    vPlayedSrc.push_back((*i));
                
                (*i)->pause();
            }
            
            this->setVisible(true);
            APPLICATION->pause();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void PauseScreen::hide()
{
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( false );
    
    this->setVisible(false);
    
    IScreen* pHudScreen = MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD );
    if( pHudScreen )
    {
        HudScreen* hudScr = dynamic_cast< HudScreen* >( pHudScreen );
        if( hudScr )
            hudScr->disableSticker( false ); //hudScr->showSticker();
    }
 
    std::vector<IScreen*> vScr = MANAGER_Screen->getScreenArray(GET_RUNNING_SCENE);
    for(auto i = vScr.begin() ; i < vScr.end() ; i++)
    {
        if(dynamic_cast<IMiniGameScreen*>((*i)))
        {
            IMiniGameScreen* scr = dynamic_cast<IMiniGameScreen*>((*i));
            scr->onUnpause();
        }
    }
    
    std::vector<IAudioSource*> srcList = MANAGER_Audio->getSourcesList();
    for(auto i = srcList.begin() ; i < srcList.end() ; i++)
    {
        for(auto j = vPlayedSrc.begin() ; j < vPlayedSrc.end() ; j++)
        {
            if((*i)->getId() == (*j)->getId() && (*i)->getState() != AudioSourceState::PLAYING)
                (*i)->play();
        }
    }
    
    APPLICATION->resume();
}

//---------------------------------------------------------------------------------------------------------------------------------