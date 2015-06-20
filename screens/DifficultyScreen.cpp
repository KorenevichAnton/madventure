#include "DifficultyScreen.h"
#include "AppController.h"

#include "Clipping.h"
#include "MainMenuScene.h"

#include "LevelSelectionScene.h"


#define BOUNCE_TIME     1.0

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
DifficultyScreen* DifficultyScreen::create()
{
    DifficultyScreen* pRet = new DifficultyScreen();
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
DifficultyScreen::DifficultyScreen()
	: m_pBackgroundPanel(NULL)
	, m_pStarPanel(NULL)
	, m_pDifficultyValue(NULL)
	, m_pLevelRecordValue(NULL)
	, m_pDifficultyListLayer(NULL)
	, m_isDifficultyListMoving(false)
    , m_levelForLoading( level_id::NONE )
    , m_levelNumForDifficultySelection( level_id::NONE )
    , m_pStar1(NULL)
    , m_pStar2(NULL)
    , m_pStar3(NULL)
{
	selfId = screen_id::DIFFICULTY;
}

//---------------------------------------------------------------------------------------------------------------------------------
DifficultyScreen::~DifficultyScreen()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool DifficultyScreen::init()
{
	if ( !IScreen::init() )
		return false;
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
	initBackground();
	initButtons();
	initLabels();
	initDifficultyScroll();

	createBackgroundFadeWithOpacity(this);
    
    this->hide();

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::update(float _dt)
{
    /*
    char str[255];
    if( MANAGER_Memory->getTempData()->complexityLevel == 0 )
        sprintf( str, "%s",  L10N_STRING( 1702 ).c_str() );
    else if( MANAGER_Memory->getTempData()->complexityLevel == 1 )
        sprintf( str, "%s",  L10N_STRING( 1703 ).c_str() );
    else
        sprintf( str, "%s",  L10N_STRING( 1704 ).c_str() );
    
    m_pDifficultyValue->setString( str );*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::initBackground()
{
    Size winSize = Director::getInstance()->getWinSize();

	m_pBackgroundPanel = CCSprite::create("difficulty/plashka.png");
	m_pBackgroundPanel->setPosition( winSize / 2 );
	addChild( m_pBackgroundPanel );

	m_pStarPanel = CCSprite::create("difficulty/star_panel.png");
	m_pStarPanel->setPosition( Vec2(m_pBackgroundPanel->getContentSize().width / 2 + 50, m_pBackgroundPanel->getContentSize().height - 50) );
	m_pBackgroundPanel->addChild( m_pStarPanel );
    
    m_pStar1 = CCSprite::create("difficulty/star1.png");
    m_pStar1->setPosition( Vec2(58, 107) );
    m_pStarPanel->addChild( m_pStar1 );
    m_pStar2 = CCSprite::create("difficulty/star2.png");
    m_pStar2->setPosition( Vec2(156, 107) );
    m_pStarPanel->addChild( m_pStar2 );
    m_pStar3 = CCSprite::create("difficulty/star3.png");
    m_pStar3->setPosition( Vec2(250, 107) );
    m_pStarPanel->addChild( m_pStar3 );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::initButtons()
{
	Size bgSize = m_pBackgroundPanel->getContentSize();

	MenuItemSprite* mainMenuButton = MenuItemSprite::create(createViewStateOfButton("btn/menu.png"),
																createViewStateOfButton("btn/menu_down.png"),
																CC_CALLBACK_1( DifficultyScreen::onMainMenuClick, this ) );
	mainMenuButton->setPosition( Vec2(bgSize.width / 3, bgSize.height / 7) );

	MenuItemSprite* restartButton = MenuItemSprite::create(createViewStateOfButton("btn/next.png"),
																createViewStateOfButton("btn/next_down.png"),
																CC_CALLBACK_1(DifficultyScreen::onNextClick, this ) );
	restartButton->setPosition( Vec2(bgSize.width - bgSize.width / 3, bgSize.height / 7) );

	Menu* menu = Menu::create(mainMenuButton, restartButton, NULL);
	menu->setPosition( Vec2(0, 0) );
	m_pBackgroundPanel->addChild( menu );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::initLabels()
{
	const int labels_vertical_offset = 40;
	Vec2 first_label_pos( 125, 540 );

	/*create difficulty label, anchor point x = 1 used for placing all labels at the same position
	auto difficultyLabel = Label::createWithTTF( L10N_STRING( 1701 ).c_str(), FONT_PATH, 26 );
	difficultyLabel->setColor( FONT_COLOR_TITLE );
	difficultyLabel->setAnchorPoint( ccp(1, 0.5) );
	difficultyLabel->setPosition( ccp( first_label_pos.x + difficultyLabel->getContentSize().width, first_label_pos.y ) );
	m_pBackgroundPanel->addChild( difficultyLabel );
    
    if( MANAGER_Memory->getTempData()->complexityLevel == 0 )
        sprintf( str, "%s", L10N_STRING( 1702 ).c_str() );
    else if( MANAGER_Memory->getTempData()->complexityLevel == 1 )
        sprintf( str, "%s", L10N_STRING( 1703 ).c_str() );
    else
        sprintf( str, "%s", L10N_STRING( 1704 ).c_str() );
    
	//create label for difficulty value
	m_pDifficultyValue = Label::createWithTTF( str, FONT_PATH, 26 );  //TO DO: need statistics
	m_pDifficultyValue->setColor( FONT_COLOR_VALUE );
	m_pDifficultyValue->setAnchorPoint( ccp(0, 0.5) );
	m_pDifficultyValue->setPosition( difficultyLabel->getPosition() + ccp( 50, 0 ) );
	m_pBackgroundPanel->addChild( m_pDifficultyValue );
    */
	//create score label
	auto scoreLabel = Label::createWithTTF( L10N_STRING( 1705 ).c_str(), FONT_PATH, 26 );
	scoreLabel->setColor( FONT_COLOR_TITLE );
	scoreLabel->setAnchorPoint( Vec2(1, 0.5) );
	scoreLabel->setPosition( Vec2( first_label_pos.x + scoreLabel->getContentSize().width, first_label_pos.y ) );
	m_pBackgroundPanel->addChild( scoreLabel );

	//create label for score value
	m_pScoreValue = Label::createWithTTF( "0", FONT_PATH, FONT_SIZE_VALUE );  //TO DO: need statistics
	m_pScoreValue->setColor( FONT_COLOR_VALUE );
	m_pScoreValue->setAnchorPoint( Vec2(0, 0.5) );
	m_pScoreValue->setPosition( scoreLabel->getPosition() + Vec2( 50, 0 ) );
	m_pBackgroundPanel->addChild( m_pScoreValue );

	//create time label
	auto timeLabel = Label::createWithTTF( L10N_STRING( 1706 ).c_str(), FONT_PATH, 26 );
	timeLabel->setColor( FONT_COLOR_TITLE );
	timeLabel->setAnchorPoint( Vec2(1, 0.5) );
	timeLabel->setPosition( Vec2( scoreLabel->getPosition().x, scoreLabel->getPosition().y - labels_vertical_offset) );
	m_pBackgroundPanel->addChild( timeLabel );

	//create label for time value
	m_pTimeValue = Label::createWithTTF( "0", FONT_PATH, FONT_SIZE_VALUE );  //TO DO: need statistics
	m_pTimeValue->setColor( FONT_COLOR_VALUE );
	m_pTimeValue->setAnchorPoint( Vec2(0, 0.5) );
	m_pTimeValue->setPosition( timeLabel->getPosition() + Vec2( 50, 0 ) );
	m_pBackgroundPanel->addChild( m_pTimeValue );
    
	//create additional label
	auto additionalLabel = Label::createWithTTF( L10N_STRING( 1707 ).c_str(), FONT_PATH, 33 );
	additionalLabel->setColor( FONT_COLOR_TITLE );
	additionalLabel->setPosition( Vec2(m_pBackgroundPanel->getContentSize().width / 2, m_pBackgroundPanel->getContentSize().height / 2 + 50) );
	m_pBackgroundPanel->addChild( additionalLabel );

	//create label for additional goal value
	m_pLevelRecordValue = Label::createWithTTF( "0", FONT_PATH, 26 );  //TO DO: need statistics
	m_pLevelRecordValue->setColor( FONT_COLOR_VALUE );
	m_pLevelRecordValue->setPosition( Vec2(additionalLabel->getPosition().x, additionalLabel->getPosition().y - labels_vertical_offset) );
	m_pBackgroundPanel->addChild( m_pLevelRecordValue );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::initDifficultyScroll()
{
	const int arrow_offset = 40;

	//init sprite for difficulty panel
	Sprite* difficultyPanel = Sprite::create("difficulty/plashka2.png");
	difficultyPanel->setPosition( Vec2(m_pBackgroundPanel->getContentSize().width / 2, m_pBackgroundPanel->getContentSize().height / 3) );
	m_pBackgroundPanel->addChild( difficultyPanel );

	//init arrows that change the state of difficulty
	MenuItemImage* leftArrow = MenuItemImage::create("difficulty/left.png",
														 "difficulty/left.png",
														 CC_CALLBACK_1( DifficultyScreen::onLeftArrowClick, this) );
	leftArrow->setPosition(Vec2(difficultyPanel->getPosition().x - difficultyPanel->getContentSize().width / 2 - arrow_offset,
							   difficultyPanel->getPosition().y));

	MenuItemImage* rightArrow = MenuItemImage::create("difficulty/right.png",
                                                      "difficulty/right.png",
                                                      CC_CALLBACK_1( DifficultyScreen::onRightArrowClick, this ) );
	rightArrow->setPosition( Vec2(difficultyPanel->getPosition().x + difficultyPanel->getContentSize().width / 2 + arrow_offset,
								difficultyPanel->getPosition().y) );

	Menu* menu = Menu::create(leftArrow, rightArrow, NULL);
    menu->setPosition( Vec2::ZERO );
	m_pBackgroundPanel->addChild( menu );

	//init clipping for scrolling layer
	Clipping* clipping = Clipping::create();
    clipping->setClipMode( clipping_mode::WORLD_ORIENTATION );
	clipping->setPosition(Vec2(difficultyPanel->getPosition().x - difficultyPanel->getContentSize().width / 2,
							  difficultyPanel->getPosition().y - difficultyPanel->getContentSize().height / 2));
	const int offset_from_borders = 20;
	clipping->setClippingRect(difficultyPanel->getPosition().x + offset_from_borders,
							  difficultyPanel->getPosition().y,
							  difficultyPanel->getContentSize().width - offset_from_borders * 2,
							  difficultyPanel->getContentSize().height);
	m_pBackgroundPanel->addChild( clipping );

	//init scrolling layer
	m_pDifficultyListLayer = Layer::create();
	m_pDifficultyListLayer->setPosition( Vec2(0, 0) );
	m_pDifficultyListLayer->setContentSize( difficultyPanel->getContentSize() );
	clipping->addChild(m_pDifficultyListLayer);
	
	//init difficulty labels on scrolling layer
	m_currentDifficultyIndex = 0;
	addDifficultyToList( L10N_STRING( 1702 ).c_str() );
	addDifficultyToList( L10N_STRING( 1703 ).c_str() );
	addDifficultyToList( L10N_STRING( 1704 ).c_str() );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::addDifficultyToList(const char* difficulty)
{
	auto difficultyLabel = Label::createWithTTF( difficulty, FONT_PATH, FONT_SIZE_VALUE );
	difficultyLabel->setColor( FONT_COLOR_TITLE );
	difficultyLabel->setAnchorPoint( Vec2(0.5, 0.5) );
    
	Size dls = m_pDifficultyListLayer->getContentSize();
	difficultyLabel->setPosition(Vec2(dls.width / 2 + dls.width * m_difficultyList.size(), dls.height / 2));
	m_pDifficultyListLayer->addChild( difficultyLabel );
    
	//add to list of difficulty
	m_difficultyList.push_back( difficultyLabel );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::onLeftArrowClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
	/*if ( m_currentDifficultyIndex == 0 )
		return;
     */
    moveToPrevDifficulty();
	/*m_currentDifficultyIndex--;
	m_isDifficultyListMoving = true;
	CCEaseElasticOut* moving = CCEaseElasticOut::create(CCMoveTo::create(BOUNCE_TIME, ccp(-m_pDifficultyListLayer->getContentSize().width * m_currentDifficultyIndex, 0)));
	m_pDifficultyListLayer->runAction(CCSequence::create( 
														 moving,
														 CCCallFunc::create(this, callfunc_selector(DifficultyScreen::endOfMoving)),
														 NULL)
									 );*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::onRightArrowClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
	/*if ( m_currentDifficultyIndex == (m_difficultyList.size() - 1) )
		return;
     */
	moveToNextDifficulty();
	/*m_currentDifficultyIndex++;
	m_isDifficultyListMoving = true;
	CCEaseElasticOut* moving = CCEaseElasticOut::create(CCMoveTo::create(BOUNCE_TIME, ccp(-m_pDifficultyListLayer->getContentSize().width * m_currentDifficultyIndex, 0)));
	m_pDifficultyListLayer->runAction(CCSequence::create( 
														 moving,
														 CCCallFunc::create(this, callfunc_selector(DifficultyScreen::endOfMoving)),
														 NULL)
									 );*/
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::moveToNextDifficulty()
{
    int next = m_currentDifficultyIndex + 1;
    if( ( m_currentDifficultyIndex == 1 && MANAGER_Memory->getTempData()->levelsProgressData[ ( int )this->m_levelNumForDifficultySelection - 1 ]->levelPassingQuality > 0 ) ||
          m_currentDifficultyIndex < 1 )
        moveDifficultyTo(next);
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::moveToPrevDifficulty()
{
    int prev = m_currentDifficultyIndex - 1;
	moveDifficultyTo(prev);
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::moveDifficultyTo(int index)
{
	if (index < 0 || index > (m_difficultyList.size() - 1))
		return;
    
    if (m_isDifficultyListMoving)
		return;

	m_currentDifficultyIndex = index;
    //set difficulty to memory manager
    MANAGER_Memory->getTempData()->complexityLevel = m_currentDifficultyIndex;
    
    m_isDifficultyListMoving = true;
	EaseElasticOut* moving = EaseElasticOut::create(CCMoveTo::create(BOUNCE_TIME, Vec2(-m_pDifficultyListLayer->getContentSize().width * m_currentDifficultyIndex, 0)));
	m_pDifficultyListLayer->runAction(Sequence::create(
														 moving,
														 CallFunc::create( CC_CALLBACK_0( DifficultyScreen::endOfMoving, this ) ),
														 NULL)
									 );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::endOfMoving()
{
	m_isDifficultyListMoving = false;

	//TODO: change info aboud difficulty
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::onMainMenuClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    this->hide();
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::onNextClick(Ref* sender)
{
    if( this->aSource )
        this->aSource->play();
    
    
    // Reset timer / statistics
    MANAGER_Level->resetLevelTimer();
    MANAGER_Memory->getTempData()->resetPlayerData();
    MANAGER_Memory->getTempData()->resetAllLevelStates();
    
    if( m_levelForLoading != level_id::NONE )
    {
        // remove global backround music
        IAudioSource* mainMenuBackMusic = MANAGER_Audio->getSource( MainMenuScene::TAG_BACKMUSIC );
        if( mainMenuBackMusic )
            mainMenuBackMusic->setGlobal( false );
        
        // replace level
        LevelManager::sharedInstance()->replaceLevel(m_levelForLoading);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::show()
{
    // reset difficulty field
    MANAGER_Memory->getTempData()->complexityLevel = m_currentDifficultyIndex;
    //
    
    LevelSelectionScene* pLevelSelectionScene = dynamic_cast<LevelSelectionScene*>( GET_RUNNING_SCENE );
    if(pLevelSelectionScene)
    {
        pLevelSelectionScene->getLevelsMenu()->setEnabled(false);
        pLevelSelectionScene->getStaticLevelsMenu()->setEnabled(false);
    }
    
    //show stars
    m_pStar1->setVisible( false );
    m_pStar2->setVisible( false );
    m_pStar3->setVisible( false );

    int passingQuality = MANAGER_Memory->getTempData()->levelsProgressData[ ( int )MANAGER_Memory->getTempData()->currentLevelIndex - 1 ]->levelPassingQuality;
    if( passingQuality == 1)
    {
        m_pStar1->setVisible( true );
    }
    else if( passingQuality == 2)
    {
        m_pStar1->setVisible( true );
        m_pStar3->setVisible( true );
    }
    else if( passingQuality == 3)
    {
        m_pStar1->setVisible( true );
        m_pStar2->setVisible( true );
        m_pStar3->setVisible( true );
    }
    
    char str[255];
    
    int index = ( int )MANAGER_Memory->getTempData()->currentLevelIndex - 1;
    if( index >= 0 )
    {
        sprintf( str, "%d", MANAGER_Memory->getTempData()->levelsProgressData[ index ]->levelCoinsGoal );
        m_pScoreValue->setString( str );
        //
        sprintf( str, "%d", MANAGER_Memory->getTempData()->levelsProgressData[ index ]->levelTimeGoal );
        m_pTimeValue->setString( str );
    
        sprintf( str, "%d", MANAGER_Memory->getTempData()->levelsProgressData[ index ]->levelCoinsRecord );
        m_pLevelRecordValue->setString( str );
    }
    
    this->setVisible( true );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::hide()
{
    auto pLevelSelectionScene = dynamic_cast<LevelSelectionScene*>( GET_RUNNING_SCENE );
    if( pLevelSelectionScene )
    {
        pLevelSelectionScene->getLevelsMenu()->setEnabled( true );
        pLevelSelectionScene->getStaticLevelsMenu()->setEnabled( true );
    }
    this->setVisible( false );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::setLevelForLoading( level_id _nextLevel )
{
    this->m_levelForLoading = _nextLevel;
    this->m_currentDifficultyIndex = 0;
    this->moveDifficultyTo( this->m_currentDifficultyIndex );
}

//---------------------------------------------------------------------------------------------------------------------------------
void DifficultyScreen::setLevelForDifficultySelection( level_id _level )
{
    this->m_levelNumForDifficultySelection = _level;
}

//---------------------------------------------------------------------------------------------------------------------------------