#include "LevelSelectionScene.h"

#include "OnLockedLevelScreen.h"
#include "DifficultyScreen.h"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
LevelSelectionScene* LevelSelectionScene::create()
{
    LevelSelectionScene* pRet = new LevelSelectionScene();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
LevelSelectionScene::LevelSelectionScene()
    : m_pLevelsMenu(NULL)
    , levelsLayer( nullptr )
    , arrowLeft( nullptr )
    , arrowRight( nullptr )
    , stages_count( 3 )
    , playerMoneyLabel( nullptr )
    , staticButtonsMenu( nullptr )
{
    for( int i = 0 ; i < 16 ; i++ )
    {
        donateLockNor[i] = nullptr;
        donateLockSel[i] = nullptr;
    }
    
	selfId = scene_id::LEVEL_SELECTION;
}

//---------------------------------------------------------------------------------------------------------------------------------
LevelSelectionScene::~LevelSelectionScene()
{
    //release screens
	MANAGER_Screen->removeScreen(this, screen_id::ON_LOCKED_LEVEL);
    MANAGER_Screen->removeScreen(this, screen_id::DIFFICULTY);
    
    
	for (int i = 0; i < m_buttonsVec.size(); i++)
    {
        GateButtonInfo* gbi = (GateButtonInfo*)(m_buttonsVec[i]->getUserData());
        delete gbi;
        gbi = NULL;
        m_buttonsVec[i]->setUserData(NULL);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool LevelSelectionScene::init()
{
    if( !IScene::init() )
        return false;
    
    
    bool res = false;
    res = MANAGER_Screen->loadScreen(this, screen_id::ON_LOCKED_LEVEL);
    assert(res);
    res = MANAGER_Screen->loadScreen(this, screen_id::DIFFICULTY);
    assert(res);
    
    this->levelsLayer = Layer::create();
    this->levelsLayer->setPosition( Vec2( 0, 0 ) );
    this->addChild( this->levelsLayer );
    
	initBackground();
	initButtons();
    
    
    // init player for button clicking sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/gui/btn_press.wav", true );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onEnter()
{
    IScene::onEnter();
    log("LevelSelectionScene::onEnter");
    
    MANAGER_Screen->attachScreen(this, screen_id::ON_LOCKED_LEVEL);
    MANAGER_Screen->attachScreen(this, screen_id::DIFFICULTY);
    
    //
    ///Reset timer / statistics
    MANAGER_Level->resetLevelTimer();
    MANAGER_Memory->getTempData()->resetPlayerData();
    MANAGER_Memory->getTempData()->resetAllLevelStates();
    //ADDING CORRECT LOCATION RESTART
    MANAGER_Memory->getTempData()->locationCoinsIntoPoints = 0;
    MANAGER_Memory->getTempData()->playerCoinsIntoPoints = 0;
    if( MANAGER_Level->getCurrentLevel() )
        MANAGER_Level->getCurrentLevel()->resetAllLevelStates();
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onExit()
{
    IScene::onExit();
    log("LevelSelectionScene::onExit");
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::clean()
{
    MANAGER_Screen->removeAllScreens( this );
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::update(float _dt)
{
    char costStringNormal[255];
    sprintf( costStringNormal, "%d", MANAGER_Memory->getTempData()->playerMoney );
    this->playerMoneyLabel->setString( costStringNormal );
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::initBackground()
{
	Size winSize = Director::getInstance()->getWinSize();

    const int offset_plate = 55;
    const int label_font_size = 23;
    
    Sprite* stagesBackground[ stages_count ];
    Sprite* stagesPlate[ stages_count ];
    Label* stagesDescriptionLabel[ stages_count ];
    
	//stages backgrounds
    stagesBackground[0] = CCSprite::create("choosing_hall/bg.png");
    stagesBackground[1] = CCSprite::create("choosing_hall/bg.png");
    stagesBackground[2] = CCSprite::create("choosing_hall/bg.png");
    for( int i = 0 ; i < stages_count ; i++ )
    {
        stagesBackground[i]->setPosition( Vec2( winSize.width / 2 + winSize.width * i, winSize.height / 2 )  );
        this->levelsLayer->addChild( stagesBackground[i] );
    }
    
    //stages plates
    stagesPlate[0] = CCSprite::create( "choosing_hall/plate.png" );
    stagesPlate[1] = CCSprite::create( "choosing_hall/plate.png" );
    stagesPlate[2] = CCSprite::create( "choosing_hall/plate.png" );
    for( int i = 0 ; i < stages_count ; i++ )
    {
        stagesPlate[i]->setPosition( Vec2( winSize.width / 2 + winSize.width * i, winSize.height - offset_plate ) );
        this->levelsLayer->addChild( stagesPlate[i] );
    }
    
    //stages description labels
    stagesDescriptionLabel[0] = Label::createWithTTF( L10N_STRING(317).c_str(), FONT_PATH, label_font_size );
    stagesDescriptionLabel[1] = Label::createWithTTF( L10N_STRING(318).c_str(), FONT_PATH, label_font_size );
    stagesDescriptionLabel[2] = Label::createWithTTF( L10N_STRING(319).c_str(), FONT_PATH, label_font_size );
    for(int i = 0 ; i < stages_count ; i++ )
    {
        stagesDescriptionLabel[i]->setColor( Color3B( 0, 0, 0) );
        stagesDescriptionLabel[i]->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
        stagesDescriptionLabel[i]->setPosition( Vec2( 214.0f, 48.0f ) );
        stagesPlate[i]->addChild( stagesDescriptionLabel[i] );
    }
    
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::initButtons()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    this->staticButtonsMenu = CCMenu::create();
    this->staticButtonsMenu->setPosition( Vec2( 0, 0 ) );
    this->addChild( this->staticButtonsMenu );
    
    MenuItemSprite* backToMenuBtn = CCMenuItemSprite::create(createViewStateOfButton("btn/menu.png"), createViewStateOfButton("btn/menu_down.png"), CC_CALLBACK_1( LevelSelectionScene::onBtnMenuClick, this ) );
    backToMenuBtn->setPosition(Vec2( 75, 75 ));
    staticButtonsMenu->addChild( backToMenuBtn );
    
    //donate button
    MenuItemSprite* donateMenuBtn = MenuItemSprite::create( CCSprite::create( "choosing_hall/donat.png" ), CCSprite::create( "choosing_hall/donat_down.png" ), CC_CALLBACK_1( LevelSelectionScene::onDonateBtn, this ) );
    donateMenuBtn->setPosition( Vec2( winSize.width - 140, 50.0f ) );
    staticButtonsMenu->addChild( donateMenuBtn );
    
    char costStringNormal[255];
    sprintf( costStringNormal, "%d", MANAGER_Memory->getTempData()->playerMoney );
    this->playerMoneyLabel = Label::createWithTTF(costStringNormal, FONT_PATH, 26);
    this->playerMoneyLabel->setAnchorPoint(Vec2(0.5, 0.5));
    this->playerMoneyLabel->setPosition(Vec2( 155, 48 ));
    this->playerMoneyLabel->setColor( FONT_COLOR_TITLE );
    donateMenuBtn->addChild( this->playerMoneyLabel, 1 );
    
    //swype buttons for relocating stage-layer with levels
    Sprite* arrowLeftUnselected = CCSprite::create( "choosing_hall/arrow_left.png" );
    Sprite* arrowLeftSelected = CCSprite::create( "choosing_hall/arrow_left_down.png" );
    Sprite* arrowLeftDisabled = CCSprite::create( "choosing_hall/arrow_left_lock.png" );
    
    Sprite* arrowRightUnselected = CCSprite::create( "choosing_hall/arrow_right.png" );
    Sprite* arrowRightSelected = CCSprite::create( "choosing_hall/arrow_right_down.png" );
    Sprite* arrowRightDisabled = CCSprite::create( "choosing_hall/arrow_right_lock.png" );
    
    //swype buttons offset
    const int offsetSwypeBtn = 50;
    
    arrowLeft = MenuItemSprite::create( arrowLeftUnselected, arrowLeftSelected, arrowLeftDisabled, CC_CALLBACK_1( LevelSelectionScene::onBtnMenuSwypeLeft, this ) );
    arrowLeft->setPosition( Vec2( offsetSwypeBtn, winSize.height / 2 ) );
    staticButtonsMenu->addChild( arrowLeft );
    
    arrowRight = MenuItemSprite::create( arrowRightUnselected, arrowRightSelected, arrowRightDisabled, CC_CALLBACK_1( LevelSelectionScene::onBtnMenuSwypeRight, this ) );
    arrowRight->setPosition( Vec2( winSize.width - offsetSwypeBtn, winSize.height / 2 ) );
    staticButtonsMenu->addChild( arrowRight );
    this->arrowLeft->setEnabled( false );
    //
    m_pLevelsMenu = CCMenu::create();
    m_pLevelsMenu->setPosition(Vec2(0, 0));
    this->levelsLayer->addChild( m_pLevelsMenu );
    
    this->initFirstStage();
    this->initSecondStage();
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::initFirstStage()
{
    MenuItem* level1_1_1 = createGateButton( MANAGER_Memory->getTempData()->levelsProgressData[0]->gameLevelsProgress,
                                             L10N_STRING( 301 ).c_str(),
                                             MANAGER_Memory->getTempData()->levelsProgressData[0]->levelPassingQuality,
                                             level_id::LVL_1_1_PART1,
                                             0,
                                             0);
    level1_1_1->setPosition(Vec2(190, 170));
    m_pLevelsMenu->addChild( level1_1_1 );
    
    MenuItem* level1_1_2 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[1]->gameLevelsProgress,
                                              L10N_STRING( 302 ).c_str(),
                                              MANAGER_Memory->getTempData()->levelsProgressData[1]->levelPassingQuality,
                                              level_id::LVL_1_1_PART2,
                                              1,
                                              0);
    level1_1_2->setPosition(Vec2(480, 170));
    m_pLevelsMenu->addChild( level1_1_2 );
    
    MenuItem* level1_1_3 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[2]->gameLevelsProgress,
                                              L10N_STRING( 303 ).c_str(),
                                              MANAGER_Memory->getTempData()->levelsProgressData[2]->levelPassingQuality,
                                              level_id::LVL_1_1_PART3,
                                              2,
                                              0);
    level1_1_3->setPosition(Vec2(800, 250));
    m_pLevelsMenu->addChild( level1_1_3 );
    
    MenuItem* level_1_2_1 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[3]->gameLevelsProgress,
                                               L10N_STRING( 304 ).c_str(),
                                               MANAGER_Memory->getTempData()->levelsProgressData[3]->levelPassingQuality,
                                               level_id::LVL_1_2_PART1,
                                               3,
                                               200);
    level_1_2_1->setPosition(Vec2(600, 400));
    m_pLevelsMenu->addChild( level_1_2_1 );
    
    MenuItem* level_1_2_2 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[4]->gameLevelsProgress,
                                               L10N_STRING( 305 ).c_str(), MANAGER_Memory->getTempData()->levelsProgressData[4]->levelPassingQuality,
                                               level_id::LVL_1_2_PART2,
                                               4,
                                               200);
    level_1_2_2->setPosition(Vec2(230, 430));
    m_pLevelsMenu->addChild( level_1_2_2 );
    
    MenuItem* level_1_3_1 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[5]->gameLevelsProgress,
                                               L10N_STRING( 306 ).c_str(),
                                               MANAGER_Memory->getTempData()->levelsProgressData[5]->levelPassingQuality,
                                               level_id::LVL_1_3_PART1,
                                               5,
                                               200);
    level_1_3_1->setPosition(Vec2(420, 580));
    m_pLevelsMenu->addChild( level_1_3_1 );
    
    MenuItem* level_1_boss = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[6]->gameLevelsProgress,
                                                L10N_STRING( 307 ).c_str(),
                                                MANAGER_Memory->getTempData()->levelsProgressData[6]->levelPassingQuality,
                                                level_id::LVL_1_BOSS,
                                                6,
                                                200);
    level_1_boss->setPosition(Vec2(820, 550));
    m_pLevelsMenu->addChild( level_1_boss );
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::initSecondStage()
{
    MenuItem* level2_boss = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[7]->gameLevelsProgress,
                                               L10N_STRING( 308 ).c_str(),
                                               MANAGER_Memory->getTempData()->levelsProgressData[7]->levelPassingQuality,
                                               level_id::NONE,
                                               7,
                                               0);
    level2_boss->setPosition(Vec2(495, 385));
    level2_boss->setVisible(false);
    m_pLevelsMenu->addChild( level2_boss );
    
    MenuItem* level3_1 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[8]->gameLevelsProgress,
                                            L10N_STRING( 309 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[8]->levelPassingQuality,
                                            level_id::NONE,
                                            8,
                                            0);
    level3_1->setPosition(Vec2(670, 435));
    level3_1->setVisible(false);
    m_pLevelsMenu->addChild( level3_1 );
    
    MenuItem* level3_2 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[9]->gameLevelsProgress,
                                            L10N_STRING( 310 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[9]->levelPassingQuality,
                                           level_id::NONE,
                                            9,
                                            0);
    level3_2->setPosition(Vec2(855, 370));
    level3_2->setVisible(false);
    m_pLevelsMenu->addChild( level3_2 );
    
    MenuItem* level3_3 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[10]->gameLevelsProgress,
                                            L10N_STRING( 311 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[10]->levelPassingQuality,
                                          level_id::NONE,
                                            10,
                                            0);
    level3_3->setPosition(Vec2(920, 530));
    level3_3->setVisible(false);
    m_pLevelsMenu->addChild( level3_3 );
    
    MenuItem* level3_boss = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[11]->gameLevelsProgress,
                                               L10N_STRING( 312 ).c_str(),
                                               MANAGER_Memory->getTempData()->levelsProgressData[11]->levelPassingQuality,
                                               level_id::NONE,
                                               11,
                                               0);
    level3_boss->setPosition(Vec2(775, 615));
    level3_boss->setVisible(false);
    m_pLevelsMenu->addChild( level3_boss );
    
    MenuItem* level4_1 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[12]->gameLevelsProgress,
                                            L10N_STRING( 313 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[12]->levelPassingQuality,
                                            level_id::NONE,
                                            12,
                                            0);
    level4_1->setPosition(Vec2(585, 610));
    level4_1->setVisible(false);
    m_pLevelsMenu->addChild( level4_1 );
    
    MenuItem* level4_2 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[13]->gameLevelsProgress,
                                            L10N_STRING( 314 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[13]->levelPassingQuality,
                                            level_id::NONE,
                                            13,
                                            0);
    level4_2->setPosition(Vec2(410, 580));
    level4_2->setVisible(false);
    m_pLevelsMenu->addChild( level4_2 );
    
    MenuItem* level4_3 = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[14]->gameLevelsProgress,
                                            L10N_STRING( 315 ).c_str(),
                                            MANAGER_Memory->getTempData()->levelsProgressData[14]->levelPassingQuality,
                                            level_id::NONE,
                                            14,
                                            0);
    level4_3->setPosition(Vec2(250, 665));
    level4_3->setVisible(false);
    m_pLevelsMenu->addChild( level4_3 );
    
    MenuItem* level4_boss = createGateButton(MANAGER_Memory->getTempData()->levelsProgressData[15]->gameLevelsProgress,
                                               L10N_STRING( 316 ).c_str(),
                                               MANAGER_Memory->getTempData()->levelsProgressData[15]->levelPassingQuality,
                                               level_id::NONE,
                                               15,
                                               0);
    level4_boss->setPosition(Vec2(65, 605));
    level4_boss->setVisible(false);
    m_pLevelsMenu->addChild( level4_boss );
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::initThirdStage()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
MenuItem* LevelSelectionScene::createGateButton(level_state state, const char* name, int stars, level_id level, int levelNum, int levelCost)
{
    //create door sprite
    string doorPath = "";
    if (state == level_state::LVL_STATE_CLOSED)
        doorPath = "choosing_hall/door-lock.png";
    if (state == level_state::LVL_STATE_OPENED || state == level_state::LVL_STATE_DONATE_LOCK)
        doorPath = "choosing_hall/door-open.png";
    if (state == level_state::LVL_STATE_COMPLETED)
        doorPath = "choosing_hall/door-pass.png";
    
    Sprite* doorSprNor = createDoorSprite(doorPath.c_str(), stars, name);
    Sprite* doorSprSel = createDoorSprite(doorPath.c_str(), stars, name);
    doorSprSel->setScale(0.9);
    
    MenuItemSprite* menuItem = MenuItemSprite::create(doorSprNor, doorSprSel, CC_CALLBACK_1( LevelSelectionScene::onLevelClick, this ) );
    
    Node* normal = menuItem->getNormalImage();
    Node* selected = menuItem->getSelectedImage();
    
    normal->setAnchorPoint(Vec2(0.5, 0.5));
    selected->setAnchorPoint(Vec2(0.5, 0.5));
    
    normal->setPosition(normal->getPosition() + normal->getContentSize() / 2);
    selected->setPosition(selected->getPosition() + selected->getContentSize() / 2);
    
    
    this->donateLockNor[levelNum] = CCSprite::create( "choosing_hall/lock.png" );
    this->donateLockNor[levelNum]->setPosition( Vec2( 55, 84 ) );
    doorSprNor->addChild( this->donateLockNor[levelNum] );
    
    this->donateLockSel[levelNum] = CCSprite::create( "choosing_hall/lock.png" );
    this->donateLockSel[levelNum]->setPosition( Vec2( 55, 84 ) );
    doorSprSel->addChild( this->donateLockSel[levelNum] );
    
    if( state == level_state::LVL_STATE_DONATE_LOCK )
    {
        this->donateLockNor[levelNum]->setVisible( true );
        this->donateLockSel[levelNum]->setVisible( true );
    }
    else
    {
        this->donateLockSel[levelNum]->setVisible( false );
        this->donateLockNor[levelNum]->setVisible( false );
    }
    
    //level cost label normal
    char costStringNormal[255];
    sprintf( costStringNormal, "%d", levelCost );
    auto levelCostLabelNor = Label::createWithTTF(costStringNormal, FONT_PATH, 22);
    levelCostLabelNor->setAnchorPoint(Vec2(0.5, 0.5));
    levelCostLabelNor->setPosition(Vec2( 68, 18 ));
    levelCostLabelNor->setColor( FONT_COLOR_TITLE );
    doorSprNor->addChild( levelCostLabelNor, 1 );
    
    //level cost label selected
    char costStringSelected[255];
    sprintf( costStringSelected, "%d", levelCost );
    auto levelCostLabelSel = Label::createWithTTF(costStringSelected, FONT_PATH, 22);
    levelCostLabelSel->setAnchorPoint(Vec2(0.5, 0.5));
    levelCostLabelSel->setPosition(Vec2( 68, 18 ));
    levelCostLabelSel->setColor( FONT_COLOR_TITLE );
    doorSprSel->addChild( levelCostLabelSel, 1 );
    
    //menuItem->setUserData( new Levels(level));
    auto gbi = new GateButtonInfo();
    gbi->level = level;
    gbi->state = state;
    gbi->cost = levelCost;
    menuItem->setUserData( gbi );
    m_buttonsVec.push_back( menuItem );
    
    return menuItem;
}

//---------------------------------------------------------------------------------------------------------------------------------
Sprite* LevelSelectionScene::createDoorSprite(const char* doorPath, int stars, const char* name)
{
    Sprite* doorSpr = CCSprite::create(doorPath);
    
    //create stars
    if (stars >= 1)
    {
        Sprite* star1 = CCSprite::create("choosing_hall/star1.png");
        star1->setPosition(Vec2(28, 147));
        doorSpr->addChild( star1 );
    }
    if (stars >= 2)
    {
        Sprite* star2 = CCSprite::create("choosing_hall/star2.png");
        star2->setPosition(Vec2(56, 154));
        doorSpr->addChild( star2 );
    }
    if (stars == 3)
    {
        Sprite* star3 = CCSprite::create("choosing_hall/star3.png");
        star3->setPosition(Vec2(84, 147));
        doorSpr->addChild( star3 );
    }
    
    /*create label
    auto levelLabel = Label::createWithTTF(name, FONT_PATH, 40);
    levelLabel->setAnchorPoint(ccp(0.5, 0));
    levelLabel->setPosition(ccp(doorSpr->getContentSize().width / 2, doorSpr->getContentSize().height));
    levelLabel->setColor( FONT_COLOR_VALUE );
    doorSpr->addChild( levelLabel );
    
    doorSpr->setContentSize(CCSizeMake(doorSpr->getContentSize().width, doorSpr->getContentSize().height + levelLabel->getContentSize().height));
    */
    
    return doorSpr;
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onBtnMenuSwypeLeft( Ref* _sender )
{
    Size winSize = Director::getInstance()->getWinSize();
    if( levelsLayer->getPosition().x < 0 )
    {
        MoveTo *move = MoveTo::create( 1, Vec2( levelsLayer->getPosition().x + winSize.width, levelsLayer->getPosition().y ) );
        this->levelsLayer->runAction( move );
        
        //disable arrow button with delay in 1.2 sec for swype
        this->arrowRight->setEnabled( false );
        this->arrowLeft->setEnabled( false );
        auto *disableDelay = Sequence::create( DelayTime::create( 1.2f ), CallFunc::create( CC_CALLBACK_0( LevelSelectionScene::enableSwypeButtons, this ) ), nullptr );
        this->runAction( disableDelay );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onBtnMenuSwypeRight( Ref *_sender )
{
    Size winSize = Director::getInstance()->getWinSize();
    if( fabs( levelsLayer->getPosition().x ) < winSize.width * (stages_count - 1) )
    {
        MoveTo *move = MoveTo::create( 1, Vec2( levelsLayer->getPosition().x - winSize.width, levelsLayer->getPosition().y ) );
        this->levelsLayer->runAction( move );
    
        //disable arrow button with delay in 1.2 sec for swype
        this->arrowRight->setEnabled( false );
        this->arrowLeft->setEnabled( false );
        auto *disableDelay = Sequence::create( DelayTime::create( 1.2f ), CallFunc::create( CC_CALLBACK_0( LevelSelectionScene::enableSwypeButtons, this ) ), nullptr );
        this->runAction( disableDelay );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::enableSwypeButtons()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    if( levelsLayer->getPosition().x == 0 )
    {
        this->arrowRight->setEnabled( true );
    }
    else if( fabs( levelsLayer->getPosition().x ) == winSize.width * (stages_count - 1) )
    {
        this->arrowLeft->setEnabled( true );
    }
    else
    {
        this->arrowLeft->setEnabled( true );
        this->arrowRight->setEnabled( true );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onLevelClick( Ref* sender )
{
    GateButtonInfo* gbi = (GateButtonInfo*)(((MenuItem*)sender)->getUserData());
    if (gbi->state == level_state::LVL_STATE_CLOSED)
    {
        IScreen* pScreen = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::ON_LOCKED_LEVEL);
        if(pScreen)
            dynamic_cast<OnLockedLevelScreen*>(pScreen)->show();
    }
    else if( gbi->state == level_state::LVL_STATE_DONATE_LOCK &&
             gbi->cost <= MANAGER_Memory->getTempData()->playerMoney )
    {
        gbi->state = level_state::LVL_STATE_OPENED;
        
        MANAGER_Memory->getTempData()->levelsProgressData[ ( int )gbi->level - 1 ]->gameLevelsProgress = level_state::LVL_STATE_OPENED;
        MANAGER_Memory->getTempData()->playerMoney = MANAGER_Memory->getTempData()->playerMoney - gbi->cost;
        
        this->donateLockSel[ ( int )gbi->level - 1 ]->setVisible( false );
        this->donateLockNor[ ( int )gbi->level - 1 ]->setVisible( false );
        MANAGER_Memory->uploadTempToPersistData();
        MANAGER_Memory->saveProfile();
    }
    else
    {
        if( this->aSource )
            this->aSource->play();
        
        IScreen* pScreen = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::DIFFICULTY);
        if(pScreen)
        {
            DifficultyScreen* pDifficultyScreen = dynamic_cast<DifficultyScreen*>(pScreen);
            
            MANAGER_Memory->getTempData()->currentLevelIndex = gbi->index;
            pDifficultyScreen->setLevelForLoading(gbi->level);
            pDifficultyScreen->setLevelForDifficultySelection( gbi->level );
            pDifficultyScreen->show();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onDonateBtn( Ref* _sender )
{
    MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::SHOP );
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onBtnMenuClick( Ref* _sender )
{
    if( this->aSource )
        this->aSource->play();
    
    Sequence* action = Sequence::create( DelayTime::create( 0.70f ), CallFunc::create( CC_CALLBACK_0( LevelSelectionScene::onBtnMenuClickRecall, this ) ), nullptr );
    this->runAction(action);
}

//---------------------------------------------------------------------------------------------------------------------------------
void LevelSelectionScene::onBtnMenuClickRecall()
{
    MANAGER_Scene->replaceScene( scene_id::MAIN_MENU );
}

//---------------------------------------------------------------------------------------------------------------------------------