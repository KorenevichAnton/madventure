#include "HintMBox3Screen.h"
#include "AppController.h"

#define HINT_MSG_FONT_SIZE      24
#define HINT_TITLE_FONT_SIZE    32

//musicbox sounds
#define SOUND_BARABAN       "audio/scenes/L1_3_1_musicBox2/baraban.wav"
#define SOUND_FORTEPIANO    "audio/scenes/L1_3_1_musicBox2/fortepiano.wav"
#define SOUND_GITARA        "audio/scenes/L1_3_1_musicBox2/gitara.wav"
#define SOUND_BELL          "audio/scenes/L1_3_1_musicBox2/triangle.wav"
#define SOUND_SKRIPKA       "audio/scenes/L1_3_1_musicBox2/skripka.wav"
#define SOUND_TRUBA         "audio/scenes/L1_3_1_musicBox2/truba.wav"

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
HintMBox3Screen* HintMBox3Screen::create()
{
    HintMBox3Screen* pRet = new HintMBox3Screen();
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
HintMBox3Screen::HintMBox3Screen():
backHint(NULL),
audioSrc( nullptr )
{
    selfId = screen_id::HINT_MBOX_3;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintMBox3Screen::~HintMBox3Screen()
{
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintMBox3Screen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    
    this->backHint = Sprite::create("textures/screens/helper/podlojka.png");
    this->backHint->setAnchorPoint(Vec2(0, 0));
    this->backHint->setPosition(Vec2(5, 200));
    this->addChild(this->backHint);
    
    m_pControlsMenu = CCMenu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0));
    this->backHint->addChild(m_pControlsMenu);
    
    MenuItemImage* btnClose = MenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( HintMBox3Screen::callbackClose, this ) );
    btnClose->setPosition(Vec2( 930, 420 ) );
    this->m_pControlsMenu->addChild(btnClose);
    
    MenuItemImage* note1 = MenuItemImage::create("textures/screens/helper/piano.png", "textures/screens/helper/piano_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote1, this ) );
    note1->setAnchorPoint( Vec2( 0, 0 ) );
    note1->setPosition( Vec2( 140, 250 ) );
    this->m_pControlsMenu->addChild( note1 );
    auto lNote1 = Label::createWithTTF( L10N_STRING( 1601 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote1->setColor( FONT_COLOR_VALUE );
    lNote1->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote1->setPosition( Vec2( note1->getContentSize().width - 60, -30 ) );
    note1->addChild( lNote1 );
    
    MenuItemImage* note2 = MenuItemImage::create("textures/screens/helper/trumpet.png", "textures/screens/helper/trumpet_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote2, this ) );
    note2->setAnchorPoint( Vec2( 0, 0 ) );
    note2->setPosition( Vec2( 270, 250 ) );
    this->m_pControlsMenu->addChild( note2 );
    auto lNote2 = Label::createWithTTF( L10N_STRING( 1602 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote2->setColor( FONT_COLOR_VALUE );
    lNote2->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote2->setPosition( Vec2( note2->getContentSize().width - 60, -30 ) );
    note2->addChild( lNote2 );
    
    MenuItemImage* note3 = MenuItemImage::create("textures/screens/helper/violin.png", "textures/screens/helper/violin_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote3, this ) );
    note3->setAnchorPoint( Vec2( 0, 0 ) );
    note3->setPosition( Vec2( 410, 250 ) );
    this->m_pControlsMenu->addChild( note3 );
    auto lNote3 = Label::createWithTTF( L10N_STRING( 1603 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote3->setColor( FONT_COLOR_VALUE );
    lNote3->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote3->setPosition( Vec2( note3->getContentSize().width - 60, -30 ) );
    note3->addChild( lNote3 );
    
    MenuItemImage* note4 = MenuItemImage::create("textures/screens/helper/guitar.png", "textures/screens/helper/guitar_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote4, this ) );
    note4->setAnchorPoint( Vec2( 0, 0 ) );
    note4->setPosition( Vec2( 540, 250 ) );
    this->m_pControlsMenu->addChild( note4 );
    auto lNote4 = Label::createWithTTF( L10N_STRING( 1604 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote4->setColor( FONT_COLOR_VALUE );
    lNote4->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote4->setPosition( Vec2( note4->getContentSize().width - 60, -30 ) );
    note4->addChild( lNote4 );
    
    MenuItemImage* note5 = MenuItemImage::create("textures/screens/helper/bell.png", "textures/screens/helper/bell_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote5, this ) );
    note5->setAnchorPoint( Vec2( 0, 0 ) );
    note5->setPosition( Vec2( 680, 250 ) );
    this->m_pControlsMenu->addChild( note5 );
    auto lNote5 = Label::createWithTTF( L10N_STRING( 1605 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote5->setColor( FONT_COLOR_VALUE );
    lNote5->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote5->setPosition( Vec2( note5->getContentSize().width - 50, -30 ) );
    note5->addChild( lNote5 );
    
    MenuItemImage* note6 = MenuItemImage::create("textures/screens/helper/drum.png", "textures/screens/helper/drum_down.png", CC_CALLBACK_1( HintMBox3Screen::menuCallbackNote6, this ) );
    note6->setAnchorPoint( Vec2( 0, 0 ) );
    note6->setPosition( Vec2( 820, 250 ) );
    this->m_pControlsMenu->addChild( note6 );
    auto lNote6 = Label::createWithTTF( L10N_STRING( 1606 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote6->setColor( FONT_COLOR_VALUE );
    lNote6->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote6->setPosition( Vec2( note6->getContentSize().width - 60, -30 ) );
    note6->addChild( lNote6 );
    
    Sprite* header = Sprite::create("textures/screens/helper/header.png");
    header->setAnchorPoint( Vec2( 0, 0 ) );
    header->setPosition( Vec2( 340, 350 ) );
    this->backHint->addChild( header );
    auto lHeader = Label::createWithTTF( L10N_STRING( 1607 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHeader->setColor( FONT_COLOR_VALUE );
    lHeader->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lHeader->setPosition( Vec2( 180, 105 ) );
    header->addChild( lHeader );
    
    /* PORT
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(9);
    
    this->m_pControlsMenu->setTouchPriority( 0 );
    */
    
    //scaling magic
    note1->setScale( 0.8 );
    note2->setScale( 0.8 );
    note3->setScale( 0.8 );
    note4->setScale( 0.8 );
    note5->setScale( 0.8 );
    note6->setScale( 0.8 );
    
    this->audioSrc = MANAGER_Audio->createStaticSource();
    if(this->audioSrc)
        this->audioSrc->setAssociate(AudioSourceAssociate::SOUND);
    
    /* Initialize a listener of touches.
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2( HintMBox3Screen::onTouchBegan, this );
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, this );
    */
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::update(float _dt)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintMBox3Screen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void HintMBox3Screen::callbackClose(cocos2d::Ref* _pSender)
{
    this->audioSrc->stop();
    
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT_MBOX_3);
    //MANAGER_Audio->setMusicVolume(1.0f);  ??
    //MANAGER_Audio->setSoundVolume(1.0f);  ??
    
    //MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT);
    //m_functionCallBackOnClose();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote1(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_FORTEPIANO);
    this->audioSrc->loadSettings(SOUND_FORTEPIANO);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_FORTEPIANO);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote2(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_TRUBA);
    this->audioSrc->loadSettings(SOUND_TRUBA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_TRUBA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote3(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_SKRIPKA);
    this->audioSrc->loadSettings(SOUND_SKRIPKA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_SKRIPKA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote4(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_GITARA);
    this->audioSrc->loadSettings(SOUND_GITARA);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_GITARA);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote5(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_BELL);
    this->audioSrc->loadSettings(SOUND_BELL);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_BELL);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintMBox3Screen::menuCallbackNote6(Ref* _pSender)
{
    this->audioSrc->loadAudioFile(SOUND_BARABAN);
    this->audioSrc->loadSettings(SOUND_BARABAN);
    assert(audioSrc);
    this->audioSrc->play();
    /*
    MANAGER_Audio->stopMusic(SOUND_TRUBA);
    MANAGER_Audio->stopMusic(SOUND_SKRIPKA);
    MANAGER_Audio->stopMusic(SOUND_GITARA);
    MANAGER_Audio->stopMusic(SOUND_BELL);
    MANAGER_Audio->stopMusic(SOUND_BARABAN);
    MANAGER_Audio->stopMusic(SOUND_FORTEPIANO);
    //
    MANAGER_Audio->playMusic(SOUND_BARABAN);
     */
}

//---------------------------------------------------------------------------------------------------------------------------------