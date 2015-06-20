#include "CompleteScreen.h"
#include "AppController.h"

#include "MainMenuScene.h"
#include "LevelMemoryData.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
CompleteScreen* CompleteScreen::create()
{
    CompleteScreen* pRet = new CompleteScreen();
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
CompleteScreen::CompleteScreen() :
     m_pCompletedLevel( (int)level_id::NONE  )
    ,m_pTotalScore( nullptr )
    ,m_pStar3( nullptr )
    ,m_pStar2( nullptr )
    ,m_pStar1( nullptr )
{
    selfId = screen_id::COMPLETE;
}

//---------------------------------------------------------------------------------------------------------------------------------
CompleteScreen::~CompleteScreen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool CompleteScreen::init()
{
	if( !IScreen::init() )
		return false;

    
	initBackground();
	initButtons();
    initLabels();
    
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    
    this->hide();

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::update(float _dt)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::initBackground()
{
    Size winSize = Director::getInstance()->getWinSize();

	createBackgroundFadeWithOpacity(this);

	m_pBackgroundPanel = CCSprite::create("completed/plashka.png");
	m_pBackgroundPanel->setPosition( winSize / 2 );
	addChild( m_pBackgroundPanel );
    
    Size bgSize = m_pBackgroundPanel->getContentSize();
    
    m_pStarPanel = Sprite::create("completed/win.png");
    m_pStarPanel->setAnchorPoint(Vec2(0.45, 0.75));
    m_pStarPanel->setPosition(Vec2(bgSize.width / 2, bgSize.height));
    m_pBackgroundPanel->addChild( m_pStarPanel );
    
    Sprite* note = CCSprite::create("completed/cay.png");
    note->setPosition(Vec2(bgSize.width - bgSize.width / 5, bgSize.height / 2));
    m_pBackgroundPanel->addChild( note );
    
    m_pBackgroundPanel->setPositionY(m_pBackgroundPanel->getPositionY() - m_pStarPanel->getContentSize().height / 8);
    
    m_pStar1 = Sprite::create("completed/star1.png");
    m_pStar1->setPosition( Vec2(58+65, 107-13) );
    m_pStarPanel->addChild( m_pStar1 );
    m_pStar2 = CCSprite::create("completed/star2.png");
    m_pStar2->setPosition( Vec2(156+62, 107-3) );
    m_pStarPanel->addChild( m_pStar2 );
    m_pStar3 = CCSprite::create("completed/star3.png");
    m_pStar3->setPosition( Vec2(250+60, 107+5) );
    m_pStarPanel->addChild( m_pStar3 );
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::initButtons()
{
	Size bgSize = m_pBackgroundPanel->getContentSize();

	MenuItemSprite* mainMenuButton = MenuItemSprite::create(createViewStateOfButton("btn/menu.png"),
																createViewStateOfButton("btn/menu_down.png"),
																CC_CALLBACK_1( CompleteScreen::onBtnMenuClick, this ) );
	mainMenuButton->setPosition( Vec2(bgSize.width / 3, bgSize.height / 9) );

	MenuItemSprite* nextButton = MenuItemSprite::create(createViewStateOfButton("btn/next.png"),
															createViewStateOfButton("btn/next_down.png"),
															CC_CALLBACK_1( CompleteScreen::onBtnNextClick, this ) );
	nextButton->setPosition( Vec2(bgSize.width - bgSize.width / 3, bgSize.height / 9) );

	/*CCMenuItemSprite* restartButton = CCMenuItemSprite::create(createViewStateOfButton("btn/restart.png"),
															   createViewStateOfButton("btn/restart_down.png"),
															   this,
															   menu_selector(CompleteScreen::onBtnRestartClick));
	restartButton->setPosition( ccp(bgSize.width - bgSize.width / 4, bgSize.height / 9) );
     */
    
	Menu* menu = Menu::create(mainMenuButton, nextButton, NULL);
	menu->setPosition( Vec2(0, 0) );
	m_pBackgroundPanel->addChild( menu );
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::initLabels()
{
	m_startLabelPos = Vec2(m_pBackgroundPanel->getContentSize().width / 12, m_pBackgroundPanel->getContentSize().height - m_pBackgroundPanel->getContentSize().height / 3);
    m_labelOffsetY = 35;
    m_labelCounter = 0;
    
    auto completedLabel = Label::createWithTTF( L10N_STRING( 1801 ).c_str(), FONT_PATH, FONT_SIZE_VALUE );
    completedLabel->setColor( FONT_COLOR_TITLE );
    completedLabel->setPosition(Vec2(m_pBackgroundPanel->getContentSize().width / 2, m_pBackgroundPanel->getContentSize().height - m_pBackgroundPanel->getContentSize().height / 4));
    m_pBackgroundPanel->addChild( completedLabel );
 
    char buf[50];
    
    m_pDifficulty = createLabelWithTitle( L10N_STRING( 1802 ).c_str(), "Master" );  //TO DO: need statistics
    
    //
	m_pScore = createLabelWithTitle( L10N_STRING( 1803 ).c_str(), "0" );  //TO DO: need statistics
	
    m_pMechanismsBroken = createLabelWithTitle( L10N_STRING( 1804 ).c_str(), "0" );  //TO DO: need statistics
	m_pMusicTasks = createLabelWithTitle( L10N_STRING( 1805 ).c_str(), "0" );  //TO DO: need statistics
	m_pTimeBonus = createLabelWithTitle( L10N_STRING( 1806 ).c_str(), "0" );  //TO DO: need statistics
	m_pSleepedCharacters = createLabelWithTitle( L10N_STRING( 1807 ).c_str(), "0" );  //TO DO: need statistics
	m_pSecretPlaces = createLabelWithTitle( L10N_STRING( 1808 ).c_str(), "0" );  //TO DO: need statistics
	//m_pAdditionalTask = createLabelWithTitle( L10N_STRING( 1809 ).c_str(), "task");  //TO DO: need statistics
    
    //total score init
    Node* totalScoreNode = CCNode::create();
    totalScoreNode->setAnchorPoint(Vec2(0.5, 0.5));
    totalScoreNode->setPosition(Vec2(m_pBackgroundPanel->getContentSize().width / 2, m_startLabelPos.y - m_labelOffsetY * (m_labelCounter + 0.5)));
    m_pBackgroundPanel->addChild( totalScoreNode );
    
    auto totalScoreTitle = Label::createWithTTF( L10N_STRING( 1810 ).c_str(), FONT_PATH, FONT_SIZE_VALUE );
    totalScoreTitle->setAnchorPoint(Vec2(0.0, 0.5));
    totalScoreTitle->setColor( FONT_COLOR_TITLE );
    totalScoreTitle->setPosition(Vec2(0,0));
    totalScoreNode->addChild( totalScoreTitle );
    
    sprintf(buf, "%d", 0 );  //TO DO: need statistics
    m_pTotalScore = Label::createWithTTF( buf, FONT_PATH, FONT_SIZE_VALUE );
    m_pTotalScore->setAnchorPoint(Vec2(0.0, 0.5));
    m_pTotalScore->setColor( FONT_COLOR_VALUE );
    m_pTotalScore->setPosition(Vec2(totalScoreTitle->getContentSize().width + 10, totalScoreTitle->getPosition().y));
    totalScoreNode->addChild( m_pTotalScore );
    
    totalScoreNode->setContentSize(Size(totalScoreTitle->getContentSize().width + m_pTotalScore->getContentSize().width, 0));
}

//---------------------------------------------------------------------------------------------------------------------------------
Label* CompleteScreen::createLabelWithTitle(const char* title, const char* labelText/* = ""*/)
{
    auto titleLabel = Label::createWithTTF( title, FONT_PATH, FONT_SIZE_VALUE );
    titleLabel->setColor( FONT_COLOR_TITLE );
    titleLabel->setAnchorPoint(Vec2(0, 0.5));
    titleLabel->setPosition(Vec2(m_startLabelPos.x, m_startLabelPos.y - m_labelOffsetY * m_labelCounter));
    m_pBackgroundPanel->addChild( titleLabel );
    
    auto text = Label::createWithTTF( labelText, FONT_PATH, FONT_SIZE_VALUE );
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setColor( FONT_COLOR_VALUE );
    text->setPosition(Vec2( titleLabel->getPosition().x + titleLabel->getContentSize().width + 10, titleLabel->getPosition().y));
    m_pBackgroundPanel->addChild( text );
    
    m_labelCounter++;
    
    return text;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::onBtnMenuClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    Sequence* action = Sequence::create( DelayTime::create( 0.70f ), CallFunc::create( CC_CALLBACK_0( CompleteScreen::onBtnMenuClickRecall, this ) ), nullptr );
    this->runAction(action);
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::onBtnMenuClickRecall()
{
    MANAGER_Scene->replaceScene( scene_id::MAIN_MENU );
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::onBtnNextClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    Sequence* action = Sequence::create( DelayTime::create( 0.70f ), CallFunc::create( CC_CALLBACK_0( CompleteScreen::onBtnNextClickRecall, this ) ), nullptr );
    this->runAction(action);
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::onBtnNextClickRecall()
{
    MANAGER_Audio->setMusicVolume(1.0);
    MANAGER_Scene->replaceScene( scene_id::LEVEL_SELECTION );
}

//---------------------------------------------------------------------------------------------------------------------------------
/*
void CompleteScreen::onRestartClick(CCObject* sender)
{
    MANAGER_Audio->setMusicVolume(1.0);
    
    //reset game-progress states
    MANAGER_Memory->getTempData()->playerCoins = 0;
    MANAGER_Memory->getTempData()->playerHP = 3;
    MANAGER_Memory->getTempData()->playerFireballAmmo = 5;
    MANAGER_Level->getCurrentLevel()->resetAllLevelStates();
    
    MANAGER_Level->replaceLevel(MANAGER_Level->getCurrentLevel()->getLevelName());
}
*/

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::show()
{
    if( ( int )this->m_pCompletedLevel )
    {
        char str[ 255 ];
        
        // current level passing quality
        m_pStar1->setVisible( false );
        m_pStar2->setVisible( false );
        m_pStar3->setVisible( false );
        
        int levelPassingPercentage = ( 100 * MANAGER_Memory->getTempData()->playerCoinsIntoPoints ) / MANAGER_Memory->getTempData()->levelsProgressData[ ( int )this->m_pCompletedLevel - 1 ]->levelCoinsGoal;
        
        // considering star's amount on the end of the level according to percent ( TODO : BALANCE )
        if( levelPassingPercentage >= 90 )
        {
            m_pStar1->setVisible( true );
            m_pStar2->setVisible( true );
            m_pStar3->setVisible( true );
        }
        else if( levelPassingPercentage >= 60 )
        {
            m_pStar1->setVisible( true );
            m_pStar3->setVisible( true );
        }
        else
        {
            m_pStar1->setVisible( true );
        }
        
        
        // get and show complexity level
        if( MANAGER_Memory->getTempData()->complexityLevel == 0 )
            sprintf( str, "%s", L10N_STRING( 1702 ).c_str() );
        else if( MANAGER_Memory->getTempData()->complexityLevel == 1 )
            sprintf( str, "%s", L10N_STRING( 1703 ).c_str() );
        else
            sprintf( str, "%s", L10N_STRING( 1704 ).c_str() );
        m_pDifficulty->setString( str );
        
        // time bonus
        int levelTimeGoal = MANAGER_Memory->getTempData()->levelsProgressData[ ( int )this->m_pCompletedLevel - 1 ]->levelTimeGoal;
        int levelTime = MANAGER_Memory->getTempData()->currentLevelTimer;
        int bonusTime = levelTimeGoal - levelTime;
        if( bonusTime > 0 )
            sprintf( str, "%d", bonusTime );
        else
            sprintf( str, "%d", 0 );
        m_pTimeBonus->setString( str );
        
        
        // player stat
        sprintf( str, "%d", MANAGER_Memory->getTempData()->statBrokenMechs );
        m_pMechanismsBroken->setString( str );
        
        sprintf( str, "%d", MANAGER_Memory->getTempData()->statMusicTasks );
        m_pMusicTasks->setString( str );
        
        sprintf( str, "%d", MANAGER_Memory->getTempData()->statSecretPlaces );
        m_pSecretPlaces->setString( str );
        
        sprintf( str, "%d", MANAGER_Memory->getTempData()->statSleepingChars );
        m_pSleepedCharacters->setString( str );
        
        // get and show score
        sprintf( str, "%d", MANAGER_Memory->getTempData()->playerCoinsIntoPoints );
        m_pScore->setString( str );
        
        // get and show total score
        sprintf( str, "%d", MANAGER_Memory->getTempData()->statTotalScore );
        m_pTotalScore->setString( str );
        
        
        this->setVisible( true );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::setCompletedLevel( int _completedLevel )
{
    this->m_pCompletedLevel = _completedLevel;
}

//---------------------------------------------------------------------------------------------------------------------------------
void CompleteScreen::hide()
{
    this->setVisible( false );
}

//---------------------------------------------------------------------------------------------------------------------------------