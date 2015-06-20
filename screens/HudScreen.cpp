#include "HudScreen.h"
#include "AppController.h"

#include "GameScene.h"

#include "PauseScreen.h"

#include "GameObject.h"
#include "PhysDynamicPlayerObject.h"
#include "GraphicAnimationObject.h"

//TEMP: for new camera test
#include "GameWorld.h"
#include "GameCamera.h"

//TEMP: for test fast player moving
#include "PhysDynamicPlayerObject.h"

#include "DialogScreen.h"


#define STICKER_INTERNAL_RADIUS     20  //sticker internal cicle radius
#define STICKER_EXTERNAL_RADIUS     90  //sticker external circle radius

using namespace std;
using namespace cocos2d;
using namespace physic;
using namespace graphic;

//---------------------------------------------------------------------------------------------------------------------------------
HudScreen* HudScreen::create()
{
    HudScreen* pRet = new HudScreen();
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
HudScreen::HudScreen()
    : m_pControlsMenu(NULL)
    , m_pStick(NULL)
    , m_pStickerBg(NULL)
    , m_pStickCenterNode(NULL)
    , m_isOnStickClick(false)
    , m_isStickActive(true)
    , m_isStickMoveX(false)
    , m_isStickMoveY(false)
    , zoomLabel(NULL)   //TEMP: for new camera test
    , stickerTouch(NULL)
    , isStickerHide(false)
    , m_labelCountNotes(nullptr)
    , m_labelCountHP(nullptr)
    , textLabelDebugDraw( nullptr )
    , textLabelGMod( nullptr )
    , m_labelLevelTimer(nullptr)
    , m_labelCoinsToPointes( nullptr )
    , m_stickerDisabler( false )
    , tableTimer( nullptr )
    , handTip( nullptr )
    , progressBarControl( nullptr )
    , progressClipping( nullptr )
    , maxProgress( 0 )
    , pauseSpriteBtn( nullptr )
{
    selfId = screen_id::HUD;
}

//---------------------------------------------------------------------------------------------------------------------------------
HudScreen::~HudScreen()
{
    
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HudScreen::init()
{
    //super init first
    if( !IScreen::init() )
        return false;
    
    this->initControls();

    
    if( DEBUG_HUD )
    {
        //add label "zoom"
        this->zoomLabel = Label::createWithTTF( "1.0", FONT_PATH, 40 );
        this->zoomLabel->setColor( Color3B(175, 175, 255) );
        this->zoomLabel->setPosition( Vec2(100, FRAME_HEIGHT - 200) );
        this->addChild(this->zoomLabel, 1);
        //
        
    
        Menu* menu = Menu::create();
        menu->setPosition( Vec2::ZERO );
        this->addChild(menu, 10);
        
        
        //add "zoom inc" button
        auto textLabelZoomInc = Label::createWithTTF( "<< + >>", FONT_PATH, 40 );
        textLabelZoomInc->setColor( Color3B(255, 255, 255) );
        
        MenuItemLabel*  menuLabelZoomInc = MenuItemLabel::create(textLabelZoomInc, CC_CALLBACK_1( HudScreen::menuCallbackZoomInc, this ) );
        menuLabelZoomInc->setPosition( Vec2(this->zoomLabel->getPositionX(), this->zoomLabel->getPositionY() + 100) );
        menuLabelZoomInc->setAnchorPoint( Vec2(0.5, 0.5) );
        
        menu->addChild(menuLabelZoomInc, 0);
        //
        
        //add "zoom dec" button
        auto textLabelZoomDec = Label::createWithTTF( "<< - >>", FONT_PATH, 40 );
        textLabelZoomDec->setColor( Color3B(255, 255, 255) );
        
        MenuItemLabel*  menuLabelZoomDec = MenuItemLabel::create(textLabelZoomDec, CC_CALLBACK_1( HudScreen::menuCallbackZoomDec, this ) );
        menuLabelZoomDec->setPosition( Vec2( this->zoomLabel->getPositionX(), this->zoomLabel->getPositionY() - 100 ) );
        menuLabelZoomDec->setAnchorPoint( Vec2( 0.5, 0.5 ) );
        
        menu->addChild( menuLabelZoomDec, 0 );
        //
    
        //"God mode" switcher
        this->textLabelGMod = Label::createWithTTF( "Полет", FONT_PATH, 40 );
        if(!MANAGER_Memory->getTempData()->gMod)
            this->textLabelGMod->setColor( Color3B( 255, 255, 255 ) );
        else
            this->textLabelGMod->setColor( Color3B( 255, 0, 0 ) );
        
        MenuItemLabel*  menuLabelGMod = MenuItemLabel::create( this->textLabelGMod, CC_CALLBACK_1( HudScreen::menuCallbackGMod, this ) );
        menuLabelGMod->setPosition( Vec2( this->zoomLabel->getPositionX(), this->zoomLabel->getPositionY() + 150 ) );
        menuLabelGMod->setAnchorPoint( Vec2( 0.5, 0.5 ) );
        
        menu->addChild( menuLabelGMod, 0 );
        //
        
        //"Debug grid" switcher
        this->textLabelDebugDraw = Label::createWithTTF( "Сетка", FONT_PATH, 40 );
        if(!MANAGER_Memory->getTempData()->debugDraw)
            this->textLabelDebugDraw->setColor( Color3B( 255, 255, 255 ) );
        else
            this->textLabelDebugDraw->setColor( Color3B( 255, 0, 0 ) );
        
        MenuItemLabel*  menuLabelDebugDraw = MenuItemLabel::create( this->textLabelDebugDraw, CC_CALLBACK_1( HudScreen::menuCallbackDebugDraw, this ) );
        menuLabelDebugDraw->setPosition( Vec2( this->zoomLabel->getPositionX() + 150, this->zoomLabel->getPositionY() + 150 ) );
        menuLabelDebugDraw->setAnchorPoint( Vec2( 0.5, 0.5 ) );
        
        menu->addChild( menuLabelDebugDraw, 0 );
        //
    }
    
    IScreen::initTouches( Touch::DispatchMode::ALL_AT_ONCE );
     
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::initControls()
{
    m_pControlsMenu = CCMenu::create();
    m_pControlsMenu->setPosition( Vec2(0, 0) );
    this->addChild( m_pControlsMenu );
    
    //sticker
    m_pStickerBg = CCSprite::create("hud/sticker_bg.png");
    m_pStickerBg->setPosition( Vec2(140, 140) );
    addChild( m_pStickerBg );
    
    m_pStickCenterNode = CCNode::create();
    m_pStickCenterNode->setPosition(m_pStickerBg->getContentSize() / 2);
    m_pStickerBg->addChild( m_pStickCenterNode );
    
    m_pStick = CCSprite::create("hud/sticker.png");
    m_pStick->setPosition( Vec2::ZERO );
    m_pStickCenterNode->addChild( m_pStick );
    
    //jump selected btn
    jumpItemUnselected = CCSprite::create("hud/jump.png");
    jumpItemUnselected->setPosition( Vec2(925, 100));
    this->addChild( jumpItemUnselected );
    
    //jump unselected btn
    jumpItemSelected = CCSprite::create("hud/jump_down.png");
    jumpItemSelected->setPosition(Vec2(925, 100));
    jumpItemSelected->setVisible(false);
    this->addChild( jumpItemSelected );
    
    //shot btn
    shotItem = MenuItemImage::create("hud/attack.png", "hud/attack_down.png", CC_CALLBACK_1( HudScreen::onShotClick, this ) );
    shotItem->setPosition(Vec2(925, 240));
    m_pControlsMenu->addChild( shotItem );
    
    //pause btn
    MenuItemImage* pauseItem = MenuItemImage::create("hud/pause.png", "hud/pause.png", CC_CALLBACK_1( HudScreen::onPauseClick, this ) );
    pauseItem->setPosition(Vec2(925, 700));
    pauseItem->setScale(1.5);
    //m_pControlsMenu->addChild( pauseItem );
    
    this->pauseSpriteBtn = Sprite::create( "hud/pause.png" );
    this->pauseSpriteBtn->setPosition( Vec2( 925, 700 ) );
    this->pauseSpriteBtn->setScale( 1.5 );
    this->addChild( this->pauseSpriteBtn );
    
    //Background count shoot
    Sprite *countShotItem = Sprite::create("hud/bg_countShoot.png");
    countShotItem->setPosition(Vec2(shotItem->getContentSize().width / 1.2, shotItem->getContentSize().height / 1.08));
    shotItem->addChild( countShotItem );
    
    char buf[255];
    sprintf(buf, "%d", MANAGER_Memory->getTempData()->playerFireballAmmo);
    m_labelCountShoot = Label::createWithTTF( buf, FONT_PATH, 23 );
    m_labelCountShoot->setColor( Color3B(183, 81, 19));
    m_labelCountShoot->setPosition( Vec2(countShotItem->getContentSize().width / 2, countShotItem->getContentSize().height / 3) );
    countShotItem->addChild(m_labelCountShoot);
    
    Sprite* imgNotes = CCSprite::create( "hud/bg_notes.png" );
    imgNotes->setPosition( Vec2( 80, 640 ) );
    this->addChild( imgNotes );
    
    Sprite* imgCloud = CCSprite::create( "hud/bg_cloud.png" );
    imgCloud->setPosition( Vec2( 90, 700 ) );
    this->addChild( imgCloud );
    
    Sprite* bgCountNotes = Sprite::create( "hud/bg_countNotes.png" );
    bgCountNotes->setPosition( Vec2( 60, 600 ) );
    this->addChild( bgCountNotes );
    
    Sprite* bgCountHP = Sprite::create( "hud/bg_countHP.png" );
    bgCountHP->setPosition( Vec2( 45, 695 ) );
    this->addChild( bgCountHP );

    sprintf(buf, "%d", MANAGER_Level->getCurrentLevel()->getLocalPlayerCoins() );
    m_labelCountNotes = Label::createWithTTF( buf, FONT_PATH, 23 );
    m_labelCountNotes->setColor( Color3B(183, 81, 19) );
    m_labelCountNotes->setPosition( Vec2( bgCountNotes->getContentSize().width / 3.38, bgCountNotes->getContentSize().height / 1.42 ) );
    bgCountNotes->addChild(m_labelCountNotes);

    sprintf(buf, "%s", " " );
    m_labelCountHP = Label::createWithTTF( buf, FONT_PATH, 23 );
    m_labelCountHP->setColor( Color3B(183, 81, 19));
    m_labelCountHP->setPosition( Vec2( bgCountHP->getContentSize().width / 2, bgCountHP->getContentSize().height / 3 ) );
    bgCountHP->addChild(m_labelCountHP);
    
    this->tableTimer = CCSprite::create( "hud/timer.png" );
    this->tableTimer->setPosition( Vec2( this->getContentSize().width / 2 , this->getContentSize().height - 35 ) );
    this->addChild( tableTimer );
    
    sprintf( buf, "%.2f", MANAGER_Memory->getTempData()->currentLevelTimer );
    m_labelLevelTimer = Label::createWithTTF( buf, FONT_PATH, 26 );
    m_labelLevelTimer->setColor( Color3B(255, 255, 255));
    m_labelLevelTimer->setPosition( Vec2( 100.0f, 45.0f ) );//ccp( this->getContentSize().width / 2 - 10 , this->getContentSize().height - 20 ) );
    this->tableTimer->addChild(m_labelLevelTimer, 1);
    
    if( DEBUG_HUD )
    {
        sprintf(buf, "Player coins: %d", MANAGER_Memory->getTempData()->playerCoinsIntoPoints);
        m_labelCoinsToPointes = Label::createWithTTF( buf, FONT_PATH, 18 );
        m_labelCoinsToPointes->setColor( Color3B(183, 81, 19) );
        m_labelCoinsToPointes->setPosition( Vec2( this->getContentSize().width / 2 , this->getContentSize().height - 40 ) );
        this->addChild(m_labelCoinsToPointes);
    }
    
    this->initMinigamesProgressBar();
}

//-----------------------------------------------------------------------------------------------------------------
void HudScreen::initMinigamesProgressBar()
{
    this->progressBarControl = Sprite::create( "textures/scenes/minigame/MiniGameControls/progressControl.png" );
    this->progressBarControl->setPosition( Vec2( this->getContentSize().width / 2 , this->getContentSize().height + 40 ) );
    this->addChild( this->progressBarControl );
    
    Sprite* backProgress = Sprite::create( "textures/scenes/minigame/MiniGameControls/backProgress.png" );
    backProgress->setPosition( Vec2( 144.0f, 37.0f ) );
    this->progressBarControl->addChild( backProgress );
    
    progressClipping = Clipping::create();
    progressClipping->setClipMode( clipping_mode::WORLD_ORIENTATION );
    progressClipping->setPosition( Vec2( backProgress->getPosition().x,
                                         backProgress->getPosition().y ) );
    
    const int progress_offset = 12;
    progressClipping->setClippingRect( backProgress->getPosition().x + backProgress->getContentSize().width + progress_offset,
                                      backProgress->getPosition().y + progressBarControl->getPosition().y - progressBarControl->getContentSize().height - progressBarControl->getContentSize().height,
                                      backProgress->getContentSize().width,
                                      backProgress->getContentSize().height * 2 );
    this->progressBarControl->addChild( progressClipping, 1 );
    
    this->maxProgress = backProgress->getContentSize().width;
    
    Layer* frontLayer = Layer::create();
    frontLayer->setPosition( Vec2(0, 0) );
    frontLayer->setContentSize( backProgress->getContentSize() );
    progressClipping->addChild(frontLayer);
    
    this->setProgressBarValue( 0 );
    
    Sprite* frontProgress = Sprite::create( "textures/scenes/minigame/MiniGameControls/frontProgress.png" );
    frontLayer->addChild( frontProgress );
}

//-----------------------------------------------------------------------------------------------------------------
void HudScreen::showTimer()
{
    MoveTo *move = MoveTo::create( 1, Vec2( this->getContentSize().width / 2 , this->getContentSize().height - 35 ) );
    this->tableTimer->runAction( move );
}

//-----------------------------------------------------------------------------------------------------------------
void HudScreen::hideTimer()
{
    MoveTo *move = CCMoveTo::create( 1, Vec2( this->getContentSize().width / 2 , this->getContentSize().height + 40 ) );
    this->tableTimer->runAction( move );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::setStickerState(bool _isActive)
{
    m_isStickActive = _isActive;
}

//---------------------------------------------------------------------------------------------------------------------------------
bool HudScreen::getStickerState() const
{
    return m_isStickActive;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::hideSticker()
{
    this->m_pStickerBg->setVisible(false);
    this->jumpItemUnselected->setVisible(false);
    this->jumpItemSelected->setVisible(false);
    this->shotItem->setVisible(false);
    this->isStickerHide = true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::showSticker()
{
    this->m_pStickerBg->setVisible(true);
    this->jumpItemUnselected->setVisible(true);
    this->jumpItemSelected->setVisible(false);
    this->shotItem->setVisible(true);
    this->isStickerHide = false;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::resetSticker()
{
    stickerTouch = NULL;
    m_isOnStickClick = false;
    m_pStick->setPosition( Vec2::ZERO );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::disableSticker( bool _state )
{
    this->m_stickerDisabler = _state;
    /* PORT
    this->m_pControlsMenu->setTouchEnabled( !_state );
    */
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::update(float _dt)
{
    char buf[255];
    
    if( DEBUG_HUD )
    {
        int minutes = MANAGER_Memory->getTempData()->currentLevelTimer / 60;
        int seconds = (int)MANAGER_Memory->getTempData()->currentLevelTimer % 60;
        
        sprintf( buf, ( ( seconds < 10) ? "%d:0%d" : "%d:%d" ) , minutes, seconds );
        if( m_labelLevelTimer )
            m_labelLevelTimer->setString( buf );
        
        sprintf(buf, "Player coins: %d", MANAGER_Memory->getTempData()->playerCoinsIntoPoints);
        if( m_labelCoinsToPointes )
            m_labelCoinsToPointes->setString( buf );
    }
    
    sprintf(buf, "%d", MANAGER_Memory->getTempData()->playerFireballAmmo);
    m_labelCountShoot->setString(buf);
    
    sprintf(buf, "%d", MANAGER_Level->getCurrentLevel()->getLocalPlayerCoins() );//MANAGER_Memory->getTempData()->playerCoins);
    m_labelCountNotes->setString(buf);
    
    int hp_counter = MANAGER_Memory->getTempData()->playerHP;
    if( hp_counter < 0 )
        hp_counter = 0;
    sprintf(buf, "%d", hp_counter );
    m_labelCountHP->setString(buf);
    
    
    if(GET_RUNNING_SCENE->getSceneId() == scene_id::GAME)
    {
        GameScene* pGameScene = dynamic_cast<GameScene*>(GET_RUNNING_SCENE);
        PhysDynamicPlayerObject* playerPhys = dynamic_cast<PhysDynamicPlayerObject*>( MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPhysic() );
    
        if (!m_isOnStickClick || !dynamic_cast<DialogScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::DIALOG))->isDialgEmpty())
        {
            if(playerPhys->getGameObject()->getLocation() != game_object_location::STREAM_WATER &&
               playerPhys->getGameObject()->getLocation() != game_object_location::ROPE)
                playerPhys->setMoveForce( 0.0f );
        }
        
        if( !this->isStickerHide &&
            dynamic_cast<DialogScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::DIALOG))->isDialgEmpty() )
        {
            if( playerPhys->getGameObject()->getLocation() != game_object_location::ROPE )
            {
                // tutorial hand hint switch from moving to jump
                if( MANAGER_Memory->getTempData()->getLevelState( "level_1_1_1_shown_hint_0" ) == 1 &&
                   ( m_pStick->getPosition().x < -STICKER_INTERNAL_RADIUS ||
                    m_pStick->getPosition().x > STICKER_INTERNAL_RADIUS ) )
                {
                    MANAGER_Memory->getTempData()->setLevelState( "level_1_1_1_shown_hint_0", 2 );
                    this->handTip->stop();
                    this->showHandHint( Vec2( 820, 100 ), 90, 2, 0.4f );
                }
                
                if( m_pStick->getPosition().x < -STICKER_INTERNAL_RADIUS )
                    MANAGER_Level->getCurrentLevel()->getPlayer()->moveLeft();
                else if( m_pStick->getPosition().x > STICKER_INTERNAL_RADIUS )
                    MANAGER_Level->getCurrentLevel()->getPlayer()->moveRight();
                else
                    MANAGER_Level->getCurrentLevel()->getPlayer()->stop();
            }
            else
            {
                MANAGER_Level->getCurrentLevel()->getPlayer()->stop();
                this->resetSticker();
            }
        }
        else
            MANAGER_Level->getCurrentLevel()->getPlayer()->stop();
    
        
        if( DEBUG_HUD )
        {
            //update zoom label
            GameCamera* camera = pGameScene->getGameWorld()->getGameCamera();
    
            char zoomStr[8];
            sprintf(zoomStr, "%.2f", camera->getDistance());
            if( this->zoomLabel )
                this->zoomLabel->setString(zoomStr);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onTouchesBegan( const vector<Touch*>& _touches, Event* _event )
{
    Touch* touch = *_touches.begin();
    
    if( m_isStickActive && this->m_pStickerBg->isVisible() && !m_stickerDisabler )
    {
        
        if( jumpItemUnselected->boundingBox().containsPoint(touch->getLocation()) )
        {
            MANAGER_Level->getCurrentLevel()->getPlayer()->jump();
            jumpItemSelected->setVisible(true);
        }
        
        if (m_pStickerBg->boundingBox().containsPoint(touch->getLocation()))
        {
            //starting timer on first move
            if( MANAGER_Level->getLevelTimerState() )
                MANAGER_Level->setFirstTouchHUD( true );
            //
            m_isOnStickClick = true;
            if(!stickerTouch)
                this->stickerTouch = touch;
            
            Vec2 origin = Vec2::ZERO;  //stick origin
            Vec2 point = m_pStickCenterNode->convertTouchToNodeSpace( touch );  //touch point
            
            //if touches goes beyond the sticker limits, set position to max limit, relatively current touches
            float circle = powf( point.x - origin.x, 2 ) + powf( point.y - origin.y, 2 );  //equation of a circle
            if( circle > powf( STICKER_EXTERNAL_RADIUS, 2 ) )
                point = ( point - origin ).getNormalized() * STICKER_EXTERNAL_RADIUS;
                
            if( m_isStickMoveX )
                m_pStick->setPositionX( point.x );
            if( m_isStickMoveY )
                m_pStick->setPositionY( point.y );
        }
        
        m_isStickMoveX = true;
        m_isStickMoveY = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onTouchesMoved( const vector<Touch*>& _touches, Event* _event )
{
    Touch* touch = *_touches.begin();
    
    if ( m_isOnStickClick && this->m_pStickerBg->isVisible() && touch->getLocation().x < m_pStickerBg->getContentSize().width )
    {
        Vec2 origin = Vec2::ZERO;  //stick origin
        Vec2 point = m_pStickCenterNode->convertTouchToNodeSpace( touch );  //touch point
        
        //if touches goes beyond the sticker limits, set position to max limit, relatively current touches
        float circle = powf( point.x - origin.x, 2 ) + powf( point.y - origin.y, 2 );  //equation of a circle
        if( circle > powf( STICKER_EXTERNAL_RADIUS, 2 ) )
            point = ( point - origin ).getNormalized() * STICKER_EXTERNAL_RADIUS;
        
        if( m_isStickMoveX )
            m_pStick->setPositionX( point.x );
        if( m_isStickMoveY )
            m_pStick->setPositionY( point.y );
    }
    else
    {
        //TEMP: GOD MODE:
        if( MANAGER_Memory->getTempData()->gMod )
        {
            //fast player moving per finger
            if( GET_RUNNING_SCENE->getSceneId() == scene_id::GAME )
            {
                Vec2 lPoint = dynamic_cast<GameScene*>( GET_RUNNING_SCENE )->getForegroundLayer()->convertTouchToNodeSpace( touch );
                if( !isStickerHide && !m_stickerDisabler )
                    movePlayerTo( lPoint / CELL_SIZE );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onTouchesEnded( const vector<Touch*>& _touches, Event* _event )
{
    jumpItemSelected->setVisible(false);
    
    for( auto it = _touches.begin(); it != _touches.end(); it++ )
    {
        if( *it == stickerTouch ) 
        {
            stickerTouch = nullptr;
            m_isOnStickClick = false;
            m_pStick->setPosition( Vec2::ZERO );
        }
        
        if( this->pauseSpriteBtn->boundingBox().containsPoint( (*it)->getLocation() ) )
        {
            IScreen* pScreen = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::PAUSE);
            if(pScreen)
                dynamic_cast<PauseScreen*>(pScreen)->show();
        }
    }

    
    //TEMP: logging
    if(dynamic_cast<GameScene*>(GET_RUNNING_SCENE))
    {
        Touch* touch = *_touches.begin();  // PORT
        Vec2 lPoint = dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->convertTouchToNodeSpace(touch);
        int squareX = (int)(lPoint.x / MANAGER_Level->getCurrentLevel()->getGameWorld()->getDimensionX() / CELL_SIZE);
        int squareY = (int)(lPoint.y / MANAGER_Level->getCurrentLevel()->getGameWorld()->getDimensionY() / CELL_SIZE);
        
        log("squareY: %d", squareY);
        log("squareX: %d", squareX);
        log("Local touch x: %f; y: %f", lPoint.x / CELL_SIZE - squareX * MANAGER_Level->getCurrentLevel()->getGameWorld()->getDimensionX(), lPoint.y / CELL_SIZE - squareY * MANAGER_Level->getCurrentLevel()->getGameWorld()->getDimensionY());
        log("Global touch x: %f; y: %f", lPoint.x / CELL_SIZE, lPoint.y / CELL_SIZE);
        
        
        const vector<GameObject*>& gameObjects = MANAGER_Level->getCurrentLevel()->getGameWorld()->getWorldMatrix()[ squareX ][ squareY ]->getGameObjects();
        for( auto it = gameObjects.begin(); it != gameObjects.end(); it++ )
        {
            if(((int)(*it)->getPosition().x + 2 > (int)(lPoint.x / CELL_SIZE) &&
                (int)(*it)->getPosition().x - 2 < (int)(lPoint.x / CELL_SIZE) &&
                (int)(*it)->getPosition().y + 2 > (int)(lPoint.y / CELL_SIZE) &&
                (int)(*it)->getPosition().y - 2 < (int)(lPoint.y / CELL_SIZE)))
                // - squareY * MANAGER_Level->getCurrentLevel()->getGameWorld()->getDimensionY())))
            {
                log("id = %d;", (*it)->getId());
                //log("obj x: %f; y: %f", (*it)->getPosition().x, (*it)->getPosition().y);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onTouchesCancelled( const vector<Touch*>& _touches, Event* _event )
{
    this->onTouchesEnded( _touches, _event );
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onJumpClick( Ref* _pSender )
{
    //starting timer on first jump
    if( MANAGER_Level->getLevelTimerState() )
        MANAGER_Level->setFirstTouchHUD( true );
    //
    MANAGER_Level->getCurrentLevel()->getPlayer()->jump();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onShotClick( Ref* _pSender )
{
    //starting timer on first shoot
    if( MANAGER_Level->getLevelTimerState() )
        MANAGER_Level->setFirstTouchHUD( true );
    //
    MANAGER_Level->getCurrentLevel()->getPlayer()->fire();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::onPauseClick( Ref* _pSender )
{
    IScreen* pScreen = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::PAUSE);
    if(pScreen)
        dynamic_cast<PauseScreen*>(pScreen)->show();
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::showHandHint( Vec2 _position, float _angle, float _duration, float _speed )
{
    this->handTip = TipHand::create( _angle, _duration, _speed );
    this->handTip->setPosition( _position );
    this->addChild( this->handTip );
    
    this->handTip->start();
}

//---------------------------------------------------------------------------------------------------------------------------------
//TEMP: for new camera test
//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::menuCallbackZoomInc( Ref* _pSender )
{
    GameCamera* camera = dynamic_cast<GameScene*>( this->getParent() )->getGameWorld()->getGameCamera();
    
    float zoom = camera->getDistance() + 0.05;
    camera->setDistance(zoom);
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::menuCallbackZoomDec( Ref* _pSender )
{
    GameCamera* camera = dynamic_cast<GameScene*>( this->getParent() )->getGameWorld()->getGameCamera();
    
    float zoom = camera->getDistance() - 0.05;
    if(zoom > 0)
        camera->setDistance(zoom);
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::menuCallbackGMod( cocos2d::Ref* _pSender )
{
    if(MANAGER_Memory->getTempData()->gMod)
        this->textLabelGMod->setColor( Color3B( 255, 255, 255 ) );
    else
        this->textLabelGMod->setColor( Color3B( 255, 0, 0 ) );
    MANAGER_Memory->getTempData()->gMod = !MANAGER_Memory->getTempData()->gMod;
}

//---------------------------------------------------------------------------------------------------------------------------------
void HudScreen::menuCallbackDebugDraw( cocos2d::Ref* _pSender )
{
    if(MANAGER_Memory->getTempData()->debugDraw)
        this->textLabelDebugDraw->setColor( Color3B( 255, 255, 255 ) );
    else
        this->textLabelDebugDraw->setColor( Color3B( 255, 0, 0 ) );
    MANAGER_Memory->getTempData()->debugDraw = !MANAGER_Memory->getTempData()->debugDraw;
}

//---------------------------------------------------------------------------------------------------------------------------------
/**TEST: Player model for camera test */
void HudScreen::movePlayerTo( cocos2d::Vec2 _dir )
{
        dynamic_cast<physic::PhysDynamicPlayerObject*>(MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPhysic())->setFreeFallSpeed(0);
        
        float dirX = _dir.x;
        float dirY = _dir.y;
        
        float curX = MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPosition().x;
        float curY = MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPosition().y;
        
        float newPosX = (dirX - curX) / 10;
        float newPosY = (dirY - curY) / 10;
        
        MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->setPosition( Vec2(MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPosition().x + newPosX,
                                                                                          MANAGER_Level->getCurrentLevel()->getPlayer()->getGameObject()->getPosition().y + newPosY) );
}

//------------------------------------------------------------------------------------------------------------------------------
void HudScreen::showProgressBar()
{
    MoveTo *move = CCMoveTo::create( 1, Vec2( this->getContentSize().width / 2 , this->getContentSize().height - 25 ) );
    this->progressBarControl->runAction( move );
}

//------------------------------------------------------------------------------------------------------------------------------
void HudScreen::hideProgressBar()
{
    MoveTo *move = CCMoveTo::create( 1, Vec2( this->getContentSize().width / 2 , this->getContentSize().height + 40 ) );
    this->progressBarControl->runAction( move );
}

//------------------------------------------------------------------------------------------------------------------------------
void HudScreen::setProgressBarValue( float _value )
{
    this->progressClipping->setClipWidth( maxProgress * _value / 100 );
}

//---------------------------------------------------------------------------------------------------------------------------------