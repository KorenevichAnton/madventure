//
//  SleepingGuards.cpp
//  MusicalAdventure
//
//  Created by madmin on 5/15/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "LevelManager.h"
#include "L1_3_Quest_1_MGScreen.h"
#include "MainMenuScene.h"

using namespace cocos2d;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
L1_3_Quest_1_MGScreen* L1_3_Quest_1_MGScreen::create()
{
    L1_3_Quest_1_MGScreen* pRet = new L1_3_Quest_1_MGScreen();
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
L1_3_Quest_1_MGScreen::L1_3_Quest_1_MGScreen():
def1(0),
def2(0),
def3(0),
countFail(0),
maxCountFail(1),
tempNum(NULL),
aSourceCurSound( nullptr ),
buttonMenu(nullptr),
curTouch(nullptr),
isWin( false ),
objGuard1( nullptr ),
objGuard2( nullptr ),
objGuard3( nullptr ),
objWokeUpGuard1( nullptr ),
objWokeUpGuard2( nullptr ),
objWokeUpGuard3( nullptr )
{
    selfId = screen_id::MG_L1_3_QUEST_1;
    
    this->numbers.clear();
    this->playBtns.clear();
    this->activeDefs.clear();
}

//---------------------------------------------------------------------------------------------------------------------------------
L1_3_Quest_1_MGScreen::~L1_3_Quest_1_MGScreen()
{

}

//---------------------------------------------------------------------------------------------------------------------------------
bool L1_3_Quest_1_MGScreen::init()
{
    //super init first
    if(!IMiniGameScreen::init())
        return false;
    
    //Init touches
    IMiniGameScreen::initTouches( Touch::DispatchMode::ONE_BY_ONE );
    
    this->_touchListener->setEnabled( false );
    //
    
    
    this->minigameControl = MiniGameControls::create();
    this->addChild(this->minigameControl, 100000);
    this->minigameControl->configOnStartButton([this](){ this->onStartGame(); });
    this->minigameControl->configOnVictoryButton([this](){ this->onVictory() ; });
    this->minigameControl->setAnchorPoint(Vec2::ZERO);
    this->minigameControl->setPosition( Vec2(-150, 0) );
    
    this->typeMusicGuard[0] = Label::createWithTTF( L10N_STRING(3101).c_str(), FONT_PATH, 40);
    this->typeMusicGuard[0]->setColor( FONT_COLOR_VALUE );
    this->typeMusicGuard[0]->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    this->typeMusicGuard[0]->setPosition( Vec2(60, 610));
    this->typeMusicGuard[0]->setVisible( false );
    this->addChild( this->typeMusicGuard[0] );
    
    this->typeMusicGuard[1] = Label::createWithTTF( L10N_STRING(3102).c_str(), FONT_PATH, 40);
    this->typeMusicGuard[1]->setColor( FONT_COLOR_VALUE );
    this->typeMusicGuard[1]->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    this->typeMusicGuard[1]->setPosition( Vec2(350, 650));
    this->typeMusicGuard[1]->setVisible( false );
    this->addChild(this->typeMusicGuard[1]);

    this->typeMusicGuard[2] = Label::createWithTTF( L10N_STRING(3103).c_str(), FONT_PATH, 40);
    this->typeMusicGuard[2]->setColor( FONT_COLOR_VALUE );
    this->typeMusicGuard[2]->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    this->typeMusicGuard[2]->setPosition( Vec2(660, 590));
    this->typeMusicGuard[2]->setVisible( false );
    this->addChild(this->typeMusicGuard[2]);
    
    //numbers of melodies
    Label* numberOne = Label::createWithTTF( L10N_STRING(3104).c_str(), FONT_PATH, 68);
    numberOne->setColor( FONT_COLOR_VALUE );
    numberOne->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberOne->setPosition( Vec2(51, 300) );
    numberOne->setVisible( false );
    this->addChild( numberOne, 3000 );
    
    Label* numberTwo = Label::createWithTTF( L10N_STRING(3105).c_str(), FONT_PATH, 68);
    numberTwo->setColor( FONT_COLOR_VALUE );
    numberTwo->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberTwo->setPosition( Vec2(351, 300) );
    numberTwo->setVisible( false );
    this->addChild( numberTwo, 3000 );
    
    Label* numberThree = Label::createWithTTF( L10N_STRING(3106).c_str(), FONT_PATH, 68 );
    numberThree->setColor( FONT_COLOR_VALUE );
    numberThree->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberThree->setPosition( Vec2(651, 300) );
    numberThree->setVisible( false );
    this->addChild( numberThree, 3000 );
    
    Label* numberFour = Label::createWithTTF( L10N_STRING(3107).c_str(), FONT_PATH, 68 );
    numberFour->setColor( FONT_COLOR_VALUE );
    numberFour->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberFour->setPosition( Vec2(51, 130) );
    numberFour->setVisible( false );
    this->addChild( numberFour, 3000 );
    
    Label* numberFive = Label::createWithTTF( L10N_STRING(3108).c_str(), FONT_PATH, 68 );
    numberFive->setColor( FONT_COLOR_VALUE );
    numberFive->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberFive->setPosition( Vec2(351, 130) );
    numberFive->setVisible( false );
    this->addChild( numberFive, 3000 );
    
    Label* numberSix = Label::createWithTTF( L10N_STRING(3109).c_str(), FONT_PATH, 68 );
    numberSix->setColor( FONT_COLOR_VALUE );
    numberSix->setAnchorPoint(  Vec2( 0.5f, 0.0f) );
    numberSix->setPosition( Vec2(651, 130));
    numberSix->setVisible(false);
    this->addChild(numberSix, 3000);
    
    this->numbers.push_back( numberOne );
    this->numbers.push_back( numberTwo );
    this->numbers.push_back( numberThree );
    this->numbers.push_back( numberFour );
    this->numbers.push_back( numberFive );
    this->numbers.push_back( numberSix );
    //
    
    //
    this->buttonMenu = Menu::create();
    this->buttonMenu->setPosition( Vec2(0, 0) );
    this->buttonMenu->setVisible( false );
    this->addChild( this->buttonMenu );
    
    MenuItemSprite* playBtn1 = MenuItemSprite::create( createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay1, this ) );
    playBtn1->setAnchorPoint( Vec2(0, 0));
    playBtn1->setPosition( Vec2(5, 200));
    this->buttonMenu->addChild(playBtn1);
    
    MenuItemSprite* playBtn2 = MenuItemSprite::create(createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay2, this ) );
    playBtn2->setAnchorPoint( Vec2(0, 0));
    playBtn2->setPosition( Vec2(305, 200));
    this->buttonMenu->addChild(playBtn2);
    
    MenuItemSprite* playBtn3 = MenuItemSprite::create(createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay3, this ) );
    playBtn3->setAnchorPoint( Vec2(0, 0));
    playBtn3->setPosition( Vec2(605, 200));
    this->buttonMenu->addChild(playBtn3);
    
    MenuItemSprite* playBtn4 = MenuItemSprite::create(createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay4, this ) );
    playBtn4->setAnchorPoint( Vec2(0, 0));
    playBtn4->setPosition( Vec2(5, 30));
    this->buttonMenu->addChild(playBtn4);
    
    MenuItemSprite* playBtn5 = MenuItemSprite::create(createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay5, this ) );
    playBtn5->setAnchorPoint( Vec2(0, 0));
    playBtn5->setPosition( Vec2(305, 30));
    this->buttonMenu->addChild(playBtn5);
    
    MenuItemSprite* playBtn6 = MenuItemSprite::create(createViewStateOfButton("btn/speaker.png"), createViewStateOfButton("btn/speaker_down.png"), CC_CALLBACK_1( L1_3_Quest_1_MGScreen::onBtnPlay6, this ) );
    playBtn6->setAnchorPoint( Vec2(0, 0));
    playBtn6->setPosition( Vec2(605, 30));
    this->buttonMenu->addChild(playBtn6);
    //
    this->playBtns.push_back(playBtn1);
    this->playBtns.push_back(playBtn2);
    this->playBtns.push_back(playBtn3);
    this->playBtns.push_back(playBtn4);
    this->playBtns.push_back(playBtn5);
    this->playBtns.push_back(playBtn6);
    //
    //TEMP FUCK
    this->def1 = 0;
    this->def2 = 0;
    this->def3 = 0;
    //
    
    initGuards();
    
    this->_touchListener->setEnabled(false);
    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::update(float _dt)
{
    if(this->_touchListener->isEnabled())
    {
        if( this->isWin )
        {
            if( vAnimGuards.size() > 0 )
            {
                graphic::GraphicAnimationObject* anim = *vAnimGuards.begin();
            
                if( anim->getState() == graphic::sequence_mob::SM_JOY && anim->getCurAnimFrameIndex() >= anim->getLastAnimIndex() - 1 )
                    this->onVictory();
            }
            else
                this->onVictory();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
bool L1_3_Quest_1_MGScreen::onTouchBegan( cocos2d::Touch *_touch, cocos2d::Event *_event )
{
    if( !this->curTouch )
    {
        if ( _touch )
        {
            this->curTouch = _touch;
            
            int i = 0;
            for( auto iter = this->numbers.begin(); iter < this->numbers.end(); iter++ )
            {
                if((*iter)->boundingBox().containsPoint((*iter)->getParent()->convertToNodeSpace(this->curTouch->getLocation())))
                {
                    if(!this->playBtns[i]->isVisible())
                        break;
                    this->oldPoint = (*iter)->getPosition();
                    this->tempNum = (*iter);
                    
                     Vec2 convertPointInNote = this->tempNum->convertToNodeSpace(this->curTouch->getLocation());
                     Vec2 newAnchornPoint = Vec2( convertPointInNote.x / this->tempNum->getWidth(), convertPointInNote.y / this->tempNum->getHeight() );
                     Vec2 posOldAnchPoint = CC_VEC2_MULT( this->tempNum->getAnchorPoint(), Vec2( this->tempNum->getContentSize() ) );
                    this->tempNum->setPosition( Vec2(this->tempNum->getPositionX() - posOldAnchPoint.x, this->tempNum->getPositionY() - posOldAnchPoint.y));
                    
                    this->tempNum->setAnchorPoint(newAnchornPoint);
                    this->tempNum->setPosition( Vec2( this->tempNum->getPositionX() + newAnchornPoint.x * this->tempNum->getWidth(), this->tempNum->getPositionY() + newAnchornPoint.y * this->tempNum->getHeight() ));
                    break;
                }
                i++;
            }
        }
    }
    
    return true;
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onTouchEnded( cocos2d::Touch *_touch, cocos2d::Event *_event )
{
        if( this->curTouch == _touch )
        {
            if( this->curTouch )
            {
                if(this->tempNum)
                {
                    bool isWin = false;
                    
                    bool isCorrect = false;
                    
                    bool onActiveDefs = false;
                    
                    ILevel* curLvl = MANAGER_Level->getCurrentLevel();
                    
                    if( curLvl )
                    {
                        if( vAnimGuards.size() > 0 )
                        {
                            graphic::GraphicAnimationObject* animGuard1 = (*vAnimGuards.begin());
                            graphic::GraphicAnimationObject* animGuard2 = (*(vAnimGuards.begin() + 1));
                            graphic::GraphicAnimationObject* animGuard3 = (*(vAnimGuards.end() - 1));
                            
                            if( animGuard1 && animGuard2 && animGuard3 )
                            {
                                graphic::sequence_mob sqmDef0 = animGuard1->getState();
                                graphic::sequence_mob sqmDef1 = animGuard2->getState();
                                graphic::sequence_mob sqmDef2 = animGuard3->getState();
                                
                                if( this->activeDefs[0]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                                {
                                    onActiveDefs = true;
                                    if( this->tempNum == this->numbers[0] && this->playBtns[0]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[0]->setVisible( false );
                                        isCorrect = true;
                                    }else if( this->tempNum == this->numbers[2] && this->playBtns[2]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[2]->setVisible( false );
                                        isCorrect = true;
                                    }
                                    if( isCorrect )
                                    {
                                        def1++;
                                        
                                        if( def1 > 1 && sqmDef0 != graphic::sequence_mob::SM_RUN )
                                        {
                                            this->typeMusicGuard[0]->setVisible( false );
                                            animGuard1->setStateAnim(graphic::sequence_mob::SM_RUN, false);
                                        }
                                    }
                                }
                                
                                if( this->activeDefs[1]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                                {
                                    onActiveDefs = true;
                                    if( this->tempNum == this->numbers[1] && this->playBtns[1]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[1]->setVisible( false );
                                        isCorrect = true;
                                    }else if( this->tempNum == this->numbers[4] && this->playBtns[4]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[4]->setVisible( false );
                                        isCorrect = true;
                                    }
                                    if( isCorrect )
                                    {
                                        def2++;
                                        
                                        if( def2 > 1 && sqmDef1 != graphic::sequence_mob::SM_RUN )
                                        {
                                            this->typeMusicGuard[1]->setVisible( false );
                                            animGuard2->setStateAnim(graphic::sequence_mob::SM_RUN, false);
                                        }
                                    }
                                }
                                //
                                if( this->activeDefs[2]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                                {
                                    onActiveDefs = true;
                                    if( this->tempNum == this->numbers[3] && this->playBtns[3]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[3]->setVisible(false);
                                        isCorrect = true;
                                    }else if( this->tempNum == this->numbers[5] && this->playBtns[5]->isVisible() )
                                    {
                                        this->tempNum->setVisible( false );
                                        this->playBtns[5]->setVisible(false);
                                        isCorrect = true;
                                    }
                                    if( isCorrect )
                                    {
                                        def3++;
                                        
                                        if( def3 > 1 && sqmDef2 != graphic::sequence_mob::SM_RUN )
                                        {
                                            this->typeMusicGuard[2]->setVisible( false );
                                            animGuard3->setStateAnim(graphic::sequence_mob::SM_RUN, false);
                                        }
                                    }
                                }
                                
                                if(def1 > 1 && def2 > 1 && def3 > 1)
                                {
                                    isWin = true;
                                    this->isWin = true;
                                    animGuard1->setStateAnim(graphic::sequence_mob::SM_JOY, false);
                                    animGuard2->setStateAnim(graphic::sequence_mob::SM_JOY, false);
                                    animGuard3->setStateAnim(graphic::sequence_mob::SM_JOY, false);
                                    
                                    this->tempNum->setPosition(this->oldPoint);
                                    this->tempNum->setAnchorPoint( Vec2(0.5, 0.0f));
                                    this->oldPoint = Vec2::ZERO;
                                    this->tempNum = NULL;
                                }
                                
                                if(isCorrect == false)
                                {
                                    if(onActiveDefs == true)
                                    {
                                        this->minigameControl->showWrong();
                                        if(this->countFail >= this->maxCountFail)
                                            restart();
                                        this->countFail++;
                                    }
                                }
                                else
                                    this->minigameControl->showCorrect();
                                
                                if(!isWin)
                                {
                                    //this->minigameControl->showWrong();
                                    //MANAGER_Audio->createStaticSource("audio/minigame/L1_3_Quest_1_MGScreen/defeat");
                                    this->tempNum->setPosition(this->oldPoint);
                                    this->tempNum->setAnchorPoint( Vec2(0.5, 0.0f));
                                    this->oldPoint = Vec2::ZERO;
                                    this->tempNum = NULL;
                                }
                            }
                        }
                        else
                        {
                            ILevel* curLvl = MANAGER_Level->getCurrentLevel();
                            if( this->activeDefs[0]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                            {
                                onActiveDefs = true;
                                if( this->tempNum == this->numbers[0] && this->playBtns[0]->isVisible() )
                                {
                                    this->playBtns[0]->setVisible( false );
                                    isCorrect = true;
                                }else if( this->tempNum == this->numbers[2] && this->playBtns[2]->isVisible() )
                                {
                                    this->playBtns[2]->setVisible( false );
                                    isCorrect = true;
                                }
                                if( isCorrect )
                                {
                                    def1++;
                                    
                                    if( def1 > 1  )
                                    {
                                        this->typeMusicGuard[0]->setVisible( false );
                                        objGuard1->setActivity( false );
                                        curLvl->showGameObject( objWokeUpGuard1->getId() );
                                    }
                                }
                            }
                            
                            if( this->activeDefs[1]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                            {
                                onActiveDefs = true;
                                if( this->tempNum == this->numbers[1] && this->playBtns[1]->isVisible() )
                                {
                                    this->playBtns[1]->setVisible( false );
                                    isCorrect = true;
                                }else if( this->tempNum == this->numbers[4] && this->playBtns[4]->isVisible() )
                                {
                                    this->playBtns[4]->setVisible( false );
                                    isCorrect = true;
                                }
                                if( isCorrect )
                                {
                                    def2++;
                                    
                                    if( def2 > 1 )
                                    {
                                        this->typeMusicGuard[1]->setVisible( false );
                                        objGuard2->setActivity( false );
                                        objWokeUpGuard2->setActivity( true );
                                    }
                                }
                            }
                            //
                            if( this->activeDefs[2]->boundingBox().containsPoint(this->tempNum->getPosition()) )
                            {
                                onActiveDefs = true;
                                if( this->tempNum == this->numbers[3] && this->playBtns[3]->isVisible() )
                                {
                                    this->playBtns[3]->setVisible(false);
                                    isCorrect = true;
                                }else if( this->tempNum == this->numbers[5] && this->playBtns[5]->isVisible() )
                                {
                                    this->playBtns[5]->setVisible(false);
                                    isCorrect = true;
                                }
                                if( isCorrect )
                                {
                                    def3++;
                                    
                                    if( def3 > 1 )
                                    {
                                        this->typeMusicGuard[2]->setVisible( false );
                                        objGuard3->setActivity( false );
                                        objWokeUpGuard3->setActivity( true );
                                    }
                                }
                            }
                            
                            if(def1 > 1 && def2 > 1 && def3 > 1)
                            {
                                isWin = true;
                                this->isWin = true;
                                
                                this->tempNum->setPosition(this->oldPoint);
                                this->tempNum->setAnchorPoint( Vec2(0.5, 0.0f));
                                this->oldPoint = Vec2::ZERO;
                                this->tempNum = NULL;
                            }
                            
                            if(isCorrect == false)
                            {
                                if(onActiveDefs == true)
                                {
                                    this->minigameControl->showWrong();
                                    if(this->countFail >= this->maxCountFail)
                                        restart();
                                    this->countFail++;
                                }
                            }
                            else
                                this->minigameControl->showCorrect();
                            
                            if(!isWin)
                            {
                                //this->minigameControl->showWrong();
                                //MANAGER_Audio->createStaticSource("audio/minigame/L1_3_Quest_1_MGScreen/defeat");
                                this->tempNum->setPosition(this->oldPoint);
                                this->tempNum->setAnchorPoint( Vec2(0.5, 0.0f));
                                this->oldPoint = Vec2::ZERO;
                                this->tempNum = NULL;
                            }
                        }
                    }
                }
            }
            this->curTouch = nullptr;
        }
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onTouchMoved( cocos2d::Touch *_touch, cocos2d::Event *_event )
{
    if( this->curTouch )
    {
        if(this->tempNum)
        {
             Vec2 posMouse = this->tempNum->getParent()->convertToNodeSpace(this->curTouch->getLocation());
             Vec2 cp =  Vec2(this->tempNum->getPositionX() + (posMouse.x - this->tempNum->getPositionX()), this->tempNum->getPositionY() + (posMouse.y - this->tempNum->getPositionY()));
            this->tempNum->setPosition(cp);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onTouchCancelled( cocos2d::Touch *_touch, cocos2d::Event *_event )
{
    this->onTouchEnded( _touch, _event );
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::setWinState(bool _isWin)
{
    this->isWin = _isWin;
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onStartGame()
{
    IMiniGameScreen::onStartGame();
    this->buttonMenu->setVisible(true);
    
    this->showHintHand();
    
    for (int i = 0; i < 3; i++)
    {
        this->typeMusicGuard[i]->setVisible( true );
    }
    
    for (int i = 0; i < this->numbers.size(); i++)
    {
        this->numbers[i]->setVisible(true);
        this->playBtns[i]->setVisible(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onVictory()
{
    IMiniGameScreen::onVictory();
    
    if( vAnimGuards.size() > 0 )
    {
        (*vAnimGuards.begin())->setState( graphic::sequence_mob::SM_JOY, false );
        (*(vAnimGuards.begin() + 1))->setState( graphic::sequence_mob::SM_JOY, false );
        (*(vAnimGuards.end() - 1))->setState( graphic::sequence_mob::SM_JOY, false );
    }
    else
    {
        this->objGuard1->setActivity( false );
        this->objGuard2->setActivity( false );
        this->objGuard3->setActivity( false );
    }
    
    this->_touchListener->setEnabled(false);
    
    for(auto i = 0; i < 3; i++)
    {
        this->typeMusicGuard[i]->setVisible(false);
    }
    
    if( this->aSourceCurSound )
        this->aSourceCurSound->stop();
    MANAGER_Level->getCurrentLevel()->onMinigameEnd(true, screen_id::MG_L1_3_QUEST_1);
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onDefeat()
{
    IMiniGameScreen::onDefeat();
    this->_touchListener->setEnabled(false);
    for(auto i = 0; i < 3; i++)
    {
        this->typeMusicGuard[i]->setVisible(false);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::initGuards()
{
    //Defenders
    Sprite* activeDefender1 = Sprite::create( "textures/scenes/minigame/L1_3_Quest_1_MGScreen/guard1.png" );
    activeDefender1->setAnchorPoint( Vec2(0, 0));
    activeDefender1->setPosition( Vec2(0, 400));
    this->addChild( activeDefender1, -1 );
    
    Sprite* activeDefender2 = Sprite::create( "textures/scenes/minigame/L1_3_Quest_1_MGScreen/guard2.png" );
    activeDefender2->setAnchorPoint( Vec2(0, 0));
    activeDefender2->setPosition( Vec2(300, 400));
    this->addChild( activeDefender2, - 1 );
    
    Sprite* activeDefender3 = Sprite::create( "textures/scenes/minigame/L1_3_Quest_1_MGScreen/guard3.png" );
    activeDefender3->setAnchorPoint( Vec2(0, 0));
    activeDefender3->setPosition( Vec2(550, 390));
    this->addChild( activeDefender3, - 1 );
    
    //
    this->activeDefs.push_back(activeDefender1);
    this->activeDefs.push_back(activeDefender2);
    this->activeDefs.push_back(activeDefender3);
    //
    
    ILevel* curLvl = MANAGER_Level->getCurrentLevel();
    
    if( curLvl )
    {
        //RAM - uploading
        if( APPLICATION->getDeviceModel() == "iPad" ||
            APPLICATION->getDeviceModel() == "iPad 2" )
        {
            objGuard1 = curLvl->getGameObjectById(1605);
            objGuard2 = curLvl->getGameObjectById(1606);
            objGuard3 = curLvl->getGameObjectById(1607);
            //
            objWokeUpGuard1 = curLvl->getGameObjectById( 1600 );
            objWokeUpGuard2 = curLvl->getGameObjectById( 1601 );
            objWokeUpGuard3 = curLvl->getGameObjectById( 1602 );
        }
        else
        {
            objGuard1 = curLvl->getGameObjectById(1500);
            objGuard2 = curLvl->getGameObjectById(1501);
            objGuard3 = curLvl->getGameObjectById(1502);
        
            if( objGuard1 && objGuard2 && objGuard3 )
            {
                graphic::GraphicAnimationObject* animGuard1 = dynamic_cast<graphic::GraphicAnimationObject*>( objGuard1->getGraphic() );
                graphic::GraphicAnimationObject* animGuard2 = dynamic_cast<graphic::GraphicAnimationObject*>( objGuard2->getGraphic() );
                graphic::GraphicAnimationObject* animGuard3 = dynamic_cast<graphic::GraphicAnimationObject*>( objGuard3->getGraphic() );
                
                if( animGuard1 && animGuard2 && animGuard3 )
                {
                    vAnimGuards.push_back( animGuard1 );
                    vAnimGuards.push_back( animGuard2 );
                    vAnimGuards.push_back( animGuard3 );
                }
            }
        }
    }
    
    if( this->isWin && vAnimGuards.size() > 0)
    {
        (*vAnimGuards.begin())->setState( graphic::sequence_mob::SM_JOY, false );
        (*(vAnimGuards.begin() + 1))->setState( graphic::sequence_mob::SM_JOY, false );
        (*(vAnimGuards.end() - 1))->setState( graphic::sequence_mob::SM_JOY, false );
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay1(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/skripki.wav";
    this->playCurSound();
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay2(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/akordeony.wav";
    playCurSound();
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay3(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/saxofony.wav";
    playCurSound();
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay4(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/baraban-skr-cl.wav";
    playCurSound();
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay5(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/fno.wav";
    playCurSound();
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onBtnPlay6(Ref* pSender)
{
    this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/truba-baraban.wav";
    playCurSound();
}
/*
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay1(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/akordeony.wav";
 playCurSound();
 }
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay2(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/skripki.wav";
 playCurSound();
 }
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay3(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/saxofony.wav";
 playCurSound();
 }
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay4(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/fno.wav";
 playCurSound();
 }
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay5(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/baraban-skr-cl.wav";
 playCurSound();
 }
 //---------------------------------------------------------------------------------------------------------------------------------
 void L1_3_Quest_1_MGScreen::onBtnPlay6(Ref* pSender)
 {
 this->curSound = "audio/minigame/L1_3_Quest_1_MGScreen/truba-baraban.wav";
 playCurSound();
 }
 */
void L1_3_Quest_1_MGScreen::playCurSound()
{
    if( this->curSound != "" )
    {
        if( this->aSourceCurSound )
            this->aSourceCurSound->stop();
        
        this->aSourceCurSound = MANAGER_Audio->createStaticSource();
        if( this->aSourceCurSound )
        {
            if( this->aSourceCurSound->loadAudioFile( this->curSound.c_str() ) )
            {
                this->aSourceCurSound->loadSettings( this->curSound.c_str() );
                this->aSourceCurSound->setAssociate( AudioSourceAssociate::SOUND );
                this->aSourceCurSound->play();
            }
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::showMinigameInterface()
{
    /*for(int i = 0 ; i < 6 ; i++)
    {
        this->numbers[i]->setVisible(true);
        this->playBtns[i]->setVisible(true);
    }*/
    this->minigameControl->setVisible(true);
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::hideMinigameInterface()
{
    for(int i = 0 ; i < this->playBtns.size(); i++)
    {
        this->numbers[i]->setVisible(false);
        this->playBtns[i]->setVisible(false);
    }
    this->minigameControl->setVisible(false);
}
//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::restart()
{
    this->countFail = 0;
    def1 = 0;
    def2 = 0;
    def3 = 0;
    
    //restart numbers
    for( int i = 0 ; i < this->numbers.size() ; i++ )
        this->numbers[i]->setVisible( true );
    
    //reset player buttons for numbers
    for (auto it = this->playBtns.begin(); it < this->playBtns.end(); it++)
        (*it)->setVisible(true);
    
    //restart labels about tracks
    for( int i = 0 ; i < 3 ; i++ )
        this->typeMusicGuard[ i ]->setVisible( true );
    
    ILevel* curLvl = MANAGER_Level->getCurrentLevel();;
    
    if( curLvl )
    {
        if( vAnimGuards.size() > 0 )
        {
            (*vAnimGuards.begin())->setState( graphic::sequence_mob::SM_WAIT );
            (*(vAnimGuards.begin() + 1))->setState( graphic::sequence_mob::SM_WAIT );
            (*(vAnimGuards.end() - 1))->setState( graphic::sequence_mob::SM_WAIT );
        }
        else
        {
            ILevel* curLvl = MANAGER_Level->getCurrentLevel();
            
            curLvl->hideGameObject( this->objWokeUpGuard1->getId() );
            curLvl->hideGameObject( this->objWokeUpGuard2->getId() );
            curLvl->hideGameObject( this->objWokeUpGuard3->getId() );
            //
            curLvl->showGameObject( this->objGuard1->getId() );
            curLvl->showGameObject( this->objGuard2->getId() );
            curLvl->showGameObject( this->objGuard3->getId() );
                                   
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::showHintHand()
{
    // Create fly hand
    Sprite* hand = Sprite::create( "textures/scenes/minigame/L1_2_MusicBox2_MGScreen/hand.png" );
    hand->setPosition( FRAME_CENTER );
    this->addChild( hand, 3000 );
    
    /* PORT
    MoveTo* moveHand = MoveTo::create( 1,  Vec2( 52, 310 ) );
    
    CCCallFuncND *callFunc = CCCallFuncND::create( this, callfuncND_selector(L1_3_Quest_1_MGScreen::onHandTakeNote), hand );
    
    FadeOut* hideHand = FadeOut::create( 0.01f );
    
    Sequence* sequence = Sequence::create( moveHand, callFunc, hideHand, NULL );
    
    hand->runAction( sequence );
     */
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::onHandTakeNote( cocos2d::Sprite* _sprite )
{
    // Add new hand with note
    Sprite* handWithNote = Sprite::create( "textures/scenes/minigame/L1_2_MusicBox2_MGScreen/handWithNumber.png" );
    handWithNote->setPosition( _sprite->getPosition() );
    this->addChild( handWithNote, 3000 );
    
    this->numbers[0]->setVisible( false );
    
    MoveTo* moveHand = MoveTo::create( 2.0f,  Vec2( 52, 480 ) );
    
    FadeOut* hideHand = FadeOut::create( 0.5f );
    
    CallFunc* callFunc = CallFunc::create( std::bind( &L1_3_Quest_1_MGScreen::functionCallEndHintHand, this ) );
    
    Sequence* sequence = Sequence::create( moveHand, hideHand, callFunc, NULL );
    
    handWithNote->runAction( sequence );
}

//---------------------------------------------------------------------------------------------------------------------------------
void L1_3_Quest_1_MGScreen::functionCallEndHintHand()
{
    this->numbers[0]->setVisible( true );
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------