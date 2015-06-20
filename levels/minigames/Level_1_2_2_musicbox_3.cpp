//
//  Level_1_2_2_musicbox_3.cpp
//  MusicalAdventure
//
//  Created by madmin on 6/4/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_2_2_musicbox_3.h"

#include "GraphicAnimationObject.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

#include "L1_2_MusicBox3_MGScreen.h"
#include "HintDurationSumScreen.h"

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_3::Level_1_2_2_musicbox_3():
musicBoxState(false),
musicBox3SumForWin(0.0f),
musicBox3True(0),
musicBox3False(0),
defeatAction(nullptr),
winAction(nullptr)
{
    level = level_id::MG_1_2_2_MUSICBOX_3;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH , "landscape1_2_musicbox_3.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH , "objects1_2_musicbox_3.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_3::~Level_1_2_2_musicbox_3()
{
    
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_2_musicbox_3* Level_1_2_2_musicbox_3::create()
{
    Level_1_2_2_musicbox_3* pLevel_1_2_2_musicbox_3 = new Level_1_2_2_musicbox_3();
    if(pLevel_1_2_2_musicbox_3)
    {
        return pLevel_1_2_2_musicbox_3;
    }
    else
    {
        delete pLevel_1_2_2_musicbox_3;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_2_musicbox_3::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onEnter()
{


    // MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_3);
    this->pHudScreen->hideSticker();
    
    this->m_minigameControl = MiniGameControls::create();
    GET_RUNNING_SCENE->addChild( this->m_minigameControl, 10 );
    this->m_minigameControl->getStartGameImage()->setVisible(false);

    //Player positioning
    //this->determinePlayerPosition();

    //on restart level from pause/defeat/win level loading
    this->restartLevel = level_id::LVL_1_2_PART2;
    
    //add sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    this->backMusicSource->loadAudioFile( MUSIC_CAVE );
    this->backMusicSource->loadSettings( MUSIC_CAVE );
    this->backMusicSource->setLooping( true );
    this->backMusicSource->play();
    this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    
    this->embiantSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MAX_VOLUME_EMBIENT_MUSIC );
    
    
    //exit musicbox animation
    graphic::GraphicAnimationObject* crystal = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(900)->getGraphic());
    crystal->setScale( Vec2( 1.5, 1.5 ) );
    
    //Logic on enter level
    this->pDialogScreen->loadDialogFromXml(51, 0, 0, "dialogs/complete.png", Vec2(700, 280), 2.0f);
    if( this->getLevelState( "atfirst_musicbox_1_level_1_2_2" ) != 1 )
    {
        //complete stat
        MANAGER_Memory->getTempData()->statSecretPlaces += 1;
        this->setLevelState( "atfirst_musicbox_1_level_1_2_2", 1 );
    }
    //
    //MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 1 );
    
    // Camera positioning
    //this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onExit()
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
void Level_1_2_2_musicbox_3::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::update(float _dt)
{
    if(pGameWorld && pGameCamera)
    {
        if( this->getLevelState( "mbox_1_go_to_1_2_2" ) == 1 )
        {
            this->setLevelState( "mbox_1_go_to_1_2_2", 0 );
            MANAGER_Level->replaceLevel( level_id::LVL_1_2_PART2 );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_2_musicbox_3::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    //gold coins on each location
    //_kind == COIN_MUSIC_BOX_3_05 ||
    //_kind == COIN_MUSIC_BOX_3_025 ||
    //_kind == COIN_MUSIC_BOX_3_0125 )
    if( _kind == COIN_GOLD_HP )
    {
        this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
        if( this->aSource )
        {
            this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            this->aSource->play();
        }
    }
    
    //coin of minigame
    if(_kind == COIN_MUSIC_BOX_3_NONE)
        musicBoxState = false;
    
    if(_kind == COIN_MUSIC_BOX_3_05)
    {
        if(!musicBoxState)
        {
            musicBox3SumForWin += 0.5f;
            pMusicBox3MinigameScreen->addNote(2);
            //AudioManager::sharedInstance()->playSound(str);
            this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            if( this->aSource )
            {
                this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
                this->aSource->play();
            }
            this->sortNotes();
            
        }
        musicBoxState = true;
    }
    if(_kind == COIN_MUSIC_BOX_3_025)
    {
        if(!musicBoxState)
        {
            musicBox3SumForWin += 0.25f;
            pMusicBox3MinigameScreen->addNote(1);
            //AudioManager::sharedInstance()->playSound(str);
            this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            if( this->aSource )
            {
                this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
                this->aSource->play();
            }
            this->sortNotes();
        }
        musicBoxState = true;
    }
    if(_kind == COIN_MUSIC_BOX_3_0125)
    {
        if(!musicBoxState)
        {
            musicBox3SumForWin += 0.125f;
            pMusicBox3MinigameScreen->addNote(0);
            //AudioManager::sharedInstance()->playSound(str);
            this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            if( this->aSource )
            {
                this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
                this->aSource->play();
            }
            this->sortNotes();
        }
        musicBoxState = true;
    }
    
    
    if(musicBox3SumForWin > 1.124f)
    {
        musicBox3SumForWin = 0.0f;
        pMusicBox3MinigameScreen->getTask();
        pMusicBox3MinigameScreen->setAnswers(0, 1);
        musicBox3False += 1;
        m_minigameControl->showWrong();
        if(musicBox3False == 4)
        {
            for(std::vector<GameObject*>::const_iterator iter = pGameWorld->getActiveGameObjects().begin() ; iter < pGameWorld->getActiveGameObjects().end() ; iter++)
            {
                //release door
                if((*iter)->getId() == 1130)
                    (*iter)->release();
                //release notes
                if((*iter)->getId() == 1100)
                    (*iter)->release();
                if((*iter)->getId() == 1101)
                    (*iter)->release();
                if((*iter)->getId() == 1102)
                    (*iter)->release();
            }
            this->onMinigameEnd(false, screen_id::MG_L1_2_MUSICBOX_3);
        }
    }
    
    if(musicBox3SumForWin > 0.99f && musicBox3SumForWin < 1.124f)
    {
        musicBox3SumForWin = 0.0f;
        pMusicBox3MinigameScreen->getTask();
        pMusicBox3MinigameScreen->setAnswers(1, 0);
        musicBox3True += 1;
        m_minigameControl->showCorrect();
        if(musicBox3True == 15)
        {
            for(std::vector<GameObject*>::const_iterator iter = pGameWorld->getActiveGameObjects().begin() ; iter < pGameWorld->getActiveGameObjects().end() ; iter++)
            {
                //release door
                if((*iter)->getId() == 1130)
                    (*iter)->release();
                //release notes
                if((*iter)->getId() == 1100)
                    (*iter)->release();
                if((*iter)->getId() == 1101)
                    (*iter)->release();
                if((*iter)->getId() == 1102)
                    (*iter)->release();
            }
            this->onMinigameEnd(true, screen_id::MG_L1_2_MUSICBOX_3);
            //LevelManager::sharedInstance()->replaceLevel(LVL_LEVEL_1_2_PART2);
        }
    }
    
    if(_kind != COIN_MUSIC_BOX_3_05 && _kind != COIN_MUSIC_BOX_3_025 && _kind != COIN_MUSIC_BOX_3_0125 && _kind != COIN_MUSIC_BOX_3_NONE)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onPortal(GameObject *_portal)
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
void Level_1_2_2_musicbox_3::onMinigameEnd(bool _isWin, screen_id _screen)
{
    if( _isWin )
    {
        this->pHudScreen->showSticker();
        this->pMusicBox3MinigameScreen->hideCounterTable();
        MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
        this->hideGameObject(1100);
        this->hideGameObject(1101);
        this->hideGameObject(1102);
        this->hideGameObject(1103);
        this->hideGameObject(1104);
        this->hideGameObject(1105);
        this->pPlayer->stop();
        
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
        this->getGameObjectById(800)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(800));
        
        MANAGER_Level->getCurrentLevel()->hideGameObject( 1103 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 1104 );
        MANAGER_Level->getCurrentLevel()->hideGameObject( 1105 );
        
        this->getGameObjectById(1200)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(1200));
        this->getGameObjectById(4500)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(4500));
    }
    else
    {
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();
        this->pMusicBox3MinigameScreen->hideCounterTable();
        
        this->defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                CallFunc::create( CC_CALLBACK_0( Level_1_2_2_musicbox_3::onDefeat, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( this->defeatAction );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onDialogueEnd( int _id )
{
    if( _id == 51 )
    {
        MANAGER_Screen->addScreen( GET_RUNNING_SCENE, screen_id::HINT_DURATION_SUM );
        HintDurationSumScreen* screen = dynamic_cast<HintDurationSumScreen*>( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HINT_DURATION_SUM ) );
        MANAGER_Memory->getTempData()->setLevelState( "hint_first_show_level_1_2_2_mb_3", 1 );
        
        if( screen )
            screen->setCallBackClose( [this](int _idDialogue){ this->onDialogueEnd( _idDialogue ); } );
    }
    
    if( _id == -1 )
    {
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_3);
        this->pMusicBox3MinigameScreen = dynamic_cast<L1_2_MusicBox3_MGScreen*>( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_3 ) );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_1_2_2", 1 );
        this->setLevelState( "musicbox_1_level_1_2_2_complete", 1 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::onDefeat()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_1_2_2", 1 );
        this->setLevelState( "musicbox_1_level_1_2_2_complete", 2 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 20, 10 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    // Camera positioning
    this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::sortNotes()
{
    int posNoteX = rand()%2;
    
    Vec2     tempPoint;
    
    GameObject* note1 = NULL;
    GameObject* note2 = NULL;
    GameObject* note3 = NULL;
    
    for (std::vector<GameObject*>::const_iterator iter = pGameWorld->getActiveGameObjects().begin(); iter < pGameWorld->getActiveGameObjects().end(); iter++)
    {
        if((*iter)->getId() == 1100)
            note1 = (*iter);
        if((*iter)->getId() == 1101)
            note2 = (*iter);
        if((*iter)->getId() == 1102)
            note3 = (*iter);
    }
    
    switch (posNoteX)
    {
        case 0:
            tempPoint = note1->getPosition();
            note1->setPosition(note2->getPosition());
            note2->setPosition(tempPoint);
            //
            tempPoint = note3->getPosition();
            note3->setPosition(note2->getPosition());
            note2->setPosition(tempPoint);
            break;
        case 1:
            tempPoint = note2->getPosition();
            note2->setPosition(note3->getPosition());
            note3->setPosition(tempPoint);
            //
            tempPoint = note1->getPosition();
            note1->setPosition(note2->getPosition());
            note2->setPosition(tempPoint);
            break;
            break;
        case 2:
            tempPoint = note3->getPosition();
            note3->setPosition(note1->getPosition());
            note1->setPosition(tempPoint);
            //
            tempPoint = note3->getPosition();
            note3->setPosition(note2->getPosition());
            note2->setPosition(tempPoint);
            break;
            break;
            
        default:
            break;
    }
    
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_2_musicbox_3::saveLevelStatData()
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