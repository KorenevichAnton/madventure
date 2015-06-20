//
//  Level_1_3_1_musicbox_2.cpp
//  MusicalAdventure
//
//  Created by ampa_games on 9/13/14.
//  Copyright (c) 2014 ampagames. All rights reserved.
//

#include "Level_1_3_1_musicbox_2.h"
#include "L1_3_MusicBox2_MGScreen.h"

#include "GraphicAnimationObject.h"

#include "HudScreen.h"

#include "GameObject.h"

#include "CompleteScreen.h"
#include "TryMoreScreen.h"
#include "DialogScreen.h"

using namespace cocos2d;

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_2::Level_1_3_1_musicbox_2() :
aSource( nullptr ),
defeatAction( nullptr ),
winAction( nullptr )
{
    level = level_id::MG_1_3_1_MUSICBOX_2;
    
    this->levelGameWorldPath = new char[50];
    sprintf(levelGameWorldPath, "%s%s", PHYS_LANDSCAPES_PATH, "landscape1_3_1_musicbox_2.xml");
    this->levelGameObjectsPath = new char[50];
    sprintf(levelGameObjectsPath, "%s%s", PHYS_OBJECTS_PATH, "objects1_3_1_musicbox_2.xml");
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_2::~Level_1_3_1_musicbox_2()
{
    
}

//-----------------------------------------------------------------------------------------------------
Level_1_3_1_musicbox_2* Level_1_3_1_musicbox_2::create()
{
    Level_1_3_1_musicbox_2* pLevel_1_3_1_musicbox_2 = new Level_1_3_1_musicbox_2();
    if(pLevel_1_3_1_musicbox_2)
    {
        return pLevel_1_3_1_musicbox_2;
    }
    else
    {
        delete pLevel_1_3_1_musicbox_2;
        return NULL;
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_1_musicbox_2::init()
{
    //set using screens
    this->pDialogScreen = dynamic_cast< DialogScreen* > ( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::DIALOG ) );
    this->pHudScreen = dynamic_cast< HudScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, screen_id::HUD) );
    
    MANAGER_Screen->addScreen(GET_RUNNING_SCENE, screen_id::MG_L1_3_MUSICBOX_2);
    this->pHudScreen->hideSticker();
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onEnter()
{
    //Player positioning
    //this->determinePlayerPosition();
    
    //on restart level from pause/defeat/win level loading
    this->restartLevel = level_id::LVL_1_BOSS;
    
    //add sounds
    this->aSource = MANAGER_Audio->createStaticSource( "audio/scenes/test/blueNotes/zolotayaNota.wav" );
    if( this->aSource )
        this->aSource->setAssociate( AudioSourceAssociate::SOUND );
    
    MANAGER_Level->getCurrentLevel()->getEmbiantSource()->setGain(0.0f);
    MANAGER_Level->getCurrentLevel()->getBackMusicSource()->setGain(0.0f);
    
    //exit musicbox animation
    graphic::GraphicAnimationObject* crystal = dynamic_cast<graphic::GraphicAnimationObject*>(this->getGameObjectById(900)->getGraphic());
    crystal->setScale( Vec2( 1.5, 1.5 ) );
    
    //Logic on enter level level
    this->pDialogScreen->loadDialogFromXml(106, 0, 0);
    if( this->getLevelState( "atfirst_musicbox_2_level_1_3_1" ) != 1 )
    {
        //complete stat
        MANAGER_Memory->getTempData()->statSecretPlaces += 1;
        this->setLevelState( "atfirst_musicbox_2_level_1_3_1", 1 );
    }
    //
    //MANAGER_Memory->getTempData()->setLevelState( "MUSIC_BOX_RESTART", 1 );
    
    // Camera positioning
    //this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onExit()
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
void Level_1_3_1_musicbox_2::release()
{
    MANAGER_Screen->removeAllScreens(GET_RUNNING_SCENE);
    delete []this->levelGameWorldPath;
    delete []this->levelGameObjectsPath;
    delete this;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::update(float _dt)
{
    //Game camera following the player
    //pGameCamera->setPosition(pPlayer->getGameObject()->getPosition());
    if(pGameWorld && pGameCamera)
    {
        if( this->getLevelState( "mbox_1_go_to_level_1_boss" ) )
        {
            this->setLevelState( "mbox_1_go_to_level_1_boss", 0 );
            MANAGER_Level->replaceLevel( level_id::LVL_1_BOSS );
        }
    }
}

//-----------------------------------------------------------------------------------------------------
bool Level_1_3_1_musicbox_2::onGetCoin(GameObject* _coin, std::string _key, PhysTriggerCoinKind _kind)
{
    //gold coins on each location
    if( _kind == COIN_GOLD_HP )
    {
        if( this->aSource )
            this->aSource->play();
    }
    
    return true;
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onPortal(GameObject *_portal)
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
                
                
                this->winAction = Sequence::create( DelayTime::create( 2.0f ),
                                                     CallFunc::create( CC_CALLBACK_0( Level_1_3_1_musicbox_2::onVictory, this ) ), nullptr );
                
                GET_RUNNING_SCENE->runAction( this->winAction );
                _portal->release();
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onMinigameEnd(bool _isWin, screen_id _screen)
{
    if( _isWin )
    {
        this->pHudScreen->showSticker();
        dynamic_cast< L1_3_MusicBox2_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, _screen ) )->hideMinigameInterface();
        //MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        
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
    }
    else
    {
        //MANAGER_Screen->removeScreen(GET_RUNNING_SCENE, _screen);
        dynamic_cast< L1_3_MusicBox2_MGScreen* >( MANAGER_Screen->getScreen( GET_RUNNING_SCENE, _screen ) )->hideMinigameInterface();
        
        this->pHudScreen->hideSticker();
        this->pPlayer->stop();
        
        this->defeatAction = Sequence::create( DelayTime::create( 2.0f ),
                                                CallFunc::create( CC_CALLBACK_0( Level_1_3_1_musicbox_2::onDefeat, this ) ), nullptr );
        GET_RUNNING_SCENE->runAction( this->defeatAction );
        
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onVictory()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_level_1_boss", 1 );
        this->setLevelState( "musicbox_1_level_1_boss_complete", 1 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::onDefeat()
{
    if( this->getLevelState( "SINGLE_MINIGAME" ) == 0 )
    {
        this->setLevelState( "mbox_1_go_to_level_1_boss", 1 );
        this->setLevelState( "musicbox_1_level_1_boss_complete", 2 );
        this->saveLevelStatData();
    }
    else
    {
        MANAGER_Scene->replaceScene( scene_id::MINIGAMES_SELECTION );
    }
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::determinePlayerPosition()
{
    //player first positioning
    MANAGER_Memory->getTempData()->playerPosition = Vec2( 23, 8 );
    
    //set player position
    this->pPlayer->getGameObject()->setPosition( MANAGER_Memory->getTempData()->playerPosition );
    
    // Camera positioning
    this->pGameCamera->setPosition( Vec2( 0, 0 ) );
}

//-----------------------------------------------------------------------------------------------------
void Level_1_3_1_musicbox_2::saveLevelStatData()
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