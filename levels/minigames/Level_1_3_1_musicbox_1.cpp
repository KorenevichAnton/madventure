//
//  Level_1_3_1_musicbox_1.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/9/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_3_1_musicbox_1.h"

#include "GraphicAnimationObject.h"

#include "GameScene.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_1::Level_1_3_1_musicbox_1():
musicBoxState(false),
rNumber(0),
timer(0),
trueAnswers(0),
falseAnswers(0),
labelDo( nullptr ),
labelRe( nullptr ),
labelMi( nullptr ),
labelFa( nullptr ),
labelSol( nullptr ),
labelLa( nullptr ),
labelTi( nullptr ),
m_minigameControl(nullptr),
aSource(nullptr),
defeatAction(nullptr),
winAction(nullptr)
{
    level = level_id::MG_1_3_1_MUSICBOX_1;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_3_musicbox_1.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_3_musicbox_1.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_1::~Level_1_3_1_musicbox_1()
{
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_1* Level_1_3_1_musicbox_1::create()
{
    Level_1_3_1_musicbox_1* pLevel_1_3_1_musicbox_1 = new Level_1_3_1_musicbox_1();
    if(pLevel_1_3_1_musicbox_1)
    {
        return pLevel_1_3_1_musicbox_1;
    }
    else
    {
        delete pLevel_1_3_1_musicbox_1;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_1_musicbox_1::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    
    srand (time(NULL));
    timer = 0;
    trueAnswers = 0;
    falseAnswers = 0;
    rNumber = 0;
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onEnter()
{
    
    MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::NOTESMINIGAME);
    this->pHudScreen->hideSticker();
    
    this->pNotesMinigameScreen = dynamic_cast<NotesMinigameScreen*>(MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::NOTESMINIGAME));
    
    this->m_minigameControl = MiniGameControls::create();
    GET_RUNNING_SCENE->addChild(this->m_minigameControl, 3000);
    this->m_minigameControl->getStartGameImage()->setVisible(false);

    //on victory button
    this->m_minigameControl->configOnVictoryButton([this](){ this->onVictoryBtn(); });
    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = level_id::LVL_1_3_PART1;
    
    //add sounds
    this->aSource = MANAGER_Audio->createStaticSource();
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    this->embiantSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    
    //exit musicbox animation
    graphic::GraphicAnimationObject* crystal = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(900)->getGraphic());
    if( crystal )
        crystal->setScale( Vec2(1.5, 1.5) );
    
     //Logic on enter level level
    this->pDialogScreen->loadDialogFromXml(70, 0, 0);
    
    this->labelDo = Label::createWithTTF( L10N_STRING( 1301 ).c_str(), FONT_PATH, 26);
    this->labelDo->setPosition(Vec2(288, 128));
    this->labelDo->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelDo, 100);
    
    this->labelRe = Label::createWithTTF( L10N_STRING( 1302 ).c_str(), FONT_PATH, 26);
    this->labelRe->setPosition(Vec2(448, 128));
    this->labelRe->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelRe, 100);
    
    this->labelMi = Label::createWithTTF( L10N_STRING( 1303 ).c_str(), FONT_PATH, 26);
    this->labelMi->setPosition(Vec2(608, 128));
    this->labelMi->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelMi, 100);
    
    this->labelFa = Label::createWithTTF( L10N_STRING( 1304 ).c_str(), FONT_PATH, 26);
    this->labelFa->setPosition(Vec2(768, 128));
    this->labelFa->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelFa, 100);
    
    this->labelSol = Label::createWithTTF( L10N_STRING( 1305 ).c_str(), FONT_PATH, 26);
    this->labelSol->setPosition(Vec2(928, 128));
    this->labelSol->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelSol, 100);
    
    this->labelLa = Label::createWithTTF( L10N_STRING( 1306 ).c_str(), FONT_PATH, 26);
    this->labelLa->setPosition(Vec2(1088, 128));
    this->labelLa->setScale(1.5);
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelLa, 100);
    
    this->labelTi = Label::createWithTTF( L10N_STRING( 1307 ).c_str(), FONT_PATH, 26);
    this->labelTi->setPosition(Vec2(1248, 128));
    this->labelTi->setScale(1.5);
    
    dynamic_cast<GameScene*>(GET_RUNNING_SCENE)->getForegroundLayer()->addChild(labelTi, 100);
    if( this->getLevelState( "atfirst_musicbox_1_level_1_3_1" ) != 1 )
    {
        //complete stat
        MANAGER_Memory->getTempData()->statSecretPlaces += 1;
        this->setLevelState( "atfirst_musicbox_1_level_1_3_1", 1 );
    }
    //
    //MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 1 );
    
    // Camera positioning
    //this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onExit()
{
    if( MANAGER_Memory->getTempData()->getLevelState( "SINGLE_MINIGAME_RESTART" ) == 0 )
        this->setLevelState( "SINGLE_MINIGAME", 0 );
    else
        MANAGER_Memory->getTempData()->setLevelState( "SINGLE_MINIGAME_RESTART", 1 );
    
    MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 0 );
    MANAGER_Level->stopLevelTimer();
    this->uploadLevelStates();
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::update(float _dt)
{
    //Game camera following the player
    //pGameCamera->setPosition(pPlayer->getGameObject()->getPosition());
    if(pGameWorld && pGameCamera)
    {
        if( this->getLevelState( "mbox_1_go_to_1_3_1" ) == 1 )
        {
            this->setLevelState( "mbox_1_go_to_1_3_1", 0 );
            MANAGER_Level->replaceLevel( level_id::LVL_1_3_PART1 );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_1_musicbox_1::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    if(pGameWorld && pGameCamera)
    {
        //gold coins on each location
        if( _kind == COIN_GOLD_HP )
        {
            if( this->aSource )
            {
                if( this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" ) )
                {
                    this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
                    this->aSource->play();
                }
            }
        }
        
        if(_coin->getId() == 4500 || _coin->getId() == 1200)
            return true;
        
        if(pNotesMinigameScreen)
        {
            if(_kind == COIN_MUSIC_BOX_3_NONE)
            {
                MANAGER_Level->getCurrentLevel()->hideGameObject( 700 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 701 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 702 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 703 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 704 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 705 );
                MANAGER_Level->getCurrentLevel()->hideGameObject( 706 );
                musicBoxState = false;
            }
            if(_kind == COIN_NOTE_1)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 0)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_C.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_C.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(700)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(700));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_2)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 1)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_D.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_D.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(701)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(701));
                    
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_3)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 2)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_E.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_E.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(702)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(702));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_4)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 3)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_F.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_F.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(703)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(703));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_5)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 4)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_G.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_G.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(704)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(704));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_6)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 5)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_A.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_A.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(705)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(705));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
            if(_kind == COIN_NOTE_7)
            {
                if(!musicBoxState)
                {
                    timer = 5;
                    if(rNumber == 6)
                    {
                        m_minigameControl->showCorrect();
                        trueAnswers += 1;
                    }
                    else
                    {
                        m_minigameControl->showWrong();
                        falseAnswers += 1;
                    }
                    
                    if( this->aSource )
                    {
                        if( this->aSource->loadAudioFile( "audio/minigame/piano/piano_H.wav" ) )
                        {
                            this->aSource->loadSettings( "audio/minigame/piano/piano_H.wav" );
                            this->aSource->play();
                        }
                    }
                    
                    this->getGameObjectById(706)->setActivity(true);
                    this->pGameWorld->addGameObjectToActive(this->getGameObjectById(706));
                    //AudioManager::sharedInstance()->playSound(str);
                    
                    this->genNewTask();
                }
                musicBoxState = true;
            }
        }
        //else
            //AudioManager::sharedInstance()->playSound(str);
        
        
        
        if(trueAnswers == 5)
        {
            this->onMinigameEnd(true, screen_id::NOTESMINIGAME);
            trueAnswers += 1;
            return false;
        }
        
        if(falseAnswers == 5)
        {
            this->onMinigameEnd(false, screen_id::NOTESMINIGAME);
            falseAnswers += 1;
            return false;
        }
        
        for(int i = 811 ; i < 823 ; i++)
        {
            if(_coin->getId() == i)
            {
                _coin->release();
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onPortal(GameObject *_portal)
{
    if(pGameWorld && pGameCamera)
    {
        if(_portal->getId() == 900)
        {
            this->pHudScreen->hideSticker();
            this->pPlayer->stop();
            
            if( pPlayer->getGameObject()->getLocation() == game_object_location::GROUND )
            {
                MANAGER_Memory->getTempData()->setLevelState( "level_complete", 1 );
                
                GameObject* pObj = this->getGameObjectById( 777 );
                
                if( pObj )
                {
                    graphic::GraphicAnimationObject* anim = dynamic_cast<graphic::GraphicAnimationObject*>(pObj->getGraphic());
                    
                    if( anim )
                    {
                        anim->setCallbackEndAnimation( graphic::sequence_mob::SM_JOY, [this](){ this->onVictory(); } );
                    }
                }
                
                _portal->release();
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onMinigameEnd(bool _isWin, screen_id _screen)
{
    m_minigameControl->setVisible( false );
    pNotesMinigameScreen = NULL;
    
    this->labelDo->setVisible( false );
    this->labelRe->setVisible( false );
    this->labelMi->setVisible( false );
    this->labelFa->setVisible( false );
    this->labelSol->setVisible( false );
    this->labelLa->setVisible( false );
    this->labelTi->setVisible( false );
    
    if( _isWin )
    {
        this->pHudScreen->showSticker();
        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
        this->getGameObjectById(20)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(20));
        this->getGameObjectById(21)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(21));
        this->getGameObjectById(22)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(22));
        this->getGameObjectById(23)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(23));
        this->getGameObjectById(26)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(26));
        this->getGameObjectById(900)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(900));
        
        this->getGameObjectById(1200)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(1200));
        
        this->getGameObjectById(4500)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(4500));
        
        this->getGameObjectById(811)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(811));
        this->getGameObjectById(812)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(812));
        this->getGameObjectById(813)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(813));
        this->getGameObjectById(814)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(814));
        this->getGameObjectById(815)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(815));
        this->getGameObjectById(816)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(816));
        this->getGameObjectById(817)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(817));
        this->getGameObjectById(818)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(818));
        this->getGameObjectById(819)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(819));
        this->getGameObjectById(820)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(820));
        this->getGameObjectById(821)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(821));
        this->getGameObjectById(822)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(822));
        
        MANAGER_Level->getCurrentLevel()->hideGameObject( 700 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 701 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 702 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 703 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 704 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 705 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 706 );
        
        MANAGER_Level->getCurrentLevel()->hideGameObject( 800 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 801 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 802 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 803 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 804 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 805 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 806 );
        
        MANAGER_Level->getCurrentLevel()->hideGameObject( 1000 );
    }
    else
    {
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();

        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
        this->defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                CallFunc::create( CC_CALLBACK_0( Level_1_3_1_musicbox_1::onDefeat, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( this->defeatAction );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onDialogueEnd( int _id )
{
    if( _id == 70 )
    {
        this->pHudScreen->showSticker();
        this->genNewTask();
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_1_3_1", 1 );
        this->setLevelState( "musicbox_1_level_1_3_1_complete", 1 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onDefeat()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_1_3_1", 1 );
        this->setLevelState( "musicbox_1_level_1_3_1_complete", 2 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::onVictoryBtn()
{
    //TEMP FOR TEST fast ending level
    this->onMinigameEnd( true, screen_id::NOTESMINIGAME );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 8, 13 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    // Camera positioning
    this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::genNewTask()
{
    if(pNotesMinigameScreen)
    {
        rNumber = rand() % 7;
        if(rNumber == 0)
        {
            pNotesMinigameScreen->showNote1();
        }
        if(rNumber == 1)
        {
            pNotesMinigameScreen->showNote2();
        }
        if(rNumber == 2)
        {
            pNotesMinigameScreen->showNote3();
        }
        if(rNumber == 3)
        {
            pNotesMinigameScreen->showNote4();
        }
        if(rNumber == 4)
        {
            pNotesMinigameScreen->showNote5();
        }
        if(rNumber == 5)
        {
            pNotesMinigameScreen->showNote6();
        }
        if(rNumber == 6)
        {
            pNotesMinigameScreen->showNote7();
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_1::saveLevelStatData()
{
    //save player data as check point data
    MANAGER_Memory->getTempData()->cpPlayerHP = MANAGER_Memory->getTempData()->playerHP;
    MANAGER_Memory->getTempData()->cpPlayerFireballAmmo = MANAGER_Memory->getTempData()->playerFireballAmmo;
    //saving coins for geting points on the end of level
    MANAGER_Memory->getTempData()->playerCoinsIntoPoints = this->localPlayerCoinsIntoPoints;
    //saving local coins state
    MANAGER_Memory->getTempData()->playerCoins = this->localPlayerCoins;
    MANAGER_Memory->getTempData()->vGetCoinIds = this->vLocalGetCoinIds;
    MANAGER_Memory->getTempData()->vKilledMobsIds = this->vLocalKilledMobsIds;
}

//-----------------------------------------------------------------------------------------------------