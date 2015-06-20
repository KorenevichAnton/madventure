#include "HintMBox4Screen.h"
#include "AppController.h"

#define HINT_MSG_FONT_SIZE      24
#define HINT_TITLE_FONT_SIZE    32

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
HintMBox4Screen* HintMBox4Screen::create()
{
    HintMBox4Screen* pRet = new HintMBox4Screen();
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
HintMBox4Screen::HintMBox4Screen():
backHint(NULL),
audioSrc(nullptr)
{
    selfId = screen_id::HINT_MBOX_4;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintMBox4Screen::~HintMBox4Screen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintMBox4Screen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    this->backHint = CCSprite::create("textures/screens/helper/podlojka.png");
    this->backHint->setAnchorPoint(Vec2(0, 0));
    this->backHint->setPosition(Vec2(5, 200));
    this->addChild(this->backHint);
    
    m_pControlsMenu = CCMenu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0));
    this->backHint->addChild(m_pControlsMenu);
    
    MenuItemImage* btnClose = MenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( HintMBox4Screen::callbackClose, this ) );
    btnClose->setPosition(Vec2( 900, 420 ) );
    this->m_pControlsMenu->addChild(btnClose);
    
    MenuItemImage* note1 = MenuItemImage::create("textures/screens/helper/piano.png", "textures/screens/helper/piano_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote1, this ) );
    note1->setAnchorPoint( Vec2( 0, 0 ) );
    note1->setPosition( Vec2( 180, 260 ) );
    note1->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note1 );
    auto lNote1 = Label::createWithTTF( L10N_STRING( 1501 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote1->setColor( FONT_COLOR_VALUE );
    lNote1->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote1->setPosition( Vec2( note1->getContentSize().width - 60, -25 ) );
    note1->addChild( lNote1 );
    
    MenuItemImage* note2 = MenuItemImage::create("textures/screens/helper/trumpet.png", "textures/screens/helper/trumpet_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote2, this ) );
    note2->setAnchorPoint( Vec2( 0, 0 ) );
    note2->setPosition( Vec2( 330, 260 ) );
    note2->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note2 );
    auto lNote2 = Label::createWithTTF( L10N_STRING( 1502 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote2->setColor( FONT_COLOR_VALUE );
    lNote2->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote2->setPosition( Vec2( note2->getContentSize().width - 60, -25 ) );
    note2->addChild( lNote2 );
    
    MenuItemImage* note3 = MenuItemImage::create("textures/screens/helper/violin.png", "textures/screens/helper/violin_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote3, this ) );
    note3->setAnchorPoint( Vec2( 0, 0 ) );
    note3->setPosition( Vec2( 480, 260 ) );
    note3->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note3 );
    auto lNote3 = Label::createWithTTF( L10N_STRING( 1503 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote3->setColor( FONT_COLOR_VALUE );
    lNote3->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote3->setPosition( Vec2( note3->getContentSize().width - 60, -25 ) );
    note3->addChild( lNote3 );
    
    MenuItemImage* note4 = MenuItemImage::create("textures/screens/helper/guitar.png", "textures/screens/helper/guitar_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote4, this ) );
    note4->setAnchorPoint( Vec2( 0, 0 ) );
    note4->setPosition( Vec2( 630, 260 ) );
    note4->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note4 );
    auto lNote4 = Label::createWithTTF( L10N_STRING( 1504 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote4->setColor( FONT_COLOR_VALUE );
    lNote4->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote4->setPosition( Vec2( note4->getContentSize().width - 60, -25 ) );
    note4->addChild( lNote4 );
    
    MenuItemImage* note5 = MenuItemImage::create("textures/screens/helper/bell.png", "textures/screens/helper/bell_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote5, this ) );
    note5->setAnchorPoint( Vec2( 0, 0 ) );
    note5->setPosition( Vec2( 780, 260 ) );
    note5->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note5 );
    auto lNote5 = Label::createWithTTF( L10N_STRING( 1505 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote5->setColor( FONT_COLOR_VALUE );
    lNote5->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote5->setPosition( Vec2( note5->getContentSize().width - 60, -25 ) );
    note5->addChild( lNote5 );
    
    MenuItemImage* note6 = MenuItemImage::create("textures/screens/helper/drum.png", "textures/screens/helper/drum_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote6, this ) );
    note6->setAnchorPoint( Vec2( 0, 0 ) );
    note6->setPosition( Vec2( 255, 130 ) );
    note6->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note6 );
    auto lNote6 = Label::createWithTTF( L10N_STRING( 1506 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote6->setColor( FONT_COLOR_VALUE );
    lNote6->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote6->setPosition( Vec2( note6->getContentSize().width - 60, -25 ) );
    note6->addChild( lNote6 );
    
    MenuItemImage* note7 = MenuItemImage::create("textures/screens/helper/clarinet.png", "textures/screens/helper/clarinet_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote7, this ) );
    note7->setAnchorPoint( Vec2( 0, 0 ) );
    note7->setPosition( Vec2( 405, 130 ) );
    note7->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note7 );
    auto lNote7 = Label::createWithTTF( L10N_STRING( 1507 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote7->setColor( FONT_COLOR_VALUE );
    lNote7->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote7->setPosition( Vec2( note7->getContentSize().width - 60, -25 ) );
    note7->addChild( lNote7 );
    
    /*
    MenuItemImage* note8 = MenuItemImage::create("textures/screens/helper/saxophone.png", "textures/screens/helper/saxophone_down.png", this, menu_selector(HintMBox4Screen::menuCallbackNote8));
    note8->setAnchorPoint( Vec2( 0, 0 ) );
    note8->setPosition( Vec2( 480, 130 ) );
    note8->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note8 );
    auto lNote8 = Label::createWithTTF( L10N_STRING( 1508 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote8->setColor( FONT_COLOR_VALUE );
    lNote8->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote8->setPosition( Vec2( note8->getContentSize().width - 60, -25 ) );
    note8->addChild( lNote8 );
    */
     
    MenuItemImage* note9 = MenuItemImage::create("textures/screens/helper/vibrofon.png", "textures/screens/helper/vibrofon_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote9, this ) );
    note9->setAnchorPoint( Vec2( 0, 0 ) );
    note9->setPosition( Vec2( 555, 130 ) );
    note9->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note9 );
    auto lNote9 = Label::createWithTTF( L10N_STRING( 1509 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote9->setColor( FONT_COLOR_VALUE );
    lNote9->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote9->setPosition( Vec2( note9->getContentSize().width - 60, -25 ) );
    note9->addChild( lNote9 );
    
    MenuItemImage* note10 = MenuItemImage::create("textures/screens/helper/accordion.png", "textures/screens/helper/accordion_down.png", CC_CALLBACK_1( HintMBox4Screen::menuCallbackNote10, this ) );
    note10->setAnchorPoint( Vec2( 0, 0 ) );
    note10->setPosition( Vec2( 705, 130 ) );
    note10->setScale( 0.8 );
    this->m_pControlsMenu->addChild( note10 );
    auto lNote10 = Label::createWithTTF( L10N_STRING( 1510 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote10->setColor( FONT_COLOR_VALUE );
    lNote10->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote10->setPosition( Vec2( note10->getContentSize().width - 60, -25 ) );
    note10->addChild( lNote10 );
    
    Sprite* header = Sprite::create("textures/screens/helper/header.png");
    header->setAnchorPoint( Vec2( 0, 0 ) );
    header->setPosition( Vec2( 340, 350 ) );
    this->backHint->addChild( header );
    auto lHeader = Label::createWithTTF( L10N_STRING( 1511 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHeader->setColor( FONT_COLOR_TITLE );
    lHeader->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lHeader->setPosition( Vec2( 180, 105 ) );
    header->addChild( lHeader );
    
    
    /* Initialize a listener of touches.
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2( HintMBox4Screen::onTouchBegan, this );
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
    */
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );
    /* PORT
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(9);
    
    this->m_pControlsMenu->setTouchPriority( 0 );
    */
    
    this->audioSrc = MANAGER_Audio->createStaticSource();
    if(this->audioSrc)
        this->audioSrc->setAssociate(AudioSourceAssociate::SOUND);
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::update(float _dt)
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintMBox4Screen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void HintMBox4Screen::callbackClose(cocos2d::Ref* _pSender)
{
    this->audioSrc->stop();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    */
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT_MBOX_4);
    //MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT);
    //m_functionCallBackOnClose();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote1(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_FORTEPIANO);
    this->audioSrc->loadSettings(SOUND_FORTEPIANO);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_FORTEPIANO);
    */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote2(Ref* _pSender)
{
    
    this->audioSrc->loadAudioFile(SOUND_TRUBA);
    this->audioSrc->loadSettings(SOUND_TRUBA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_TRUBA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote3(Ref* _pSender)
{
    
    this->audioSrc->loadAudioFile(SOUND_SKRIPKA);
    this->audioSrc->loadSettings(SOUND_SKRIPKA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_SKRIPKA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote4(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_GITARA);
    this->audioSrc->loadSettings(SOUND_GITARA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_GITARA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote5(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_TRIANGLE);
    this->audioSrc->loadSettings(SOUND_TRIANGLE);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_TRIANGLE);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote6(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_BARABAN);
    this->audioSrc->loadSettings(SOUND_BARABAN);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_BARABAN);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote7(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_KLARNET);
    this->audioSrc->loadSettings(SOUND_KLARNET);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_KLARNET);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote8(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_SAX);
    this->audioSrc->loadSettings(SOUND_SAX);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_SAX);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote9(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_VIBRAPHONE);
    this->audioSrc->loadSettings(SOUND_VIBRAPHONE);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_VIBRAPHONE);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox4Screen::menuCallbackNote10(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_AKKORDEON);
    this->audioSrc->loadSettings(SOUND_AKKORDEON);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_AKKORDEON);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_TRIANGLE);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_KLARNET);
    MANAGER_Audio->stopMusic(SOUND_SAX);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_VIBRAPHONE);
    
    MANAGER_Audio->playMusic(SOUND_AKKORDEON);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------