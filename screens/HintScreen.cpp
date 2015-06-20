#include "HintScreen.h"
#include "AppController.h"
#include "HudScreen.h"

#define HINT_MSG_FONT_SIZE      24
#define HINT_TITLE_FONT_SIZE    32

using namespace cocos2d;

//---------------------------------------------------------------------------------------------------------------------------------
HintScreen* HintScreen::create()
{
    HintScreen* pRet = new HintScreen();
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
HintScreen::HintScreen():
backLightIndex( 0 ),
aSource( nullptr )
{
    for( int i = 0 ; i < 4 ; i++ )
        backLight[i] = nullptr;
    
    selfId = screen_id::HINT;
}

//---------------------------------------------------------------------------------------------------------------------------------
HintScreen::~HintScreen()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    this->backLightIndex = MANAGER_Memory->getTempData()->getLevelState( "level_1_2_1_room_task" );
    
    dynamic_cast<HudScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::HUD))->hideSticker();
    
    this->backHint = Sprite::create("textures/screens/helper/podlojka.png");
    this->backHint->setAnchorPoint(Vec2(0, 0));
    this->backHint->setPosition(Vec2(5, 140));
    this->addChild(this->backHint);
    
    m_pControlsMenu = CCMenu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0));
    this->backHint->addChild(m_pControlsMenu);
    
    MenuItemImage* btnClose = MenuItemImage::create("btn/close.png", "btn/close_D.png", CC_CALLBACK_1( HintScreen::callbackClose, this ) );
    btnClose->setPosition(Vec2( 950, 420 ) );
    this->m_pControlsMenu->addChild(btnClose);
    
    //
    this->backLight[0] = Sprite::create( "textures/screens/helper/2.png" );
    this->backLight[0]->setAnchorPoint( Vec2( 0, 0 ) );
    this->backLight[0]->setPosition( Vec2( 140, 250 ) );
    this->backHint->addChild( this->backLight[0], 20 );
    
    this->backLight[1] = Sprite::create( "textures/screens/helper/3.png" );
    this->backLight[1]->setAnchorPoint( Vec2( 0, 0 ) );
    this->backLight[1]->setPosition( Vec2( 300, 250 ) );
    this->backHint->addChild( this->backLight[1], 20 );
    
    this->backLight[2] = Sprite::create( "textures/screens/helper/4.png" );
    this->backLight[2]->setAnchorPoint( Vec2( 0, 0 ) );
    this->backLight[2]->setPosition( Vec2( 460, 250 ) );
    this->backHint->addChild( this->backLight[2], 20 );
    
    this->backLight[3] = Sprite::create( "textures/screens/helper/5.png" );
    this->backLight[3]->setAnchorPoint( Vec2( 0, 0 ) );
    this->backLight[3]->setPosition( Vec2( 620, 250 ) );
    this->backHint->addChild( this->backLight[3], 20 );
    
    for( int i = 0 ; i < 4 ; i++ )
        this->backLight[i]->setVisible( false );
    //
    
    
    MenuItemImage* note1 = MenuItemImage::create("textures/screens/helper/2_D.png", "textures/screens/helper/2.png", CC_CALLBACK_1( HintScreen::menuCallbackNote1, this ) );
    note1->setAnchorPoint( Vec2( 0, 0 ) );
    note1->setPosition( Vec2( 140, 250 ) );
    this->m_pControlsMenu->addChild(note1);
    auto lNote1 = Label::createWithTTF( L10N_STRING( 1401 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote1->setColor( FONT_COLOR_VALUE );
    lNote1->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote1->setPosition( Vec2( note1->getContentSize().width / 2, -25 ) );
    note1->addChild( lNote1 );
    
    MenuItemImage* note2 = MenuItemImage::create("textures/screens/helper/3_D.png", "textures/screens/helper/3.png", CC_CALLBACK_1( HintScreen::menuCallbackNote2, this ) );
    note2->setAnchorPoint( Vec2( 0, 0 ) );
    note2->setPosition( Vec2( 300, 250 ) );
    this->m_pControlsMenu->addChild( note2 );
    auto lNote2 = Label::createWithTTF( L10N_STRING( 1402 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote2->setColor( FONT_COLOR_VALUE );
    lNote2->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote2->setPosition( Vec2( note2->getContentSize().width / 2, -25 ) );
    note2->addChild( lNote2 );
    
    MenuItemImage* note3 = MenuItemImage::create("textures/screens/helper/4_D.png", "textures/screens/helper/4.png", CC_CALLBACK_1( HintScreen::menuCallbackNote3, this ) );
    note3->setAnchorPoint( Vec2( 0, 0 ) );
    note3->setPosition( Vec2( 460, 250 ) );
    this->m_pControlsMenu->addChild( note3 );
    auto lNote3 = Label::createWithTTF( L10N_STRING( 1403 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote3->setColor( FONT_COLOR_VALUE );
    lNote3->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote3->setPosition( Vec2( note3->getContentSize().width / 2, -25 ) );
    note3->addChild( lNote3 );
    
    MenuItemImage* note4 = MenuItemImage::create("textures/screens/helper/5_D.png", "textures/screens/helper/5.png", CC_CALLBACK_1( HintScreen::menuCallbackNote4, this ) );
    note4->setAnchorPoint( Vec2( 0, 0 ) );
    note4->setPosition( Vec2( 620, 250 ) );
    this->m_pControlsMenu->addChild( note4 );
    auto lNote4 = Label::createWithTTF( L10N_STRING( 1404 ).c_str(), FONT_PATH,  HINT_MSG_FONT_SIZE );
    lNote4->setColor( FONT_COLOR_VALUE );
    lNote4->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote4->setPosition( Vec2( note4->getContentSize().width / 2, -25 ) );
    note4->addChild( lNote4 );
    
    MenuItemImage* note5 = MenuItemImage::create("textures/screens/helper/6_D.png", "textures/screens/helper/6.png", CC_CALLBACK_1( HintScreen::menuCallbackNote5, this ) );
    note5->setAnchorPoint( Vec2( 0, 0 ) );
    note5->setPosition( Vec2( 780, 250 ) );
    this->m_pControlsMenu->addChild( note5 );
    auto lNote5 = Label::createWithTTF( L10N_STRING( 1405 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE );
    lNote5->setColor( FONT_COLOR_VALUE );
    lNote5->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lNote5->setPosition( Vec2( note5->getContentSize().width / 2, -25 ) );
    note5->addChild( lNote5 );
    
    Sprite* header = Sprite::create("textures/screens/helper/header.png");
    header->setAnchorPoint( Vec2( 0, 0 ) );
    header->setPosition( Vec2( 340, 350 ) );
    this->backHint->addChild( header );
    auto lHeader = Label::createWithTTF( L10N_STRING( 1406 ).c_str(), FONT_PATH, HINT_TITLE_FONT_SIZE );
    lHeader->setColor( FONT_COLOR_TITLE );
    lHeader->setAnchorPoint( Vec2( 0.5, 0.5 ) );
    lHeader->setPosition( Vec2( 180, 105 ) );
    header->addChild( lHeader );
    
    auto lContent1 = Label::createWithTTF( L10N_STRING( 1407 ).c_str(), FONT_PATH, HINT_MSG_FONT_SIZE, Size( 800, 200 ), TextHAlignment::CENTER );
    lContent1->setColor( FONT_COLOR_VALUE );
    lContent1->setAnchorPoint( Vec2( 0, 0 ) );
    lContent1->setPosition( Vec2( 130, 0 ) );
    this->backHint->addChild( lContent1 );
    
    
    IScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    dynamic_cast< EventListenerTouchOneByOne* >( pEventListener )->setSwallowTouches( true );

    
    this->aSource = MANAGER_Audio->createStaticSource();
    this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::onEnter()
{
    IScreen::onEnter();
    
    Sequence* bl1 = Sequence::create(DelayTime::create(0.2f), CallFunc::create( CC_CALLBACK_0( HintScreen::showBackLight, this ) ), NULL);
    this->runAction( bl1 );
    Sequence* bl2 = Sequence::create(DelayTime::create(0.4f), CallFunc::create( CC_CALLBACK_0( HintScreen::showBackLight, this ) ), NULL);
    this->runAction( bl2 );
    Sequence* bl3 = Sequence::create(DelayTime::create(0.6f), CallFunc::create( CC_CALLBACK_0( HintScreen::showBackLight, this ) ), NULL);
    this->runAction( bl3 );
    Sequence* bl4 = Sequence::create(DelayTime::create(0.8f), CallFunc::create( CC_CALLBACK_0( HintScreen::showBackLight, this ) ), NULL);
    this->runAction( bl4 );
    Sequence* bl5 = Sequence::create(DelayTime::create(1.0f), CallFunc::create( CC_CALLBACK_0( HintScreen::showBackLight, this ) ), NULL);
    this->runAction( bl5 );
    Sequence* hbl = Sequence::create(DelayTime::create(1.2f), CallFunc::create( CC_CALLBACK_0( HintScreen::hideBackLight, this ) ), NULL);
    this->runAction( hbl );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::showBackLight()
{
    this->backLight[ this->backLightIndex ]->setVisible( !this->backLight[ this->backLightIndex ]->isVisible() );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::hideBackLight()
{
    for( int i = 0 ; i < 4 ; i++ )
        this->backLight[ i ]->setVisible( false );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::update(float _dt)
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HintScreen::onTouchBegan(Touch *pTouch, Event *pEvent)
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
void HintScreen::callbackClose(cocos2d::Ref* _pSender)
{
    MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, screen_id::HINT);
    MANAGER_Level->getCurrentLevel()->onDialogueEnd( 0 );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::menuCallbackNote1(Ref* _pSender)
{
    if(aSource)
    {
        this->aSource->loadAudioFile(SOUND_FULL);
        this->aSource->loadSettings(SOUND_FULL);
        this->aSource->play();
        this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::menuCallbackNote2(Ref* _pSender)
{
    if(aSource)
    {
        this->aSource->loadAudioFile( SOUND_HALFTH );
        this->aSource->loadSettings(SOUND_HALFTH);
        this->aSource->play();
        this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::menuCallbackNote3(Ref* _pSender)
{
    if(aSource)
    {
        this->aSource->loadAudioFile( SOUND_FOURTH );
        this->aSource->loadSettings(SOUND_FOURTH);
        this->aSource->play();
        this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::menuCallbackNote4(Ref* _pSender)
{
    if(aSource)
    {
        this->aSource->loadAudioFile( SOUND_EIGHTH );
        this->aSource->loadSettings(SOUND_EIGHTH);
        this->aSource->play();
        this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void HintScreen::menuCallbackNote5(Ref* _pSender)
{
    if(aSource)
    {
        this->aSource->loadAudioFile( SOUND_SIXTEENTH );
        this->aSource->loadSettings(SOUND_SIXTEENTH);
        this->aSource->play();
        this->aSource->setAssociate(AudioSourceAssociate::SOUND);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------