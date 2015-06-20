//
//  Level_1_2_1_musicbox_2.cpp
//  MusicalAdventure
//
//  Created by ampa_games on 8/25/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_2_1_musicbox_2.h"

#include "GraphicAnimationObject.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"
#include "L1_2_MusicBox2_MGScreen.h"

using namespace cocos2d;

//-----------------------------------------------------------------------------------------------------
Level_1_2_1_musicbox_2::Level_1_2_1_musicbox_2()
{
    level = level_id::MG_1_2_1_MUSICBOX_2;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH, "landscape1_2_1_musicbox_2.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH, "objects1_2_1_musicbox_2.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_1_musicbox_2::~Level_1_2_1_musicbox_2()
{
    
}

//-----------------------------------------------------------------------------------------------------
Level_1_2_1_musicbox_2* Level_1_2_1_musicbox_2::create()
{
    Level_1_2_1_musicbox_2* pLevel_1_2_1_musicbox_2 = new Level_1_2_1_musicbox_2();
    if(pLevel_1_2_1_musicbox_2)
    {
        return pLevel_1_2_1_musicbox_2;
    }
    else
    {
        delete pLevel_1_2_1_musicbox_2;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_1_musicbox_2::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_2);
    this->pHudScreen->hideSticker();

    this->aSource = MANAGER_Audio->createStaticSource( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onEnter()
{
    
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = level_id::LVL_1_2_PART1;
    
    //add sounds
    this->backMusicSource->loadAudioFile( MUSIC_CAVE );
    this->backMusicSource->loadSettings( MUSIC_CAVE );
    this->backMusicSource->setLooping( true );
    this->backMusicSource->play();
    this->backMusicSource->setAssociate( AudioSourceAssociate::MUSIC );
    
    this->embiantSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    this->backMusicSource->setGain( MIN_VOLUME_EMBIENT_MUSIC );
    
    //exit musicbox animation
    graphic::GraphicAnimationObject* crystal = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(900)->getGraphic());
    crystal->setScale( Vec2( 1.5, 1.5) );

    //Logic on enter level
    this->pDialogScreen->loadDialogFromXml(43, 0, 0);
    if( this->getLevelState( "atfirst_musicbox_2_level_1_2_1" ) != 1 )
    {
        //complete stat
        MANAGER_Memory->getTempData()->statSecretPlaces += 1;
        this->setLevelState( "atfirst_musicbox_2_level_1_2_1", 1 );
    }
    //
    //MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 1 );
    
    
    /* PORT
    IScreen* screen = MANAGER_Screen->getScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_2);
    if( screen )
    {
        L1_2_MusicBox2_MGScreen* mi = dynamic_cast<L1_2_MusicBox2_MGScreen*>(screen);
        
        if( mi )
            mi->setCallBackDialogHint( [this](){ this->onDialogHint(); } );
    }
    */
    
    // Camera positioning
    //this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onExit()
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
void Level_1_2_1_musicbox_2::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::update(float _dt)
{
    if(pGameWorld && pGameCamera)
    {
        if(pGameWorld && pGameCamera)
        {
            if( this->getLevelState( "mbox_2_go_to_1_2_1" ) == 1 )
            {
                this->setLevelState( "mbox_2_go_to_1_2_1", 0 );
                MANAGER_Level->replaceLevel( level_id::LVL_1_2_PART1);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_2_1_musicbox_2::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    //gold coins on each location
    if(_kind == COIN_GOLD_HP)
    {
        this->aSource->loadAudioFile( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
        if( this->aSource )
        {
            this->aSource->loadSettings( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
            this->aSource->play();
        }
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onPortal(GameObject *_portal)
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
void Level_1_2_1_musicbox_2::onMinigameEnd(bool _isWin, screen_id _screen)
{
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
        
        for(int i = 1000 ; i < 1016 ; i++)
        {
            this->getGameObjectById(i)->setActivity(true);
            this->pGameWorld->addGameObjectToActive(this->getGameObjectById(i));
        }
        
        this->getGameObjectById(900)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(900));
        
        this->getGameObjectById(800)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(800));
        
        this->getGameObjectById(4500)->setActivity(true);
        this->pGameWorld->addGameObjectToActive(this->getGameObjectById(4500));
    }
    else
    {
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();
        
        auto defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                CallFunc::create( CC_CALLBACK_0( Level_1_2_1_musicbox_2::onDefeat, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( defeatAction );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onSwitchArea(GameObject* _switcher)
{
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onDialogueEnd( int _id )
{
    if( _id == 43 )
    {
        /* PORT
         L1_2_MusicBox2_MGScreen* screenMG = dynamic_cast< L1_2_MusicBox2_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_2) );
        
        screenMG->showHintHand();
        */
        
        
        MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_2_MUSICBOX_2);
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_2_go_to_1_2_1", 1 );
        this->setLevelState( "musicbox_2_level_1_2_1_complete", 1 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::onDefeat()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_2_go_to_1_2_1", 1 );
        this->setLevelState( "musicbox_2_level_1_2_1_complete", 2 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 25, 7 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    // Camera positioning
    this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_2_1_musicbox_2::saveLevelStatData()
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
void Level_1_2_1_musicbox_2::onDialogHint()
{
    this->pDialogScreen->loadDialogFromXml( 108 );
}

//-----------------------------------------------------------------------------------------------------